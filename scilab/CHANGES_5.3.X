               Changes between version 5.2.X and 5.3.0 of Scilab
               =================================================


Performance:
============

* Faster access to primitive table (internal). This change improves the overall
  speed of Scilab.

* Improved access to the list of functions increases the performances.

FFTW:
=====

* On Windows, Scilab can use FFTW3 wrapper of Intel Math Kernel Library.
  Select "Download FFTW3 wrapper optimized by Intel Math Kernel Library" during
  installation.

Graphics:
=========

* the set, get and uicontrol functions are now overloadable.

* Datatips tools added, the frequency response plots ("bode", "black", "nyquist",
  "gainplot",...) supports cutomized datatips.

* bug 2651 fixed - "plot2d" was not working when the x argument was not given
                   and some optional arguments were given.

* bug 3100 fixed - The "xselect" function has been made obsolete, in accordance
                   with its help page.

* bug 3975 fixed - The "xload" function was not able to load the contents of a previously
                   saved graphics figure into a different one.

* bug 4531 fixed - "legend" macros was not displaying any error when no 'Polyline' exist.

* bug 4658 fixed - When some graphic functions were called without any arguments,
                   the error was not explicit enough.

* bug 5284 fixed - In some case function "legend" drew line segments in its box
                   in a reverse order.

* bug 6098 fixed - It was not possible get values by clicking on point of Bode diagram

* bug 6532 fixed - In some case text entity rendering order was wrong.

* bug 6676 fixed - When we executed "twinkle(gca())", data related to the
                   current axes were not twinkling.

* bug 6786 fixed - The "polarplot" function did not correctly display the polar
                   coordinates grid when negative rho values were plotted.

* bug 6835 fixed - Performing an interactive rotation after having executed the
                   "plot3d" function caused Scilab to freeze on 64-bit Windows.

* bug 6848 fixed - The patterns of the lines was poorly exported.

* bug 6958 fixed - The "tics_labels" property from "axes" rejected column vector of labels.

* bug 6959 fixed - axis.tics_labels did not accept $..$ LaTeXed labels

* bug 7024 fixed - Error messages from "twinkle" were not standard.

* bug 7028 fixed - When scilab printed an "axis" graphic handle, string array
                   size for "tics_labels" was erroneous.

* bug 7065 fixed - "getColorIndex" used some global variables prohibiting
                   user's calls.

* bug 7067 fixed - The "isoview" macro created an unnecessary "plot" object.

* bug 7075 fixed - When the "links" property was set for a handle of type
                   "legend", the property type was not checked.

* bug 7079 fixed - The "mtlb_get" used a "h" global variable instead of the
                   given "H" argument.

Localization:
=============

* Polish localization added. Thanks to Paweł Dąbrowski, Ernest Życiński,
  Sebastian Sławski, Kamil Dąbrowsk, Kamil Strojek, Kamil Dąbrowski,
  Michał Snoc, Piotr Seroka...

* bug 7059 fixed - Remove the tag X-Launchpad-Export-Date to avoid huge and
                   useless diff after a launchpad synchronization

API_Scilab:
===========

* Error(x) is now deprecated and will be removed in Scilab 5.4.
  Please consider SciError(x).

Documentation:
==============

* It is now possible to use LaTeX inline in the documentation. Thanks to
  Calixte Denizet for this feature. (See bug #4329)

* bug 6630 fixed - JavaHelp does not update existing image files when
                   building help.

* bug 6852 fixed - JavaHelp did not center the LaTeX formulas containing an
                   attribute align="center"

Optimization:
=============

* Simulated annealing: Added documentation for accept_func_default and
  accept_func_vfsa.

* fminsearch: updated printing of neldermead, optimbase and optimsimplex objects.

* fminsearch: added demo for dimensionnality effect of the Nelder-Mead algorithm.

Xcos:
=====

* modelica compiler 1.12.1

* The default palettes can now be manipulated by the user.

* LaTeX formulas can be used on any TextBlock using the Scilab tag based
  description.

* builnewblock.sci rewritten to use scilab dynamic link module tools.

* bug 4156 fixed - scicos defines some temp. variables on stack without remove.

* bug 4705 fixed - scicos used a non existing file BEpsf

* bug 4898 fixed - The scifunction_block stored list() parameters on the ipar
                   field instead of the opar field.

* bug 4977 fixed - Some icons were duplicated between Gui and Xcos.

* bug 5161 fixed - The export to SVG did not work with images or html.

* bug 5596 fixed - It was not possible to change text block properties (font
                   and color)

* bug 6440 fixed - xcos demos did not work and returned an error "Unable to
                   open file".

* bug 6527 fixed - There was several instances of the same variable name
                   in the workspace when a data set was exported via a
                   "To Workspace" block.

* bug 6661 fixed - "Code Generation" menu failed.

* bug 6672 fixed - "Code generation" failed if superblock had
                   some gain blocks.

* bug 6674 fixed - modelica generic block did not work.

* bug 6668 fixed - Xcos modelica demos did not work with the new
                   modelica compiler.

* bug 6814 fixed - Link point positions (link.xx and link.yy) were not exported
                   as column vectors.

* bug 6900 fixed - The block interfunction was escaped when calling scilab
                   macros.

* bug 6903 fixed - modelicat called ocamlrun (wrong dependency on Windows).

* bug 6981 fixed - the Scilab 'exit' call was killing Xcos session without
                   prompting the user if modified diagrams must be saved before
                   exiting.

* bug 7099 fixed - xml2modelica called ocamlrun (wrong dependency on Windows).

* bug 7053 fixed - Rotated saved blocks were not loaded with the right rotation
                   value.

* bug 7143 fixed - Simulation values were not valid if the exponential sign is
                   an 'e'.

* bug 7175 fixed - demo bridge rectifier did not build on Windows.

Dynamic link:
=============

* bug 4265 fixed - In the dynamic link functions (ilib_*), when a compiler was
                   not available, the execution continued.

Compilation:
============

* On Windows, Pentium III class (or equivalent) with SSE instructions is
  required.

* Scilab_f2c.sln can be exported to VS 2010 format.

* Windows 2000 is no more supported as operating system.

* Scilab Mac OS X distribution relies only on Java 1.6 and is therefor now
  only available under 64 bits OS (Most of 10.5 and all 10.6 Mac OS X versions)

* Under Mac OS X, uses /usr/libexec/java_home instead of hardcoded paths to
  retrieve JAVA_HOME

* tclsci module is dynamically loaded by Scilab.

* pvm module is dynamically loaded by Scilab.

* fix many warnings detected by Intel Fortran 11.1.051 and more on Windows.

* Better detection system of the version of the dependencies

* Add a check of the minimal installations of hdf5 (1.8.4), batik (1.7)

* /usr/share/*/lib added to the Java search path for OpenSuse

* Option --enable-compilation-tests added to the configure. With this option,
  if TestNG has been found, tests will be launched at compilation time.

* bug 4300 fixed - Moving Command Queue management to core.

* bug 6432 fixed - Checkstyle disabled on SWIG (Java/JNI) generated files

* bug 6643 fixed - --without-gui failed to disable Ocaml build (A xcos
                   dependency)

* bug 6857 fixed - make CXXFLAGS="-myflag" was removing some C++ arguments.

helptools:
==========

* %helps_modules (global variable in scilab) is defined only when
  scilab build help and no more at scilab's startup.

Matlab to Scilab conversion tools:
==================================

* Unitary tests added.

* Matlab-Scilab equivalents dictionnary added in documentation.

* In 'mfile2sci', option only_double is now set to %T as default.

* Conversion updated with new Scilab functions:
  - 'filesep' now converted by 'filesep' instead of 'pathconvert'.

Core:
=====

* Under GNU/Linux, Unix and Mac OS X, for security purposes, temporary
  directory name are now randomized and permissions are limited to the user.
  (See bug #3493)

* Under GNU/Linux, Unix and Mac OS X, if TMPDIR is set, Scilab will honor it
  to store its temporary files (See bug #3493)

* Under non-Windows system, the name of the temporary directory has been changed
  from SD_* to SCI_TMP_* to match the current Windows behavior.

* bug 6986 fixed - edit('toto.sce') created a file toto.sce.sci containing
                   a function named toto.sce

Tests:
======

* Mechanism of unitary tests for Java code based on TestNG added and used.

Text Rendering:
===============

* Antialiasing activated by default to render text

License:
========

* The demos files (modules/*/demos/**/*.dem.*) are now released in the public
  domain. (See bug #4902)

Obsolete Functions:
===================

* 'MSDOS' obsolete see 'getos' (SEP #39).

Removed functions (previously declared as obsolete):
====================================================

* 'textprint', 'pol2tex' replaced by 'prettyprint'.

* 'tk_savefile' replaced by 'uigetfile'.

* 'tk_getdir' replaced by 'uigetdir'.

* 'tk_choose' replaced by 'x_choose'.

* 'sci2excel' replaced by 'write_csv'.

* 'excel2sci' replaced by 'read_csv'.

* 'x_message_modeless' replaced by 'messagebox'.

* 'sethomedirectory' replaced by 'SCIHOME'.

* 'getcwd' replaced by 'pwd'.

* 'xbasc' replaced by 'clf'.

* 'getf' replaced by 'exec'.

* 'NumTokens' replaced by 'tokens'.

* 'sort' replaced by 'gsort'.

* 'scilab_demos' replaced by 'demo_gui'.

* 'with_gtk' replaced by 'getversion'.

* 'readc_' replaced by 'input'.


ATOMS:
======

* The ATOMS GUI ergonomics has been improved.

* ATOMS GUI: When a module is not up-to-date, a warning is displayed on the
  corresponding sheet.

* ATOMS GUI: Display the release date of the watched module.

* bug 6596 fixed - When the ATOMS GUI started, nothing was displayed. Now, the
                   list of already installed modules appears on the home page.

* bug 6842 fixed - If the installed version of a module was not the most recent
                   one, the ATOMS GUI claimed that the newest version was
                   installed.

* bug 6976 fixed - ATOMS GUI: Webpages URLs linked with a module were missing
                   from the presentation frame.

* bug 6977 fixed - ATOMS GUI: In the presentation frame, URLs and emails were
                   not hyperlinked.

* bug 7149 fixed - An error message of atomsDESCRIPTIONread() was badly formated.

Bug fixes:
==========

* bug 1304 fixed - macr2tree now works with a Scilab expression as input.

* bug 1371 fixed - The representation of floating numbers with exponent larger 
                   than 100 or smaller than -100 was incorrect.

* bug 2076 fixed - pspect did not use the last data window.

* bug 2613 fixed - Incorrect stack recovery for catched errors in some
                   specific contexts (for loops, ..)

* bug 2623 fixed - non regression test script fixed

* bug 2654 partially fixed - exec returned a wrong error if line had more
                   than 4096 characters.

* bug 2775 fixed - Multiplication of 0 times infinity did not work because of
                   ATLAS under Windows.

* bug 3688 fixed - The assignment using ":" subscript for a dimension that does not exist
                    did not work for hypermatrices.

* bug 3780 fixed - The function loadmatfile did not ignore comments found in
                   ASCII files while loading them, resulting in an error message.
                   For Matlab compatibility, when this function is called with
                   one input argument load(filename), it now looks for a binary
                   file (if filename as no extension or .mat extension) or for an
                   ASCII file if filename has an other extension.

* bug 3796 fixed - In some situation the "tf2ss()" function failed to
                   compute correctly the state space representation of
                   a transfer function.

* bug 3811 fixed - Documention about "typeof" and overload prefixes was not
                   up-to-date (help overloading).

* bug 3937 fixed - Buttons size in "x_choices" window was related to the labels one.

* bug 4075 fixed - a basic uicontrol example crashed scilab 5.0.3

* bug 4117 fixed - Closing via the user interface a graphics figure which had a
                   non-existing event handler attached did not destroy
                   its associated data structure.

* bug 4224 fixed - uigetfile masks were case insensitive on Windows.

* bug 4359 fixed - M(:,:)='anything' produced erroneous result.

* bug 4362 fixed - test_run("gui","checkbox") no more freezes Scilab.

* bug 4572 fixed - scilab did a SIGSEGV after stacksize('max') + stacksize().

* bug 4625 fixed - intersci-n was broken on Windows.

* bug 4640 fixed - plot2d() put CPU utilization at 100% on Windows.

* bug 4672 fixed - Internal function AddFunctionInTable renamed to
                   getFunctionByName to avoid confusions on the actual usage
                   of this function. Thanks to Yuvraj Tomar.

* bug 4699 fixed - xml2test.pl: Paths were badly displayed in some cases.

* bug 4783 fixed - comparison between sparse complex matrix and a full one was
                   wrong when the sparse matrix contains a  zero row.

* bug 4799 fixed - Sometime the bode, nyquist , ... function reported a
                   "non convergence" error

* bug 4841 fixed - functions created by 'getd' were unknown to scilab.

* bug 4948 fixed - a simple Runge Kutta method for solving a second order
                   differential equation had a strange behaviour.

* bug 4857 fixed - help file of 'gsort' updated about sort of complex numbers
                   and overloading.

* bug 4985 fixed - mgetl did not use fgets (C function).

* bug 4996 fixed - mgetl() was not located in the right module. It has been
                   moved to "fileio" module

* bug 5110 fixed - incomplete mgetstr documentation.

* bug 5123 fixed - help about '%helps' was wrong.

* bug 5136 fixed - information about dependant platform display of
                   exponential form (%e) was missing in the prinft_conversion
                   documentation.

* bug 5149 fixed - test_run(): Calls to exec() were no more logged by diary().

* bug 5216 fixed - URLs are now active in uicontrols of style "text". This
                   feature is used in ATOMS GUI (modules description).

* bug 5324 fixed - "File open" dialog box focused on parent directory of current
                   directory, on Windows.

* bug 5432 fixed - a link to http://www.tcl.tk/doc/ added in help files about
                   Tcl/Tk.

* bug 5512 fixed - The display order of input arguments was not described in
                   the disp() documention.

* bug 5541 fixed - In the API documentation, the information that data must be
                   allocated before calling API functions was missing.

* bug 5575 fixed - Wrong german translation fixed (Standart => Standard)

* bug 5583 fixed - exec() did not manage correctly UTF-8 and ANSI/ASCII files
                   encoded.

* bug 5588 fixed - Assigning null() in a tlist or mlist field shifted all remaining
                   values with respect to  field names.

* bug 5609 fixed - documentation stated incorrectly fieldnames works on cells

* bug 5612 fixed - There was no way for deleting a field in a struct.
                   It is now available using null().

* bug 5629 fixed - export_to_hdf5 to a file overwrote a directory
                   with the same name (now trigger an error).

* bug 5630 fixed - export_to_hdf5 to a file with a directory named
                   the same altered the permission of the directory.

* bug 5724 fixed - erfinv and erf missed references to similar functions

* bug 6332 fixed - stacksize had problems on Mac OS X and 64 bit platforms.

* bug 6349 fixed -  The callback of an 'edit' uicontrol was only executed when it lost focus.

* bug 6419 fixed - gstacksize() There was no way to retrieve minimum
                   from Scilab.

* bug 6428 fixed - fscanfMat did not manage format parameter as fprintfMat.

* bug 6435 fixed - error 4 returned a wrong message.

* bug 6447 fixed - unix_g() and dos() did not support properly neither UTF8
                   nor ANSI encoded output.

* bug 6449 fixed - regexp returned a wrong value with a string returned by mgetl.

* bug 6451 fixed - length returned a wrong value with a string returned by mgetl.

* bug 6452 fixed - part returned a wrong value with a string returned by mgetl.

* bug 6484 fixed - file did not return filename of a opened fortran file.

* bug 6504 fixed - Copy/paste the contents of message boxes was not available.

* bug 6510 fixed - modules/helptools/help_en_US.xsl and
                   modules/helptools/help_fr_FR.xsl were useless but not removed.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64 bits
                   Windows.

* bug 6516 fixed - fullfile did not remove blanks and not create a valid path.

* bug 6535 fixed - The property 'ListboxTop' of the graphic user interface object
                   'listbox' had no effect.

* bug 6543 fixed - neldermead issues for automatic restarts.

* bug 6549 fixed - genlib function had a behavior which was not consistent with
                   the load function.

* bug 6550 fixed - lib(".") returned a relative path in library variable
                   created.

* bug 6556 fixed - fprintf() returned an error if it's called 2 times (or more)
                   on the same file.

* bug 6635 fixed - Incorrectly drawn Bode plots (calfrq bug).

* bug 6666 fixed - emptystr() gave a wrong result instead of complaining
                   about hypermatrix.

* bug 6682 fixed - mprintf, msprintf, mfprintf did not manage octal (%o) format.

* bug 6731 fixed - Information on the output argument was missing from the
                   lib() documentation.

* bug 6740 fixed - It was not possible to launch Scilab as Minimized
                   or Maximized Window.

* bug 6744 fixed - p_margin() returned an erroneous result.

* bug 6745 fixed - g_margin() returned an erroneous result.

* bug 6750 fixed - TMPDIR constant had no help page.

* bug 6774 fixed - Calling get_param generated a "funcprot" warning when
                   the value of an option which was a function.

* bug 6783 fixed - Solving a system of linear equation by backslash ("\")
                   caused a crash, when using complex numbers on Windows.

* bug 6784 fixed - It was not possible to move a Scilab installation without
                   breaking the (previously installed) ATOMS packages load.

* bug 6827 fixed - The error printed by spchol if the variable was complex
                   was wrong.

* bug 6830 fixed - C2F(scistring) in stack2.c was broken for macro names of
                   exactly 2 characters.

* bug 6831 fixed - in cli (=nwni) mode, tbx_build_help failed.

* bug 6861 fixed - yulewalk example failed on Windows.

* bug 6862 fixed - Help page for 'vectorfind' missed some explanations about
                   input arguments types. Moreover there were missing checks
                   about input arguments number and types in the function.

* bug 6864 fixed - Some error messages were wrong in data_structures module.

* bug 6867 fixed - The error message when base2dec() was called with an invalid
                   base representation was wrong.

* bug 6889 fixed - 'fscanfMat' crashed with wrong formatted files.

* bug 6891 fixed - 'whos' did not display global variables.

* bug 6893 fixed - Some files +~JF****.tmp where created outside of the Scilab
                   temporary directory.

* bug 6895 fixed - completion crashed with -NW mode, on some linux.

* bug 6901 fixed - This script with 'exec' produced a weird warning.

* bug 6923 fixed - Some localized messages were wrong in 'qmr'.

* bug 6924 fixed - Some error messages were wrong in 'gmres' and 'qmr'.

* bug 6932 fixed - Unitary test for function 'exit' failed in Linux binary
                   version.

* bug 6933 fixed - Some temporary files (dict_*.tmp) were created when using
                   search engine of the help browser.

* bug 6934 fixed - Denormalized floating point numbers were not present in
                   Scilab's master.

* bug 6940 fixed - An error message was wrong in 'pvm_send'.

* bug 6946 fixed - information about optional input arguments 'column separator'
                   and 'decimal mark' were missing in the write_csv documentation.

* bug 6947 fixed - Some demos in "Optimization and Simulation" failed,
                   without write rights in SCI subdirectory.

* bug 6951 fixed - stacksize had a strange behaviour.

* bug 6952 fixed - stacksize('max') did not return maximum value.

* bug 6962 fixed - bvode generated an error on Windows.

* bug 6964 fixed - Scilab could detect a wrong SCI path on Windows.

* bug 6971 fixed - after launching a toolbox by loader.sce, variable v was
                   predefined.

* bug 6990 fixed - "and" and "or" did not work on hypermatrices.

* bug 7006 fixed - exec removed spaces at the end of filename when it returned
                   an error.

* bug 7014 fixed - ilib_verbose value was modified after toolbox_skeleton.

* bug 7019 fixed - resume() produced an incorrect error message.

* bug 7021 fixed - Scilab did not start and returned a system error (193)
                   on Windows.

* bug 7033 fixed - newfun was randomly crashing (mostly on 32 bits) writing to
                   unallocated memory.

* bug 7036 fixed - the plot2d document was incomplete (missing optional argument).

* bug 7042 fixed - Insertion of an empty matrix into a struct (to remove a row or column)
                   was buggy when multiple index is used.

* bug 7044 fixed - calling sequence in help of 'strcat' was wrong.

* bug 7057 fixed - isdigit and isalphanum crashed with a matrix of string
                   as input argument.

* bug 7077 fixed - uigetfont crashed Scilab when the cancel button was clicked.

* bug 7078 fixed - A wrong error message was displayed when setting the "value"
                   property of an "edit" uicontrol.

* bug 7094 fixed - If badly called, ode made Scilab crash.

* bug 7109 fixed - 'lu' function with some complex matrices crashed on Windows.

* bug 7123 fixed - No completion was possible if the name of a variable is
                   preceded by ~ or @ or < or >

* bug 7161 fixed - wavread failed to load some .wav files with error 44.

* bug 7170 fixed - Scilab equivalent for Matlab "uigetdir" was "tk_getdir",
                   it has been replaced by "uigetdir".

* bug 7181 fixed - The display of a struct having no fields did not work.
