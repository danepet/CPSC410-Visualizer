#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
 
#define DATA_SIZE   1024
#define DATE_TIME_SIZE 256
#define ERROR  -1
  
/**
* This file was adapted from: 
* http://cboard.cprogramming.com/c-programming/100490-creating-our-own-fprintf-function-our-library.html
* 
* It is a new definition of printf, and must be linked with the program that is to be analyzed, using
* LD_PRELOAD. Dyninst was unable to deal with the time_t type that we needed to print timestamps, so we
* had to override printf and use our version to automatically prepend a timestamp to all printfs and output
* them to a file.
**/

int printf (const  char *format, ...)
{
        int l_nNumOfChars=0;
        char l_carrTmpData[DATA_SIZE]={0};
        char l_carrTmpDateTime[DATE_TIME_SIZE]={0};
        char l_carrTmpDataAndTime[DATA_SIZE + DATE_TIME_SIZE]={0};
        struct timeval timeNalue;

        FILE *pfile;
        pfile = fopen("/home/james00794/cs410project/CPSC410/Tetraminos/static_lib/dynamicOutput.txt", "a");
 
        // va_list type that allows us to deal with an arbitrary number of parameters
        va_list args;
        va_start(args, format);
         
        if( gettimeofday(&timeNalue, NULL) != ERROR )
        {   
            // get the current time
            struct tm *st_pTimeSt = localtime(&timeNalue.tv_sec);
            if(st_pTimeSt)
            {
                // copy into l_carrTmpDateTime buffer
                snprintf(l_carrTmpDateTime,DATE_TIME_SIZE, "%04d-%02d-%02d %02d:%02d:%02d.%02d",
                        st_pTimeSt->tm_year+1900, st_pTimeSt->tm_mon+1, st_pTimeSt->tm_mday, st_pTimeSt->tm_hour, st_pTimeSt->tm_min, st_pTimeSt->tm_sec,(timeNalue.tv_usec/1000));
            }
 
        }
        vsnprintf(l_carrTmpData, DATA_SIZE, format, args);
        va_end(args);
 
        if(l_carrTmpData[0] == '\n' )
        {
            l_carrTmpData[0]=' ';
            snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"\n%s - %s",l_carrTmpDateTime,l_carrTmpData );
 
        }
        else
        {
            snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"%s - %s",l_carrTmpDateTime,l_carrTmpData );
        }
        // print out to the dynamicOutput.txt file
        fprintf(pfile, "%s", l_carrTmpDataAndTime);
        fclose(pfile);
        return  l_nNumOfChars;     
         
}

// leaving these here for reference
// use these to compile into shared object
// 1031  g++ -fpic -c mylib.cpp
// 1033  g++ -shared -o libMyLib.so mylib.o
