//Ellen.cpp
//Implements definitions of Ellen functionality
//"We wanted to end the cycle of war between man and machine," - Ellen Tigh
//josh@mindaptiv.com

//includes
//custom
#include "Ellen.h"

using namespace std;


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
void produceUsername(struct cylonStruct& et)
{
	struct passwd* pw;
	uid_t uid;

	uid = geteuid();
	pw = getpwuid(uid);
	if (pw)
	{
		const char* charUname = pw->pw_name;
		string strUname(charUname);
		et.username = strUname;
	}//END if pw
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

void produceAccountPicture(struct cylonStruct& et)
{

}//END produceAccountPicture

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
	//Credit to David R. Nadeau @ nadeausoftware.com for partial method code
	//Variable Declaration
	struct sysinfo info;
	int byteConverter = 1024;
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
} //END produceProcessorInfo

void produceMemoryInfo(struct cylonStruct& et)
{

} //END produceMemoryInfo

void produceDeviceInfo(struct cylonStruct& et)
{

} //END produceDeviceInfo

void produceLog(struct cylonStruct& et)
{

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
	produceUsername(ellen);
	produceDeviceName(ellen);
	produceDateTimeZone(ellen);
	produceProcessorInfo(ellen);
	produceMemoryInfo(ellen);
	produceAccountPicture(ellen);
	produceDeviceInfo(ellen);

	//close libs
	closeLibs();

	//return
	return ellen;
}//END buildEllen
//END builders
