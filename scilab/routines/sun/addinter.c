/* Copyright INRIA */

#include <string.h> 
#include <stdio.h>
#if defined(THINK_C) || defined (__MWERKS__)  
#include "::graphics:Math.h"
#else
#include "../graphics/Math.h"
#endif
#include "men_Sutils.h"
#include "link.h"

#include "addinter.h" 

extern int C2F(namstr) __PARAMS((integer *id, integer *str, integer *n, integer *job));
extern int C2F(funtab) __PARAMS((int *id, int *fptr, int *job));  
extern int C2F(error)  __PARAMS((integer *n));  
extern void GetenvB __PARAMS(( char *name,char *env, int len));


#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#define OK 1
#define FAIL 0

#define MAXINTERF 50
#define INTERFSIZE 25 

typedef struct 
{
  char name[INTERFSIZE]; /** name of interface **/
  void (*func)();        /** entrypoint for the interface **/
  int Nshared; /** id of the shared library **/
  int ok;    /** flag set to 1 if entrypoint can be used **/
} Iel;

Iel DynInterf[MAXINTERF];
static int LastInterf=0;
static void SciInterInit();
static void DynFuntab __PARAMS((int *Scistring,int *ptrstrings,int *nstring,int k1));

int Use_cpp_code;
char * Use_c_cpp;

/************************************************
 * Dynamically added interface to Scilab 
 ************************************************/

void C2F(addinter)(descla,ptrdescla,nvla,iname,desc,ptrdesc,nv,
		   c_cpp,lib_cpp,err)
     int *desc,*ptrdesc,*nv;         /* ename */
     int *descla,*ptrdescla,*nvla;   /* files */
     char *iname;                    /* interface name */
     char *c_cpp;                    /* C++ compiler */
     int *lib_cpp;                   /* for cpp library */
     int *err;
{
  int ierr,i,rhs=2,ilib=0,inum;
  char **files,*names[2];
  *err=0;

  Use_cpp_code=*lib_cpp;
  Use_c_cpp = (char *) malloc((strlen(c_cpp) +1) * sizeof(char));
  strcpy(Use_c_cpp,c_cpp);

  ScilabMStr2CM(descla,nvla,ptrdescla,&files,err);
  if ( *err == 1) return;
  names[0]=iname;
  names[1]=(char *)0;

  SciLinkInit();
  SciInterInit();

  /** Try to unlink the interface if it was previously linked **/
  
  for ( i = 0 ; i < LastInterf ; i++) 
    {
      if (strcmp(iname,DynInterf[i].name)==0) 
	{
	  /** check if my os accepts unlink **/
	  if ( LinkStatus() == 1) 
	    {
	      C2F(isciulink)(&DynInterf[i].Nshared);
	    }
	  break;
	}
    }

  /** Try to find a free position in the interface table : inum **/
  inum=-1;
  for ( i = 0 ; i < LastInterf ; i++) 
    {
      if ( DynInterf[i].ok == 0 ) inum= i;
    }
  inum = ( inum == -1 ) ? LastInterf : inum ;

  /** Linking Files and add entry point name iname */

  if ( inum >=  MAXINTERF ) 
    {
      /*      sciprint("Maximum number of dynamic interfaces %d\r\n",MAXINTERF);
	      sciprint("has been reached\r\n");*/
      *err=1;
      return;
    }

  SciLink(0,&rhs,&ilib,files,names,"f");

  if ( ilib < 0 ) 
    {
      *err=ilib;  return;
    }

  /** store the linked function in the interface function table DynInterf **/
  DynInterf[inum].Nshared = ilib;

  if ( SearchInDynLinks(names[0],&DynInterf[inum].func) < 0 ) 
    {
      /*sciprint("addinter failed for %s Not  found!\r\n",iname);*/
      *err=2;
      return;
    }
  else
    {
      strncpy(DynInterf[inum].name,iname,INTERFSIZE);
      DynInterf[inum].ok = 1;
    }
  if ( inum == LastInterf ) LastInterf++;

  /** we add all the Scilab new entry names 
    in the scilab function table funtab **/

  DynFuntab(desc,ptrdesc,nv,inum+1);
  for (i=0;i< *nvla;i++) FREE(files[i]); FREE(files);
  ShowInterf();
}


static void SciInterInit()
{
  static int first_entry=0;
  if ( first_entry == 0) 
    {
      int i;
      for ( i= 0 ; i < MAXINTERF ; i++) 
	DynInterf[i].ok=0;
      first_entry++;
    }
}

/*********************************
 * unlink shared hp only 
 *********************************/

#if (defined(hppa))

extern int Call_shl_load;

void hppa_sci_unlink_shared()
{
  if ((LinkStatus() == 1) && (Call_shl_load))
      C2F(isciulink)(&DynInterf[0].Nshared);
}

#endif

/*********************************
 * used in C2F(isciulink)(i) 
 *********************************/

void RemoveInterf(Nshared)
     int Nshared;
{
  int i;
  for ( i = 0 ; i < LastInterf ; i++ ) 
    {
      if ( DynInterf[i].Nshared == Nshared ) 
	{
	  DynInterf[i].ok = 0;
	  break;
	}
    }
}

/*********************************
 * show the interface table 
 *********************************/

ShowInterf()
{
  int i;
  for ( i = 0 ; i < LastInterf ; i++ ) 
    {
      if ( DynInterf[i].ok == 1 ) 
	sciprint("Interface %d %s\r\n",i,DynInterf[i].name);
    }
}

#define nsiz 6 

/************************************************
 * add the set of functions associated to 
 *   dynamically added interface k1 
 *   in scilab function table with id 10000*k1+i 
 ************************************************/


static void DynFuntab(Scistring,ptrstrings,nstring,k1)
     int *Scistring,*nstring,*ptrstrings;
     int k1;
{
  int id[nsiz],zero=0,trois=3,fptr,fptr1,quatre=4;
  int li=1,ni,*SciS,i;
  SciS= Scistring;
  for ( i=1 ; i < *nstring+1 ; i++) 
    {
      ni=ptrstrings[i]-li;
      li=ptrstrings[i];
      C2F(namstr)(id,SciS,&ni,&zero);
      fptr1= fptr= (DynInterfStart+k1)*100 +i;
      C2F(funtab)(id,&fptr1,&quatre); /* clear previous def set fptr1 to 0*/
      C2F(funtab)(id,&fptr,&trois);  /* reinstall */
      SciS += ni;
    }
}

/************************************************
 * Used when one want to call a function added 
 * with addinterf the dynamic interface number 
 * is given by k1=(*k/100)-1
 ************************************************/

void C2F(userlk)(k) 
     integer *k;
{
  int k1 = *k - (DynInterfStart+1) ;
  int imes = 9999;
  if ( k1 >= LastInterf || k1 < 0 ) 
    {
      sciprint("Invalid interface number %d",k1);
      C2F(error)(&imes);
      return;
    }
  if ( DynInterf[k1].ok == 1 ) 
    (*DynInterf[k1].func)();
  else 
    {
      sciprint("Interface %s not linked\r\n",DynInterf[k1].name);
      C2F(error)(&imes);
      return;
    }
}


/******************************************************
 * Test for scilab library loaded when needed 
 * similar to addinter but we do not add the function list 
 * in fundef since it is already present 
 * sometimes we need to link several interfaces with the same code 
 ******************************************************/

int  SciLibLoad(num_names,names,files,nums,err)
     char *names[];
     char **files;
     int *err;
     int nums[],num_names;
{
  int ierr,i,rhs=2,inum,ilib=0,j;
  SciLinkInit();
  SciInterInit();
  *err=0;
  
  for ( j=0 ; j < num_names ; j++) 
    {
      /** Try to find a free position in the interface table : inum **/
      inum=-1;
      for ( i = 0 ; i < LastInterf ; i++) 
	{
	  if ( DynInterf[i].ok == 0 ) inum= i;
	}
      inum = ( inum == -1 ) ? LastInterf : inum ;
      /** Linking Files and add entry point name iname */
      if ( inum >=  MAXINTERF ) 
	{
	  sciprint("Maximum number of dynamic interfaces %d\r\n",MAXINTERF);
	  sciprint("has been reached\r\n");
	  *err=1;
	  return -1 ;
	}
      else 
	nums[j]=inum;
      if ( inum == LastInterf ) LastInterf++;
    }
  SciLink(0,&rhs,&ilib,files,names,"f");
  if ( ilib < 0 ) 
    {
      *err=1;  return -1;
    }
  /** store the linked function in the interface function table DynInterf **/

  for ( j=0 ; j < num_names ; j++) 
    {
      DynInterf[nums[j]].Nshared = ilib;
      if ( SearchInDynLinks(names[0],&DynInterf[nums[j]].func) < 0 ) 
	{
	  sciprint("addinter failed for %s Not  found!\r\n",names[j]);
	  return -1;
	}
      else
	{
	  strncpy(DynInterf[nums[j]].name,names[j],INTERFSIZE);
	  DynInterf[nums[j]].ok = 1;
	}
    }
  ShowInterf();
  return 0;
}

#define MAX_ENV 256 

BuildName(name,str)
     char *name,*str;
{
  int  nc= MAX_ENV;
  GetenvB("SCI",name,nc);
  strcat(name,"/libs/");
  strcat(name,str);
}

CallDynInterf(pos,num_names,namepos,names,nums,files)
     int *pos;
     char *names[];
     char *files[];
     int namepos,num_names,nums[];
{
  int imes = 9999;
  if ( *pos == -1 || DynInterf[*pos].ok == 0) 
    {
      /** need to load or reload the interface **/
      int pos1, err=0;
      SciLibLoad(num_names,names,files,nums,&err);
      if (err != 1) *pos = nums[namepos];
    }
  if ( DynInterf[*pos].ok == 1 ) 
    (*DynInterf[*pos].func)();
  else 
    {
      sciprint("Interface %s not linked\r\n",DynInterf[*pos].name);
      C2F(error)(&imes);
      return;
    }
}  







