            Changes between version 5.4.X and 6.0.0 of Scilab
            =================================================


File input/output
==================

* The use of I/O console is now allowed with functions: mget, mgetl, mgetstr, mput, mputl and mputstr.

* mclearerr now returns a flag indicating the file identifier validity.


Removed functions (previously declared as obsolete)
====================================================

* maxfile function is now obsolete.

* intersci has been removed. Please use swig instead


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
