//Ellen.cpp
//Implements definitions of Ellen functionality
//"We wanted to end the cycle of war between man and machine," - Ellen Tigh
//josh@mindaptiv.com

//includes
//custom
#include "Ellen.h"

//namespace
using namespace std;

//Dyanamic Library Stuff
//Constant for how many versions in the future we look forward for checking lib #'s
dynLib allLibs[libCount];

libFunc libsdlFunctions[libsdlCount] =
{
		{
				SDL_NUMJOYSTICKS,
				NULL
		},

		{
				SDL_ISGAMECONTROLLER,
				NULL
		}
};

libFunc libusbFunctions[libusbCount] =
{
		{
			LIBUSB_INIT,
			NULL
		},

		{
			LIBUSB_GET_DEVICE_LIST,
			NULL
		},

		{
			LIBUSB_GET_DEVICE_DESCRIPTOR,
			NULL
		},

		{
			LIBUSB_GET_ACTIVE_CONFIG_DESCRIPTOR,
			NULL
		},

		{
			LIBUSB_FREE_DEVICE_LIST,
			NULL
		},

		{
			LIBUSB_EXIT,
			NULL
		},
};



void fillTable()
{
	//Fill allLibs with info

	//libusb
	allLibs[libusb].libName = LIBUSB_LIB_NAME;
	allLibs[libusb].libAddr = NULL;
	allLibs[libusb].versionNumber = LIBUSB_LATEST_VERSION;
	allLibs[libusb].funcCount = libusbCount;
	allLibs[libusb].functions = libusbFunctions;
	allLibs[libusb].opened = false;

	//libsdl
	allLibs[libsdl].libName = LIBSDL_LIB_NAME;
	allLibs[libsdl].libAddr = NULL;
	allLibs[libsdl].versionNumber = LIBSDL_LATEST_VERSION;
	allLibs[libsdl].funcCount = libsdlCount;
	allLibs[libsdl].functions = libsdlFunctions;
	allLibs[libsdl].opened = false;
}//END method

void openLibs()
{
	for (int i = 0; i < libCount; i++)
	{
		//Iterate over versions until we find one that successfully dlopens
		for (int j = (allLibs[i].versionNumber + NUMBER_OF_VERSIONS_TO_LOOK_FORWARD); j >= 0; j--)
		{
			//attempt dlopen
			if(j == 0)
			{
				//just use file name for if no numbered version works
				allLibs[i].libAddr  = dlopen(allLibs[i].libName, RTLD_LAZY);
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
				cout<<"Found lib "<<allLibs[i].libName<<endl;
				//set opened to true
				allLibs[i].opened = true;

				//Grab the functions
				for (int k = 0; k < allLibs[i].funcCount; k++ )
				{
					/*char* errstr;
					errstr = dlerror();
					printf("This happened: (%s)\n", errstr);
					*/

					//Grab function address
					allLibs[i].functions[k].funcAddr = dlsym(allLibs[i].libAddr, allLibs[i].functions[k].funcName);

					if(allLibs[i].functions[k].funcAddr != NULL)
					{
						cout<<"Found method "<<allLibs[i].functions[k].funcName<<endl;
					}
					else
					{
						cout<<"Didn't find method "<<allLibs[i].functions[k].funcName<<endl;
					}
				}//END function for loop
			}//end if lib loaded successfully
		}//END inner for
	}//END outer for
}//END func

void closeLibs()
{
	//iterate through allLibs
	for (int i = 0; i < libCount; i++)
	{
		//Variable Declaration
		int close;

		//if the library was marked as opened
		if(allLibs[i].opened)
		{
			//Close it
			close = dlclose(allLibs[i].libAddr);
			allLibs[i].opened = false;
		}
		else
		{
			close = 0;
		}//end IF opened

		if (close != 0)
		{
			//Could possibly handle errors somehow here.  Have yet to reach this point when dlclose fails
		}//END if error
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
	produceControllerInfo(et);

	//Grab total count
	et.detectedDeviceCount = et.detectedDevices.size();
} //END produceDeviceInfo

void produceControllerInfo(struct cylonStruct& et)
{
	//Cast functions
	SDL_NumJoysticks_t 		_SDL_NumJoysticks 		= (SDL_NumJoysticks_t) allLibs[libsdl].functions[SDL_NumJoysticks_e].funcAddr;
	SDL_IsGameController_t	_SDL_IsGameController 	= (SDL_IsGameController_t) allLibs[libsdl].functions[SDL_IsGameController_e].funcAddr;

	//grab gamepad count
	int gamepadCount = _SDL_NumJoysticks();

	cout<<"Gamepads: "<<gamepadCount<<endl;

	//Credit to SDL Wiki for partial method code
	for (int i = 0; i < gamepadCount; i++)
	{
		if (_SDL_IsGameController(i) )
		{
			cout<<"Joystick "<<i<<" is supported by the game controller interface!"<<endl;
		}
	}
}

void produceUsbDeviceInfo(struct cylonStruct& et)
{
	if (!allLibs[libusb].opened)
	{
		//Get out, this method won't work without libusb loaded!
		return;
	}

	//Credit to LibUSB examples for partial method code
	//Variable Declaration:
	//device array
	libusb_device** devices;
	libusb_context* context;
	int result;
	std::list<struct deviceStruct> detectedDevices;

	//Convert method to type
	libusb_init_t _libusb_init = (libusb_init_t) allLibs[libusb].functions[libusb_init_e].funcAddr;

	//init lib & session
	result = _libusb_init(&context);

	//check for errors
	if(result < 0)
	{
		return;
	}

	//Convert method to type
	libusb_get_device_list_t _libusb_get_device_list = (libusb_get_device_list_t) allLibs[libusb].functions[libusb_get_device_list_e].funcAddr;

	//grab devices
	_libusb_get_device_list(context, &devices);

	//check for errors
	if(result < 0)
	{
		return;
	}

	//Variable Declaration
	libusb_device *device;
	int i = 0;

	int	interfaceClass;

	//iterate over the devices retrieved
	while ( (device = devices[i++]) != NULL )
	{
		//Variable Declaration
		struct libusb_device_descriptor descriptor;
		struct libusb_config_descriptor* config;
		const struct libusb_interface* interface;
		const struct libusb_interface_descriptor* interfaceDescriptor;
		struct deviceStruct devStrc;
		int 	result;

			//Convert method to type
			libusb_get_device_descriptor_t _libusb_get_device_descriptor = (libusb_get_device_descriptor_t) allLibs[libusb].functions[libusb_get_device_descriptor_e].funcAddr;

			//Grab description for device
			result = _libusb_get_device_descriptor(device, &descriptor);

			//Check for errors
			if (result < 0)
			{
				//build device
				devStrc = buildBlankDevice();

				//Do nothing else for this device
				continue;
			}//END if error

			//convert method to type
			libusb_get_active_config_descriptor_t _libusb_get_active_config_descriptor = (libusb_get_active_config_descriptor_t) allLibs[libusb].functions[libusb_get_active_config_descriptor_e].funcAddr;

			//get config descriptor
			result = _libusb_get_active_config_descriptor(device, &config);

			//check for errors
			if (result < 0)
			{
				//build device
				devStrc = buildUsbDevice(device, descriptor);

				//do nothing else for this device
				continue;
			}

			//get interface class
			interface = config->interface;
			interfaceDescriptor = interface->altsetting;
			interfaceClass = interfaceDescriptor->bInterfaceClass;

			//set fields of deviceStruct via descriptor
			devStrc = buildUsbDevice(device, descriptor, interfaceClass);

			//place deviceStruct in the back of the devices list
			detectedDevices.insert(detectedDevices.end(), devStrc);
		}//END while

		//convert method to type
		libusb_free_device_list_t _libusb_free_device_list = (libusb_free_device_list_t) allLibs[libusb].functions[libusb_free_device_list_e].funcAddr;

		//free the list
		_libusb_free_device_list(devices, 1);

		//convert method to type
		libusb_exit_t _libusb_exit = (libusb_exit_t) allLibs[libusb].functions[libusb_exit_e].funcAddr;

		//exit from lib
		_libusb_exit(context);

		//TODO: handle special device types (controllers, etc.)

		//grab the lsusb output (if available)
		//Variable Declaration
		char buffer[8192];
		const char* consoleCommand = "lsusb";

		//open the pipe
		FILE* pipe = popen(consoleCommand, "r");

		//Check for errors
		if(!pipe)
		{
			//pipe command failed, we're done here
			return;
		}

		//read into the buffer
		size_t bytes_read = fread (buffer, 1, sizeof(buffer), pipe);

		//close pipe
		pclose(pipe);

		//parse result for every device we found so far
		while(!detectedDevices.empty())
		{
			//Build string and convert it for strstr checking
			//Credit to Benoit @ stackoverflow for partial method code
			ostringstream oss;
			oss << hex<< setfill('0')<<setw(4)<<detectedDevices.front().vendorID;
			std::string spotStr = oss.str();
			oss << dec;
			spotStr = spotStr + ":" + detectedDevices.front().id;

			const char* spot = spotStr.c_str();
			char* match = strstr(buffer, spot);

			//Check if match exists
			if (match != NULL)
			{
				//For storing the name ripped out of the buffer
				char englishName[bytes_read];
				int dontcare;
				int dontcare2;

				//Search buffer and rip name
				sscanf(match, "%x:%x %[^\t\n]", &dontcare, &dontcare2, englishName);
				detectedDevices.front().name = englishName;
			}//END if match is null

			//TODO: remove all other unnecessary loggings (Kenny Loggins?)

			//empty list and move contents to cylon's list
			et.detectedDevices.push_back(detectedDevices.front());
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
	cout<<"Devices: "<<endl;

	//credit to kmpofighter @ cplusplus.com for partial method code
	for(list<deviceStruct>::const_iterator iterator = et.detectedDevices.begin(), end = et.detectedDevices.end(); iterator != end; ++iterator)
	{
		cout<<"\t"<<"Name: "<<iterator->name<<endl;
		cout<<"\t"<<"Type: "<<iterator->deviceType<<endl;
		cout<<"\t"<<"Vendor ID: "<<hex<<iterator->vendorID<<dec<<endl;
		cout<<"\t"<<"Id: "<<iterator->id<<endl;
		cout<<"\t"<<"Orientation: "<<iterator->orientation<<endl;
		cout<<"\t"<<"Controller Index: "<<iterator->controllerIndex<<endl;
		cout<<"\t"<<"Display Index: "<<iterator->displayIndex<<endl;
		cout<<"\t"<<"Storage Index: "<<iterator->storageIndex<<endl<<endl;
	}
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

struct deviceStruct buildUsbDevice(struct libusb_device* usbDev, struct libusb_device_descriptor descriptor)
{
	//Variable Declaration
	struct deviceStruct device;

	//set default values for fields that are unused in this context
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isDefault		= ERROR_INT;
	device.orientation		= NO_ROTATION;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex  = ERROR_INT;
	device.storageIndex		= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;
	device.name				= ERROR_STRING;
	device.isEnabled		= 1;

	//grab fields from arguments
	device.vendorID			= descriptor.idVendor;

	//Convert id to string and save it
	//Credit to Benoit @ stackoverflow for partial method code
	ostringstream s;
	s << hex<< setfill('0')<<setw(4)<< descriptor.idProduct;
	device.id = s.str();
	s << dec;

	//grab bDeviceClass and interpret
	int devType = descriptor.bDeviceClass;

	//class per interface
	if (devType == LIBUSB_CLASS_PER_INTERFACE)
	{
		//TODO: parse interface, if necessary
		device.deviceType = ERROR_TYPE;
	}
	else if (devType == LIBUSB_CLASS_AUDIO)
	{
		device.deviceType = AUDIO_RENDER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_COMM)
	{
		device.deviceType = AUDIO_CAPTURE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_HID)
	{
		device.deviceType = HID_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PHYSICAL)
	{
		device.deviceType = PHYSICAL_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PRINTER)
	{
		device.deviceType = PRINTER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PTP || devType == LIBUSB_CLASS_IMAGE)
	{
		device.deviceType = IMAGE_SCANNER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_MASS_STORAGE)
	{
		device.deviceType = STORAGE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_HUB)
	{
		device.deviceType = HUB_TYPE;
	}
	else if (devType == LIBUSB_CLASS_DATA)
	{
		device.deviceType = COMMUNICATIONS_DATA_TYPE;
	}
	else if (devType == LIBUSB_CLASS_SMART_CARD)
	{
		device.deviceType = SMART_CARD_TYPE;
	}
	else if (devType == LIBUSB_CLASS_CONTENT_SECURITY)
	{
		device.deviceType = CONTENT_SECURITY_TYPE;
	}
	else if (devType == LIBUSB_CLASS_VIDEO)
	{
		device.deviceType = VIDEO_CAPTURE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PERSONAL_HEALTHCARE)
	{
		device.deviceType = PERSONAL_HEALTHCARE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_DIAGNOSTIC_DEVICE)
	{
		device.deviceType = ERROR_TYPE;
	}
	else if (devType == LIBUSB_CLASS_WIRELESS)
	{
		device.deviceType = WIRELESS_PHONE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_APPLICATION)
	{
		device.deviceType = ERROR_TYPE;
	}
	else if (devType == LIBUSB_CLASS_VENDOR_SPEC)
	{
		//Set error type
		device.deviceType = ERROR_TYPE;
	}
	else
	{
		device.deviceType = ERROR_TYPE;
	}

	//Return
	return device;
}//END build device via libusb device/descriptor combo

struct deviceStruct buildUsbDevice(struct libusb_device* usbDev, struct libusb_device_descriptor descriptor, int interfaceClass )
{
	//Variable Declaration
	struct deviceStruct device;

	//set default values for fields that are unused in this context
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isDefault		= ERROR_INT;
	device.orientation		= NO_ROTATION;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex  = ERROR_INT;
	device.storageIndex		= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;
	device.name				= ERROR_STRING;
	device.isEnabled		= 1;

	//grab fields from arguments
	device.vendorID			= descriptor.idVendor;

	//Convert id to string and save it
	//Credit to Benoit @ stackoverflow for partial method code
	ostringstream s;
	s << hex<< setfill('0')<<setw(4)<< descriptor.idProduct;
	device.id = s.str();
	s << dec;

	//grab bDeviceClass and interpret
	int devType = descriptor.bDeviceClass;

	//use interface type instead
	if (devType == LIBUSB_CLASS_PER_INTERFACE)
	{
		devType = interfaceClass;
	}

	//class per interface
	if (devType == LIBUSB_CLASS_PER_INTERFACE)
	{
		//should already be using class interface if we get here, so this is bizarre and probably wrong
		device.deviceType = ERROR_INT;
	}
	else if (devType == LIBUSB_CLASS_AUDIO)
	{
		device.deviceType = AUDIO_RENDER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_COMM)
	{
		device.deviceType = AUDIO_CAPTURE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_HID)
	{
		device.deviceType = HID_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PHYSICAL)
	{
		device.deviceType = PHYSICAL_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PRINTER)
	{
		device.deviceType = PRINTER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PTP || devType == LIBUSB_CLASS_IMAGE)
	{
		device.deviceType = IMAGE_SCANNER_TYPE;
	}
	else if (devType == LIBUSB_CLASS_MASS_STORAGE)
	{
		device.deviceType = STORAGE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_HUB)
	{
		device.deviceType = HUB_TYPE;
	}
	else if (devType == LIBUSB_CLASS_DATA)
	{
		device.deviceType = COMMUNICATIONS_DATA_TYPE;
	}
	else if (devType == LIBUSB_CLASS_SMART_CARD)
	{
		device.deviceType = SMART_CARD_TYPE;
	}
	else if (devType == LIBUSB_CLASS_CONTENT_SECURITY)
	{
		device.deviceType = CONTENT_SECURITY_TYPE;
	}
	else if (devType == LIBUSB_CLASS_VIDEO)
	{
		device.deviceType = VIDEO_CAPTURE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_PERSONAL_HEALTHCARE)
	{
		device.deviceType = PERSONAL_HEALTHCARE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_DIAGNOSTIC_DEVICE)
	{
		device.deviceType = ERROR_TYPE;
	}
	else if (devType == LIBUSB_CLASS_WIRELESS)
	{
		device.deviceType = WIRELESS_PHONE_TYPE;
	}
	else if (devType == LIBUSB_CLASS_APPLICATION)
	{
		device.deviceType = ERROR_TYPE;
	}
	else if (devType == LIBUSB_CLASS_VENDOR_SPEC)
	{
		//Set error type
		device.deviceType = ERROR_TYPE;
	}
	else
	{
		device.deviceType = ERROR_TYPE;
	}

	//Return
	return device;
}
//END builders
