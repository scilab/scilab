        Changes between version 6.0.0-alpha-1 and 6.0.0-alpha-2 of Scilab
        =================================================================

New Features
=============

* valgrind error detection added to test_run (Linux Only)

* Introduction of a call stack limit. Default maximum depth is setup to 1000 and
  can be changed by new function recursionlimit or by preferences interface.

Deprecated Behaviors
=====================

* <-- JVM NOT MANDATORY --> is obsolete, use <-- CLI SHELL MODE --> instead.


Scilab Bug Fixes
================

* Bug #12928 fixed - int functions with %nan and %inf return wrong values.

* Bug #14059 fixed - Lack of performance on deletion of matrix elements.

* Bug #14055 fixed - overload on matrix concatenation did not called with [].

* Bug #14028 fixed - force flag of genlib did not rebuild bin file.

* Bug #14024 fixed - Print of macrofile display a debug message
                     instead of macro prototype.

* Bug #13709 fixed - unique function sometimes returned wrong index values.

* Bug #13750 fixed - Calling ss2ss function with flag = 2 returned an error.

* Bug #13780 fixed - size with two input and output arguments did not return an error.

* Bug #13843 fixed - Scilab crashed when polarplot and plot2d were called with wrong strf value.

* Bug #13853 fixed - plzr() returned wrong results for discrete-time systems with a numeric time step.

* Bug #13872 fixed - Non regression test added for unique function (the indices returned were wrong)

* Bug #13890 fixed - getd did not return loaded symbols in previous scope.

* Bug #13894 fixed - Default working directory of the previous session did not work.

* Bug #13907 fixed - Avoids the gray background on the right panel of the palette Browser.

* Bug #13944 fixed - The menu "Toolboxes" was missing.

* Bug #13983 fixed - who_user returned wrong values.

* Bug #14020 fixed - Incorrect carriage return ascii code.

* Bug #14023 fixed - It was not possible to concatenate cells.

* Bug #14025 fixed - head_comments did not take into account compiled functions.

* Bug #14036 fixed - tag and user_data properties were not displayed and not documented for light entity.

* Bug #14038 fixed - Encoded integers were no longer accepted for list extraction.

* Bug #14040 fixed - graphic property setting fails when using array of handles

* Bug #14047 fixed - wrong behaviour of break ( continue ) in if outside of loop.

* Bug #14049 fixed - genlib hangs if an unexpected endfunction occurs.

* Bug #14057 fixed - grand(m,n) returned a wrong error and grand(m,n,p) called an overloading function instead of returning an error.

* Bug #14058 fixed - Scilab crashed with 'file("close", file())' instruction

* Bug #14082 fixed - m=1; m()=1; made Scilab crash.

* Bug #14093 fixed - atanh returns NaN for values with an absolute value greater than 1

* Bug #14095 fixed - Scilab crashed when a .fig file was loaded with loadmatfile function.

* Bug #14105 fixed - New block comments /*...*/ feature was not documented.

* Bug #14107 fixed - lstcat of a string and a list did not produce consistent results.

* Bug #14109 fixed - lsq function crashed Scilab when Scilab version depended on mkl library.

* Bug #14113 fixed - Scilab 6 did not detect infinite loop.

* Bug #14141 fixed - recursive insertion: gcf().attribute=value => "Wrong insertion : function or macro are not expected".

* Bug #14144 fixed - Scilab crashed with int64(2^63).

* Bug #14149 fixed - hdf5 could not restore hypermatrix with good dimensions.

* Bug #14150 fixed - The Windows SDK was not found on Windows 8.1.

* Bug #14156 fixed - mfscanf returned an empty matrix when datafile contained a header.

* Bug #14159 fixed - Matplot crashed Scilab on boolean input.


            Changes between version 5.5.2 and 6.0.0-alpha-1 of Scilab
            =========================================================



This file details the technical changes done in Scilab 6.0.0-alpha-1 release.

For a more complete overview of the changes in this release, please consult the "What's new" page of the help, by simply typing "help" in Scilab console.



New Features
=============

* scatter/scatter3 plot with different mark colors is now available.

* parulacolormap is now available.

* name2rgb can now handle a single string and a matrix of strings.


Installation
============

* Under Windows, MKL packages are now included in Scilab installer and no more
  downloaded while installing Scilab.


Compilation
===========

* A C++11 compliant compiler is now needed.

* Java minimal version switch to 1.8.

* Ant minimal version switched to 1.9.0 (for Java 1.8 compatibility).

* ecj minimal version switched to 4.4.0 (for Java 1.8 compatibility).

* --without-xcos now only disable Xcos compilation. Xcos graphical interface is disabled
using --without-gui.


Packaging & Supported Operating Systems
=======================================

* Scilab embedded JVM has been upgraded to Java 1.8. To run or compile Scilab 6.0.0-alpha-1 you need at least:
   - Windows:
        Windows 8 (Desktop)
        Windows 7
        Windows Vista SP2
        Windows Server 2008 R2 SP1 (64-bit)
        Windows Server 2012 (64-bit)
   - Mac OS X:
        Intel-based Mac running Mac OS X 10.8.3+, 10.9+
   - Linux:
        Red Hat Enterprise Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
        Oracle Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
        Ubuntu Linux 12.04 LTS, 13.x
        Suse Linux Enterprise Server 10 SP2+, 11.x
For more information, please consult: http://java.com/en/download/help/sysreq.xml


Signal Processing
==================

* amell function:
  - now checks its parameters are real numbers,
  - now throws an error if the second parameter is not a scalar.


Input/Output File
==================

* The use of I/O console is now allowed with the following functions:
  mget, mgetl, mgetstr, mput, mputl and mputstr.

* mclearerr now returns a flag indicating the file identifier validity.

* fileinfo can now take a row vector as input.

* msprintf no more returns an error message when there are too many input arguments (more values that format needs).


Data Structures
================

* cells and structs are now native types, hence improving performances.

* cells:
  - insertion and extraction must be done via () or {}.
  - .dims and .entries fields have been removed, please use size and () instead.

* struct
  - dims field has been removed, please use size instead.

* hypermatrix:
  - hypermatrices are natively managed (without mlist overloading).
  - typeof function now returns real type like constant, string, ... instead of hypermat
  - type function returns real type like 1, 10, ... instead of 17 (mlist).
  - .dims and .entries fields have been removed, please use size and () instead.

Removed Functions
==================

* maxfiles function is now obsolete.

* intersci has been removed. Please use swig instead.

* numdiff has been removed. Please use numderivative instead.

* derivative has been removed. Please use numderivative instead.

* curblockc has been removed. Please use curblock instead.

* extract_help_examples has been removed. (Undocumented and based on former help format).

* Symbolic module functions have been removed:
  - addf
  - cmb_lin
  - ldivf
  - mulf
  - rdivf
  - solve
  - subf
  - trianfml
  - trisolve
  - block2exp

Removed Functions (based on former Scilab stack)
================================================

* comp.

* errcatch.

* iserror.

* fun2string.

* getvariablesonstack.

* gstacksize.

* macr2lst.

* stacksize.

* code2str.

* str2code.

* -mem option (used to set stacksize at startup).


Modified Functions
===================

* exec: exec of macro executes the body in the current scope but the prototype must have zero input and output arguments.
* error: an error number in input is deprecated.
* impl: Recall impl with the same parameters as in its previous stop is now available.
* ode: y0 is restricted to a column vector.
* pppdiv: Return a scalar of type 'constant' when the rank is 0.
* pdiv: Return a matrix of type 'constant' when all the rank are 0.
* typeof: Add a 2nd argument "overload" of get overloaded name (typeof(cell(), "overload") -> "ce")


Syntax Modifications
=====================

* Declaration of a number on two lines is no longer allowed:
 1..
 2

* {"foo"} returns a cell not a string matrix.

* Declaring non-homogenous strings ("string' or 'string") is no more allowed.

* New shortcut operators have been introduced: && and ||.


Deprecated Behaviors
=====================

* {} becomes different of [] and will be used on cells.

* Syntax %i:10 is now deprecated (only real scalars can be used).

* Writing string or string matrix in a file does not add blanck space before each value:
    write("TMPDIR/test", ["test", "sample"; "sample", "test"])
    "-" + mgetl("TMPDIR/test") + "-"

* while/else control instruction is no more supported.

* (a=b) executed like a == b is now deprecated and returns an error.


Scilab Bug Fixes
================

* Bug #6064 fixed - scatter function did not exist in Scilab.

* Bug #8990 fixed - zoom_auto feature was missing on the plot toolbar.

* Bug #9621 fixed - A tlist with undefined fields can now be saved.

* Bug #12202 fixed - Mixing int8 and doubles with colon operator led to wrong results.

* Bug #13289 fixed - Using non-integer indexes for mlists made Scilab crash.

* Bug #13517 fixed - isdef crashed Scilab when called with a vector of strings
                     as input in a function and after a declaration of variable.

* Bug #13807 fixed - Invalid margins were computed when figure was not visible.

* Bug #13829 fixed - mean and sum functions returned wrong results for hypermatrices.

* Bug #13834 fixed - Drawing a high number of strings in a figure generated a Java exception.

* Bug #13838 fixed - Sparse and complex substraction made Scilab crash.

* Bug #13854 fixed - Under some operating systems, SciNotes did not initialize a new document at startup.

* Bug #13866 fixed - There were some issues with FFTW3 library.

* Bug #13881 fixed - datatipRemoveAll did not work.

* Bug #13924 fixed - rationals r1==r2 and r1~=r2 might sometimes be wrong.

* Bug #13925 fixed - SciNotes used the wrong paired bracket highlight style.

* Bug #13941 fixed - Internal timestamps of HDF5 files prevented having a fixed
                     hash for an unvarying set of saved objects.

* Bug #13942 fixed - Now the palette browser tree is always resized when
                     expanded/collapsed.

* Bug #13971 fixed - A space has been added between Scilab prompt and cursor.

* Bug #13972 fixed - Wildcard ('*') was not managed in printf expressions.


Known issues
=============

* Xcos does not work in this version.

* Debugging capabilities are not yet replugged.

* Profiling capabilities are not yet replugged.

* This version will start in English by default.
