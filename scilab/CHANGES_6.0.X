            Changes between version 5.4.X and 6.0.0 of Scilab
            =================================================


File input/output:
==================

* The use of I/O consol is now allowed with functions : mget, mgetl, mgetstr, mput, mputl and mputstr.


Removed functions (previously declared as obsolete):
====================================================

* maxfile function is now obsolete.

* intersci has been removed. Please use swig instead

Functions modified:
===================

* impl : Recall impl with the same parameters as in its previous stop is now available.
* ode  : y0 is restricted to a column vector.

