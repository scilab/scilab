                     Main Changes Scilab 3.0 -> 3.1
                     ==============================
NEW FUNCTIONALITIES
===================
xsave and xload function now work for new graphic mode
A Scilab code debugger is now available under scipad (thanks to F Vogel)

OTHERS IMPROVEMENTS
===================
Graphic editor has been extended

Graphic entity proterties have been extended

the besseli, besselj, besselk and bessely functions have been extended
to the complex case and to non regularily spaced alpha


INCOMPATIBLITIES
================
The semantics of besseli, besselj, besselk and bessely functions has been changed and extended. The 
oldbesseli, oldbesselj, oldbesselk and oldbessely correspond to the old obsolete semantics.


                     Main Changes Scilab 2.7 -> 3.0
                     ==============================
NEW FUNCTIONALITIES
===================
 NUMERICAL ALGORITHMS
  - arpack algorithms for sparse matrix eigenvalues/eigenvectors computation added 
  - Levenberg-Marquardt algorithm of Minpack used to define function lsqrsolve
  - beta and legendre function added (thanks to Bruno Pincon)
  - New 1D 2D 3D interpolation functions  (thanks to Bruno Pincon)
 GRAPHICS and GUI
  - graphical editor available
  - demoplayer, variable browser, variable editor GUI added (thanks to Jaime Urzua Grez)
 MISC
  - Scicos has been extended to handle non causal systems description (supported by RNTL)
  - Matlab to Scilab translator has been re-written, and extended to current Matlab syntax.
OTHERS IMPROVEMENTS
===================
 SCICOS
  - Better discontinuities handling in Scicos
  - An enlarged set of blocks
  - A new API for simulation functions
 GRAPHICS
  - New graphic mode has been debugged and extended. It is now the default mode
  - More efficient 3D Zoom (thanks to E Segre)
  - Helvetica (Arial on Windows) fonts added (corresponding font identifiers are
    from 6 to 9) (thanks to B Pincon)
  - Graphic windows refresh improved under Windows
  - figure sizes can now be taken into account in Postscript export 
 GUI
  - Improved scipad, shortcuts, language handling, colorization, bug fixes.. 
    (thanks to Enrico Segre and Francois Vogel)
   - New imbedded help and html browser  (thanks to Jaime Urzua Grez)
   - Main Scilab window improvedunder  Windows (standard editing facilities, menus,...)
 SYNTAX
  - continue instruction added
  - [a(i,j),b(..),..]=foo(..) syntax is now handled
  - for loop variable is no more removed at the end of the loops
  - logical expressions shortcuts handled in if and while
  - functions with no argument can be called without () if called as a command
  - More efficient number parsing (thanks to Bruno Pincon)
 MISC
  - N dimensional matrices efficiency improved (thanks to Bruno Pincon)
  - cd, ls, clc, clf, tohome functions added
  - Elementary functions evaluation in the complex plane revisited  (thanks to Bruno Pincon)
  - History of commands rewritten to make it persistant between calls to Scilab


 PORTS
 - Scilab has been adapated to IA64 architecture

INCOMPATIBLITIES
================
- returned arguments f1 f2 f3 of interp have been transposed
- variable named cd, ls, color,... should be renamed  to avoid run time warnings
- The instructions <function_name>; used to load the sub-functions of the file 
  <function_name>.sci within a function have to be rewritten <function_name>=<function_name>;
