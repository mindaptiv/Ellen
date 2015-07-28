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
#include <stdio.h> //libc
#include <pwd.h>   //libc
#include <unistd.h> //libc
#include <cstring>

#include <iostream> //libstdc++

//other
#include <dlfcn.h>
#include <sstream>

//Constants
//Error values for missing/invalid/forbidden data fields
static const std::string ERROR_STRING 	= "0";
static const int ERROR_INT 				= 0;

//Number of versions beyond "Latest Version" of a library to check for, in case if a library updates after release
static const int NUMBER_OF_VERSIONS_TO_LOOK_FORWARD = 5;

//The number of methods to load for a given library
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
	libc,
	libCount
};
//END structs

//Method Declaration
//Dynamic Linking
void fillTable();
void openLibs();
void closeLibs();

//Producers
void produceUsername(struct cylonStruct& et);
void produceDeviceName(struct cylonStruct& et);
void produceAccountPicture(struct cylonStruct& et);
void produceDateTime(struct cylonStruct& et);
void produceTimeZone(struct cylonStruct& et);
void produceProcessorInfo(struct cylonStruct& et);
void produceMemoryInfo(struct cylonStruct& et);
void produceDeviceInfo(struct cylonStruct& et);

//Builders
struct cylonStruct buildEllen();
struct deviceStruct buildDevice();


#endif /* ELLEN_H_ */
