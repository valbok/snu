/*
 * Get time in milliseconds since the initial call.
 */

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#endif

typedef unsigned long long   TIME;
#define INVALID_TIME    ((unsigned long long)(-1))
TIME gettime()
{
   TIME t;

   static time_t      base_sec  = 0;
   static suseconds_t base_usec = 0;
   struct timeval     tv;
   gettimeofday(&tv, NULL);
   if (base_sec == 0)
   {
      base_sec  = tv.tv_sec;
      base_usec = tv.tv_usec;
      return(0);
   }
   else
   {
      t = (TIME)(((tv.tv_sec - base_sec) * 1000) + ((tv.tv_usec - base_usec) / 1000));
      return(t);
   }
}
