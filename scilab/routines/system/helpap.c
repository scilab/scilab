/* Copyright INRIA/ENPC */

#include <stdio.h>
#include "../graphics/Math.h"
#include "../sun/h_help.h"

#ifdef __STDC__
#include <stdlib.h>
#else 
extern  char  *getenv();
#endif

extern int C2F(xscion)();
extern int help_popped_status();
/******************************************
 * Calls scilab help or xhelp for string str 
 * buf is a buffer array 
 ******************************************/

#define NEW 


#ifdef NEW 


/** nouvelle version ( voir h_help_data.c ) **/
void C2F(iscihelp)(buf,str,ierr) 
     char *str,*buf;
     integer *ierr;
{
  /** if ( help_popped_status() == 1) 
    sciprint("Sorry, You cannot use interative help\r\nwhen help menu was previously activated\n");
  else 
  **/
  *ierr= Sci_Help(str);
}

#else 

void C2F(iscihelp)(buf,str,ierr) 
     char *str,*buf;
     integer *ierr;
{

#ifdef gcwin32
  char *local = (char *) 0;
#endif
  char mode[2]; 
  int flag;
  mode[0]='x';mode[1] = '\0';
  C2F(xscion)(&flag);
  *ierr=0;
  if ( flag == 0) 
    mode[0] = '\0';
  else
    mode[0] = 'x';
  /** XXXXX : gcwin32 scripts can't execute gcwin32 executable in batch
    up to now : we use the standard windows system function 
    that's why we need the getenv 
    ***/

#ifdef gcwin32
  local = getenv("SCI");
  if ( local != (char *) 0)
    sprintf(buf, "%s/bin/scilab -%shelp %s ",local,mode,str);
  else
    /** maybe scilab is in the path ? **/
    sprintf(buf, "scilab -%shelp   %s",mode,str);
  if (winsystem(buf,0))
    {
      sciprint("help error: winsystem failed\r\n");
      *ierr=1;
    }
#else
  sprintf(buf,"$SCI/bin/scilab -%shelp %s", mode,str);
  if (  system(buf)  == 19*256 ) 
    {
      sciprint("No help on %s\r\n",str);
    }
#endif
}
#endif /* NEW*/

/******************************************
 * Calls scilab apropos for string str 
 * buf is a buffer array 
 ******************************************/

#ifdef NEW 

void C2F(isciap)(buf,str,ierr) 
     char *str,*buf;
     integer *ierr;
{
  /** if ( help_popped_status() == 1) 
    sciprint("Sorry, You cannot use interative apropos\r\nwhen help menu was previously activated\n");
  else 
  **/
  *ierr= Sci_Apropos(str);
}

#else 

void C2F(isciap)(buf,str,ierr) 
     char *str,*buf;
     integer *ierr;
{
#ifdef gcwin32
  char *local = (char *) 0;
#endif
  int flag;
#ifdef __STDC__
  char mode[2] ="x";
#else 
  char mode[2];
  mode[0] ='x';mode[1] ='\0';
#endif
  C2F(xscion)(&flag);
  *ierr=0;
  if ( flag == 0) 
    mode[0] = '\0';
  else
    mode[0] = 'x';
  /** XXXXX : gcwin32 scripts can't execute gcwin32 executable in batch
    up to now : we use the standard windows system function 
    that's why we need the getenv 
    ***/

#ifdef gcwin32
  local = getenv("SCI");
  if ( local != (char *) 0)
    sprintf(buf, "%s/bin/scilab -%sk %s ",local,mode,str);
  else
    /** maybe scilab is in the path ? **/
    sprintf(buf, "scilab -%sk   %s",mode,str);
  if (winsystem(buf,0))
    {
      sciprint("help error: winsystem failed\r\n");
      *ierr=1;
    }
#else
  sprintf(buf,"$SCI/bin/scilab -%sk %s", mode,str);
  if (  system(buf)  == 19*256 ) 
    {
      sciprint(" Topic  %s not found \r\n",str);
    }
#endif
}

#endif /* NEW*/
