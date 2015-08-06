//Ellen.cpp
//Implements definitions of Ellen functionality
//"We wanted to end the cycle of war between man and machine," - Ellen Tigh
//josh@mindaptiv.com

//includes
//custom
#include "Ellen.h"

//namespace
using namespace std;

//TODO: remove this
//Test
void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8];

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}//END print devices

void testLibUSB()
{
	//Variable Declaration:

	//device array
	libusb_device** devices;
	libusb_context* context;
	ssize_t count;
	int result;

	//init lib & session
	result = libusb_init(&context);

	//check for errors
	if(result < 0)
	{
		return;
	}

	//grab devices
	count = libusb_get_device_list(context, &devices);

	//check for errors
	if(result < 0)
	{
		return;
	}

	cout<<count<<endl;

	//print device info
	print_devs(devices);

	//free the list
	libusb_free_device_list(devices, 1);

	//exit
	libusb_exit(context);
}//END test usb lib
//END test

//Dyanamic Library Stuff
//Constant for how many versions in the future we look forward for checking lib #'s
dynLib allLibs[libCount];

libFunc libcFunctions[LIBC_FUNCTION_COUNT] =
{
		{
			LIBC_GETEUID,
			NULL
		},

		{
			"",
			NULL
		},

		{
			"",
			NULL
		},

		{
			"",
			NULL
		}
};//END libcFunctions

void fillTable()
{
	//Fill allLibs with info
	allLibs[libc].libName = LIBC_LIB_NAME;
	allLibs[libc].libAddr = NULL;
	allLibs[libc].versionNumber = LIBC_LATEST_VERSION;
	allLibs[libc].funcCount = LIBC_FUNCTION_COUNT;
	allLibs[libc].functions = libcFunctions;


}//END method

void openLibs()
{
	for (int i = 0; i< libCount; i++)
	{
		//LOG
		cout<<"Current lib: "<<i<<endl;

		//Iterate over versions until we find one that successfully dlopens
		for (int j = (allLibs[i].versionNumber + NUMBER_OF_VERSIONS_TO_LOOK_FORWARD); j >= 0; j--)
		{
			//LOG
			cout<<"Checking for library version # "<<j<<endl;

			//attempt dlopen
			if(j == 0)
			{
				//just use file name for if no numbered version works
				allLibs[i].libAddr  = dlopen(allLibs[i].libName, RTLD_NOW);
			}
			else
			{
				//build file name to check in dlopen()
				stringstream ss;
				ss << allLibs[i].libName << "." << j;
				string strName = ss.str();
				const char* libraryName = strName.c_str();

				//attempt to open
				allLibs[i].libAddr = dlopen(libraryName, RTLD_LAZY);
			}//END  elsif J == 0

			//if lib loaded successfully
			if(allLibs[i].libAddr != NULL)
			{
				cout<<"Library found!"<<endl;
				//Grab the functions
				for (int k = 0; k < allLibs[i].funcCount; k++ )
				{
					allLibs[i].functions[k].funcAddr = dlsym(allLibs[i].libAddr, allLibs[i].functions[k].funcName);

					//TODO: handle if dlsym fails?
					if(allLibs[i].functions[k].funcAddr == NULL)
					{
						//LOG
						cout<<"Method "<<k<<", "<<allLibs[i].functions[k].funcName<<" not found!"<<endl;
					}
				}//END function for loop

				//successfully grabbed lib and functions
				return;
			}//end if lib loaded successfully
		}//END inner for
	}//END outer for
}//END func

void closeLibs()
{
	//iterate through allLibs
	for (int i = 0; i < libCount; i++)
	{
		//close it
		//LOG
		cout<<"Closing library #"<<i<<endl;

		int close = dlclose(allLibs[i].libAddr);

		if (close != 0)
		{
			//TODO: handle failure to close shared library
			//LOG
			cout<<"Uh oh that's not right?"<<endl;
		}//END if
	}//END for
}//END method
//END DYLIB stuff

//Producers
void produceUserProfile(struct cylonStruct& et)
{
	//set default
	et.pictureLocation = ERROR_INT;

	struct passwd* pw;
	uid_t uid;

	uid = geteuid();
	pw = getpwuid(uid);
	if (pw)
	{
		const char* charUname = pw->pw_name;
		string strUname(charUname);
		et.username = strUname;

		//Build file name
		string avatarName = AVATAR_PATH + et.username;

		//Check if file exists
		ifstream f(avatarName.c_str());
		if (f.good())
		{
			//set picture Path and Type
			et.picturePath = avatarName;
			et.pictureType = AVATAR_TYPE;
		}
		else
		{
			//set defaults
			et.picturePath = ERROR_STRING;
			et.pictureType = ERROR_STRING;
		}

		//close file
		f.close();

	}//END if pw
	else
	{
		//set defaults
		et.username   	= ERROR_STRING;
		et.picturePath 	= ERROR_STRING;
		et.pictureType  = ERROR_STRING;
	}
}//END produceUsername

void produceDeviceName(struct cylonStruct& et)
{
	//Method Credit to Saurabh Gupta @ ccplusplus.com
	//Variable Declaration
	int result;
	char buf[32];

	//grab host name
	result = gethostname(buf, sizeof buf);

	//if error, set to default value
	if (result != 0)
	{
		et.deviceName = "0";
		return;
	}

	//otherwise, set deviceName field
	string strDeviceName(buf);
	et.deviceName = strDeviceName;
}//END produceDeviceName

void produceDateTimeZone(struct cylonStruct& et)
{
	//Credit to cppreference.com for partial method implementation
	//Variable Declaration
	int yearZero = 1900;
	int minutesInAnHour = 60;

	//Grab time
	time_t timeLocal = std::time(NULL);

	//Convert to local time
	tm* localTime = std::localtime(&timeLocal);

	//Set components of date/time
	et.milliseconds = 0;
	et.seconds 		= localTime->tm_sec;
	et.minutes		= localTime->tm_min;
	et.hours		= localTime->tm_hour;
	et.day			= localTime->tm_wday;
	et.date			= localTime->tm_mday;
	et.month		= localTime->tm_mon + 1;
	et.year			= localTime->tm_year + yearZero;
	et.dst			= localTime->tm_isdst;
	et.timeZoneName = localTime->tm_zone;
	et.timeZone		= localTime->tm_gmtoff/minutesInAnHour;
}//END produceTimeZone

void produceProcessorInfo(struct cylonStruct& et)
{
	//Grab processor count
	et.processorCount = sysconf(_SC_NPROCESSORS_ONLN);

	//Credit to MakeLinux.net for partial method code
	//Variable Declaration
	FILE* fp;
	char buffer[1024];
	size_t bytes_read;
	char* match;
	float clock_speed;
	float mhzToHz = 1000000;
	int   processorLevel;

	//Read contents of /proc/cpuinfo into the buffer
	fp = fopen("/proc/cpuinfo", "r");
	bytes_read = fread (buffer, 1, sizeof(buffer), fp);
	fclose(fp);

	//if read failed or exceeds buffer, get out
	if (bytes_read == 0 || bytes_read > sizeof(buffer))
	{
		//Set defaults
		et.hertz 			= ERROR_INT;
		et.processorLevel 	= ERROR_INT;
		et.architecture 	= ERROR_STRING;
	}
	else
	{
		//Add null termination to buffer
		buffer[bytes_read] = '\0';

		//Grab CPU speed
		match = strstr(buffer, "cpu MHz");
		if (match == NULL)
		{
			//Set Default
			et.hertz = ERROR_INT;
		}
		else
		{
			sscanf(match, "cpu MHz : %f", &clock_speed);
			et.hertz = clock_speed * mhzToHz;
		}//END if match is null

		//Grab architecture
		match = strstr(buffer, "model");

		if (match == NULL)
		{
			//Set default
			et.processorLevel = ERROR_INT;
		}
		else
		{
			sscanf(match, "model : %d", &processorLevel);
			et.processorLevel = (uint16_t)processorLevel;
		}//END if match is null
	}//END if

	//Credit to Amber @ stack overflow for partial method code
	//Grab utsname struct that contains architecture info
	struct utsname unameData;
	int result = uname(&unameData);

	//Check for errors
	if (result != 0)
	{
		//set default
		et.architecture = ERROR_STRING;
	} //end if result != 0
	else
	{
		et.architecture = unameData.machine;
	}
} //END produceProcessorInfo

void produceMemoryInfo(struct cylonStruct& et)
{
	//Credit to David R. Nadeau @ nadeausoftware.com for partial method code
	//Variable Declaration
	struct sysinfo info;
	float lowMemory = 0.99;

	//Grab info
	sysinfo(&info);

	//Set cylonStruct memory fields
	et.memoryBytes 				= info.totalram;
	et.bytesAvails 				= info.freeram;
	et.threshold   				= info.totalram * lowMemory;
	et.pageSize 				= sysconf (_SC_PAGESIZE);
	et.allocationGranularity 	= et.pageSize;
	et.minAppAddress 			= ERROR_INT;
	et.maxAppAddress			= ERROR_INT;

	//Calculate low memory
	if(info.freeram/info.totalram >= lowMemory)
	{
		et.lowMemory = 1;
	}
	else
	{
		et.lowMemory = 0;
	}

	//set osArch
	if (__WORDSIZE % 2)
	{
		//set default
		et.osArchitecture = ERROR_INT;
	}
	else
	{
		et.osArchitecture = __WORDSIZE;
	}
} //END produceMemoryInfo

void produceDeviceInfo(struct cylonStruct& et)
{
	//Grab USB Devices
	produceUsbDeviceInfo(et);

	//Grab total count
	et.detectedDeviceCount = et.detectedDevices.size();
} //END produceDeviceInfo

void produceUsbDeviceInfo(struct cylonStruct& et)
{
	//Credit to LibUSB examples for partial method code
	//Variable Declaration:

	//device array
	libusb_device** devices;
	libusb_context* context;
	ssize_t count;
	int result;
	std::list<struct deviceStruct> detectedDevices;

	//init lib & session
	result = libusb_init(&context);

	//check for errors
	if(result < 0)
	{
		return;
	}

	//grab devices
	count = libusb_get_device_list(context, &devices);

	//check for errors
	if(result < 0)
	{
		return;
	}

	//TODO: Remove this
	//LOG
	cout<<"USB Devices Detected: "<<count<<endl;


	libusb_device *device;
	int i = 0, j = 0;
	uint8_t path[8];

	//iterate over the devices retrieved
	while ( (device = devices[i++]) != NULL )
	{
		//Variable Declaration
		struct  libusb_device_descriptor descriptor;
		struct	deviceStruct devStrc;
		int 	result;

			//Grab description for device
			result = libusb_get_device_descriptor(device, &descriptor);

			//Check for errors
			if (result < 0)
			{
				//build device
				devStrc = buildUsbDevice(device);

				//Do nothing else for this device
				continue;
			}//END if error

			//set fields of deviceStruct via descriptor
			devStrc = buildUsbDevice(device, descriptor);

			//place deviceStruct in the back of the devices list
			detectedDevices.insert(detectedDevices.end(), devStrc);
		}//END while

		//free the list
		libusb_free_device_list(devices, 1);

		//exit
		libusb_exit(context);

		//TODO: handle special device types (controllers, etc.)
		//Alright, now grab the lsusb output (if available)

		//Variable Declaration
		FILE* fp;
		char buffer[4096];
		const char* consoleCommand = "lsusb";

		//open the pipe
		FILE* pipe = popen(consoleCommand, "r");

		//Check for errors
		if(!pipe)
		{
			//pipe command failed, we're done here
			return;
		}

		std::string strResult = "";

		while(!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
			{
				strResult += buffer;
			}
		}

		//close pipe
		pclose(pipe);

		//parse result for every device we found so far
		//TODO: parse stuff


		//Add devices into cylon device list
		while(!detectedDevices.empty())
		{
			et.detectedDevices.insert(et.detectedDevices.end(), detectedDevices.front());
			detectedDevices.pop_front();
		}//END WHILE
}//END produce USB device info

void produceLog(struct cylonStruct& et)
{
	cout<<"Cylon @: "<<&et<<endl;
	cout<<"Username: "<<et.username<<endl;
	cout<<"Device Name: "<<et.deviceName<<endl;
	cout<<"Timestamp: "<<et.day<<", "<<et.month<<"/"<<et.day<<"/"<<et.year<<" "<<et.hours<<":"<<et.minutes<<":"<<et.seconds<<":"<<et.milliseconds<<" "<<et.timeZoneName<<" "<<et.timeZone<<" DST: "<<et.dst<<endl;
	cout<<"Profile Picture Location: "<<et.picturePath<<" Type: "<<et.pictureType<<endl;
	cout<<"Processor Architecture: "<<et.architecture<<endl;
	cout<<"Processor Level: "<<et.processorLevel<<endl;
	cout<<"Processor Count: "<<et.processorCount<<endl;
	cout<<"Processor Clock Speed: "<<et.hertz<<"Hz"<<endl;
	cout<<"OS Architecture: "<<et.osArchitecture<<endl;
	cout<<"Installed Memory: "<<et.memoryBytes<<endl;
	cout<<"Available Memory: "<<et.bytesAvails<<endl;
	cout<<"Low Memory Threshold: "<<et.threshold<<endl;
	cout<<"Low Memory? "<<et.lowMemory<<endl;
	cout<<"Page Size: "<<et.pageSize<<endl;
	cout<<"Allocation Granularity: "<<et.allocationGranularity<<endl;
	cout<<"Min/Max App Address: "<<et.minAppAddress<<"/"<<et.maxAppAddress<<endl;
	cout<<"Detected Device Count: "<<et.detectedDeviceCount<<endl;
	cout<<"Error: "<<et.error<<endl;
}//END produceLog
//END PRODUCERS

//Builders
struct cylonStruct buildEllen()
{
	//Variable Declaration
	struct cylonStruct ellen;

	//fill table
	fillTable();

	//open libs
	openLibs();

	//producers
	produceUserProfile(ellen);
	produceDeviceName(ellen);
	produceDateTimeZone(ellen);
	produceProcessorInfo(ellen);
	produceMemoryInfo(ellen);
	produceDeviceInfo(ellen);
	produceLog(ellen);

	//close libs
	closeLibs();

	//return
	return ellen;
}//END buildEllen

//Blank builder
struct deviceStruct buildBlankDevice()
{
	//Variable Declaration
	struct deviceStruct device;

	//set default values for fields that are unused in this context
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isDefault		= ERROR_INT;
	device.isEnabled		= 1;
	device.orientation		= NO_ROTATION;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex  = ERROR_INT;
	device.storageIndex		= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;
	device.vendorID			= ERROR_INT;
	device.name				= ERROR_STRING;
	device.id				= ERROR_STRING;
	device.deviceType		= ERROR_INT;

	//Return
	return device;
}//END build blank device

struct deviceStruct buildUsbDevice(struct libusb_device* usbDev)
{
	//Variable Declaration
	struct deviceStruct device;

	//set default values for fields that are unused in this context
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isDefault		= ERROR_INT;
	device.isEnabled		= 1;
	device.orientation		= NO_ROTATION;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex  = ERROR_INT;
	device.storageIndex		= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;

	//TODO: fill these in
	//grab fields from arguments
	device.vendorID			= ERROR_INT;
	device.name				= ERROR_STRING;
	device.id				= ERROR_STRING;
	device.deviceType		= ERROR_INT;

	//Return
	return device;
}//END build device via libusb_device

struct deviceStruct buildUsbDevice(struct libusb_device* usbDev, struct libusb_device_descriptor descriptor)
{
	//Variable Declaration
	struct deviceStruct device;

	//set default values for fields that are unused in this context
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isDefault		= ERROR_INT;
	device.isEnabled		= 1;
	device.orientation		= NO_ROTATION;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex  = ERROR_INT;
	device.storageIndex		= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;

	//TODO: fill these in
	//grab fields from arguments
	device.vendorID			= ERROR_INT;
	device.name				= ERROR_STRING;
	device.id				= ERROR_STRING;
	device.deviceType		= ERROR_INT;

	//Return
	return device;
}//END build device via libusb device/descriptor combo
//END builders
