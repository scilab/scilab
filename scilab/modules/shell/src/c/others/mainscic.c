/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CONRET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "core_math.h"
#include "version.h"
#include "realmain.h" /* realmain */
#include "scimem.h" /* nofpex */
#include "scilabmode.h"
#include "getcommandlineargs.h"
#include "texmacs.h"
#include "x_main.h"
#include "LaunchScilabSignal.h"
#include "Thread_Wrapper.h"
#include "core_math.h"
#include "setgetlanguage.h"
#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

#ifdef __APPLE__
#include <pthread.h>
#include <CoreFoundation/CoreFoundation.h>
#include <sys/stat.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

/*--------------------------------------------------------------------------*/
#define MIN_STACKSIZE 180000
/*--------------------------------------------------------------------------*/
int  sci_show_banner=1;
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
__threadSignal	LaunchScilab;
__threadLock	LaunchScilabLock;
/*--------------------------------------------------------------------------*/

#ifdef __APPLE__

typedef struct {
  int   no_startup_flag_l;
  char  *initial_script;
  InitScriptType initial_script_type;
  int memory;
} thread_parm_t;

/*
Some parts of the next three functions have been taken from simpleJavaLauncher.

                        
  Copyright:   © Copyright 2003 Apple Computer, Inc. All rights reserved.
  
  Disclaimer:  IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
        ("Apple") in consideration of your agreement to the following terms, and your
        use, installation, modification or redistribution of this Apple software
        constitutes acceptance of these terms.  If you do not agree with these terms,
        please do not use, install, modify or redistribute this Apple software.

        In consideration of your agreement to abide by the following terms, and subject
        to these terms, Apple grants you a personal, non-exclusive license, under Apple's
        copyrights in this original Apple software (the "Apple Software"), to use,
        reproduce, modify and redistribute the Apple Software, with or without
        modifications, in source and/or binary forms; provided that if you redistribute
        the Apple Software in its entirety and without modifications, you must retain
        this notice and the following text and disclaimers in all such redistributions of
        the Apple Software.  Neither the name, trademarks, service marks or logos of
        Apple Computer, Inc. may be used to endorse or promote products derived from the
        Apple Software without specific prior written permission from Apple.  Except as
        expressly stated in this notice, no other rights or licenses, express or implied,
        are granted by Apple herein, including but not limited to any patent rights that
        may be infringed by your derivative works or by other works in which the Apple
        Software may be incorporated.

        The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
        WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
        WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
        PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
        COMBINATION WITH YOUR PRODUCTS.

        IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
        CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
        GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
        ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
        OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
        (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
        ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** 
 * Set the name of the application (the mac os x way)
 * @param name the name of the application
 */
static void setAppName(const char * name) {
  char a[32];
  pid_t id = getpid();
  sprintf(a,"APP_NAME_%ld",(long)id);
  setenv(a, name, 1);
}

/**
 * Actually launch Scilab under Mac OS X. Need to be in a specific thread.
 * See http://wiki.scilab.org/Compiling_Scilab_5.x_under_MacOSX
 * @param param The structure containing the missing argument mandatory for realmain
 * @return the result of the operation (0 if OK ...)
 */
static int *launchMacOSXEnv(thread_parm_t *param){
  thread_parm_t *p = (thread_parm_t *)param;
  int ret=-1;
  {
    CFStringRef targetJVM = CFSTR("1.5");
    CFBundleRef JavaVMBundle;
    CFURLRef    JavaVMBundleURL;
    CFURLRef    JavaVMBundlerVersionsDirURL;
    CFURLRef    TargetJavaVM;
    UInt8 pathToTargetJVM [PATH_MAX] = "\0";
    struct stat sbuf;

    /*
     * This piece of code is mandatory because Mac OS X implementation of Java has a bug here.
     * Cocoa does not know how to handle the new window created this way.
     * See: http://lists.apple.com/archives/Java-dev/2009/Jan/msg00062.html
     * Or Mac Os X bug #6484319
     * Thanks to Mike Swingler
     */
    ProcessSerialNumber psn;
    GetCurrentProcess(&psn);
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    /* End of the workaround */

    // Look for the JavaVM bundle using its identifier
    JavaVMBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.JavaVM") );

    if(JavaVMBundle != NULL) {
      // Get a path for the JavaVM bundle
      JavaVMBundleURL = CFBundleCopyBundleURL(JavaVMBundle);
      CFRelease(JavaVMBundle);

      if(JavaVMBundleURL != NULL) {
        // Append to the path the Versions Component
        JavaVMBundlerVersionsDirURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundleURL,CFSTR("Versions"),true);
        CFRelease(JavaVMBundleURL);

        if(JavaVMBundlerVersionsDirURL != NULL) {
          // Append to the path the target JVM's Version
          TargetJavaVM = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundlerVersionsDirURL,targetJVM,true);
          CFRelease(JavaVMBundlerVersionsDirURL);
          if(TargetJavaVM != NULL) {
            if(CFURLGetFileSystemRepresentation (TargetJavaVM,true,pathToTargetJVM,PATH_MAX )) {
              // Check to see if the directory, or a sym link for the target JVM directory exists, and if so set the
              // environment variable JAVA_JVM_VERSION to the target JVM.
              if(stat((char*)pathToTargetJVM,&sbuf) == 0) {
                // Ok, the directory exists, so now we need to set the environment var JAVA_JVM_VERSION to the CFSTR targetJVM
                // We can reuse the pathToTargetJVM buffer to set the environement var.
                if(CFStringGetCString(targetJVM,(char*)pathToTargetJVM,PATH_MAX,kCFStringEncodingUTF8)) {
                  setenv("JAVA_JVM_VERSION", (char*)pathToTargetJVM,1);
		  ret=0;
		} else {
		  fprintf(stderr, "Could not get the path to the target JVM.\n");
		}
	      } else {
		fprintf(stderr, "Error checking symlink for the target jvm.\n");
	      }
	    } else {
	      fprintf(stderr, "Error getting file system representation for bundle url.\n");
	      CFRelease(TargetJavaVM);
	    }
	  } else {
	    fprintf(stderr, "Error appending version component to bundle url.\n");
	  }
	} else {
	  fprintf(stderr, "Error appending path component to bundle url.\n");
	}
      } else {
	fprintf(stderr, "Error copying bundle url.\n");
      }
    } else {
      fprintf(stderr, "Error: cant find bundle: com.apple.JavaVM.\n");
    }
  }

  if (ret==0) {
    /* Call the actual startup script of Scilab */
    ret=realmain(p->no_startup_flag_l, p->initial_script, p->initial_script_type, p->memory);
  }
    free(p);
  return ret;

} 

/* call back for dummy source used to make sure the CFRunLoop doesn't exit right away */
/* This callback is called when the source has fired. */
static void sourceCallBack (  void *info  ) {}

/* Specific wrapper for mac os X which is going to call realmin in a specific thread.
 * Takes the same args as realmain 
 */ 
static int initMacOSXEnv(int no_startup_flag_l, char *initial_script, InitScriptType initial_script_type, int memory){

  CFRunLoopSourceContext sourceContext;
  /* Start the thread that runs the VM. */
  pthread_t vmthread;
  setAppName("Scilab");

  /* Create the structure which is going to be giving to the function inside the thread */
  thread_parm_t         *param=NULL;
  param = malloc(sizeof(thread_parm_t));
  param->no_startup_flag_l = no_startup_flag_l;
  param->initial_script = initial_script;
  param->initial_script_type = initial_script_type;
  param->memory = memory;

  /* create a new pthread copying the stack size of the primordial pthread */
  struct rlimit limit;
  size_t stack_size = 0;
  int rc = getrlimit(RLIMIT_STACK, &limit);
  if (rc == 0) {
    if (limit.rlim_cur != 0LL) {
      stack_size = (size_t)limit.rlim_cur;
    }
  }
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);
  pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
  if (stack_size > 0) {
    pthread_attr_setstacksize(&thread_attr, stack_size);
  }

  /* Start the thread that we will start the JVM on. */
  pthread_create(&vmthread, &thread_attr,  launchMacOSXEnv, (void*)param);
  pthread_attr_destroy(&thread_attr);

  /* Create a a sourceContext to be used by our source that makes */
  /* sure the CFRunLoop doesn't exit right away */
  sourceContext.version = 0;
  sourceContext.info = NULL;
  sourceContext.retain = NULL;
  sourceContext.release = NULL;
  sourceContext.copyDescription = NULL;
  sourceContext.equal = NULL;
  sourceContext.hash = NULL;
  sourceContext.schedule = NULL;
  sourceContext.cancel = NULL;
  sourceContext.perform = &sourceCallBack;

  /* Create the Source from the sourceContext */
  CFRunLoopSourceRef sourceRef = CFRunLoopSourceCreate (NULL, 0, &sourceContext);

  /* Use the constant kCFRunLoopCommonModes to add the source to the set of objects */
  /* monitored by all the common modes */
  CFRunLoopAddSource (CFRunLoopGetCurrent(),sourceRef,kCFRunLoopCommonModes);

  /* Park this thread in the runloop */
  CFRunLoopRun();

  return 0;
}
#endif


int main(int argc, char **argv)
{
  int i;
  int  no_startup_flag=0;
  int  memory = 8000000; // MIN_STACKSIZE;

  char * initial_script = NULL;
  InitScriptType initial_script_type = SCILAB_SCRIPT;
  /* This bug only occurs under Linux 32 bits */
#if defined(linux) && defined(__i386__)
  setFPUToDouble();
#endif

  InitializeLaunchScilabSignal();

  #if (defined __GNUC__  )
		putenv ("COMPILER=gcc");
	#else
		putenv ("COMPILER=cc or another");
	#endif

#if defined(netbsd) || defined(freebsd)
/* floating point exceptions */
fpsetmask(0);
#endif

#ifdef WITHOUT_GUI
  setScilabMode(SCILAB_NWNI);
#else
  setScilabMode(SCILAB_STD);
#endif

  setCommandLineArgs(argv, argc);


  /* scanning options */
  for ( i=0 ; i < argc ; i++)
  {
      if ( strcmp(argv[i],"-nw") == 0)
      {
      	setScilabMode(SCILAB_NW);
      }
      else if ( strcmp(argv[i],"-nwni") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-display") == 0 || strcmp(argv[i],"-d") == 0)
      {
		  /* @TODO Buffer overflow here */
		  char dpy[128];
		  sprintf(dpy,"DISPLAY=%s",argv[++i]);
		  putenv(dpy);
      }
      else if ( strcmp(argv[i],"-l") == 0)
      {
		  /* @TODO Buffer overflow here */
		  char lang[128];
		  char *argLang=strdup(argv[++i]);

		  /* Export the locale. This is going to be used by setlanguage("") in
			 modules/localization/src/c/InitializeLocalization.c */
		  if (strcmp(argLang,"en")==0) {/* backward compatiblity en => en_US */
			  setenvc("LANG","en_US"); 
		  }else{
			  if (strcmp(argLang,"fr")==0) { /* backward compatiblity fr => fr_FR */
				  setenvc("LANG","fr_FR");
			  }else{
				  setenvc("LANG",argLang);
			  }
		  }
      }
      else if ( strcmp(argv[i],"-nb") == 0)  { sci_show_banner = 0; }
      else if ( strcmp(argv[i],"-ns") == 0)  { no_startup_flag = 1;}
      else if ( strcmp(argv[i],"-mem") == 0) { i++; memory = Max(atoi(argv[i]),MIN_STACKSIZE );}
      else if ( strcmp(argv[i],"-f") == 0)   { initial_script = argv[++i];}
      else if ( strcmp(argv[i],"-e") == 0)
      {
		  initial_script = argv[++i];
		  initial_script_type = SCILAB_CODE;
      }
      else if ( strcmp(argv[i],"--texmacs") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      	settexmacs();
      }
      else if ( strcmp(argv[i],"-nogui") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-version") == 0) {disp_scilab_version();exit(1);}
    }

#ifndef WITH_GUI
  if(getScilabMode() != SCILAB_NWNI)
  {
    fprintf(stderr, "Scilab was compiled without its GUI. Run scilab with the -nwni option.\n");
    exit(1);
  }
#endif

#ifndef __APPLE__
  return realmain(no_startup_flag,initial_script,initial_script_type,memory);
#else
  /* Mac OS X doesn't work the same way as Microsoft Windows or GNU/Linux */
  return initMacOSXEnv(no_startup_flag,initial_script,initial_script_type,memory);
#endif
}
/*--------------------------------------------------------------------------*/
