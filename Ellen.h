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
	char*  funcName;
	void*  funcAddr;
};

//Dynamically linked library
struct dynLib
{
	char*  libName;
	int    versionNumber;
	void*  libAddr;
	libFunc* functions; //TODO: static note?
};

enum libraries
{
	error,
	libc,
	libCount
};
//END structs

//Variables
extern dynLib allLibs[libCount];
//END Variables

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
