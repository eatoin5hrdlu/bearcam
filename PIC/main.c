/*
 * MAIN.C
 */

#include "camera.h"

#define LENGTH(a) (sizeof(a)/sizeof((a)[0]))

#ifdef DEBUG

char text[15] = "eH KJ.\n135790\n";

int flash;
bit
flasher(void)
{
  flash++;
  if (flash % 100 == 0)
    {
       // RA1 = !RA1;
       sleep(100);
       return 1;
    }
   return 0;
}
#endif

void
sleep(int c)
{
int i,j;
   while(c--)
     {
       for (i=0; i < 50; i++)
	    {
	       j = i+4;
           if (j > 20 && j < 22) {
              if (j<3) {
	         	c = c - j/10;
              }
           }
	     }
     }
}



void
main(void)
{
  int i;
  initialize();
  while(1)
    {
      check_switches();
      sleep(200);

#ifdef DEBUG
      if ( flasher() )
	  {
      	for(i=0; i<15; i++)
        {
	  		putch(text[i]);
        }
      }

#endif

    }
}
