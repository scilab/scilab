                Scilab for Windows (XP/Vista/Seven)
                *******************************

******************************************************************************
0 - COPYRIGHT NOTICE
******************************************************************************

 Scilab is a free software released under the terms of the CeCILL license.
 This license is compatibility with the GNU General Public License (GPL).
 See the included license: "COPYING" in English
                           "COPYING-FR" in French

 Scilab is also using some codes with other licenses. Please see
 SCI/modules/*/license.txt for details.

******************************************************************************
I - TO RUN SCILAB (BINARY VERSION)
******************************************************************************

    1 - You have already installed Scilab by executing "scilab-5.3.exe".
        By default Scilab is in "C:\Program Files\scilab-5.3" directory.

    2 - Run Scilab by executing "Scilab 5.3" from the startup menu.
        In fact, it is a link to "wscilex.exe" in Scilab subdirectory "bin",
        so you can run Scilab by executing directly "wscilex.exe".

    3 - Scilab has been compiled with Visual C++ .NET 2008 Pro
          (Intel C 2011 XE & Fortran 2011 XE, see III below).
        All examples of dynamic link given with Scilab are prepared
        for Visual C++.

    4 - If you want use Microsoft Visual C++ 2008 Express with Scilab 5.3
        http://www.microsoft.com/express/vc/

    5 - If you have any problems or suggestions concerning Scilab,
        http://bugzilla.scilab.org/index.cgi


******************************************************************************
II - TO UNINSTALL SCILAB (BINARY VERSION)
******************************************************************************

    To uninstall Scilab you can use the unintaller. You will find it in the
    "Scilab 5.3" item of the Program Group.
    You can also use the Add/Remove Programs of the Control Panel.


******************************************************************************
III - TO COMPILE SCILAB FROM A SOURCE VERSION
******************************************************************************

    We have compiled this distribution with Visual Studio 2008 (Intel C 2011 XE &
    Fortran 2011 XE).

    To compile with Visual C++ .NET 2008,  see Readme_visual.txt

******************************************************************************
IV - DOCUMENTATION
******************************************************************************

 In addition to Scilab online documentation (obtained by typing "help item" or
 clicking on the help button).

 An online help is also available:
 http://help.scilab.org/

 You will find scilab wiki :
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

 bin/      : executable code and scripts.

 scilex  : executable code of Scilab (Console version).
 wscilex  : executable code of Scilab (Windows version).

******************************************************************************
VI - OTHER RESOURCES
******************************************************************************

  1 - If you have any problem or suggestion to improve Scilab
    http://bugzilla.scilab.org/

    Don't hesitate to browse other the bug reports to see workarounds.

  2 - User and development mailing lists are available:

    http://www.scilab.org/communities/developer_zone/tools/mailing_list
    Archives of these mailing lists are available:
    http://mailinglists.scilab.org/

  3 - Exchange files, script, data, experiences, etc:
    http://fileexchange.scilab.org/

  4 - Browse our packages available through ATOMS:
    http://atoms.scilab.org/
******************************************************************************
