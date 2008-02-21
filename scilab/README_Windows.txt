                Scilab for Windows (2000/XP/Vista)
                *******************************
 
******************************************************************************
0 - COPYRIGHT NOTICE
******************************************************************************
 
 Scilab is free software. 
 See the included license: "license.txt" in English
                           "licence.txt" in French
 
******************************************************************************
I - TO RUN SCILAB (BINARY VERSION)
******************************************************************************
 
    1 - You have already installed Scilab by executing "scilab-5.0.exe".
        By default Scilab is in "C:\Program Files\scilab-5.0" directory.
 
    2 - Run Scilab by executing "Scilab 5.0" from the startup menu.
        In fact, it is a link to "wscilex.exe" in Scilab subdirectory "bin", 
        so you can run Scilab by executing directly "wscilex.exe".
 
    3 - Scilab has been compiled with Visual C++ .NET 2005 Pro
	      (Intel C 10 & Fortran 10, see III below).
        All examples of dynamic link given with Scilab are prepared
        for Visual C++.
 
    4 - If you want use Microsoft Visual C++ 2005 or 2008 Express with Scilab 5.0
        download and install "Microsoft Plateform SDK R2"
        http://msdn.microsoft.com/vstudio/express/visualc/usingpsdk/
 
    5 - If you want use LCC-Win32 (C compiler) with Scilab 5.0
        See :
        http://wiki.scilab.org/Using_LCC-Win32_compiler_on_Windows_for_incremental_link_with_Scilab_5.x/
        You can download at http://www.cs.virginia.edu/~lcc-win32/
    
    6 - If you have any problems or suggestions concerning Scilab,
        http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi        
        or better post a message to newsgroup:
                         comp.soft-sys.math.scilab


******************************************************************************
II - TO UNINSTALL SCILAB (BINARY VERSION)
******************************************************************************
 
    To uninstall Scilab you can use the unintaller. You will find it in the 
    "Scilab 5.0" item of the Program Group.
    You can also use the Add/Remove Programs of the Control Panel.
 
 
******************************************************************************
III - TO COMPILE SCILAB FROM A SOURCE VERSION
******************************************************************************
 
    We have compiled this distribution with Visual Studio 2005 (Intel C 10 &
    Fortran 10). 

    To compile with Visual C++ .NET 2005,  see Readme_visual.txt 
 
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
 
 etc/scilab.start : startup file for Scilab instructions in this file
              are executed when Scilab is executed. Note that
              you can also have your own startup file "scilab.ini" 
              in your SCIHOME directory.
 
 Makefile.incl.mak: file included by all Makefile's.
 
 Scilab.sln : Visual Studio 2005 Scilab Solution with Intel Fortran.
 
 Scilab_f2c.sln : Visual Studio 2005 Scilab Solution without Intel Fortran.
    
 bin/	   : executable code and scripts.

 		scilex  : executable code of Scilab (Console version).
		wscilex  : executable code of Scilab (Windows version).
  
 ******************************************************************************

		       