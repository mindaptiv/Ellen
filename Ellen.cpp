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
	//TODO: cleanup
	//int result = SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	/*if (result < 0)
	{

		cout<<"I told them it means peace among worlds!"<<endl;
		return;
	}
*/
	//add mappings
	SDL_GameControllerAddMappingsFromFile("./src/Ellen/SDL_GameControllerDB/gamecontrollerdb.txt");

	//Cast functions
	SDL_NumJoysticks_t 		_SDL_NumJoysticks 		= (SDL_NumJoysticks_t) allLibs[libsdl].functions[SDL_NumJoysticks_e].funcAddr;
	SDL_IsGameController_t	_SDL_IsGameController 	= (SDL_IsGameController_t) allLibs[libsdl].functions[SDL_IsGameController_e].funcAddr;

	//grab gamepad count
	int gamepadCount = _SDL_NumJoysticks();

	//Credit to SDL Wiki for partial method code
	for (int i = 0; i < gamepadCount; i++)
	{
		SDL_Joystick* joy;
		joy = SDL_JoystickOpen(i);
		if(!joy)
		{
			//Retrieval failed, go on to next device
			continue;
		}

		//Use SDL_GameController Class
		if(_SDL_IsGameController(i))
		{
			//Open controller via index
			SDL_GameController* sdlPad = SDL_GameControllerOpen(i);

			//Make sure controller open worked
			if(!sdlPad)
			{
				//Retrieval failed, go on to next device
				continue;
			}
			else
			{
				//build structs
				deviceStruct 	device			= buildControllerDevice(i, SDL_GameControllerName(sdlPad), SDL_JoystickInstanceID(joy));
				controllerStruct controller 		= buildController(device, i, SDL_JoystickInstanceID(joy));

				//credit to davidgow.net for partial input code
				controller.thumbLeftX 		= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_LEFTX), false);
				controller.thumbLeftY		= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_LEFTY), false);
				controller.leftTrigger		= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_TRIGGERLEFT), true);
				controller.thumbRightX		= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_RIGHTX), false);
				controller.thumbRightY		= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_RIGHTY), false);
				controller.rightTrigger 	= normalizeAxis(SDL_GameControllerGetAxis(sdlPad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), true);

				//add to lists for ellen
				et.controllers.push_back(controller);
				device.controllerIndex = et.controllers.size() - 1;
				et.detectedDevices.push_back(device);
			}//END if sdlController successfully opened
		}//END if sdl is controller

		//use SDL_Joystick class
		else
		{
			//Use Joystick class to build device and controller structs
			deviceStruct		device 		= buildControllerDevice(i, SDL_JoystickName(joy), SDL_JoystickInstanceID(joy));
			controllerStruct 	controller	= buildController(device, i, SDL_JoystickInstanceID(joy));

			//add to lists for ellen
			et.controllers.push_back(controller);
			device.controllerIndex = et.controllers.size() - 1;
			et.detectedDevices.push_back(device);
		}
	}//END for


}//END Method

void produceUsbDeviceInfo(cylonStruct& et)
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
			spotStr = spotStr + ":" + detectedDevices.front().id_string;

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

			//empty list and move contents to cylon's list
			et.detectedDevices.push_back(detectedDevices.front());
			detectedDevices.pop_front();
		}//END WHILE
}//END produce USB device info

void produceLog(struct cylonStruct& et)
{
//TODO: restore this
	/*
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
*/
	//credit to kmpofighter @ cplusplus.com for partial method code
	for(list<deviceStruct>::const_iterator iterator = et.detectedDevices.begin(), end = et.detectedDevices.end(); iterator != end; ++iterator)
	{
		cout<<"\t"<<"Name: "<<iterator->name<<endl;
		cout<<"\t"<<"Type: "<<iterator->deviceType<<endl;
		cout<<"\t"<<"Vendor ID: "<<hex<<iterator->vendorID<<dec<<endl;
		cout<<"\t"<<"Id: "<<iterator->id_int<<endl;
		cout<<"\t"<<"Orientation: "<<iterator->orientation<<endl;
		cout<<"\t"<<"Controller Index: "<<iterator->controllerIndex<<endl;
		cout<<"\t"<<"Display Index: "<<iterator->displayIndex<<endl;
		cout<<"\t"<<"Storage Index: "<<iterator->storageIndex<<endl<<endl;
	}

	cout<<"Controllers: "<<endl;
	for(list<controllerStruct>::const_iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
	{
		cout<<"\t"<<"Controller #: "<<iterator->userIndex<<endl;
		cout<<"\t"<<"Instance ID: "<<iterator->id<<endl;
		cout<<"\t"<<"Name: "<<iterator->superDevice.name<<endl;
		cout<<"\t"<<"Buttons Mask: "<<hex<<iterator->buttons<<dec<<endl;
		cout<<"\t"<<"Left Trigger: "<<iterator->leftTrigger<<endl;
		cout<<"\t"<<"Right Trigger: "<<iterator->rightTrigger<<endl;
		cout<<"\t"<<"Left X: "<<iterator->thumbLeftX<<endl;
		cout<<"\t"<<"Left Y: "<<iterator->thumbLeftY<<endl;
		cout<<"\t"<<"Right X: "<<iterator->thumbRightX<<endl;
		cout<<"\t"<<"Right Y: "<<iterator->thumbRightY<<endl<<endl;
	}
	cout<<"Log done"<<endl;
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
	//closeLibs();

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
	device.id_string		= ERROR_STRING;
	device.id_int			= ERROR_INT;
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
	device.id_int			= descriptor.idProduct;

	//Convert id to string and save it
	//Credit to Benoit @ stackoverflow for partial method code
	ostringstream s;
	s << hex<< setfill('0')<<setw(4)<< descriptor.idProduct;
	device.id_string = s.str();
	s << dec;

	//grab bDeviceClass and interpret
	int devType = descriptor.bDeviceClass;

	//class per interface
	if (devType == LIBUSB_CLASS_PER_INTERFACE)
	{
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
	device.id_int			= descriptor.idProduct;

	//Convert id to string and save it
	//Credit to Benoit @ stackoverflow for partial method code
	ostringstream s;
	s << hex<< setfill('0')<<setw(4)<< descriptor.idProduct;
	device.id_string = s.str();
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

struct deviceStruct buildControllerDevice(int index, const char* deviceName, int instanceID)
{
	//Variable Declaration
	struct deviceStruct device;

	//set defaults
	device.panelLocation 	= ERROR_INT;
	device.inLid			= ERROR_INT;
	device.inDock			= ERROR_INT;
	device.isEnabled		= 1;
	device.orientation		= NO_ROTATION;
	device.vendorID			= ERROR_INT;
	device.displayIndex		= ERROR_INT;
	device.controllerIndex	= ERROR_INT; //value to be set later
	device.storageIndex 	= ERROR_INT;
	device.sensorsIndex		= ERROR_INT;

	//set device type
	device.deviceType 		= CONTROLLER_TYPE;

	//grab if default if it is the 0 indexed player
	if(index == 0)
	{
		device.isDefault = 1;
	}
	else
	{
		device.isDefault = ERROR_INT;
	}

	//grab names
	device.name 		= deviceName;
	device.id_string	= "" + instanceID;
	device.id_int		= instanceID;

	//Return
	return device;
}


struct controllerStruct buildBlankController()
{
	//Variable Declaration
	struct controllerStruct controller;

	//Set fields to basic defaults
	controller.userIndex   = -1; //some schemes have 0-3, 1-4, start with negative number to be safe just in case
	controller.packetNumber = ERROR_INT;
	controller.buttons      = ERROR_INT;
	controller.leftTrigger  = ERROR_INT;
	controller.rightTrigger = ERROR_INT;
	controller.thumbLeftX   = ERROR_INT;
	controller.thumbLeftY	= ERROR_INT;
	controller.thumbRightX  = ERROR_INT;
	controller.thumbRightY 	= ERROR_INT;

	//Return
	return controller;
}

//Build a controllerStruct using an SDL_GameController pointer
struct controllerStruct buildController(deviceStruct device, int index, int id)
{
	//Variable Declaration
	struct controllerStruct controller;

	//Set parent deviceStruct
	controller.superDevice = device;

	//set user index
	controller.userIndex = index;

	//set instance id of controller
	controller.id = id;
	cout<<"Controller built with instance id: "<<controller.id<<endl;

	//set fields that may change later
	controller.buttons = 0;

	//Set fields to basic defaults
	controller.packetNumber = ERROR_INT;

	//Return
	return controller;
}//END builder

//END builders

//Controllers
//parse gamecontroller and joystick events via event handling
void pollControllerEvents(struct cylonStruct& et)
{
	//Variable Declaration
	SDL_Event event;

	//while there are events to be handled
	while(SDL_PollEvent(&event))
	{
	if(event.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if(event.cbutton.state == SDL_PRESSED)
			{
				//iterate over controllers to find which one we're going to use
				for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
				{
					if(event.cbutton.which == (int)iterator->id)
					{
						iterator->buttons = pollButtons(iterator->buttons, event, true);
					}//END if id's match
				}//END for
			}//END If Pressed
		}//END outer if

		else if(event.type == SDL_CONTROLLERBUTTONUP)
		{
			if(event.cbutton.state == SDL_RELEASED )
			{
				//iterate over controllers to find which one we're going to use
				for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
				{
					if(event.cbutton.which == (int)iterator->id)
					{
						iterator->buttons = pollButtons(iterator->buttons, event, true);
					}//END if id's match
				}//END for
			}//END if released
		}//END outer if

		else if(event.type == SDL_CONTROLLERAXISMOTION)
		{

					//iterate over controllers to find which one we're going to use
					for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
					{
						if(event.caxis.which == (int)iterator->id)
						{
							if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
							{
								iterator->thumbLeftX = normalizeAxis(event.caxis.value, false);
							}
							else if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
							{
								iterator->thumbLeftY = normalizeAxis(event.caxis.value, false);
							}
							else if(event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
							{
								iterator->thumbRightX = normalizeAxis(event.caxis.value, false);
							}
							else if(event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
							{
								iterator->thumbRightY = normalizeAxis(event.caxis.value, false);
							}
							else if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
							{
								iterator->leftTrigger = normalizeAxis(event.caxis.value, true);
							}
							else if(event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
							{
								iterator->rightTrigger = normalizeAxis(event.caxis.value, true);
							}//END if decide axis
						}//END if ID's match
					}//END iterator
				}//END if controller axis motion

		else if(event.type == SDL_CONTROLLERDEVICEADDED)
		{
			//credit to Ryan C Gordon @ libsdl.org for clarification
			//which for this event == joystick index for added device
			//which for this event == instance id for the removed
			cout<<"controller added"<<endl;

			//Attempt to open controller
			SDL_GameController* gamePad 	= SDL_GameControllerOpen(event.cdevice.which);
			SDL_Joystick*		joystick 	= SDL_JoystickOpen(event.cdevice.which);
			bool				existingInstance = false;

			if(!gamePad || !joystick)
			{
				//do nothing for now, open failed
			}
			else
			{
				//iterate over all controllers and verify
				for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
				{
					//pick the right controller struct
					if((int)iterator->id == (int)SDL_JoystickInstanceID(joystick))
					{
						existingInstance = true;
					}//END if Id's match
				}//END iterator

				//if that controller isn't already in our list, add it to our list
				if(!existingInstance)
				{
					//SDL_JoystickInstanceID()
					deviceStruct device 		= buildControllerDevice(event.cdevice.which, SDL_GameControllerNameForIndex(event.cdevice.which), SDL_JoystickInstanceID(joystick));
					controllerStruct controller = buildController(device, event.cdevice.which, SDL_JoystickInstanceID(joystick));

					//credit to davidgow.net for partial input code
					controller.thumbLeftX 		= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX), false);
					controller.thumbLeftY		= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTY), false);
					controller.leftTrigger		= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_TRIGGERLEFT), true);
					controller.thumbRightX		= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_RIGHTX), false);
					controller.thumbRightY		= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_RIGHTY), false);
					controller.rightTrigger 	= normalizeAxis(SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), true);

					//add to lists for ellen
					et.controllers.push_back(controller);
					device.controllerIndex = et.controllers.size() - 1;
					et.detectedDevices.push_back(device);
				}//END if new device
			}//end if open gamePad/joystick successful

			//Now recompute the player indexes
			if(!existingInstance)
			{
				synchControllerDevices(et);

				//TODO: remove this
				produceLog(et);
			}
		}//END if controller device added

		else if(event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			cout<<"controller removed "<<event.cdevice.which<<endl;
			//which for this event == instance id for the removed

			//iterate over all controllers and find the one to purge
			for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
			{
				//pick the right controller struct
				if((int)iterator->id == event.cdevice.which)
				{
					//Remove the deviceStruct from devices
					for(list<deviceStruct>::iterator iteratorDevices = et.detectedDevices.begin(), end = et.detectedDevices.end(); iteratorDevices != end; ++iteratorDevices)
					{
						//make sure devices have the same name, same type as controllers, and same instance ID
						if
						(
								(iterator->superDevice.id_int == iteratorDevices->id_int) &&
								(iterator->superDevice.name   == iteratorDevices->name) &&
								(iterator->superDevice.deviceType == CONTROLLER_TYPE) &&
								(iteratorDevices->deviceType == CONTROLLER_TYPE)
						)
						{
							et.detectedDevices.erase(iteratorDevices);

							//due to double adding being prevented in CONTROLLERDEVICEADDED events,
							//we can break now since there should never be two devices with same instance id
							break;
						}
					}

					//Remove the controllerStruct from controllers
					et.controllers.erase(iterator);

					//due to double adding being prevented in CONTROLLERDEVICEADDED events,
					//we can break now since there should never be two devices with same instance id
					break;
				}//END if need to erase controllerStruct
			}//END iterator

			//Now recompute the player indexes
			synchControllerDevices(et);

			//TODO: remove this
			produceLog(et);
		}//END controller device removed

		else if(event.type == SDL_CONTROLLERDEVICEREMAPPED)
		{
			//NOTE: currently unused, but can handled stuff here if we decide to support remapping in-app  down the road
		}

		else if(event.type == SDL_JOYBUTTONDOWN)
		{
			if(event.jbutton.state == SDL_PRESSED)
			{

				//iterate over joysticks to find which one we're going to use
				for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
				{
					//pick the right controller struct
					if((event.jbutton.which == (int)iterator->id) && !SDL_IsGameController(event.jbutton.which))
					{
						//NOTE: not standardized therefore not 100% reliable
						iterator->buttons = pollButtons(iterator->buttons, event, false);

						//Handle triggers as buttons for Playstation-based SDL_Joysticks
						/*
						 * LT => button == 8
						 * RT => button == 9
						 */
						if(		(iterator->superDevice.name.find("Playstation") != std::string::npos)||
								(iterator->superDevice.name.find("PlayStation") != std::string::npos)||
								(iterator->superDevice.name.find("playstation") != std::string::npos)||
								(iterator->superDevice.name.find("Play Station") != std::string::npos)||
								(iterator->superDevice.name.find("play station") != std::string::npos)||
								(iterator->superDevice.name.find("PS1") != std::string::npos)||
								(iterator->superDevice.name.find("PSX") != std::string::npos)||
								(iterator->superDevice.name.find("PS2") != std::string::npos)||
								(iterator->superDevice.name.find("PS3") != std::string::npos)||
								(iterator->superDevice.name.find("Dualshock") != std::string::npos)||
								(iterator->superDevice.name.find("Dual Shock") != std::string::npos)||
								(iterator->superDevice.name.find("DualShock") != std::string::npos)||
								(iterator->superDevice.name.find("Sony") != std::string::npos)||
								(iterator->superDevice.name.find("sony") != std::string::npos)||
								(iterator->superDevice.name.find("Sixaxis") != std::string::npos)||
								(iterator->superDevice.name.find("SixAxis") != std::string::npos)||
								(iterator->superDevice.name.find("sixaxis") != std::string::npos)||
								(iterator->superDevice.name.find("Six Axis") != std::string::npos)||
								(iterator->superDevice.name.find("six axis")!= std::string::npos)
						)
						{
							if(event.jbutton.button == 8)
							{
								iterator->leftTrigger = 1.0f;
							}
							else if(event.jbutton.button == 9)
							{
								iterator->rightTrigger = 1.0f;
							}
						}//END if playstation device

					}//END if ID's match & is NOT a GameController
				}//END for
			}//END if pressed
		}//END outer if

		else if(event.type == SDL_JOYBUTTONUP)
		{
			if(event.jbutton.state == SDL_RELEASED)
			{
				//iterate over controllers to find which one we're going to use
				for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
				{
					//TODO: only search for TODO dont use list incase if you typed like TODOR
					if((event.jbutton.which == (int)iterator->id) && !SDL_IsGameController(event.jbutton.which))
					{
						//NOTE: not standardized therefore not 100% reliable
						iterator->buttons = pollButtons(iterator->buttons, event, false);

						//Handle triggers as buttons for Playstation-based SDL_Joysticks
						/*
						 * LT => button == 8
						 * RT => button == 9
						 */
						if(		(iterator->superDevice.name.find("Playstation") != std::string::npos)||
								(iterator->superDevice.name.find("PlayStation") != std::string::npos)||
								(iterator->superDevice.name.find("playstation") != std::string::npos)||
								(iterator->superDevice.name.find("Play Station") != std::string::npos)||
								(iterator->superDevice.name.find("play station") != std::string::npos)||
								(iterator->superDevice.name.find("PS1") != std::string::npos)||
								(iterator->superDevice.name.find("PSX") != std::string::npos)||
								(iterator->superDevice.name.find("PS2") != std::string::npos)||
								(iterator->superDevice.name.find("PS3") != std::string::npos)||
								(iterator->superDevice.name.find("Dualshock") != std::string::npos)||
								(iterator->superDevice.name.find("Dual Shock") != std::string::npos)||
								(iterator->superDevice.name.find("DualShock") != std::string::npos)||
								(iterator->superDevice.name.find("Sony") != std::string::npos)||
								(iterator->superDevice.name.find("sony") != std::string::npos)||
								(iterator->superDevice.name.find("Sixaxis") != std::string::npos)||
								(iterator->superDevice.name.find("SixAxis") != std::string::npos)||
								(iterator->superDevice.name.find("sixaxis") != std::string::npos)||
								(iterator->superDevice.name.find("Six Axis") != std::string::npos)||
								(iterator->superDevice.name.find("six axis")!= std::string::npos)
						)
						{
							if(event.jbutton.button == 8)
							{
								iterator->leftTrigger = 0.0f;
							}
							else if(event.jbutton.button == 9)
							{
								iterator->rightTrigger = 0.0f;
							}
						}//END if playstation device
					}//END if ID's match & is NOT a GameController
				}//END for
			}//END if released
		}//END outer if

		else if(event.type == SDL_JOYAXISMOTION)
		{

			//iterate over controllers to find which one we're going to use
			for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
			{
				//select the right controllerStruct
				if((event.jaxis.which == (int)iterator->id) && !SDL_IsGameController(event.jaxis.which))
				{
					//USE tested/assumed playstation axis mapping
					/*
					 * LY == 0
					 * LX == 1
					 * RX == 3
					 * RY == 2
					 */
					//TODO: replace this if statement with a function
					if(		(iterator->superDevice.name.find("Playstation") != std::string::npos)||
							(iterator->superDevice.name.find("PlayStation") != std::string::npos)||
							(iterator->superDevice.name.find("playstation") != std::string::npos)||
							(iterator->superDevice.name.find("Play Station") != std::string::npos)||
							(iterator->superDevice.name.find("play station") != std::string::npos)||
							(iterator->superDevice.name.find("PS1") != std::string::npos)||
							(iterator->superDevice.name.find("PSX") != std::string::npos)||
							(iterator->superDevice.name.find("PS2") != std::string::npos)||
							(iterator->superDevice.name.find("PS3") != std::string::npos)||
							(iterator->superDevice.name.find("Dualshock") != std::string::npos)||
							(iterator->superDevice.name.find("Dual Shock") != std::string::npos)||
							(iterator->superDevice.name.find("DualShock") != std::string::npos)||
							(iterator->superDevice.name.find("Sony") != std::string::npos)||
							(iterator->superDevice.name.find("sony") != std::string::npos)||
							(iterator->superDevice.name.find("Sixaxis") != std::string::npos)||
							(iterator->superDevice.name.find("SixAxis") != std::string::npos)||
							(iterator->superDevice.name.find("sixaxis") != std::string::npos)||
							(iterator->superDevice.name.find("Six Axis") != std::string::npos)||
							(iterator->superDevice.name.find("six axis")!= std::string::npos)
					)
					{
						if(event.jaxis.axis == 0)
						{
							iterator->thumbLeftY = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 1)
						{
							iterator->thumbLeftX = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 2)
						{
							iterator->thumbRightY = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 3)
						{
							iterator->thumbRightX = normalizeAxis(event.jaxis.value, false);
						}
					}//END if PS device

					//use Xbox-based mapping
					/*
					 * LX == 0
					 * LY == 1
					 * LT == 2
					 * RX == 3
					 * RY == 4
					 * RT == 5
					 */
					else
					{
						if(event.jaxis.axis == 0)
						{
							iterator->thumbLeftX = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 1)
						{
							iterator->thumbLeftY = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 2)
						{
							//Triggers are not 0 to +32K in SDL_Joystick devices
							iterator->leftTrigger = (float) (( ( (event.jaxis.value - OLD_THUMB_MIN) * NEW_TRIGGER_RANGE)/OLD_THUMB_RANGE) + NEW_TRIGGER_MIN);
						}
						else if(event.jaxis.axis == 3)
						{
							iterator->thumbRightX = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 4)
						{
							iterator->thumbRightY = normalizeAxis(event.jaxis.value, false);
						}
						else if(event.jaxis.axis == 5)
						{
							iterator->rightTrigger = (float) (( ( (event.jaxis.value - OLD_THUMB_MIN) * NEW_TRIGGER_RANGE)/OLD_THUMB_RANGE) + NEW_TRIGGER_MIN);
						}//END if axis selection
					}//END if Xbox controller
				}//END if ID's match
			}//END iterator
		}//END if joystick axis motion event

		else if(event.type == SDL_JOYHATMOTION)
		{
			//iterate over controllers to select correct struct
			for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
			{
				//if the controller ID's match
				if((event.jhat.which == (int)iterator->id) && !SDL_IsGameController(event.jaxis.which))
				{
					if(event.jhat.value== SDL_HAT_LEFTUP)
					{
						iterator->buttons |= UP_DPAD;
						iterator->buttons |= LEFT_DPAD;

					}
					else if(event.jhat.value== SDL_HAT_UP)
					{
						iterator->buttons |= UP_DPAD;
					}
					else if(event.jhat.value== SDL_HAT_RIGHTUP)
					{
						iterator->buttons |= RIGHT_DPAD;
						iterator->buttons |= UP_DPAD;
					}
					else if(event.jhat.value== SDL_HAT_LEFT)
					{
						iterator->buttons |= LEFT_DPAD;
					}
					else if(event.jhat.value== SDL_HAT_CENTERED)
					{
						if((iterator->buttons & UP_DPAD) == UP_DPAD)
						{
							iterator->buttons -= UP_DPAD;
						}
						if((iterator->buttons & DOWN_DPAD) == DOWN_DPAD)
						{
							iterator->buttons -= DOWN_DPAD;
						}
						if((iterator->buttons & LEFT_DPAD) == LEFT_DPAD)
						{
							iterator->buttons -= LEFT_DPAD;
						}
						if((iterator->buttons & RIGHT_DPAD) == RIGHT_DPAD)
						{
							iterator->buttons -= RIGHT_DPAD;
						}
					}
					else if(event.jhat.value == SDL_HAT_RIGHT)
					{
						iterator->buttons |= RIGHT_DPAD;
					}
					else if(event.jhat.value == SDL_HAT_LEFTDOWN)
					{
						iterator->buttons |= LEFT_DPAD;
						iterator->buttons |= DOWN_DPAD;
					}
					else if(event.jhat.value == SDL_HAT_DOWN)
					{
						iterator->buttons |= DOWN_DPAD;
					}
					else if(event.jhat.value == SDL_HAT_RIGHTDOWN)
					{
						iterator->buttons |= RIGHT_DPAD;
						iterator->buttons |= DOWN_DPAD;
					}
					else if(event.jhat.value  == 0)
					{
						if((iterator->buttons & UP_DPAD) == UP_DPAD)
						{
							iterator->buttons -= UP_DPAD;
						}
						if((iterator->buttons & DOWN_DPAD) == DOWN_DPAD)
						{
							iterator->buttons -= DOWN_DPAD;
						}
						if((iterator->buttons & LEFT_DPAD) == LEFT_DPAD)
						{
							iterator->buttons -= LEFT_DPAD;
						}
						if((iterator->buttons & RIGHT_DPAD) == RIGHT_DPAD)
						{
							iterator->buttons -= RIGHT_DPAD;
						}
					}//END inner if
				}//END if controllers match
			}//END iterator
		}//END if SDL Joystick Hat Event

		else if(event.type == SDL_JOYDEVICEADDED)
		{
			cout<<"joystick added"<<endl;
		}

		else if(event.type == SDL_JOYDEVICEREMOVED)
		{
			cout<<"joystick removed"<<endl;
		}

	}//END WHILE SDL_PollEvent
}//END Poller

//set a buttons field for a given event
//controller.buttons mapping
/*
 * 1st Byte: (Right)(Left)(Down)(Up)
 * 2nd Byte: (Right Thumbstick)(Left Thumbstick)(Back)(Start)
 * 3rd Byte: (Don't Care)(Home)(Right Bumper)(Left Bumper)
 * 4th Byte: (Y)(X)(B)(A)
 */
uint16_t pollButtons(uint16_t buttons, SDL_Event event, bool isGameController)
{
	//Variable Declaration
	int button;
	Uint8 state;

	//If isGameController flag is set, we need to use the cbutton member variable of the event
	//If the flag is false, we need to use the jbutton member variable of the event
	if(isGameController)
	{
		button 	= event.cbutton.button;
		state	= event.cbutton.state;
	}
	else if (!isGameController)
	{
		button 	= event.jbutton.button;
		state	= event.jbutton.state;
	}


	if(button == SDL_CONTROLLER_BUTTON_A)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= A_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & A_BUTTON) == A_BUTTON))
		{
			buttons -= A_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_B)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= B_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & B_BUTTON) == B_BUTTON))
		{
			buttons -= B_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_X)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= X_BUTTON;
		}
		else if(event.cbutton.state == SDL_RELEASED && ((buttons & X_BUTTON)== X_BUTTON))
		{
			buttons -= X_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_Y)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= Y_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & Y_BUTTON) == Y_BUTTON))
		{
			buttons -= Y_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_DPAD_UP)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= UP_DPAD;
		}
		else if(state == SDL_RELEASED && ((buttons & UP_DPAD) == UP_DPAD))
		{
			buttons -= UP_DPAD;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= DOWN_DPAD;
		}
		else if(state == SDL_RELEASED && ((buttons & DOWN_DPAD) == DOWN_DPAD))
		{
			buttons -= DOWN_DPAD;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= LEFT_DPAD;
		}
		else if(state == SDL_RELEASED && ((buttons & LEFT_DPAD)== LEFT_DPAD))
		{
			buttons -= LEFT_DPAD;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= RIGHT_DPAD;
		}
		else if(state == SDL_RELEASED && ((buttons & RIGHT_DPAD) == RIGHT_DPAD))
		{
			buttons -= RIGHT_DPAD;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_START)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= START_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & START_BUTTON) == START_BUTTON))
		{
			buttons -= START_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_BACK)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= SELECT_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & SELECT_BUTTON) == SELECT_BUTTON))
		{
			buttons -= SELECT_BUTTON;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= LEFT_THUMB;
		}
		else if(state == SDL_RELEASED && ((buttons & LEFT_THUMB) == LEFT_THUMB))
		{
			buttons -= LEFT_THUMB;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= RIGHT_THUMB;
		}
		else if(state == SDL_RELEASED && ((buttons & RIGHT_THUMB) == RIGHT_THUMB))
		{
			buttons -= RIGHT_THUMB;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= LEFT_SHOULDER;
		}
		else if(state == SDL_RELEASED && ((buttons & LEFT_SHOULDER) == LEFT_SHOULDER))
		{
			buttons -= LEFT_SHOULDER;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= RIGHT_SHOULDER;
		}
		else if(state == SDL_RELEASED && ((buttons & RIGHT_SHOULDER) == RIGHT_SHOULDER))
		{
			buttons -= RIGHT_SHOULDER;
		}
	}
	else if(button == SDL_CONTROLLER_BUTTON_GUIDE)
	{
		if(state == SDL_PRESSED)
		{
			buttons |= HOME_BUTTON;
		}
		else if(state == SDL_RELEASED && ((buttons & HOME_BUTTON) == HOME_BUTTON))
		{
			buttons -= HOME_BUTTON;
		}
	}//END IF

	//return new buttons mask
	return buttons;
}//END poll buttons

//Normalize a polled axis
float normalizeAxis(float oldAxisValue, bool isTrigger)
{
	//Variable Declaration
	float newAxisValue;

	if(isTrigger)
	{
		newAxisValue = (float) (( ( (oldAxisValue - OLD_TRIGGER_MIN) * NEW_TRIGGER_RANGE)/OLD_TRIGGER_RANGE) + NEW_TRIGGER_MIN);
	}
	else
	{
		newAxisValue = (float) (( ( (oldAxisValue - OLD_THUMB_MIN) * NEW_THUMB_RANGE)/OLD_THUMB_RANGE) + NEW_THUMB_MIN);
	}

	//return
	return newAxisValue;
}

//TODO: fill this in
bool isPSX(std::string gamepadName)
{
	return false;
}

void synchControllerDevices(struct cylonStruct& et)
{
	//Should not change until next event, but capture now to be safe
	int numJoysticks = SDL_NumJoysticks();

	//iterate over all SDL_Joysticks via index value
	for(int i = 0; i < numJoysticks; i++)
	{
		//Open SDL_Joystick on that index
		SDL_Joystick* joystick = SDL_JoystickOpen(i);

		if(!joystick)
		{
			//set the error code in the cylonStruct if SDL_Joystick API fails
			et.error = CONTROLLERS_LIST_ID_SYNCH_ERROR;
		}

		else
		{
			//Grab the instance ID
			int joystickInstanceID = SDL_JoystickInstanceID(joystick);
			if (joystickInstanceID < 0)
			{
				et.error = CONTROLLERS_LIST_ID_SYNCH_ERROR;
				return;
			}

			//For synching controllerIndex in detectedDevices;
			int controllersIndex = 0;

			//iterate over all controllerStructs
			for(list<controllerStruct>::iterator iterator = et.controllers.begin(), end = et.controllers.end(); iterator != end; ++iterator)
			{
				if((int)joystickInstanceID == (int)iterator->id)
				{
					// set the userIndex (player number) to that of the current joystick
					iterator->userIndex = i;

					//set the superDevice's controllerIndex to that of the current position in the controllers list, making them synched
					iterator->superDevice.controllerIndex = controllersIndex;

					//TODO: check what else has its superDevice modified

				}//END if match

				//iterate over all deviceStructs
				for(list<deviceStruct>::iterator devicerator = et.detectedDevices.begin(), end = et.detectedDevices.end(); devicerator != end; ++devicerator)
				{
					//If the device is a controller type device corresponding to the SDL_Joystick with the same instance ID
					if( ((int) iterator->id == (int)devicerator->id_int) && devicerator->deviceType == CONTROLLER_TYPE)
					{
						devicerator->controllerIndex = controllersIndex;
					}//END if mathc
				}//END for all deviceStructs

				//increment controllersIndex tracker
				controllersIndex++;
			}//END for all controllerStructs
		}//END if joystick opened correctly
	}//END for all Joystick index vals
}//END Synchronize method
//END Controllers
