//Ellen.cpp
//Implements definitions of Ellen functionality
//"We wanted to end the cycle of war between man and machine," - Ellen Tigh
//josh@mindaptiv.com

//includes
#include "Ellen.h"
#include <iostream>
#include <dlfcn.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

//Dyanamic Library Stuff
//Constant for how many versions in the future we look forward for checking lib #'s
int constant = 5;

void fillTable()
{

}

void openLibs()
{
	for (int i = 0; i< libCount; i++)
	{
		//Grab the current library
		//TODO: wrong?
		dynLib* current = allLibs[i];

		//Iterate over versions until we find one that successfully dlopens
		for (int j = current->versionNumber + constant; j >= 0; j++)
		{
			//attempt dlopen
			current->libAddr = dlopen(current->libName, RTLD_LAZY);

			if(current != NULL)
			{
				//Grab the functions
				libFunc* functions = current->functions;

				/*
				for (int k = 0; TODO: what goes here?/wrong?; k++ )
				{
					functions[j] = dlsym(current->libAddr, functions[j].funcName);
				}//END function grabbing for
				*/

				//successfully grabbed lib and functions
				break;
			}//end if lib loaded successfully
		}//END inner if
	}//END outer if
}//END func

void closeLibs()
{
	//iterate through allLibs
	for (int i = 0; i < libCount; i++)
	{
		//grab current library
		dynLib* current = allLibs[i];

		//close it
		int close = dlclose(current->libAddr);

		if (close != 0)
		{
			//TODO: handle failure to close shared library
		}//END if
	}//END for
}//END method


//END DYLIB stuff


//Producers
void produceUsername(struct cylonStruct& et)
{


	//attempt to open library
	void* handle = dlopen("libc.so.6", RTLD_LAZY);

	//if failed access to libc
	if(!handle)
	{
		//LOG
		cout<<"LIBC failed to open\n";

		//TODO set name to default?
	}
	else
	{
		//LOG
		cout<<"LIBC successfully loaded"<<endl;

		//clear errors
		dlerror();

		struct passwd* pw;
	    uid_t uid;

		uid = geteuid();
		pw = getpwuid(uid);
		if (pw)
		{
			const char* charUname = pw->pw_name;
			string strUname(charUname);
			et.username = strUname;
			cout<<et.username<<endl;
		}//END if pw
	}//END elif !handle

	dlclose(handle);
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

	//producers
	produceUsername(ellen);
	produceDeviceName(ellen);
	produceDateTime(ellen);
	produceTimeZone(ellen);
	produceProcessorInfo(ellen);
	produceMemoryInfo(ellen);
	produceAccountPicture(ellen);
	produceDeviceInfo(ellen);

	//return
	return ellen;
}//END buildEllen

//END builders
