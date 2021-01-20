/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021 - Stépahen Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v3.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */


/* AdvCLIManagement.i */
/** 
 * Windows: swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ AdvCLIManagement.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module AdvCLIManagement
%{
extern char * GetCurrentPrompt();
%}

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("sciconsole");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        System.exit(-1);
    } catch (UnsatisfiedLinkError e)    {
           System.err.println("The native library sciconsole does not exist or cannot be found.");
        if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
           System.err.println(e.getLocalizedMessage());
           System.err.println("Current java.library.path is : "+System.getProperty("java.library.path"));
           System.exit(-1);
        }else{
           System.err.println("Continuing anyway because of CONTINUE_ON_JNI_ERROR");
        }
    }
  }
%}

extern char * GetCurrentPrompt();



