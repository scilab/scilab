            Scilab version 4.1.1 for Windows (2000/XP/Vista)
            ************************************************

******************************************************************************
0 - COPYRIGHT NOTICE
******************************************************************************

 Scilab is free software.
 See the included license: "license.txt" in English
                           "licence.txt" in French

******************************************************************************
I - TO RUN SCILAB (BINARY VERSION)
******************************************************************************

    1 - You have already installed Scilab by executing "scilab-4.1.1.exe".
        By default Scilab is in "C:\Program Files\scilab-4.1.1" directory.

    2 - Run Scilab by executing "Scilab 4.1.1" from the startup menu.
        In fact, it is a link to "wscilex.exe" in Scilab subdirectory "bin",
        so you can run Scilab by executing directly "wscilex.exe".

    3 - Scilab has been compiled with Visual C++ .NET 2003
        (Intel C 9.0 & Fortran 9.0, see III below).
        All examples of incremental link given with Scilab are prepared
        for Visual C++.

    4 - If you want use Microsoft Visual C++ 2005 Express with Scilab 4.1.1
        download and install "Windows Server 2003 SP1 Platform SDK"
        at http://www.microsoft.com/downloads
        and download and install "Microsoft Visual C++ 2005 Express"
        at http://msdn.microsoft.com/vstudio/express/visualc/

    5 - If you want use LCC-Win32 (C compiler) with Scilab 4.1.1
        See Readme_LCC.txt in lcc directory.
        You can download at http://www.cs.virginia.edu/~lcc-win32/

    6 - If you have any problems or suggestions concerning Scilab,
        http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi
        or better post a message to newsgroup:
        comp.soft-sys.math.scilab

******************************************************************************
II - TO UNINSTALL SCILAB (BINARY VERSION)
******************************************************************************

    To uninstall Scilab you can use the unintaller. You will find it in the
    "Scilab 4.1.1" item of the Program Group.
    You can also use the Add/Remove Programs of the Control Panel.

******************************************************************************
III - TO COMPILE SCILAB FROM A SOURCE VERSION
******************************************************************************

    We have compiled this distribution with Visual Studio 2003 (Intel C 9.0 &
    Fortran 9.0).

    1 - To compile with Visual C++ .NET 2003 & Intel Fortran 9.0,
        see Readme_visual.txt 

    2 - To compile with  Visual C++ 6.0
        type "make all". (use f2c to translate fortran to c.)


******************************************************************************
IV - DOCUMENTATION
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
V - SOME IMPORTANT FILES AND DIRECTORIES
******************************************************************************

 scilab.star          : startup file for Scilab instructions in this file
                        are executed when Scilab is executed. Note that
                        you can also have your own startup file "scilab.ini"
                        in your current directory.

 Makefile.incl.mak    : file included by all Makefile's.

 Makefile.mak         : the main Makefile to make Scilab.

 config/Makefile.mak  : directory used by the main Makefile

 demos/               : demos directory. Some of the demos can be executed by
                        clicking on the "demos" button.

 examples/            : examples of dynamic links and interface code.

 macros/              : functions directory: this directory is divided into 
                        subdirectories corresponding to specific topics
                        (control, polynomials,...). Each subdirectory contains
                        the source code of the macros (files *.sci). 

 routines/            : directory of fortran or C routines.

 man/                 : files for online help

 util/                : useful routines and ASCII files to manage Scilab.

 bin/                 : executable code and scripts.
                        scilex : executable code of Scilab (Console version).
                        wscilex : executable code of Scilab (Windows version).

 intersci/            : Generator of interface program between Scilab and
                        routines.

 maple/               : Maple code to link Maple with Scilab.

 pvm3/                : PVM 3 for Scilab.

 tcl/                 : TCL/TK for Scilab.

 ******************************************************************************
