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
#include <string>

using namespace std;

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

		register struct passwd *pw;
		register uid_t uid;

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
