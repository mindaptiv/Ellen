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
#include <iomanip>			//libstdc++
#include <fstream>			//libstdc++
#include <limits.h>			//libstdc++

//other
#include <dlfcn.h>				//libdl
#include <libusb-1.0/libusb.h>	//libusb

//credit to xc3sprog for partial dynamic libusb code ideas
typedef int 	(*libusb_init_t)(libusb_context** context);
typedef void 	(*libusb_exit_t)(libusb_context* context);
typedef ssize_t (*libusb_get_device_list_t)(libusb_context* context, libusb_device*** list);
typedef void    (*libusb_free_device_list_t)(libusb_device** list, int unref_devices);
typedef int 	(*libusb_get_device_descriptor_t)(libusb_device* device, libusb_device_descriptor* descriptor);
typedef int 	(*libusb_get_active_config_descriptor_t)(libusb_device* device, libusb_config_descriptor** config);


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
static const int LIBUSB_LATEST_VERSION 					= 0;
static const char* LIBUSB_LIB_NAME 						= /*"libusb.so";*/ "libusb-1.0.so.0.1.0";
static const char* LIBUSB_INIT 							= "libusb_init";
static const char* LIBUSB_GET_DEVICE_LIST 				= "libusb_get_device_list";
static const char* LIBUSB_GET_DEVICE_DESCRIPTOR 		= "libusb_get_device_descriptor";
static const char* LIBUSB_GET_ACTIVE_CONFIG_DESCRIPTOR 	= "libusb_get_active_config_descriptor";
static const char* LIBUSB_FREE_DEVICE_LIST 				= "libusb_free_device_list";
static const char* LIBUSB_EXIT 							= "libusb_exit";

static const int LIBSDL_LATEST_VERSION					= 0;
static const char* LIBSDL_LIB_NAME						= "libSDL2-2.0.so.0.2.0";
static const char* SDL_NUMJOYSTICKS						= "SDL_NumJoysticks";
static const char* SDL_ISGAMECONTROLLER					= "SDL_IsGameController";

//static const char*

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
	bool	opened;
};

enum libraries
{
//	libusb,
	libsdl,
	libusb,
	libCount
};

enum usbFunctions
{
	libusb_init_e,
	libusb_get_device_list_e,
	libusb_get_device_descriptor_e,
	libusb_get_active_config_descriptor_e,
	libusb_free_device_list_e,
	libusb_exit_e,
	libusbCount
};

enum sdlFunctions
{
	SDL_NumJoysticks_e,
	SDL_IsGameController_e,
	libsdlCount
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
