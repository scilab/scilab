            Changes between version 5.5.2 and 6.0.0 of Scilab
            =================================================

Installation
============

* Under Windows, MKL packages are now included in Scilab installer and no more
  downloaded while installing Scilab.


Compilation
===========

 * Scilab requires conforming C++11 compilers


File input/output
==================

* The use of I/O console is now allowed with functions: mget, mgetl, mgetstr, mput, mputl and mputstr.

* mclearerr now returns a flag indicating the file identifier validity.

* fileinfo can now take a row vector as input.


Removed functions
==================

* maxfile function is now obsolete.

* intersci has been removed. Please use swig instead.

* numdiff removed. Please use numderivative instead.

* derivative removed. Please use numderivative instead.

* curblockc removed. Please use curblock instead.


Removed functions (based on old Scilab stack)
==============================================

* getvariablesonstack removed.

* gstacksize removed.


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


Scilab Bug Fixes
================

* Bug #9621 fixed - A tlist with undefined fields can now be saved.

* Bug #12202 fixed - Mixing int8 and doubles with colon operator lead to wrong results.

* Bug #13289 fixed - Using non-integer indexes for mlists made Scilab crash.

* Bug #13517 fixed - isdef crashed Scilab when called with a vector of strings
                     as input in a function and after a declaration of variable.

* Bug #13838 fixed - Sparse and complex substraction made Scilab crash.

* Bug #13972 fixed - Wildcard ('*') was not managed in printf expressions.

