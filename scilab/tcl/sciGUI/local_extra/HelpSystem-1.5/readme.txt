26/5/2003
HelpSystem 1.5
Copyright (c) 2000-2003 Andrei Gratchev. 


--------------------------------------------------------------------------
What is HelpSystem?

The HelpSystem is Help browser for your Tk programms. Source of help files 
is based on HTML language. Pure HTML files are supported also (with some 
restrictions). Can be used as stand-alone or embedded in your programm.

Main features of HelpSystem: 
 - shows hyper-text help based on HTML 
 - auto creates index of topics (for .help files) 
 - can auto create contents and index of topics (for .html files) 
 - saves history of showed topics 
 - interactive showing 
 - can be embedded or separated window 
 - and more...


--------------------------------------------------------------------------
Installation and use

- On Unix Platform:
  Uncompress the file HelpSystem-1.5.tar.gz

  To use the HelpSystem:
  - If you have uncompressed the archive file under the Tcl Library Path
    directory, you only need to do:
      % package require help
  - If not, you have to specify the HelpSystem installation path in 
    auto_path global variable:
      % lappend auto_path <install_path>
      % package require help

  To launch the demos, you need to cd into the demo subdirectory:
      $ cd <install_path>/demo
  Change directory to appropriated demo:
      $ cd <demo_name>
  And execute "demo.tcl":
      $ ./demo.tcl
      or
      $ wish demo.tcl

- On Windows and others Platforms:
  Uncompress the file HelpSystem-1.5.zip

  To use the HelpSystem:
  - If you have uncompressed the archive file under the Tcl Library Path
    directory, you only need to do:
      % package require help
  - If not, you have to specify the HelpSystem installation path in 
    auto_path global variable:
      % lappend auto_path <install_path>
      % package require help

  To launch the demo :
      Double click on "demo.tcl" in the appropriated folder in demo 
      subdirectory

  NOTE: You must have Tcl/Tk 8.1 distribution package installed.

Distribution contains these directories:

HelpSystem-1.5  Root directory and HelpSystem Tcl sources
   help         HelpSystem's manual with browser
   browser      Simple browser of help files
   demo         Demo sources


--------------------------------------------------------------------------
Documentation

HelpSystem's manual are available in the help subdirectory.
Change directory to "<install_path>/help" and launch "help.tcl".


--------------------------------------------------------------------------
Contacts

Mail         mailto:grand@deversys.com
Home page    http://midc.miem.edu.ru/HelpSystem/

Bug Report and FAQ : See Home Page



