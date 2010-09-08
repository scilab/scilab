// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=intc(a,b,f)
//If f is a complex-valued macro,intc(a,b,f) computes
//the integral from a to b of f(z)dz along the straight
//line a-b of the complex plane.

//First compile f if necessary:
// Copyright INRIA
if type(f)==11 then comp(f),end;
//Define two functions which define the real part and
//imaginary part of f(g(t))*g'(t) where g(t) is a 
//parametrization of the line a-b.
deff('<r>=real1(t,a,b,f)','r=real(f((1-t)*a+t*b)*(b-a))')
deff('<r>=imag1(t,a,b,f)','r=imag(f((1-t)*a+t*b)*(b-a))')
r=intg(0,1,list(real1,a,b,f))+%i*intg(0,1,list(imag1,a,b,f))
endfunction
