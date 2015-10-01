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
#include <SDL2/SDL.h>			//libsdl

//controllerStruct thumbsticks
static const float OLD_THUMB_MIN = -32768.0;
static const float OLD_THUMB_MAX =  32767.0;
static const float OLD_THUMB_RANGE = (OLD_THUMB_MAX - OLD_THUMB_MIN);
static const float NEW_THUMB_MIN = -1.0;
static const float NEW_THUMB_MAX = 	1.0;
static const float NEW_THUMB_RANGE = (NEW_THUMB_MAX - NEW_THUMB_MIN);

//controllerStruct triggers
static const float OLD_TRIGGER_MIN = 0.0;
static const float OLD_TRIGGER_MAX = 32767.0;
static const float OLD_TRIGGER_RANGE = (OLD_TRIGGER_MAX - OLD_TRIGGER_MIN);
static const float NEW_TRIGGER_MIN = 0.0;
static const float NEW_TRIGGER_MAX = 1.0;
static const float NEW_TRIGGER_RANGE = NEW_TRIGGER_MAX - NEW_TRIGGER_MIN;


//credit to xc3sprog for partial dynamic libusb code ideas
typedef int 	(*libusb_init_t)(libusb_context** context);
typedef void 	(*libusb_exit_t)(libusb_context* context);
typedef ssize_t (*libusb_get_device_list_t)(libusb_context* context, libusb_device*** list);
typedef void    (*libusb_free_device_list_t)(libusb_device** list, int unref_devices);
typedef int 	(*libusb_get_device_descriptor_t)(libusb_device* device, libusb_device_descriptor* descriptor);
typedef int 	(*libusb_get_active_config_descriptor_t)(libusb_device* device, libusb_config_descriptor** config);

//SDL function types
typedef SDL_bool 			(*SDL_IsGameController_t)(int joystick_index);
typedef int 				(*SDL_NumJoysticks_t)(void);
typedef SDL_Joystick* 		(*SDL_JoystickOpen_t)(int device_index);
typedef SDL_GameController* (*SDL_GameControllerOpen_t)(int joystick_index);
typedef const char*			(*SDL_GameControllerName_t)(SDL_GameController* gamecontroller);
typedef SDL_JoystickID		(*SDL_JoystickInstanceID_t)(SDL_Joystick* joystick);
typedef Sint16				(*SDL_GameControllerGetAxis_t)(SDL_GameController* gamecontroller, SDL_GameControllerAxis axis);
typedef int					(*SDL_PollEvent_t)(SDL_Event* event);
typedef const char*			(*SDL_GameControllerNameForIndex_t)(int joystick_index);
typedef const char*			(*SDL_JoystickNameForIndex_t)(int device_index);
typedef Sint16				(*SDL_JoystickGetAxis_t)(SDL_Joystick* joystick, int axis);
typedef SDL_bool			(*SDL_SetHint_t)(const char* name, const char* value);
typedef SDL_Window*			(*SDL_CreateWindow_t)(const char* title, int x, int y, int w, int h, Uint32 flags);
typedef	int					(*SDL_Init_t)(Uint32 flags);
typedef int					(*SDL_GL_SetAttribute_t)(SDL_GLattr attr, int value);
typedef const char*			(*SDL_JoystickName_t)(SDL_Joystick* joystick);

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
static const char* SDL_JOYSTICKOPEN						= "SDL_JoystickOpen";
static const char* SDL_GAMECONTROLLEROPEN				= "SDL_GameControllerOpen";
static const char* SDL_GAMECONTROLLERNAME				= "SDL_GameControllerName";
static const char* SDL_JOYSTICKINSTANCEID				= "SDL_JoystickInstanceID";
static const char* SDL_GAMECONTROLLERGETAPPS			= "SDL_GameControllerGetAxis";
static const char* SDL_POLLEVENT						= "SDL_PollEvent";
static const char* SDL_GAMECONTROLLERNAMEFORINDEX		= "SDL_GameControllerNameForIndex";
static const char* SDL_JOYSTICKNAMEFORINDEX				= "SDL_JoystickNameForIndex";
static const char* SDL_JOYSTICKGETAXIS					= "SDL_JoystickGetAxis";
static const char* SDL_SETHINT							= "SDL_SetHint";
static const char* SDL_CREATEWINDOW						= "SDL_CreateWindow";
static const char* SDL_INIT								= "SDL_Init";
static const char* SDL_GL_SETATTRIBUTE					= "SDL_GL_SetAttribute";
static const char* SDL_JOYSTICKNAME						= "SDL_JoystickName";

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
	SDL_JoystickOpen_e,
	SDL_GameControllerOpen_e,
	SDL_GameControllerName_e,
	SDL_JoystickInstanceID_e,
	SDL_GameControllerGetAxis_e,
	SDL_PollEvent_e,
	SDL_GameControllerNameForIndex_e,
	SDL_JoystickNameForIndex_e,
	SDL_JoystickGetAxis_e,
	SDL_SetHint_e,
	SDL_CreateWindow_e,
	SDL_Init_e,
	SDL_JoystickName_e,
	SDL_GL_SetAttribute_e,
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
void produceControllerInfo(struct cylonStruct& et);
void produceDisplayInfo(struct cylonStruct& et);
void produceLog(struct cylonStruct& et);

//Builders
struct cylonStruct buildEllen();
struct deviceStruct buildBlankDevice();
struct deviceStruct buildUsbDevice(struct libusb_device*);
struct deviceStruct buildUsbDevice(struct libusb_device*, struct libusb_device_descriptor descriptor);
struct deviceStruct buildUsbDevice(struct libusb_device* device, struct libusb_device_descriptor descriptor, int interfaceClass);
struct deviceStruct buildControllerDevice(int index, const char* deviceName, int instanceID);
struct deviceStruct buildDisplayDevice(const char* displayName, int i);
struct controllerStruct buildBlankController();
struct controllerStruct buildController(deviceStruct device, int index, int id);
struct displayStruct buildBlankDisplay();
struct displayStruct buildDisplay(struct deviceStruct device, int i);

//Controller methods
void sdlInit();
void pollControllerEvents(struct cylonStruct& et);
uint16_t pollButtons(uint16_t buttons, SDL_Event event, bool isGameController);
float normalizeAxis(float oldAxisValue, bool isTrigger);
bool isPSX(std::string gamepadName);
void synchControllerDevices(struct cylonStruct& et);

#endif /* ELLEN_H_ */
