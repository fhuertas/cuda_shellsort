/* 
 * File:   openClUtilities.h
 * Author: paco
 *
 * Created on 29 de mayo de 2012, 17:22
 */

#ifndef OPENCLUTILITIES_H
#define	OPENCLUTILITIES_H

#define SELECTED_PLATFORM_VENDOR "NVIDIA corporation"
#define SELECTED_PLATFORM_NAME "NVIDIA CUDA"
#define SELECTED_DEVICE_NAME "NVS 3100M"

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define DEBUG

#ifdef	__cplusplus
extern "C" {
#endif
void selectPlatformAndDevice(const char* platform_name, const char* device_name, cl_platform_id *platform_id, cl_device_id *device_id);


/**
 * Read the code of the a file. 
 * the memory is assigned dinamicaly 
 */
char* readCode(const char* fileName);




#ifdef	__cplusplus
}
#endif

#endif	/* OPENCLUTILITIES_H */

