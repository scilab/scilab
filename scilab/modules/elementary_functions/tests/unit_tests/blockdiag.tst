// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ----------------------------------------------------------------------------

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- IMPOSED ENGLISH -->
//
// --------------------------
// Unit tests for blockdiag()
// --------------------------

// Decimal numbers and encoded integers:
R = [1 2];
C = [3 ; 4]
M = [5 6 ; 7 8];
ref = [1,2,0,0,0;0,0,3,0,0;0,0,4,0,0;0,0,0,5,6;0,0,0,7,8];
for it = [0 1 2 4 8 11 12 14 18]
    b = blockdiag(iconvert(R,it),iconvert(C, it), iconvert(M,it));
    assert_checkequal(b, iconvert(ref,it));
end

// Strings
b = blockdiag(string(R), string(C), string(M));
assert_checkequal(b, strsubst(string(ref),"0",""));

// Booleans
b = blockdiag([%T %T], [%T %T]', [%T %T ; %T %T], %T)
T = %T; F = %F;
ref = [T,T,F,F,F,F;F,F,T,F,F,F;F,F,T,F,F,F;F,F,F,T,T,F;F,F,F,T,T,F;F,F,F,F,F,T];
assert_checkequal(b, ref);
b = blockdiag(sparse([%T %T]), sparse([%T %T]'), sparse([%T %T ; %T %T]), sparse(%T));
assert_checkequal(b, sparse(ref));
b = blockdiag([%T %T], sparse([%T %T]'), [%T %T ; %T %T], sparse(%T));
assert_checkequal(b, sparse(ref));

// Polynomials
z = %z;
b = blockdiag([z 1-z^2], [1 3*z]', z.^[1 3 ; 2 4], z^5)
ref = [ z,1-z^2,0*z,0*z,0*z,0*z;
        0*z,0*z,1,0*z,0*z,0*z;
        0*z,0*z,3*z,0*z,0*z,0*z;
        0*z,0*z,0*z,z,z^3,0*z;
        0*z,0*z,0*z,z^2,z^4,0*z;
        0*z,0*z,0*z,0*z,0*z,z^5];
assert_checkequal(b, ref);

// Rationals
b = blockdiag([1/z z/(1-z)], [3*z z^2]', (1+z)./z.^[1 3 ; 2 4], 1/z^5);
ref =  [1,z,0*z,0*z,0*z,0*z;0*z,0*z,3*z,0*z,0*z,0*z;0*z,0*z,z^2,0*z,0*z,0*z;0*z,0*z,0*z,1+z,1+z,0*z;0*z,0*z,0*z,1+z,1+z,0*z;0*z,0*z,0*z,0*z,0*z,1] ./ [z,1-z,1,1,1,1;1,1,1,1,1,1;1,1,1,1,1,1;1,1,1,z,z^3,1;1,1,1,z^2,z^4,1;1,1,1,1,1,z^5];
assert_checkequal(b, ref);

 
// Errors
msg = ["Function not defined for given argument type(s),"
       "  Check arguments or define the overloading function %ip_blockdiag()."];
assert_checkerror("blockdiag(1:$, 2:$)", msg);
