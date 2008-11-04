                          Changes Scilab 5.0.2 -> 5.0.3
                          =============================

Scilab consortium would like to thank all people who helped us by reporting
bugs, submiting patches, fixing bugs, testing ...

Compilation:
============

- DLINUX option for the gcc removed (useless)

Enhancement:
============

- The toolbox skeleton is now released into the public domain to facilitate
  the reuse of the code

- Scilab 5.0.3 uses GIWS 1.0.1

- Update of the license.txt of each module

- License changed to Public Domain for most of the examples (especially the toolbox skeleton and the examples in the dynamic link module)

- Localization files updated

Bug fixes:
==========

* Path to the default Fedora/Redhab JVM added to the startup script

* rmdir & removedir had problems with the recursive delete under GNU/Linux

* bug 2647 fixed - Bad display of some numbers in special cases due to a gfortran bug

* bug 3484 fixed - Scilab crashes when scicos is invoked from the command line.

* bug 3529 fixed - rmdir was trying to change the permission on the file/directory. This behavior has been removed.

* bug 3566 fixed - gsort does not complain when called with a complex array and just ignores the imaginary part

* bug 3567 fixed - sort called with a complex array sort it according to the absolute value of the elements but it is not explained in the help pages...

* bug 3588 fixed - The figure number is missing from the title of graphic windows when Scilab in launched in French.

* bug 3598 fixed - when entering the commands ls(1) or cd(1) or dir(1) or help(1), scilab crashes.

* bug 3602 fixed - format("e",dd) was causing a bug in gettklib (format of the number)

* bug 3604 fixed - Problems with getting Frame uicontrol position (Scilab crash)

* bug 3605 fixed - Could not use javasci in the Linux binary

* bug 3607 fixed - Wrong values in Java configuration was causing the memory issues

* bug 3621 fixed - Wrong setting of figure_position in the default figure.

* bug 3646 fixed - Scilab GUI doesn't start on some Windows

* bug 3647 fixed - 'call' doesn't manage external functions named with more 23 characters

* bug 3655 fixed - Bug in GIWS 1.0 was not returning the proper Java exception.

* bug 3663 fixed - getshortpathname & getlongpathname could return '' and not a correct path

* bug 3692 fixed - Problem with the simplified chinese version


                          Changes Scilab 5.0.1 -> 5.0.2
                          =============================

Documentation:
==============

- xmltojar, xmltohtml, xmltops, xmltopdf now export by default into
SCI/modules/helptools/{jar,html,ps,pdf} (bug 3492)

- Online Help management:
  + xmltojar, xmltohtml, xmltops, xmltopdf now export by default into
    SCI/modules/helptools/{jar,html,ps,pdf)
  + Examples have been added :
    - SCI/modules/helptools/example_2
    - SCI/modules/helptools/example_3
  + The system that complete an XML directory with the default language is
    now available when building help per directory too (=> Toolboxes).
    (see bug 3531 - http://bugzilla.scilab.org/show_bug.cgi?id=3531)
  + xmltojar, xmltohtml, xmltops, xmltopdf : A list of generated file is returned
    (see request 540 - http://requestzilla.scilab.org/show_bug.cgi?id=540)
  + xmltojar, xmltohtml, xmltops, xmltopdf unitary tests have been added

- Update of the documentation: error_table, ilib_gen_Make (Bug 1403)

- Update of path in the UMFPACK modules help & demos

- Unable the documentation in UTF8

Enhancement:
============

 - Some parts of the SCI/bin/scilab script relative to old and obsolete stuff 
  (graphic export) have been removed.

 - Update of the example in the dynamic_link modules and in the toolbox skeleton
  (use the actual filename instead of the .o)

 - Useless input argument removed in the function ilib_gen_Make

Localization:
=============

 - Update of the russian localization

Bug fixes:
==========

 - In the binary, the path to the fortran include directory was wrong.

 - bug 3532 fixed - link doesn't manage column vector for "sub-names" argument 
                    (as scilab 4.x).

 - bug 3540 fixed - With the linux binary version, a symbolic link of 
                    <SCILAB INSTALL PATH>/bin/scilab in /usr/local/bin (for example)
                    did not work.

 - bug 3551 fixed - bvodeS and bvode hang and close Scilab 5.0.1

 - bug 3520 fixed - lex_sort returns incorrect results

 - bug fixed when trying to plot a polyline with only one point 
   (causes PlotSparse to fail)

 - bug in sort compatibility with scilab 4.x

 - bug 3557 fixed - popupmenu uicontrols did not work correctly

 - bug 3560 fixed - property enable used with uicontrols produced an error

 - bug 3526 fixed - File loader.sce missing from the SCI/contrib directory for Linux
 
 - bug 3510 fixed - Bug with the simplified chinese and russian localization 
                    made Scilab unusuable under Windows localized in these languages.
					
 - bug 3572 fixed - Bug with grayplot handles having different dimensions on X and Y.

 - In the binary, a file was missing which was causing some issues while 
   handling Mat-Files. Please note that the Windows binary and sources releases
   are not impacted by this change.

 - bug 3586 fixed - xmltopdf fails on windows

                     Main Changes Scilab 4.X -> 5.0
                     ==============================

License:
========
 - License changed to CeCILL (fully compatible with the GPL v2)
	More information about the CeCILL http://www.cecill.info/index.en.html
	Therefore, Scilab is now a 100% free software.

Graphical User Interface:
=========================
 - Advanced docking system
 - Follow the Tango Desktop Project http://tango.freedesktop.org/
 - GUI re-written in Java Swing:
	+ Same GUI for all OS (Menubars, toolbars...)
	+ Window docking available using FlexDock https://flexdock.dev.java.net/
 - Uicontrols are now Java objects: they can be included in a graphic window with graphics and are managed as graphic handles in Scilab
 - Uimenus are now Java objects and are managed as graphic handles in Scilab
 - Uimenus and Uicontrols can be saved and loaded like other graphic entities using save and load functions
 - Dialogs are now Java objects (x_message, x_dialog...)
 - Completion on variable and function names available using TAB key or CTRL-SPACE

Event Handling Functions:
=========================
 - All the event handling functions "xclick", "xgetmouse" and the "event_handler"
	+ they have been rewritten in Java   
	+ they use the same "-1000" code to signal the "window close" actions 
	  (previously "xclick" and "xgetmouse" used "-100" code) 
 - Obsolete functions:
	+ Using xgetmouse with a flag to avoid the event queue to be cleared is 
	  now obsolete. This functionnality will be removed in Scilab 5.1.

Graphics:
=========
 - New rendering engine based on JOGL (Java Binding for the OpenGL API):
	+ Support 3D hardware acceleration.
	+ Facet ordering issues fixed with the use of Z-buffer.
 - Text possibilities extended:
	+ Support for floating point sizes.
	+ Text object are able to handle matrices of strings.
 - All graphic objects are fully operational both in 2D and 3D modes.
 - Clipping is operational both in 2D and 3D modes.
 - New property "filled" added to Axes object which controls axes box transparency.
 - New functions:
	+ swap_handles - Permute two handles in the graphic hierarchy.
	+ relocate_handle - Move handles inside the graphic hierarchy.
	+ get_figure_handle - Get a figure handle from its id without changing current figure.
	+ rotate_axes - Interactive rotation of an Axes handle.
	+ is_handle_valid - Check wether a set of graphic handles is still valid.
 - New colormaps ( See demonstrations -> graphics -> Colormap ):
	+ autumncolormap    - red through orange to yellow colormap
	+ bonecolormap      - gray colormap with a light blue tone
	+ coolcolormap      - cyan to magenta colormap
	+ coppercolormap    - black to a light copper tone colormap
	+ oceancolormap     - linear blue colormap
	+ pinkcolormap      - sepia tone colorization on black and white images
	+ rainbowcolormap   - red through orange, yellow, green,blue to violet colormap
	+ springcolormap    - magenta to yellow colormap
	+ summercolormap    - green to yellow colormap
	+ whitecolormap     - completely white colormap
	+ wintercolormap    - blue to green colormap
 - Behavior changes:
	+ clf now deletes user menus which are children of the figure
	  (Created by addmenu or uimenu).
 - Obsolete functions:
	+ xclea, xaxis, loadplots and xtape functions have been removed because of 
	  their incompatibilities with the new graphic syntax.

Graphic file export:
====================
  - xs2jpeg, xs2png, xs2svg, xs2pdf functions added to export graphics under
	JPEG, PNG, SVG and PDF formats.
  - Option color in xs2eps and xs2ps has been removed.
  - Export GUI has been rewritten to match the new Java Swing style.


Numerical precision:
====================
 - Numerical precision of Scilab under Linux 32 bits has been switched to
	double precision (53 bits). Therefore, the precision on some specific 
	computations is improved and the consistency between Microsoft Windows &
	GNU/Linux enhanced.

Signal processing:
==================
 - FFTW (Fastest Fourier Transform in the West) features added in Scilab.
	+ fftw                - fast fourier transform that use fftw library
	+ fftw_flags          - set computation method of fast fourier transform of
	                        the fftw function
	+ fftw_forget_wisdom  - reset fftw wisdom
	+ get_fftw_wisdom     - return fftw wisdom
	+ set_fftw_wisdom     - set fftw wisdom
	+ disposefftwlibrary  - free fttw library
	+ fftwlibraryisloaded - checks if fftw library is loaded


Sparse LU factorization:
========================
 - UMFPACK features added in Scilab. Many thanks to Bruno Pincon.
	+ umfpack       - Solve sparse linear system 
	+ umf_lufact    - LU factorisation of a sparse matrix 
	+ umf_lusolve   - Solve a linear sparse system given the LU factors
	+ umf_luinfo    - Get information on LU factors
	+ umf_luget     - Retrieve LU factors at the Scilab level 
	+ taucs_chfac   - Cholesky factorisation of a sparse Symmetric Positive 
	                  Definite (s.p.d.) matrix
	+ taucs_chsolve - Solve a linear sparse (s.p.d.) system given the Cholesky factors
	+ taucs_chinfo  - Get information on Cholesky factors
	+ taucs_chget   - Retrieve the Cholesky factorization at the Scilab level
	+ res_with_prec - Computes the residual r = Ax-b with precision
	+ cond2sp       - Computes an approximation of the 2-norm condition number 
	                  of a s.p.d. sparse matrix
	+ condestsp     - Estimate the condition number of a sparse matrix
	+ PlotSparse    - Plot the pattern of non nul elements of a sparse matrix 
	+ ReadHBSparse  - Read a Harwell-Boeing sparse format file 
	                  (See http://math.nist.gov/MatrixMarket/ for examples)


Optimization:
=============
  + qpsolve       - Linear quadratic programming solver
  + qp_solve      - Linear quadratic programming solver builtin
  - quapro        - This function has been moved in a toolbox (due to license restrictions from the Universidad de Cantabria)
  - linpro        - This function has been moved in a toolbox (due to license restrictions from the Universidad de Cantabria)

Genetic Algorithms:
===================
 - Genetic algorithms have been added into Scilab. Many thanks to Yann Collette
	+ coding_ga_binary 	- Performs conversion between binary and continuous representation
	+ coding_ga_identity	- "no-operation" conversion function
	+ crossover_ga_binary 	- Crossover function for binary code
	+ crossover_ga_default	- Crossover function for continuous variable functions
	+ init_ga_default     	- Initialize a population 
	+ mutation_ga_binary  	- Performs binary mutation 
	+ mutation_ga_default 	- Continuous variable mutation
	+ optim_ga              - Flexible genetic algorithm 
	+ optim_nsga            - Multi-objective Niched Sharing Genetic Algorithm
	+ optim_nsga2           - Multi-objective Niched Sharing Genetic Algorithm version 2
	+ pareto_filter         - Extracts non dominated solution from a set
	+ selection_ga_elitist	- 'Elitist' selection function 
	+ selection_ga_random 	- Random selection of individuals 


Param/parameters module:
========================
 - This package allows to deal with list of parameters efficiently 
   (like optim_set in Matlab from The Mathworks (TM)). Many thanks to Yann Collette
	+ add_param            - Add a parameter to a list of parameters
	+ get_param            - Get the value of a parameter in a parameter list
	+ init_param           - Initialize an empty parameter list
	+ is_param             - Check if a parameter is present in a parameter list
	+ list_param           - List all the parameters name in a list of parameters 
	+ remove_param         - Remove a parameter and its associated value from a list of parameters 
	+ set_param            - Set the value of a parameter in a parameter list 


Simulated Annealing:
====================
 - Simulated annealing (SA) is a generic probabilistic meta-algorithm for the 
   global optimization problem. Many thanks to Yann Collette
	+ compute_initial_temp - Compute the initial temperature of the simulated annealing
	+ neigh_func_csa       - The classical neighborhood relationship for the simulated annealing 
	+ neigh_func_default   - Computes a neighbor of a given point
	+ neigh_func_fsa       - The Fast Simulated Annealing neghborhood relationship 
	+ neigh_func_vfsa      - The Very Fast Simulated Annealing neighborhood relationship 
	+ optim_sa             - Simulated Annealing optimization method 
	+ temp_law_csa         - The classical temperature decrease law
	+ temp_law_default     - Computes the temperature of the next temperature stage 
	+ temp_law_fsa         - The Szu and Hartley Fast simulated annealing
	+ temp_law_huang       - The Huang temperature decrease law for the simulated annealing
	+ temp_law_vfsa        - The Very Fast Simulated Annealing from L. Ingber 


Other Numerical Functions
=========================
 - New functions 
        + filter           - signal filtering 
        + erfinv           - inverse of the erf function
        +                  - support for rational fraction hypermatrices added
 - Bug fixes
        + Major bug in qld fixed

Input/Output functions (files):
===============================
 - New functions:
	+ getdrives           - get the drive letters of all mounted filesystems on the computer.
	+ getrelativefilename - given an absolute directory and an absolute filename, returns a relative file name.
	+ fullpath            - creates an full path name for the specified relative path name.
	+ is_absolute_path    -
	+ createdir           - make new directory
	+ removedir           - remove a directory
	+ deletefile          - delete a file
	+ fileext             - returns extension for a file path
	+ filesep             - returns directory separator for current platform
	+ pathsep             - returns path separator for current platform
	+ maxfiles            - sets the limit for the number of files a scilab is allowed to have open simultaneously.
	+ findfiles           - Finds all files with a given filespec

 - Profile changed:
	+ ls under Linux/Unix has lost his second input argument (unix/linux command line option). It is now based on listfiles. ls in Scilab 5 will always return one file per line (2 per lines before)
	+ cd and chdir now return a boolean instead of 0 or an error as it used to. T (true) means that operation was successful while F (false) means that operation failed
	
 - Rewrite in native code:
	+ isdir
	+ newest


Regular expression features:
============================
 - Based on the library PCRE
 - Involved functions
	+ grep(...,'r')
	+ regexp
	+ strindex(...,'r')
	+ strsubst(...,'r')


String manipulation:
====================
 - New functions:
	+ blanks     - create string of blank characters 
	+ isalphanum - check that characters of a string are alphanumerics
	+ isascii    - tests if character is a 7-bit US-ASCII character
	+ isdigit    - check that characters of a string are digits between 0 and 9
	+ isletter   - check that characters of a string are alphabetics letters
	+ regexp     - find a substring that matches the regular expression string
	+ strchr     - locate first occurrence of character in string
	+ strcmp     - compare character strings
	+ strcmpi    - compare character strings (case independent)
	+ strcspn    - get span until character in string
	+ strncpy    - copy characters from strings
	+ strrchr    - locate last occurrence of character in string
	+ strrev     - returns string reversed
	+ strspn     - get span of character set in string
	+ strstr     - locate substring
	+ strtod     - convert string to double
	+ strtok     - split string into tokens


System relative Functions:
=========================
 - New functions:
	+ getdebuginfo          - returns informations about scilab and your system
	+ getscilabmode         - returns scilab mode
	+ getvariablesonstack   - get variable names on stack of scilab
	+ getmd5                - get the MD5 checksum of strings or files.
	+ getmodules            - returns list of modules installed in Scilab
	+ with_module           - Checks if a Scilab module is installed
	+ islittleendian        - Checks if machine architecture is "little-endian"
	+ librarieslist         - get scilab libraries
	+ libraryinfo           - get macros and path of a scilab library
	+ completion            - returns words that start with the text you pass as parameter.
	+ add_profiling         - prepare a function for profiling
	+ remove_profiling      - remove  profiling instruction in a function
	+ reset_profiling       - reset  profiling data of a function

 - Rewrite in native code:
        + warning
  
 - Profile changed:
	+ unix_g                - a third output argument (optional) has been added to get standard error output in a variable.


Time and date functions:
========================
 - New functions:
	+ clock   - return current time as date vector
	+ eomday  - return last day of month
	+ weekday - return day of week
	+ now     - return current date and time


Binary, octal and hexadecimal representation:
=============================================
 - New functions:
	+ bin2dec  - return integer corresponding to a binary form
	+ bitand   - AND applied to binary representation of inputs argument
	+ bitcmp
	+ bitget
	+ bitor    - OR applied to binary representation of inputs arguments
	+ bitxor   - XOR applied to binary representation of inputs arguments
	+ dec2bin  - return binary representation
	+ dec2oct  - return octal representation of integers


Jvm module:
===========
	+ with_embedded_jre        - checks if scilab uses a embedded JRE
	+ system_getproperty       - gets the system property indicated by a specified key.
	+ javaclasspath            - set and get dynamic Java class path
	+ javalibrarypath          - set and get dynamic java.library.path


Localization:
=============
 - All the standard, warning and error messages have been rewritted to uniform output and also to provide more feedbacks to the user.
 - Scilab is coming with the included french localization (basic GUI for the beta version)
 - Russian localization (by Vorona Aleksey)
 - Traditionnal chinese localization (by Yung-Jang Lee)
 - Simplified chinese localization (by Shiqi Yu)
 - New functions:
	+ gettext(string) / _(string) - get text translated into the current locale
	                                and domain
	+ dgettext                    - get text translated into the current
	                                locale and a specific domain domain
	+ setlanguage(string)
	+ getlanguage()


Online Help management:
=======================
 - New Java help browser
 - New help system based on DocBook
 - New functions:
	+ check_help
	+ del_help_chapter
	+ add_module_help_chapter
	+ del_module_help_chapter
	+ xmltojar (the documentation provided in Scilab)
	+ xmltopdf
	+ xmltohtml
 - loaddefaultbrowser function and %browsehelp global variable have been removed:
   The new Java help browser is the only help browser.
 - For more information, have a look to :
        + http://wiki.scilab.org/howto/scilab_documentation_kit
        + SCI/modules/helptools/doc directory

Metanet : graph and network toolbox
===================================
 - Data structures have been reorganized and made more flexible (user
   can define and handle its own data fields for nodes and edges)
 - New functions:
	+ hilite_edges   - highlights a set of edges
	+ unhilite_edges - unhighlights a set of edges
	+ hilite_nodes   - highlights a set of nodes
	+ unhilite_nodes - unhighlights a set of nodes
	+ add_edge_data  - associates new data fields to the edges data structure 
	                   of a graph
	+ add_node_data  - associates new data fields to the nodes data structure
	                   of a graph
	+ arc_length
	+ nodedatafields - returns the vector of node data fields names
	+ set_edges_id
	+ set_nodes_id
	+ show_arcs      - highlights a set of arcs
	+ show_edges     - highlights a set of edges
	+ update_graph   - converts an old graph data structure to the current one


Windows_tools:
===============
 - New functions:
	+ consolebox          - show or hide console box
	+ findfileassociation - searches for and retrieves a file association-related string from the registry.
	+ dos                 - shell (cmd) command execution 
	+ win64               - determine whether Windows x64 version of Scilab
	+ istssession         - determine if scilab is launched from a ms terminal service session.
	+ getsystemmetrics    - retrieves the specified system metric or system configuration setting.
	+ createGUID          - creates a Globally Unique IDentifier (GUID)
	+ setdefaultlanguage  - set and save default language on Windows


History manager:
================
 - New functions:
	+ displayhistory          - displays current scilab history
	+ addhistory              - add lines to current history
	+ saveconsecutivecommands - Save consecutive duplicate commands
	+ saveafterncommands      - Save the history file after n statements are added to the file.
	+ gethistoryfile          - get filename used for scilab's history
	+ sethistoryfile          - set filename for scilab history
	+ removelinehistory       - remove the Nth line in history.
	+ historysize             - get number of lines in history
	+ historymanager          - enable or disable history manager

 - Rewrite:
	+ loadhistory
	+ savehistory
	+ gethistory
	+ resethistory


Consoles:
=========
 - Based on a joint project Artenum ( http://www.artenum.com ) - INRIA/Scilab called Jrosetta
 - Completion
	+ Functions
	+ Variables
	+ Paths
 - Advanced completion also available in NW & NWNI modes

Modules/toolboxes template:
===========================
 - A template model has been defined to normalize how modules and toolboxes 
	should be.
	+ See http://wiki.scilab.org/Scilab_Module_Architecture for module
	+ See contrib/toolbox_skeleton/ for toolboxes


Source reorganisation:
======================
 - Source code split into dedicated and clear modules
 - Clear separation between the Scilab gateway (argument management, etc) and 
	the code
 - Use XML declaration for the list of functions
 - Use XML declaration for the list of modules (and activation or not)
 - Massive cleanup of the code 
 - Thousand of warnings removed
 - First step to a future Scilab API


New Scilab Functions:
=====================
 - Function rewritten from Fortran to C for better maintenance


Scilab modes:
=============
 - Scilab is now available in four modes:
	+ Normal mode
		The standard Java
	+ NW (No window) mode
		Run in command line but can launch help or rendering/graphics 
		(loads the Java Virtual Machine)
	+ NWNI (No window No interactive) mode
		Run in command line but no help, TCL or rendering
		(does not load the Java Virtual Machine)
	+ API mode
		Scilab as API/computing engine


GNU/Linux - Unix:
=================
 - Please note that most of these points are outputs of the SCOS project ( http://www.oscos.org/ )
 - New configure (based on autoconf)
 - New compilation process (based on automake and libtool)
 - Parallel build (make -j)
 - FHS compliant
 - Switched to dynamic libraries
 - Advanced numerical libraries can be used (Atlas, MKL...)
 - Use librairies when available on the system
 - Build by default with gfortran
 - Versioned libraries
 - Easier profiling (memory and call graph)
 - Better support of 64 bits CPU
 - Better startup script
	+ Debug
	+ Profiling (valgrind)


Windows:	
========
 - Compilation process :
	+ Visual Studio 2008 solution (requires Intel Fortran 10.1) support (x86 & x64)
	+ Visual Studio 2008 express solution (fortran conversion with f2c) support
 - Dynamic libraries	
 - Atlas 3.8 support (Core 2 Duo and more support)
 - Intel Math Kernel Library support
 - Better support on Windows Vista


Dynamic/incremental link:
=========================
 - Module renamed to dynamic link
 - Visual Studio 2008 support
 - Intel Fortran 10 support on Windows
 - Under Linux/Unix, now based on basic autotools for the detection of compiler
	and compilation.
 - New functions:
        + getdynlibext - get the extension of dynamic libraries on your operating system.
 - Function improved:
        + ilib_for_link & ilib_build - now accept the actual filename (not only filename.o but also filename.c or filename.f)


TCL/TK:
=======
 - New event loop 
	+ Tcl/Tk thread separated from the Scilab one.

 - Provided with version 8.5 of TCL/TK
 - Removed Functions:
	+ demoplay is obsolete
	+ sciGUIhelp is obsolete 
 - Some TCL/related functions have been tagged as obsolete 
   (See http://wiki.scilab.org/obsolete_scilab_function )
 - TCL_CreateSlave now allows to create "safe" or "unsafe" slave interpreters (new option added).
   Default switched from safe to unsafe interp.


Java/Scilab API:
================
 - Refactoring of the Java API


Tests:
======
 - Tests are now embedded into Scilab (binary and sources)
 - Many tests added 
	+ Unitary
	+ Non regression
	+ Benchmarks
 - New functions:
        + test_run
        + unitary_test_run
        + nonreg_test_run

OTHERS IMPROVEMENTS:
====================
 - Update to the new version of Lapack (3.1.1)

Internal API:
=============
 - Used in functions GetRhsVar, PutLhsVar... The datatype naming have been 
	changed to:
	+ "c" => STRING_DATATYPE
	+ "S" => MATRIX_OF_STRING_DATATYPE
	+ "d" => MATRIX_OF_DOUBLE_DATATYPE
	+ "r" => MATRIX_OF_RATIONAL_DATATYPE
	+ "I" => MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE
	+ "i" => MATRIX_OF_INTEGER_DATATYPE
	+ "b" => MATRIX_OF_BOOLEAN_DATATYPE
	+ "z" => MATRIX_OF_COMPLEX_DATATYPE
	+ "s" => SPARSE_MATRIX_DATATYPE
	+ "l" => LIST_DATATYPE
	+ "t" => TYPED_LIST_DATATYPE
	+ "m" => MATRIX_ORIENTED_TYPED_LIST_DATATYPE
	+ "p" => SCILAB_POINTER_DATATYPE
	+ "h" => GRAPHICAL_HANDLE_DATATYPE
	+ "f" => EXTERNAL_DATATYPE
	These declarations are in modules/core/includes/stackTypeVariable.h
 - define TRUE_ and FALSE_ removed for TRUE and FALSE
 - Many other things... Have a look to the sources...


Scicos:
=======
 - Test framework currently under development:
        + Unitary tests
        + Non-regression tests (vs. Scicos 4.2 in Scilab 4.1.2)

Known incompatibilty
--------------------

  - The Metanet graph data structure has been completely redesigned
    see update_graph for translation, Graph saving is not more
    possible in a formatted way.

  - Due to a bug fix the xstringb x and y arguments meaning had
    changed "lower" and "left" are now relative to user coordinates
    order.

  - The xgetmouse flag option has been removed 

  - Due to license restrictions from the Universidad de Cantabria, the quapro 
	and linpro functions are no more available in the Scilab distribution, 
	but as an external toolbox.

  - Some uses of the pow function could cause issues for base ~ 1.0 and large
	exponent (See http://sources.redhat.com/bugzilla/show_bug.cgi?id=706)

