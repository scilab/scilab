            Changes between version 5.5.2 and 6.0.0-alpha-1 of Scilab
            =========================================================

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

* A conforming C++11 compiler is now needed.

* Java minimal version switch to 1.8.

* Ant minimal version switched to 1.9.0 (for compatibility with Java 1.8).

* ecj minimal version switched to 4.4.0 (for compatibility with Java 1.8).

* --without-xcos now only disable Xcos compilation. Xcos Graphic interface is disabled
using --without-gui.


Packaging & Supported operating systems
=======================================

* Scilab embedded JVM has been upgraded to Java 1.8 then to run or compile Scilab you need at least:
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
For more information, see: http://java.com/en/download/help/sysreq.xml


Signal processing
==================

* amell function:
  - now checks its parameters be real numbers
  - now throws an error if the second parameter isn't scalar


File input/output
==================

* The use of I/O console is now allowed with functions: mget, mgetl, mgetstr, mput, mputl and mputstr.

* mclearerr now returns a flag indicating the file identifier validity.

* fileinfo can now take a row vector as input.

* msprintf no more returns an error message when there are too many input argument (more values that format needs).


Data structures
================

* cells and structs are now native types, hence improving performances.


Removed functions
==================

* maxfile function is now obsolete.

* intersci has been removed. Please use swig instead.

* numdiff removed. Please use numderivative instead.

* derivative removed. Please use numderivative instead.

* curblockc removed. Please use curblock instead.

* extract_help_examples removed. (Undocumented and based on old help format).

* Symbolic module functions removed:
  - addf
  - cmb_lin
  - ldivf
  - mulf
  - rdivf
  - solve
  - subf
  - trianfml
  - trisolve


Removed functions (based on old Scilab stack)
==============================================

* fun2string removed.

* getvariablesonstack removed.

* gstacksize removed.

* macr2lst removed.

* stacksize removed.

* code2str removed.

* str2code removed.

* -mem option removed (used to set stacksize at startup).


Functions modified
===================

* impl   : Recall impl with the same parameters as in its previous stop is now available.
* ode    : y0 is restricted to a column vector.
* pppdiv : return a scalar of type 'constant' when the rank is 0.
* pdiv   : return a matrix of type 'constant' when all the rank are 0.


Syntax clarifications
=====================

* Declaration of a number on two lines is no longer allowed:
 1..
 2

* {"foo"} returns a cell not a string matrix.


Deprecated behavior
====================

* Syntax %i:10 is now deprecated.

* write string or string matrix in file does not add black space before each value
    write("TMPDIR/test", ["test", "sample"; "sample", "test"])
    "-" + mgetl("TMPDIR/test") + "-"

* while/else control instruction is no more supported.


Scilab Bug Fixes
================

* Bug #6064 fixed - scatter function did not exist in Scilab.

* Bug #8990 fixed - zoom_auto feature was missing on the plot toolbar.

* Bug #9621 fixed - A tlist with undefined fields can now be saved.

* Bug #12202 fixed - Mixing int8 and doubles with colon operator lead to wrong results.

* Bug #13289 fixed - Using non-integer indexes for mlists made Scilab crash.

* Bug #13517 fixed - isdef crashed Scilab when called with a vector of strings
                     as input in a function and after a declaration of variable.

* Bug #13807 fixed - Invalid margins were computed when figure was not visible.

* Bug #13829 fixed - mean and sum functions returned wrong results for hypermatrices.

* Bug #13834 fixed - Drawing a high number of strings in a figure generated a Java exceptions.

* Bug #13838 fixed - Sparse and complex substraction made Scilab crash.

* Bug #13854 fixed - On some operating systems, SciNotes did not initialize a new document at startup.

* Bug #13866 fixed - There were some issues with FFTW3 library.

* Bug #13881 fixed - datatipRemoveAll did not work.

* Bug #13924 fixed - rationals r1==r2 and r1~=r2 might sometimes be wrong.

* Bug #13925 fixed - SciNotes used the wrong paired bracket highlight style.

* Bug #13941 fixed - Internal timestamps of HDF5 files prevented having a fixed
                     hash for an unvarying set of saved objects.

* Bug #13972 fixed - Wildcard ('*') was not managed in printf expressions.


