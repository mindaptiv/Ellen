/*
 * ellen.h
 *
 *  Created on: Jul 16, 2015
 *      Author: josh@mindaptiv.com
 */

#ifndef ELLEN_H_
#define ELLEN_H_

//includes
#include "Cylon.h"
#include <string>

using namespace std;

//Structs
//Function loaded from library
struct libFunc
{
	string funcName;
	void*  funcAddr;
};

//Dynamically linked library
struct dynLib
{
	string libName;
	int    versionNumber;
	void*  libAddr;
	libFunc* functions; //TODO: static note?
};

enum libraries
{
	error,
	libc,
	count,
};
//END structs

//Variables
dynLib allLibs[libraries::count];
//END Variables

//Method Declaration
//Dynamic Linking
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
