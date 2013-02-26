                    Changes between version 5.4.X and 5.5.0
                    =======================================

New features
=============

* Special functions improved:
 - erfi - The imaginary error function.
 - dawson - Compute the Dawson (scaled imaginary error) function.

* Functions erf, erfc, erfcx and calerf support complex arguments.

* New functions introduced:
 - getURLcontent - Return the content of an URL (HTTP, HTTPS, FTP...)
 - getURL - Download an URL (HTTP, HTTPS, FTP...)
 - splitURL - Split a URL (HTTP, HTTPS, FTP...)


* Interactive point-and-click curve editor, now the user can select/unselect
  hide/unhide and delete curves, copy, cut and paste selected curves between figures,
  now figures can be copied as an image to system clipboard(only under Windows).

* History

* Native datatips

* Modifications of data

* Graphic editor


Syntax changes
===============

* Vector ^ scalar syntax declared obsolete. See bug #11524


Obsolete
=========

* The third argument of poly function will be more strict.
    Only the following strings will be allowed : "roots", "coeff", "c", "r".

* Arguments option and direction of gsort will be more strict in all case.
  Only the following strings will be allowed : option : "r" "c" "g" "lr" "lc"
                                              direction : "d" "i"

Xcos
====

* Update Sundials to lastest "2.5.0", keeping our modifications

Bug fixes
==========

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

