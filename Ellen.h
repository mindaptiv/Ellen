/*
 * ellen.h
 *
 *  Created on: Jul 16, 2015
 *      Author: mindmin
 */

#ifndef ELLEN_H_
#define ELLEN_H_


//includes
#include "Cylon.h"

//Method Declaration
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
