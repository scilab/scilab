            Changes between version 5.4.X and 6.0.0 of Scilab
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


Removed functions (previously declared as obsolete)
====================================================

* maxfile function is now obsolete.

* intersci has been removed. Please use swig instead.

* numdiff removed. Please use numderivative instead.

* derivative removed. Please use numderivative instead.


Functions modified
===================

* impl   : Recall impl with the same parameters as in its previous stop is now available.
* ode    : y0 is restricted to a column vector.
* pppdiv : return a scalar of type 'constant' when the rank is 0.
* pdiv   : return a matrix of type 'constant' when all the rank are 0.


Deprecated behavior
====================

* Syntax %i:10 is now deprecated.

* Declaration of a number on two lines is no longer allowed:
 1..
 2

* write string or string matrix in file does not add black space before each value
    write("TMPDIR/test", ["test", "sample"; "sample", "test"])
    "-" + mgetl("TMPDIR/test") + "-"


Removed functions
==================

* derivative removed. Please use numderivative instead.

* curblockc removed. Please use curblock instead.


Scilab Bug Fixes
================

* Bug #9621 fixed - A tlist with undefined fields can now be saved.

* Bug #12202 fixed - Mixing int8 and doubles with colon operator lead to wrong results.

* Bug #13517 fixed - isdef crashed Scilab when called with a vector of strings
                     as input in a function and after a declaration of variable.

* Bug #13838 fixed - Sparse and complex substraction made Scilab crash.

