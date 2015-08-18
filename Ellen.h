/*
 * ellen.h
 *
 *  Created on: Jul 16, 2015
 *      Author: josh@mindaptiv.com
 */

#pragma once

#ifndef ELLEN_H_
#define ELLEN_H_

//includes
//custom
#include "Cylon.h"


//included in Linux Standard Base
#include <stdio.h>  		//libc
#include <pwd.h>    		//libc
#include <unistd.h> 		//libc
#include <cstring>  		//libc
#include <ctime>    		//libc
#include <sys/sysinfo.h>	//libc
#include <sys/utsname.h>    //libc
#include <sys/stat.h>		//libc
#include <stdlib.h>         //libc
#include <sstream>			//libstdc++
#include <iostream> 		//libstdc++
#include <iomanip>
#include <fstream>			//libstdc++
#include <limits.h>			//libstdc++

//other
#include <dlfcn.h>
#include <libusb-1.0/libusb.h>

//Constants
//path to profile image
static const std::string AVATAR_PATH = "/var/lib/AccountsService/icons/";
static const std::string AVATAR_TYPE = ".png";

//Error values for missing/invalid/forbidden data fields
static const std::string ERROR_STRING 	= "0";
static const int ERROR_INT 				= 0;

//Number of versions beyond "Latest Version" of a library to check for, in case if a library updates after release
static const int NUMBER_OF_VERSIONS_TO_LOOK_FORWARD = 5;

//The number of methods to load for a given library
static const int LIBUSB_FUNCTION_COUNT 					= 6;
static const int LIBUSB_LATEST_VERSION 					= 0;
static const char* LIBUSB_LIB_NAME 						= "libusb.so";//"x86_64-linux-gnu/libusb.so";
static const char* LIBUSB_INIT 							= "usb_init";
static const char* LIBUSB_GET_DEVICE_LIST 				= "usb_get_device_list";
static const char* LIBUSB_GET_DEVICE_DESCRIPTOR 		= "usb_get_device_descriptor";
static const char* LIBUSB_GET_ACTIVE_CONFIG_DESCRIPTOR 	= "usb_get_active_config_descriptor";
static const char* LIBUSB_FREE_DEVICE_LIST 				= "usb_free_device_list";
static const char* LIBUSB_EXIT 							= "usb_exit";

//FUNCTIONS TO CALL
/*
 * libusb_init(&context)
 * libusb_get_device_list(context, &devices)
 * libusb_get_device_descriptor(device, &descriptor)
 * libusb_get_active_config_descriptor(device, &config)
 * libusb_free_device_list(devices, 1)
 * libusb_exit(context)
 *
 */


static const int LIBC_FUNCTION_COUNT = 4;
static const int LIBC_LATEST_VERSION = 6;
static const char* LIBC_LIB_NAME = "libc.so";
static const char* LIBC_GETEUID = "geteuid";

//Structs
//Function loaded from library
struct libFunc
{
	const char*  funcName;
	void*  funcAddr;
};

//Dynamically linked library
struct dynLib
{
	const char*  libName;
	void*  libAddr;
	int    versionNumber;
	int    funcCount;
	libFunc* functions;
};

enum libraries
{
//	libc,
	libusb,
	libCount
};
//END structs

//Method Declaration
//Dynamic Linking
void fillTable();
void openLibs();
void closeLibs();

//Producers
void produceUserProfile(struct cylonStruct& et);
void produceDeviceName(struct cylonStruct& et);
void produceDateTimeZone(struct cylonStruct& et);
void produceTimeZone(struct cylonStruct& et);
void produceProcessorInfo(struct cylonStruct& et);
void produceMemoryInfo(struct cylonStruct & et);
void produceDeviceInfo(struct cylonStruct& et);
void produceUsbDeviceInfo(struct cylonStruct& et);
void produceLog(struct cylonStruct& et);

//Builders
struct cylonStruct buildEllen();
struct deviceStruct buildBlankDevice();
struct deviceStruct buildUsbDevice(struct libusb_device*);
struct deviceStruct buildUsbDevice(struct libusb_device*, struct libusb_device_descriptor descriptor);
struct deviceStruct buildUsbDevice(struct libusb_device* device, struct libusb_device_descriptor descriptor, int interfaceClass);

#endif /* ELLEN_H_ */
