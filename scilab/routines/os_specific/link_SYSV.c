/* Copyright INRIA/ENPC */
/*********************************
 * Link version for SYSV machine 
 *********************************/


#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/file.h>


#ifndef hppa
#include <dlfcn.h>
#else
#include <dl.h>
#endif

#if (defined(sun) && defined(SYSV)) 
#include <unistd.h>
#include <sys/wait.h>
#endif

#if defined(sun)  && !defined(netbsd) && !defined(freebsd)
#if defined(SYSV)
#include <sys/vnode.h>
#include <archives.h>
#else
#define RTLD_NOW 1
#include <dlfcn.h>
#endif
#endif

#if defined(linux) || defined(netbsd) || defined(freebsd)
#include <unistd.h>
#include <sys/wait.h>
#endif 

#if !defined(linux) && !defined(netbsd)  && !defined(freebsd)
#ifndef hppa
#if (defined(sun) && ! defined(SYSV)) 
#else 
#include <sys/mode.h>
#endif 
#endif
#endif 

#if !defined(linux) && !defined(netbsd) && !defined(freebsd)
#ifdef __alpha
#include <c_asm.h>
#endif
#endif


#ifdef sgi
#define vfork fork
#endif

#if (defined(__alpha) && !defined(netbsd) && !defined(freebsd)) || defined sgi
#include <a.out.h>
#endif

#include <string.h>


#define round(x,s) (((x) + ((s)-1)) & ~((s)-1))
#define Min(x,y)	(((x)<(y))?(x):(y))
#define Max(x,y)	(((x)>(y))?(x):(y))

/* extern char *strchr();  */
extern int Use_cpp_code;
extern char * Use_c_cpp;

extern char * get_sci_tmp_dir();

static void Sci_Delsym __PARAMS((int ));
static int Sci_dlopen __PARAMS((char *loaded_files[]));
static int Sci_dlsym __PARAMS((char *ename,int  ishared,char * strf));
static int SetArgv  __PARAMS((char *argv[], char *files[],int first,int max,int *err));
static int SetArgv1  __PARAMS((char *argv[], char *files,int first,int max,int *err));

int Call_shl_load = 0;


/*************************************
 * New version : link entry names 
 *   from new shared lib created with 
 *   files.
 *   return in ilib the number of the shared archive 
 *   or -1 or -5
 *   -1 : the shared archive was not loaded 
 *   -5 : pb with one of the entry point 
 *************************************/

void SciLink(iflag,rhs,ilib,files,en_names,strf)
     int iflag,*ilib,*rhs;
     char *files[],*en_names[],*strf;
{
  int i;
  if ( iflag == 0 )
    {
      *ilib  = Sci_dlopen(files);
    }
  if (*ilib  == -1 ) return;
  sciprint("shared archive loaded\r\n");
  if ( *rhs >= 2) 
    {
      i=0 ;
      while ( en_names[i] != (char *) 0)
	{
	  if ( Sci_dlsym(en_names[i],*ilib,strf) == FAIL) 
	    *ilib=-5;
	  i++;
	}
    }
}

/**************************************
 * return 1 if link accepts multiple file iin one call
 * or 0 elsewhere 
 *************************************/

int LinkStatus()
{
  return(1);
}

/**************************************
 * Unlink a shared lib 
 *************************************/

void C2F(isciulink)(i) 
     integer *i;
{
  /* delete entry points in shared lib *i */
  Sci_Delsym(*i);
  /* delete entry points used in addinter in shared lib *i */
  RemoveInterf(*i);
}


#if defined(hppa)
int main() {}

/**************************************
 * This routine force the call of global constructors when
 * "shl_load" is called, and the call of global destructors
 * when "shl_unload" is called (ONLY on HPUX systems). 
 *************************************/

void call_ctor_dtor(handle,loading)
     shl_t handle;
     int loading;
{
  void (*call_sym)();

  if (loading)
    {
      if (shl_findsym(&handle,"_GLOBAL__DI",TYPE_PROCEDURE,
		      (void *) &call_sym))
	{
	  sciprint("No global constructor(s) call\r\n");
	  return;
	}
    }
  else
    {
      if (shl_findsym(&handle,"_GLOBAL__DD",TYPE_PROCEDURE,
		      (void *) &call_sym))
	{
	  sciprint("No global destructor(s) call\n");
	  return;
	}
    }

  (*call_sym)();
}
#endif

/*************************************
 * This routine 
 *   changes the file list <<loaded_files>>
 *   to a shared archive and call dlopen 
 *   the shared lib handler is stored in a Table 
 *   The return value is == -1 if the dlopen failed 
 *************************************/

#define MAXARGV 128



#ifndef hppa
#define SHARED_SUF ".so"
#else
#define SHARED_SUF ".sl"
#endif



static int Sci_dlopen(loaded_files)
     char *loaded_files[];
{
  int i=0;
#ifndef hppa
  void *hd1 = (void *) 0;
#else
  shl_t hd1;
  void (*init_ctor)() = 0;
#endif
  char tmp_file[TMPL],*libs,*getenv();
  /** XXXXX **/
  if ( strncmp(loaded_files[0],"scilab",6) !=0)
    {
      if ( loaded_files[0] != NULL && loaded_files[1] == NULL 
	   && strstr(loaded_files[0],SHARED_SUF)!= NULL)
	{
	  strcpy(tmp_file,loaded_files[0]);
	  sciprint("Loading shared executable %s\r\n",loaded_files[0]);
	}
      else 
	{
	  int rep;
	  if (Use_cpp_code)
	    rep=CreateCppShared(loaded_files,tmp_file);
	  else
	    rep=CreateShared(loaded_files,tmp_file);
	  if (rep == -1) 
	    return(-1);
	}
      /* this will load the shared library */
#ifndef hppa
      Call_shl_load = 0;
      hd1 = dlopen(tmp_file, RTLD_NOW);
#else
      Call_shl_load = 1;
      hd1 = shl_load(tmp_file, BIND_IMMEDIATE | BIND_VERBOSE,0L);
#endif
    }
  else
    {
      /* use symbols from scilab */
      /* XXXXXX Only on sun  */
#if defined(sun) && !defined(netbsd) && !defined(freebsd)
      hd1 = dlopen((char *)0, RTLD_NOW);
#else
#ifdef hppa
      hd1 = PROG_HANDLE;
#else
      sciprint("Link : scilab is not a valid first argument on your machine\r\n");
      return(-1);
#endif
#endif
    }
  /* this will load the shared library */
#ifndef hppa
  if ( hd1 == (void *) NULL || hd1 < (void *) 0 ) {
    sciprint("%s\r\n",dlerror());
#else
  if (  hd1 == NULL) {
    sciprint("link error\r\n");
#endif
    return(-1);
  }
  for ( i = 0 ; i < Nshared ; i++ ) 
    {
      if ( hd[i].ok == FAIL) 
	{
	  hd[i].shl =  (unsigned long)hd1;
	  strcpy(hd[i].tmp_file,tmp_file);
	  hd[i].ok = OK;
	  return(i);
	}
    }
  
  if ( Nshared == ENTRYMAX ) 
    {
      sciprint("You can't open shared files maxentry %d reached\r\n",ENTRYMAX);
      return(FAIL);
    }

  strcpy(hd[Nshared].tmp_file,tmp_file);
  hd[Nshared].shl = (unsigned long)hd1;
  hd[Nshared].ok = OK;
  Nshared ++;
  return(Nshared-1);
}

/** creates a shared executable from the set of files **/
  
int CreateShared(loaded_files,tmp_file)
  char *loaded_files[];
 char *tmp_file;
{
  int argc=3,err=0;
  static int count=0;
  int i=0;
  char *libs,*tmpdir;
  libs=getenv("SYSLIBS");
   /** XXXXX **/
  sciprint("linking files ");
  while ( loaded_files[i] != NULL) 
    {
      sciprint("%s ",loaded_files[i]);
      i++;
    }

  sciprint(" to create a shared executable\r\n");
  count++;
  /* be sure that tmpdir exists */
  tmpdir = get_sci_tmp_dir();
  sprintf(tmp_file, "%s/SL_%d_XXXXXX",tmpdir,count);
  
  mktemp(tmp_file);
  {
    int pid, status, wpid;
    static char *argv[MAXARGV] = {
      /*   0        1         2    3  4   */
#if defined(sun) && !defined(netbsd) && !defined(freebsd)
#if defined(SYSV)
      "/usr/ucb/ld", "-r", "-o", 0, 0
#else 
      "/usr/bin/ld", "-o", 0, 0,0 
#endif
#else
#ifdef linux
      "/usr/bin/ld", "-shared", "-o", 0, 0  
#else
#ifdef hppa
      "/bin/ld", "-b", "-o", 0, 0
#else
#if defined(netbsd) || defined(freebsd)
      "/usr/bin/ld", "-shared", "-o", 0, 0  
#else
      "/bin/ld", "-shared", "-o", 0, 0  
#endif
#endif
#endif
#endif
    };
#if (defined(sun) && !defined(SYSV) && !defined(netbsd) && !defined(freebsd)) 
     argc = 2;
#endif
     argv[argc] = tmp_file; argc++;
     argc = SetArgv(argv,loaded_files,argc,MAXARGV,&err);
     if ( err == 1 ) return(-1);
     if (libs) 
       {
	 argc = SetArgv1(argv,libs,argc,MAXARGV,&err);
	 if ( err == 1 ) return(-1);
       }
     argv[argc] = (char *) 0;

#ifdef DEBUG
     for ( i=0 ; i < argc ; i++) 
       sciprint("arg[%d]=%s\r\n",i,argv[i]);
#endif	
     
     if ((pid = vfork()) == 0) {
       execv(argv[0], argv);
       _exit(1);
     }
     if (pid < 0) {
       sciprint("can't create new process: \r\n");
       return(-1);
     }
     while ((wpid = wait(&status)) != pid)
       if (wpid < 0) {
	 sciprint("no child !\r\n");
	 return(-1);
       }
     if (status != 0) {
       sciprint("ld returned bad status: %x\r\n", status);
       return(-1);
     }
   }

   return 0;
}

/** creates a shared executable from the set of C++ files **/

int CreateCppShared(loaded_files,tmp_file)
  char *loaded_files[];
 char *tmp_file;
{
  int argc=3,err=0;
  static int count=0;
  int i=0;
  char *libs, *exec_cpp, *tmpdir;
  libs=getenv("SYSLIBS");
   /** XXXXX **/
  sciprint("linking files ");
  while ( loaded_files[i] != NULL) 
    {
      sciprint("%s ",loaded_files[i]);
      i++;
    }

  sciprint(" to create a shared executable\r\n");
  count++;
  tmpdir = get_sci_tmp_dir();
  sprintf(tmp_file, "%s/SL_%d_XXXXXX",tmpdir,(int) getpid(),count);
  
  mktemp(tmp_file);
  {
    int pid, status, wpid;
    static char *argv[MAXARGV];
    argv[0] = "sc_cpp";
    argv[1] = (char *) malloc((strlen(Use_c_cpp)+1)*sizeof(char));
    strcpy(argv[1],Use_c_cpp);
    argv[2] = "-o";
    argv[argc] = tmp_file; argc++;
    argc = SetArgv(argv,loaded_files,argc,MAXARGV,&err);
    if ( err == 1 ) return(-1);
    if (libs) 
      {
	argc = SetArgv1(argv,libs,argc,MAXARGV,&err);
	if ( err == 1 ) return(-1);
      }
    argv[argc] = (char *) 0;
    
#ifdef DEBUG
     for ( i=0 ; i < argc ; i++) 
       sciprint("arg[%d]=%s\r\n",i,argv[i]);
#endif	
     exec_cpp = (char *) malloc((strlen(getenv("SCI")) + 16) * 
			     sizeof(char));
     strcpy(exec_cpp,getenv("SCI"));
     strcat(exec_cpp,"/scripts/sc_cpp");

     if ((pid = vfork()) == 0) {
       execv(exec_cpp,argv);
       _exit(1);
     }
     if (pid < 0) {
       sciprint("can't create new process: \r\n");
       return(-1);
     }
     while ((wpid = wait(&status)) != pid)
       if (wpid < 0) {
	 sciprint("no child !\r\n");
	 return(-1);
       }
     if (status != 0) {
       sciprint("collect2 returned bad status: %x\r\n", status);
       return(-1);
     }
  }

   return 0;
}


/*************************************
 * This routine load the entryname ename 
 *     from shared lib ishared 
 * return FAIL or OK 
 *************************************/

static int Sci_dlsym(ename,ishared,strf)
     int ishared;
     char *ename;
     char *strf;
{
#ifdef hppa
  shl_t hd1;
  int irep = 0;
#endif
  int ish = Min(Max(0,ishared),ENTRYMAX-1);
  char enamebuf[MAXNAME];
  if ( strf[0] == 'f' )
    Underscores(1,ename,enamebuf);
  else 
    Underscores(0,ename,enamebuf);

  /* lookup the address of the function to be called */
  if ( NEpoints == ENTRYMAX ) 
    {
      sciprint("You can't link more functions maxentry %d reached\r\n",ENTRYMAX);
      return(FAIL);
    }
  if ( hd[ish].ok == FAIL ) 
    {
      sciprint("Shared lib %d does not exists\r\n",ish);
      return(FAIL);
    }
  /** entry was previously loaded **/
  if ( SearchFandS(ename,ish) >= 0 ) 
    {
      sciprint("Entry name %s is already loaded from lib %d\r\n",ename,ish);
      return(OK);
    }
#ifndef hppa
  EP[NEpoints].epoint = (function) dlsym((void *) hd[ish].shl, enamebuf);
  if ( (unsigned long) EP[NEpoints].epoint == (unsigned long) 0 )

#else
  hd1 = (shl_t)  hd[ish].shl;
  irep= shl_findsym(&hd1, enamebuf,TYPE_PROCEDURE,&(EP[NEpoints].epoint));
  if ( irep == -1 )
#endif
    {
#ifdef linux 
      const char *loc;
#else
      char *loc;
#endif
      sciprint("%s is not an entry point \r\n",enamebuf);
#ifndef hppa
      loc = dlerror();
      if ( loc != NULL) sciprint("%s \r\n",loc);
#else
      sciprint("link error\r\n");
#endif
      return(FAIL);
    }
  else 
    {
      /* we don't add the _ in the table */
      sciprint("Linking %s \r\n",ename);
      strncpy(EP[NEpoints].name,ename,MAXNAME);
      EP[NEpoints].Nshared = ish;
      NEpoints++;
    }
  return(OK);  
}


/***************************************************
 * Delete entry points associated with shared lib ishared
 * then delete the shared lib 
 ****************************************************/


static void Sci_Delsym( ishared) 
     int ishared;
{
  int ish = Min(Max(0,ishared),ENTRYMAX-1);
  int i=0;
  for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
      if ( EP[i].Nshared == ish )
	{
	  int j;
	  for ( j = i ; j <= NEpoints - 2 ; j++ )
	    {
	      EP[j].epoint = EP[j+1].epoint;
	      EP[j].Nshared = EP[j+1].Nshared;
	      strcpy(EP[j].name,EP[j+1].name);
	    }
	  NEpoints--;
	}
    }
  if ( hd[ish].ok != FAIL)
    {
#ifndef hppa
      dlclose((void *) hd[ish].shl);
#else
      shl_unload((shl_t)  hd[ish].shl);
#endif
      /* Warning : two cases are possible here 
       *  the .so archive was created by the user and we must no delete it 
       *  or the .so archive was built during link call and we must delete it. 
       *  if the shared object .so name begins with "/tmp/SD_......" we assume 
       *  that we are in the second case 
       */
      if ( strncmp(hd[ish].tmp_file,"/tmp/SD_",8)==0) 
	{
	  unlink(hd[ish].tmp_file);
	}
      hd[ish].ok = FAIL;
    }
}

  
/******************************************************
 * Utility function 
 * files is a null terminated array of char pointers 
 * files[i] is broken into pieces ( separated by ' ') 
 * and positions are stored in argv starting at position 
 * first 
 *******************************************************/

static int SetArgv(argv,files,first,max,err)
     char *argv[];
     char *files[];
     int first,max,*err;
{
  int i=0,j=first;
  *err=0;
  while ( files[i] != (char *) NULL) 
    {
      j= SetArgv1(argv,files[i],j,max,err);
      if (*err == 1) return(j);
      i++;
    }
  return(j);
}

static int SetArgv1(argv,files,first,max,err)
     char *argv[];
     char *files;
     int first,max,*err;
{
  int j=first;
  char *loc = files;
  while ( *loc == ' ' && *loc != '\0'  ) loc++;
  while ( *loc != '\0' )
    {
      argv[j] = loc; j++;
      if ( j == max ) 
	{
	  sciprint("Link too many files \r\n");
	  *err=1;
	  break;
	}
      if ( ( loc  = strchr(loc, ' ')) != (char *) 0) 
	{ 
	  *loc = 0;	loc++;
	  while ( *loc == ' ' && *loc != '\0'  ) loc++;
	}
      else
	{
	  break;
	}
    }
  return(j);
}
  
