/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_xpause.h"
#ifdef HAVE_USLEEP
#include <unistd.h>
#endif
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_xpause) _PARAMS((char *fname,unsigned long fname_len))
{
	integer m1,n1,l1,sec=0;

	CheckLhs(0,1);
	CheckRhs(1,1);
	if (Rhs == 1) 
	{ 
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		CheckScalar(1,m1,n1);
		sec = (integer) *stk(l1);
		if (sec <=0)
		{
			error_scilab(999,"time_error_6",fname);
			return 0;
		}
	
	#ifdef _MSC_VER
		{
			int ms = (sec)/1000; /** time is specified in milliseconds in scilab**/
			if (ms > 0) Sleep(ms); /* Number of milliseconds to sleep. */
		}
	#else
		{
			unsigned useconds;
			useconds=(unsigned) sec;
			if (useconds != 0)  
				#ifdef HAVE_USLEEP
					{ usleep(useconds); }
				#else
					#ifdef HAVE_SLEEP
					{  sleep(useconds); }
					#endif
				#endif
		}
	#endif
     }
	 LhsVar(1)=0;
	 C2F(putlhsvar)();
	 return 0;
}
/*-----------------------------------------------------------------------------------*/ 
