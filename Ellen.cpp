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

}//END produceDeviceName

void produceAccountPicture(struct cylonStruct& et)
{

}//END produceAccountPicture

void produceDateTime(struct cylonStruct& et)
{

}//END produceTimeZone

void produceTimeZone(struct cylonStruct& et)
{

}//END produceDateTime

void produceProcessorInfo(struct cylonStruct& et)
{

} //END produceProcessorInfo

void produceMemoryInfo(struct cylonStruct& et)
{

} //END produceMemoryInfo

void produceDeviceInfo(struct cylonStruct& et)
{

} //END produceDeviceInfo

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
	produceDateTime(ellen);
	produceTimeZone(ellen);
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
