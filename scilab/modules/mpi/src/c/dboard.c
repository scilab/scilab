/* dboard.c
   see pi_send.c and pi_reduce.c    */

#include <stdlib.h>
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
     {
     double x_coord,       /* x coordinate, between -1 and 1  */
            y_coord,       /* y coordinate, between -1 and 1  */
            pi,            /* pi  */
            r;             /* random number between 0 and 1  */
     int score,            /* number of darts that hit circle */
         n;
     unsigned long cconst; /* used to convert integer random number */
                           /* between 0 and 2^31 to double random number */
                           /* between 0 and 1  */

     cconst = 2 << (31 - 1);
     score = 0;

     /* "throw darts at board" */
     for (n = 1; n <= darts; n++)  {
          /* generate random numbers for x and y coordinates */
          r = (double)random()/cconst;
          x_coord = (2.0 * r) - 1.0;
          r = (double)random()/cconst;
          y_coord = (2.0 * r) - 1.0;

          /* if dart lands in circle, increment score */
          if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
               score++;
          }

     /* calculate pi */
     pi = 4.0 * (double)score/(double)darts;
     return(pi);
     } 
