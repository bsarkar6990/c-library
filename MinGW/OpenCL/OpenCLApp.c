#include "CL/opencl.h"#include <stdio.h>#include <time.h>#include <vector>#include <string>#include <sstream>int main(){	printf("Hello");	cl_uint num_of_platforms = 0;    clGetPlatformIDs(0, 0, &num_of_platforms);	printf("\nNumber of Platform %x\n",num_of_platforms);	cl_device_type primary_device_type=CL_DEVICE_TYPE_GPU;   // try to use this device type first    cl_device_type secondary_device_type=CL_DEVICE_TYPE_CPU;	std::vector<cl_platform_id> platforms(num_of_platforms);	clGetPlatformIDs(num_of_platforms, &platforms[0], 0);	std::string required_platform_subname = "";	int primary_platform_index = num_of_platforms,        // for primary_device_type            secondary_platform_index = num_of_platforms;	cl_uint selected_platform_index = num_of_platforms;	std::string returnvalue="";	cl_int err = CL_SUCCESS;	for(cl_uint i = 0; i < num_of_platforms; ++i)    {        // Get the length for the i-th platform name.        size_t platform_name_length = 0;        err = clGetPlatformInfo(                platforms[i],                CL_PLATFORM_NAME,                0,                0,                &platform_name_length        );               // Get the name itself for the i-th platform.        std::vector<char> platform_name_buffer(platform_name_length);        err = clGetPlatformInfo(                platforms[i],                CL_PLATFORM_NAME,                platform_name_length,                &platform_name_buffer[0],                0        );        std::string platform_name = &platform_name_buffer[0];        std::string selection_marker;    // additional message will be printed to log        if(!required_platform_subname.empty())        {            // The fist way of platform selection: by name.            // Decide if the found i-th platform is the required one.            // In this example only the first match is selected,            // while the rest matches are ignored.            if(                    platform_name.find(required_platform_subname) &&                    selected_platform_index == num_of_platforms // have not selected yet                    )            {                selected_platform_index = i;                selection_marker = "  [Selected]";                // Do not exit here and continue the enumeration to see all available platforms,            }        }        else        {            // The second way of platform selection: by device type            // Get the number of devices of primary_device_type            cl_uint num_devices = 0;            err = clGetDeviceIDs(platforms[i], primary_device_type, 0, 0, &num_devices);            // Do not check with SAMPLE_CHECK_ERRORS here, because err may contain            // CL_DEVICE_NOT_FOUND which is processed below            if(err != CL_DEVICE_NOT_FOUND)            {                // Handle all other type of errors from clGetDeviceIDs here                              if(primary_platform_index == num_of_platforms)                {                    primary_platform_index = i;                    selection_marker = "  [Primary]";                }            }            else            {                // Get the number of devices of secondary_device_type                // (similarly to primary_device_type).                err = clGetDeviceIDs(platforms[i], secondary_device_type, 0, 0, &num_devices);                // Do not check with SAMPLE_CHECK_ERRORS here, because err may contain                // CL_DEVICE_NOT_FOUND which is processed below                if(err != CL_DEVICE_NOT_FOUND)                {                    // Handle all other type of errors from clGetDeviceIDs here                                        if(secondary_platform_index == num_of_platforms)                    {                        secondary_platform_index = i;                        selection_marker = "  [Secondary]";                    }                }            }        }        // Print platform name with an optional selection marker        printf("    [%u] %s", i, (platform_name + selection_marker).c_str());        returnvalue=returnvalue+"\n"+(platform_name + selection_marker).c_str()+"\n";    }    if(required_platform_subname.empty())    {        // Select between primary and secondary device type        if(primary_platform_index != num_of_platforms)        {            selected_platform_index = primary_platform_index;        }        else if(secondary_platform_index != num_of_platforms)        {            selected_platform_index = secondary_platform_index;        }    }    if(selected_platform_index == num_of_platforms)    {        printf("There is no found a suitable OpenCL platform");     }	std::string deviceInfo =returnvalue;	printf(deviceInfo.c_str());	printf("\nPress enter to exit....\n");	getchar();	printf("Bye\n");	int milli_seconds = 1000;	clock_t start_time = clock(); 	while (clock() < start_time + milli_seconds); 	return 0;}