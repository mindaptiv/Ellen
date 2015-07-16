//Ellen.cpp
//Implements definitions of Ellen functionality
//"We wanted to end the cycle of war between man and machine," - Ellen Tigh
//josh@mindaptiv.com

//includes
#include "Ellen.h"
#include <string>
#include <iostream>
#include <dlfcn.h>

//Producers
void produceUsername(struct cylonStruct& et)
{
	//attempt to open library
	void* handle = dlopen("libc.so.6", RTLD_LAZY);

	//if failed access to libc
	if(!handle)
	{
		//LOG
		std::cout<<"LIBC failed to open\n";

		//TODO set name to default?
	}
	else
	{
		//LOG
		std::cout<<"LIBC successfully loaded\n";

		//clear errors
		dlerror();

		//define method type
		typedef char* (*getLogin_t)();

		//try to load getlogin()
		getLogin_t getLogin = (getLogin_t) dlsym(handle, "getlogin");
		char* error = dlerror();

		if (error)
		{
			//LOG
			std::cout<<"Could not find getLogin()\n";

			//TODO set default name?
		}
		else
		{
			//LOG
			std::cout<<"Found getlogin()!\n";

			//Call getlogin()
			char* charStarName = getLogin();

			if(charStarName != NULL)
			{
				//Set username
				std::string uname(charStarName);
				et.username = uname;
			}
			else
			{
				//username null, set to default
				et.username = "0";
			}


		}//END else found getlogin

		dlclose(handle);
	}//END else lib loaded
	std::cout<<et.username + "\n";
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

}//END produceProcessorInfo

void produceMemoryInfo(struct cylonStruct& et)
{

}//END produceMemoryInfo

void produceDeviceInfo(struct cylonStruct& et)
{

}//END produceDeviceInfo

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
