#include "openClUtilities.h"



void selectPlatformAndDevice(const char* platform_name, const char* device_name, cl_platform_id *platform_id, cl_device_id *device_id){

    cl_uint maxPlatforms = 0;
    cl_uint num_platforms = 0;
    while (num_platforms == maxPlatforms) {
        int prev_platforms = maxPlatforms;
        maxPlatforms += 10;
        cl_platform_id platformIDs[maxPlatforms];
        clGetPlatformIDs(maxPlatforms, platformIDs, &num_platforms);
        for (int i = prev_platforms; i < prev_platforms + num_platforms; i++) {
            size_t wordSize = strlen(platform_name)+1;
            char currentPlatformName[wordSize];
            char *pchar;
            pchar = currentPlatformName;
            clGetPlatformInfo(platformIDs[i], CL_PLATFORM_NAME, wordSize, pchar, NULL);
#ifdef DEBUG
            printf("Checking Platform:%s==%s\n", platform_name, pchar);
#endif
            if (strcmp(platform_name, currentPlatformName) == 0) {
                *platform_id = platformIDs[i];
                cl_uint maxDevices = 0;
                cl_uint numDevices = 0;
                while (numDevices == maxDevices) {
                    int prev_devices = maxDevices;
                    maxDevices += 10;
                    cl_device_id deviceIDs[maxDevices];
                    clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, maxDevices, deviceIDs, &numDevices);
                    for (int j = prev_devices; j < prev_devices + numDevices; j++) {
                        wordSize = strlen(device_name)+1;
                        char currentDeviceName[wordSize];
                        //pchar = currentDeviceName;
                        clGetDeviceInfo(deviceIDs[j], CL_DEVICE_NAME, wordSize, currentDeviceName, NULL);
#ifdef DEBUG
                        cl_uint m;
                        clGetDeviceInfo(deviceIDs[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &m, NULL);
                        printf("Checking Device:%s==%s\n", device_name, currentDeviceName);
                        printf("Checking CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:%d\n", m);
#endif
                        if (strcmp(device_name, currentDeviceName) == 0) {
#ifdef DEBUG
                        printf("Finded platform=%s;device=%s\n", currentPlatformName, currentDeviceName);
#endif
                            *device_id = deviceIDs[j];
                            return;
                        }
                    }
                }
            }
        }
    }
}

#define BUFFER_SIZE 1024

char* readCode(const char* fileName) {
    FILE *fil = fopen(fileName, "r");
    char *code = NULL;
    int codeSize = 0;
    while (!feof(fil)) {
        char buffer[BUFFER_SIZE];
        size_t sizeReaded = fread(buffer, sizeof (char), BUFFER_SIZE, fil);
        if (code == NULL) {
            code = (char*) malloc(sizeof (char) * sizeReaded);
            strncpy(code, buffer, sizeReaded);
            codeSize = sizeReaded;
        } else {
            code = (char*) realloc(code, sizeof (char) * (codeSize + sizeReaded));
            strncpy(code + (codeSize * sizeof (char)), buffer, sizeReaded);
            codeSize += sizeReaded;
        }
    }
    fclose(fil);
    return code;
}
