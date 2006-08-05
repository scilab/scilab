/* Copyright INRIA/ENPC */
#include <stdio.h>
#include "../dld/dld.h"



#if defined(__STDC__)
extern void getpro_(char *,long int);
extern char * dld_strerror(int code);
#else 
extern void getpro_();
extern char * dld_strerror();
#endif 


/*************************************
 * New version : link entry names 
 *   from new shared lib created with 
 *   files.
 *************************************/

static int lastlink={0};

void SciLink(iflag,rhs,ilib,files,en_names,strf)
     int iflag,*ilib,*rhs;
     char *files[],*en_names[],*strf;
{
  char enamebuf[MAXNAME];
  int i,ii,err=0;
  if ( iflag != 0 ) 
    {
      *ilib=-3; /* aout link : First argument  can't be a number */
      return;
    }
  if ( *rhs <=1 ) 
    {
      *ilib=-6; /* aout link, expecting more than one argument */
      return;
    }
  if ( *rhs >= 2  &&  en_names[1] != (char *) 0 )
    {
      *ilib=-4;/* aout link : en_names must be of size 1 */
      return;
    }
  /** 
  if ( files[1] != (char *) 0 )
    {
      *ilib=-5; / *aout link : files must be of size 1 * /
      return;
    }
    **/
  if ( NEpoints != 0 && strcmp(en_names[0],EP[NEpoints-1].name) == 0) 
    {
      ii= NEpoints-1;
    }
  else 
    {
      ii= NEpoints;
      NEpoints++;
      if ( NEpoints == ENTRYMAX ) 
	{
	  *ilib=-2; /* You can't link more functions maxentry reached n*/
	}
    }
  if ( strf[0] == 'f' )
    Underscores(1,en_names[0],enamebuf);
  else 
    Underscores(0,en_names[0],enamebuf);
  if ( files[1] != (char *) 0 )
    C2F(dynload1)(&ii,enamebuf,files,&err);
  else 
    C2F(dynload)(&ii,enamebuf,files[0],&err);
  if ( err == 0 ) 
    {
      strncpy(EP[ii].name,en_names[0],MAXNAME);
      EP[ii].Nshared = ii;
      hd[ii].ok = OK;
      Nshared = NEpoints;
      *ilib= ii;
    }
  else
    {
      if ( NEpoints != 0) NEpoints--;
      *ilib=-2;
    }
}
/**************************************
 * return 1 if link accepts multiple file iin one call
 * or 0 elsewhere 
 *************************************/

int LinkStatus()
{
  return(0);
}

/**************************************
 * Unlink a shared lib 
 *************************************/

void C2F(isciulink)(i) 
     integer *i;
{
  sciprint("aout link : No unlink up to now \r\n");
}


#define DLDLINK

int dyninit_()
{
  static int err=0;
  static int initialised=0;
  /* required initialization. */
  if (!initialised) 
    {
      char prog[200];
      getpro_(prog,sizeof(prog)-1);
      /*sciprint("Link Initialisation");*/
      err =  dld_init (dld_find_executable(prog));
      /*sciprint(" %s\n",dld_find_executable(prog));*/
      if(!err) 
	initialised=1;
      else {
	sciprint("dld_init error %s\n",dld_strerror(err));
	  
      }
    };
  return err;
}

/* print out all the undefined symbols */

void list_undefined () 
{
  char **list = dld_list_undefined_sym ();
  if (list) 
    {
      register int i;
      sciprint("There are a total of %d undefined symbols:\n",
	       dld_undefined_sym_count);
      
      for (i = 0; i < dld_undefined_sym_count; i++)
	sciprint("%d: %s\n", i+1, list[i]);
    } 
  else
    sciprint("No undefined symbols\n");
}



#define MAXCHAR 256
#if defined(__STDC__)
int C2F(dynload)(int *ii,char ename1[],char loaded_files[],int *err)
#else 
     int dynload_(ii,ename1,loaded_files,err)
     int *ii;char ename1[],loaded_files[];int *err;
#endif
{
  function func;
  char current_object_file[MAXCHAR];
  int i,j;
  char current_char;

#ifdef DEBUG
  sciprint("ename1 [%s]\r\n",ename1);
  sciprint("lastlink %d, entry=%d\n",lastlink,*ii);
#endif

  sciprint("linking  \"%s\" defined in \"%s\"\n", ename1,loaded_files);
  
  *err = 0;
  if ( (*err = dyninit_())) return; /* Error on init */
  /* on scane et on charge the objects files */
  /* unloading if necessary */
  i = 0;j=0;
  while (1){
    current_char = loaded_files[i];
    if((current_char == ' ') || (current_char == '\0') || (i == strlen(loaded_files)))
      {
	current_object_file[j] = '\0';
	if(strlen(current_object_file)>0){
	  /** if this file was previously linked i must unlink it **/
	  if ( *ii <= lastlink && lastlink != 0  )
	    {
	      dld_unlink_by_file (current_object_file,1);
#ifdef DEBUG
	      sciprint("unloading : \"%s\"\r\n",current_object_file);
#endif
	    };
	};
	j = -1;
	if(loaded_files[i] == '\0')   break;
      } 
    else 
      { 
	current_object_file[j] = loaded_files[i];
      }
    i++;j++;
    if ( j > MAXCHAR ) 
      {
	*err=1 ;
	sciprint("filename too long");
      };
  };

  /* loading */
  i = 0;j=0;
  while (1){
    current_char = loaded_files[i];
    if((current_char == ' ') || (current_char == '\0') || (i == strlen(loaded_files)))
      {
	current_object_file[j] = '\0';
	if(strlen(current_object_file)>0){
#ifdef DEBUG
	  sciprint("loading : \"%s\"\r\n",current_object_file);
#endif
	  *err = dld_link (current_object_file);
	  if(*err){
	    sciprint("problem when loading : \"%s\"\n",current_object_file);
	    sciprint("dld_link error %s \n",dld_strerror (*err));
	    *err=99;
	    return;
	  };
	};
	j = -1;
	if(loaded_files[i] == '\0')   break;
      } 
    else 
      { 
	current_object_file[j] = loaded_files[i];
      }
    i++;j++;
    if ( j > MAXCHAR ) 
      {
	*err=1 ;
	sciprint("filename too long");
	  
      };
  };
  
  /* grap the entry point for function "ename"  */
  if (dld_function_executable_p (ename1))
    {
      func = (function) dld_get_func (ename1);
      if ( func  == (function) 0)
	{
	  sciprint("error when finding \"%s\" in \"%s\"\n",ename1,loaded_files);
	    
	  sciprint("dld_get_func error %s\n",dld_strerror (*err));
	    
	  *err=1;
	  return;
	};
#ifdef DEBUG
      sciprint("procedure numero %d \"%s\" located in \"%s\"\r\n",*ii,ename1,loaded_files);
#endif
      EP[*ii].epoint = func;
      lastlink=lastlink+1;
    }
  else 
    {
      sciprint("error [%s] not executable \n",ename1);
      list_undefined ();
      *err=1;
    };
};


#if defined(__STDC__)
int C2F(dynload1)(int *ii,char ename1[],char *loaded_files[],int *err)
#else 
     int dynload_(ii,ename1,loaded_files,err)
     int *ii;char ename1[], *loaded_files[];int *err;
#endif
{
  function func;
  int i,j;

#ifdef DEBUG
  sciprint("ename1 [%s]\r\n",ename1);
  sciprint("lastlink %d, entry=%d\n",lastlink,*ii);
#endif
  sciprint("linking  \"%s\" defined in ",ename1);
  i=0;
  while ( loaded_files[i] != NULL) 
    {
      sciprint("\"%s\" ", loaded_files[i]); i++;
    }
  sciprint("\r\n");
  *err = 0;
  if ( (*err = dyninit_())) return; /* Error on init */
  /* scaning and loading  objects files */
  /* unloading if necessary */
  i = 0;
  while (loaded_files[i] != NULL )
    {
      if(strlen(loaded_files[i])>0)
	{
	  /** if this file was previously linked i must unlink it **/
	  if ( *ii <= lastlink && lastlink != 0  )
	    {
	      dld_unlink_by_file (loaded_files[i],1);
#ifdef DEBUG
	      sciprint("unloading : \"%s\"\r\n",loaded_files[i]);
#endif
	    }
	}
      i++;
    }
  /* loading */
  i = 0;
  while (loaded_files[i] != NULL)
    {
      if(strlen(loaded_files[i])>0){
#ifdef DEBUG
	  sciprint("loading : \"%s\"\r\n",loaded_files[i]);
#endif
	  *err = dld_link (loaded_files[i]);
	  if(*err){
	    sciprint("problem when loading : \"%s\"\n",loaded_files[i]);
	    sciprint("dld_link error %s \n",dld_strerror (*err));
	    *err=99;
	    return;
	  }
	}
      i++;
    }
  
  /* grap the entry point for function "ename"  */
  if (dld_function_executable_p (ename1))
    {
      func = (function) dld_get_func (ename1);
      if ( func  == (function) 0)
	{
	  sciprint("error when finding \"%s\" in \"%s\",... \n",ename1,loaded_files[0]);
	  sciprint("dld_get_func error %s\n",dld_strerror (*err));
	  *err=1;
	  return;
	};
#ifdef DEBUG
      sciprint("procedure numero %d \"%s\" located in \"%s\" .... \r\n",*ii,ename1,loaded_files[0]);
#endif
      EP[*ii].epoint = func;
      lastlink=lastlink+1;
    }
  else 
    {
      sciprint("error [%s] not executable \n",ename1);
      list_undefined ();
      *err=1;
    };
};




