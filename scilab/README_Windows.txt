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
 
    1 - You have already installed Scilab by executing "scilab-5.2.exe".
        By default Scilab is in "C:\Program Files\scilab-5.2" directory.
 
    2 - Run Scilab by executing "Scilab 5.2" from the startup menu.
        In fact, it is a link to "wscilex.exe" in Scilab subdirectory "bin", 
        so you can run Scilab by executing directly "wscilex.exe".
 
    3 - Scilab has been compiled with Visual C++ .NET 2008 Pro
	      (Intel C 11.0 & Fortran 11.0, see III below).
        All examples of dynamic link given with Scilab are prepared
        for Visual C++.
 
    4 - If you want use Microsoft Visual C++  2008 Express with Scilab 5.2
        http://www.microsoft.com/express/vc/
 
    5 - If you want use LCC-Win32 (C compiler) with Scilab 5.2
        See :
        http://wiki.scilab.org/Using_LCC-Win32_compiler_on_Windows_for_incremental_link_with_Scilab_5.x/
        You can download at http://www.cs.virginia.edu/~lcc-win32/
    
    6 - If you have any problems or suggestions concerning Scilab,
        http://bugzilla.scilab.org/index.cgi        


******************************************************************************
II - TO UNINSTALL SCILAB (BINARY VERSION)
******************************************************************************
 
    To uninstall Scilab you can use the unintaller. You will find it in the 
    "Scilab 5.2" item of the Program Group.
    You can also use the Add/Remove Programs of the Control Panel.
 
 
******************************************************************************
III - TO COMPILE SCILAB FROM A SOURCE VERSION
******************************************************************************
 
    We have compiled this distribution with Visual Studio 2008 (Intel C 11.0 &
    Fortran 11.0). 

    To compile with Visual C++ .NET 2008,  see Readme_visual.txt 
 
******************************************************************************
IV - DOCUMENTATION
******************************************************************************
 
 In addition to Scilab online documentation (obtained by typing "help item" or
 clicking on the help button), you will find scilab wiki :
 http://wiki.scilab.org/

******************************************************************************
V - SOME IMPORTANT FILES AND DIRECTORIES
******************************************************************************
 
 etc/scilab.start : startup file for Scilab instructions in this file
              are executed when Scilab is executed. Note that
              you can also have your own startup file "scilab.ini" 
              in your SCIHOME directory.
 
 
 Scilab.sln : Visual Studio 2008 Scilab Solution with Intel Fortran.
 
 Scilab_f2c.sln : Visual Studio 2008 Scilab Solution without Intel Fortran.
    
 bin/	   : executable code and scripts.

 scilex  : executable code of Scilab (Console version).
 wscilex  : executable code of Scilab (Windows version).
  
******************************************************************************

		       