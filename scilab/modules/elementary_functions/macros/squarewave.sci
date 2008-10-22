// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s = squarewave(t,%)
//square(t) generates a square wave with period 2*Pi 
//square(t) is like sin(t), only it creates a square wave 
//with peaks of +1 to -1 instead of a sine wave.

//square(t,%) generates an unsymetric  square wave: %, is the 
//percent of the period in which the signal is positive.
[lhs,rhs]=argn()
if rhs<2 then %=50,end
s=2*bool2s(modulo(t,2*%pi) < 2*%pi*%/100)-1
endfunction
