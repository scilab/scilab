// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=intl(a,b,z0,r,f,ea,er)
    //If f is a complex-valued function, intl(a,b,z0,r,f) computes
    //the integral of f(z)dz along the complex plane curve defined by
    //  z0 + r.exp(%i*t)
    //for a<=t<=b .
    //(part of the circle with center z0 and radius r with phase between a and b)
    //!

    //First compile f if necessary:

    rhs=argn(2)
    if rhs<7 then
        er=%eps;
    end
    if rhs<6 then
        ea=1.d-12;
    end

    if type(f)==11 then
        comp(f)
    end;
    //Define two functions for the real part and
    //imaginary part of f(g(t))*g'(t) where g(t) is a
    //parametrization of the circle.
    deff("y=real1(t)",[
    "z=r*exp(%i*((1-t)*a+t*b))"
    "y=real(f(z+z0)*%i*(b-a)*z)"] )

    deff("y=imag1(t)",[
    "z=r*exp(%i*((1-t)*a+t*b))"
    "y=imag(f(z+z0)*%i*(b-a)*z)"] )

    r=intg(0,1,real1,ea,er)+%i*intg(0,1,imag1,ea,er)
endfunction
