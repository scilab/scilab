                Scilab version 3.0 for Windows (95/98/NT/2000/XP)
                *************************************************
 
******************************************************************************
0 - COPYRIGHT NOTICE
******************************************************************************
 
 Scilab is free software. 
 See the included license: "license.txt" in English
                           "licence.txt" in French
 
******************************************************************************
I - TO RUN SCILAB (BINARY VERSION)
******************************************************************************
 
    1 - You have already installed Scilab by executing "scilab-3.0.exe".
        By default Scilab is in "C:\Program Files\scilab-3.0" directory.
 
    2 - Run Scilab by executing "Scilab 3.0" from the startup menu.
        In fact, it is a link to "wscilex.exe" in Scilab subdirectory "bin", 
        so you can run Scilab by executing directly "wscilex.exe",
	or "Scilab.bat".
	If you want use Visual Studio (6,.NET) with Scilab 3.0,
	execute "Scilab 3.0 (requires Visual Studio)" from the startup menu.
 
    3 - Scilab has been compiled with Visual C++ .NET 2003
	(Intel C 8.0 & Fortran 8.0, see IV below).
        All examples of incremental link given with Scilab are prepared
        for Visual C++.
 
    4 - If you want use LCC-Win32 (C compiler) with Scilab 3.0
        See Readme_LCC.txt in lcc directory.
        You can download at http://www.cs.virginia.edu/~lcc-win32/
    
    5 - If you have any problems or suggestions concerning Scilab,
        http://www.scilab.org/cgi-bin/bugzilla_bug/index.cgi        
        or better post a message to newsgroup:
  	comp.soft-sys.math.scilab


******************************************************************************
II - TO UNINSTALL SCILAB (BINARY VERSION)
******************************************************************************
 
    To uninstall Scilab you can use the unintaller. You will find it in the 
    "Scilab 3.0" item of the Program Group.
    You can also use the Add/Remove Programs of the Control Panel.
 
******************************************************************************
III - TO CUSTOMIZE SCILAB FONTS, WINDOW SIZE and SCILAB MENUS 
******************************************************************************
 
    When you enter Scilab for the first time the window may be too
    large and the fonts not properly choosen.  Just change this
    with mouse and menus (right mouse click in the scilab window),
    when you close scilab your changes are saved.
    	
    For the popup help, just change the size and it will keep it for 
    next calls.
 
    You can also edit the files "bin/wscilabE.mnu" and
    "bin/wgscilabE.mnu" to customize the Scilab menus. It's quite
    easy to change them.
 
******************************************************************************
IV - TO COMPILE SCILAB FROM A SOURCE VERSION
******************************************************************************
 
    We have compiled this distribution with Visual Studio 2003 (Intel C 8.0 &
    Fortran 8.0). It is possible to compile it with egcs or Cygwin compiler.

    1 - To compile with Visual C++.NET 2003 & Intel Fortran 8.0,
        see Readme_visual.txt 
 
    2 - To compile with Visual C++ 4.0,  5.0 or 6.0, edit the beginning of the 
        file "Makefile.incl.mak". Then type "nmake /f Makefile.mak".
 
        If you want to compile with TCL/TK interface, you need to uncomment
        the corresponding lines in "Makefile.incl.mak". 
 
        If you want to compile with PVM interface, you need to uncomment
        the corresponding lines in "Makefile.incl.mak". You also need to modify
        the pathnames of the compiler in the file "conf/WIN32.def" of PVM.
 

 
******************************************************************************
V - DOCUMENTATION
******************************************************************************
 
 In addition to Scilab online documentation (obtained by typing "help item" or
 "apropos keyword" or clicking on the help button), you will find 
 compressed postscript and pdf documentation by anonymous ftp on
 "ftp.inria.fr:/INRIA/Projects/Scilab"
 or from Web site
 "http://www.scilab.org":
 
 Internals.ps.gz, Internals.pdf: Scilab's internals
 Intro.ps.gz, Intro.pdf: Introduction to Scilab
 Lmi.ps.gz, Lmi.pdf: Linear Matrix Inequalities Optimization Toolbox
 Manual.ps.gz, Manual.pdf: On line Manual
 Metanet.ps.gz, Metanet.pdf: Metanet User's Guide and Tutorial
 Scicos.ps.gz, Scicos.pdf: Scicos: a Dynamic System Builder and Simulator 
 Signal.ps.gz, Signal.pdf: Signal Processing Toolbox
 Intersci.ps.gz, Intersci.pdf: Interfacing Tool
 
 Note that these documents are not uptodate.

******************************************************************************
VI - SOME IMPORTANT FILES AND DIRECTORIES
******************************************************************************
 
 scilab.star: startup file for Scilab instructions in this file
              are executed when Scilab is executed. Note that
              you can also have your own startup file "scilab.ini" 
              in your current directory.
 
 Makefile.incl.mak: file included by all Makefile's.
 
 Version.incl: contains the scilab version. It is used by a few Makefile.
 
 Makefile.mak: the main Makefile to make Scilab.
 
 config/Makefile.mak  : directory used by the main Makefile
 
 demos/    : demos directory. Some of the demos can be executed by clicking
             on the "demos" button.
 
 examples/ : examples of dynamic links and interface code.
 
 macros/   : functions directory: this directory is divided into 
             subdirectories corresponding to specific topics
             (control, polynomials,...). Each subdirectory contains 
             the source code of the macros (files *.sci). 
 
 libs/	   : directory of libraries: archives of object files needed 
             to link Scilab.
 
 routines/ : directory of fortran or C routines.
 
 man/	   : files for online help
 
 tests/    : set of exec files to test Scilab.
 
 util/     : useful routines and ASCII files to manage Scilab.
 
 bin/	   : executable code and scripts.

 		scilab.bat : shell-script to call Scilab.
 		scilex  : executable code of Scilab (Console version).
		wscilex  : executable code of Scilab (Windows version).
        	intersci: generator of interface program between Scilab and 
                           routines.
 
 intersci/ : Generator of interface program between Scilab and routines.
 
 maple/    : Maple code to link Maple with Scilab.
 
 pvm3/	  : PVM 3 for Scilab.
 
 tcl/      : TCL/TK for Scilab.
 
 ******************************************************************************
 			       THAT'S ALL FOLKS
 ******************************************************************************
