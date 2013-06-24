// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for cross function.
// =============================================================================

A=[1;2;3];
B=[2;3;4];
C=[-1;2;-1];
assert_checkequal(cross(A,B), C);
assert_checkequal(cross(A',B'),C');
assert_checkequal(cross([A,A],[B,B]),[C,C]);
assert_checkequal(cross([A,A,A],[B,B,B]),[C,C,C]);
assert_checkequal(cross(int8(A),int8(B)),int8(C));
assert_checkequal(cross(uint8(A),uint8(B)),uint8(C));
assert_checkequal(cross(int16(A),int16(B)),int16(C));
assert_checkequal(cross(uint16(A),uint16(B)),uint16(C));
assert_checkequal(cross(int32(A),int32(B)),int32(C));
assert_checkequal(cross(uint32(A),uint32(B)),uint32(C));
A=[%i;2;1+%i];
B=[1;%i;2+%i];
C=[5+%i;2-%i;-3];
assert_checkequal(cross(A,B),C);
s=poly(0,"s");
A=[s;1+s;3];
B=[1;2+s;s];
C=[s^2-2*s-6;3-s^2;s^2+s-1];
assert_checkequal(cross(A,B),C);
A=[%t;%f;%t];
B=[%t;%f;%f];
C=[0;1;0];
assert_checkequal(cross(A,B),C);
A=sparse([1;0;3]);
B=sparse([2;3;0]);
C=sparse([-9;6;3]);
assert_checkequal(cross(A,B),C);

// Error messages
errmsg1=msprintf(_("Wrong number of input arguments."));
assert_checkerror("cross(A,B,C)", errmsg1);
errmsg2=msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "cross",2);
assert_checkerror("cross(A)", errmsg2);
errmsg3=msprintf(_("%s: Wrong type for input argument #%d: A real, complex, boolean or polynomial matrix expected.\n"), "cross", 1);
assert_checkerror("cross([""s"";""t"";""u""],B)", errmsg3);
errmsg4=msprintf(_("%s: Wrong type for input argument #%d: A real, complex, boolean or polynomial matrix expected.\n"), "cross", 2);
assert_checkerror("cross(A,[""s"";""t"";""u""])", errmsg4);
errmsg5=msprintf(_("%s: Wrong size for input argument #%d: A matrix of size 1x3 or 3xN expected.\n"), "cross", 1);
assert_checkerror("cross([1;2;3;4],[1;2;3;4])", errmsg5);
errmsg6=msprintf(_("%s: Wrong size for input arguments: Same sizes expected.\n"), "cross");
assert_checkerror("cross(A,[2;3])",errmsg6);
A = rand(2,3);
errmsg = msprintf(_("%s: Wrong size for input argument #%d: A matrix of size 1x3 or 3xN expected.\n"), "cross", 1);
assert_checkerror("cross(A,A)", errmsg);
