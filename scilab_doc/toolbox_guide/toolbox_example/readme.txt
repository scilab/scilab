                Guide for the toolboxes contributions
                =====================================



Guide (version xxx)
====================

Information about mytoolbox can be found at 
http://xxx.xxx/mytoolbox.html

The program has been written by 
XXXXXXXXX 


Installation
============
To Install this toolbox: (Scilab-4.0)

We Suppose here that <PATH> stands for the path of the directory
containing this readme file.

- On Unix/Linux systems
     * User
        Has to execute,once and for all, the following instruction 
        within Scilab:
        exec('<PATH>/builder.sce') 
        
        Should execute the following instruction within Scilab:
        exec('<PATH>/loader.sce') 
        before using the toolbox, he can also put it in his
        .scilab startup file for automatic loading.

- On Windows systems
     * user
        Has to execute, once and for all, the following instruction 
        within Scilab:
        exec <PATH>\builder.sce     
        
Should execute the following instruction within Scilab:
        exec <PATH>\loader.sce
        before using the toolbox, he can also put it in his
        .scilab startup file for automatic loading.


Contents
========
readme.txt		: this file
license.txt		: license and copyright of your package
builder.sce		: script to launche the sub-builders
loader.sce		: script to launche the sub-loaders

src			: directory of Fortran and C routines
	buildsrc.sce	: script for building library (compilation and linking)
     	loadsrc.sce	: script for loading library (generated)

sci_gateway		: directory of interfaces programs

includes		: .h files

macros			: directory of Scilab functions
	builmacros.sce 	: script for building library
	loadmacros.sce  	: script for loading library
	*.sci		: source versions
	*.bin		: precompiled binary versions (generated)
	lib		: scilab library binary save (generated)

help			: directory for help.
	buildhelp.sce	: translates the xml files to html
	loadhelp.sce	: add your help functions files in the help Scilab browser   
	*.xml		: xml help file 
	*.html		: html help file (generated) 
	whatis		: short function description (generated) 

etc			: .html, .pdf, .txt, .jpeg, ...
	
demos			: examples to illustrate your toolbox

unit tests		: directory of tests
	tests.sce	: script to test toolbox_example 