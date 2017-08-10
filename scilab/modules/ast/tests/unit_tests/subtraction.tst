// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

s = %s;
empty = [];
r = 2;
R = [1,2;3,4];
R3(:,:,1) = R;
R3(:,:,2) = R';
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];
C3(:,:,1) = C;
C3(:,:,2) = C';
e = eye();
ec = (5+%i) * eye();
p = 1 + %s - %s**2;
pc = 1 + %s - %s**2 + ( 2 - 3 * %s + 4 * %s**2 ) * %i;
P = [2*p, -3*p;4*p,-5*p];
PC = [2*pc, -3*pc;4*pc,-5*pc];
sp = sparse(1);
spc = sparse([1+%i]);
SP = sparse([1,2;4,5;3,10],[1,2,3]);
SPC = sparse([1,2;4,5;3,10],[1,2,3]) * ( 1 + 4*%i);

i8 = int8(-8);
ui8 = uint8(8);
I8 = int8([-8 -16 ; -24 -32]);
UI8 = uint8([8 16 ; 24 32]);

I16 = int16([-16 -32 ; -48 -64]);
UI16 = uint16([16 32 ; 48 64]);
i16 = int16(-16);
ui16 = uint16(16);

i32 = int32(-32);
ui32 = uint32(32);
I32 = int32([-32 -64 ; -96 -128]);
UI32 = uint32([32 64 ; 96 128]);

warning("off"); // WARNING_EMPTY_OPS
assert_checkequal(empty - empty, []);
assert_checkequal(empty - r, []);
assert_checkequal(empty - c, []);
assert_checkequal(empty - R, []);
assert_checkequal(empty - C, []);
assert_checkequal(empty - e, []);
assert_checkequal(empty - ec, []);
assert_checkequal(empty - p, []);
assert_checkequal(empty - pc, []);
assert_checkequal(empty - P, []);
assert_checkequal(empty - PC, []);
assert_checkequal(empty - SP, []);
assert_checkequal(empty - SPC, []);
assert_checkequal(empty - i8, []);
assert_checkequal(empty - I8, []);
assert_checkequal(empty - ui8, []);
assert_checkequal(empty - UI8, []);
assert_checkequal(empty - i16, []);
assert_checkequal(empty - I16, []);
assert_checkequal(empty - ui16, []);
assert_checkequal(empty - UI16, []);
assert_checkequal(empty - i32, []);
assert_checkequal(empty - I32, []);
assert_checkequal(empty - ui32, []);
assert_checkequal(empty - UI32, []);

assert_checkequal(r - empty, []);
assert_checkequal(r - r, 0);
assert_checkequal(r - c,  1-%i*2);
assert_checkequal(r - R, [1,0;-1,-2]);
assert_checkequal(r - C, [ 1-%i*2,-%i*4;-1-%i*6,-2-%i*8]);
assert_checkequal(r - e, 1);
assert_checkequal(r - ec, -3-%i);
assert_checkequal(r - p, 1-s+s^2);
assert_checkequal(r - pc, 1-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2);
assert_checkequal(r - P, [-2*s+2*s^2,5+3*s-3*s^2;-2-4*s+4*s^2,7+5*s-5*s^2]);
assert_checkequal(r - PC, [-%i*4+(-2+%i*6)*s+(2-%i*8)*s^2,5+%i*6+(3-%i*9)*s+(-3+%i*12)*s^2;-2-%i*8+(-4+%i*12)*s+(4-%i*16)*s^2,7+%i*10+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(r - SP, [2,1,2,2,2,2,2,2,2,2;2,2,2,2,2,2,2,2,2,2;2,2,2,2,2,2,2,2,2,-1;2,2,2,2,0,2,2,2,2,2]);
assert_checkequal(r - SPC, [ 2, 1-%i*4, 2, 2, 2, 2, 2, 2, 2, 2; 2, 2, 2, 2, 2, 2, 2, 2, 2, 2; 2, 2, 2, 2, 2, 2, 2, 2, 2,-1-%i*12; 2, 2, 2, 2,-%i*8, 2, 2, 2, 2, 2]);
assert_checkequal(r - i8, int8(10));
assert_checkequal(r - I8, int8([10,18;26,34]));
assert_checkequal(r - ui8, uint8(250));
assert_checkequal(r - UI8, uint8([250,242;234,226]));
assert_checkequal(r - i16, int16(18));
assert_checkequal(r - I16, int16([18,34;50,66]));
assert_checkequal(r - ui16, uint16(65522));
assert_checkequal(r - UI16, uint16([65522,65506;65490,65474]));
assert_checkequal(r - i32, int32(34));
assert_checkequal(r - I32, int32([34,66;98,130]));
assert_checkequal(r - ui32, uint32(4294967266));
assert_checkequal(r - UI32, uint32([4294967266,4294967234;4294967202,4294967170]));

assert_checkequal(c - empty, []);
assert_checkequal(c - r, -1+%i*2);
assert_checkequal(c - c, 0*%i);
assert_checkequal(c - R, [%i*2,-1+%i*2;-2+%i*2,-3+%i*2]);
assert_checkequal(c - C, [0,-1-%i*2;-2-%i*4,-3-%i*6]);
assert_checkequal(c - e, %i*2);
assert_checkequal(c - ec, -4+%i);
assert_checkequal(c - p, %i*2-s+s^2);
assert_checkequal(c - pc, (-1+%i*3)*s+(1-%i*4)*s^2);
assert_checkequal(c - P, [-1+%i*2-2*s+2*s^2,4+%i*2+3*s-3*s^2;-3+%i*2-4*s+4*s^2,6+%i*2+5*s-5*s^2]);
assert_checkequal(c - PC, [-1-%i*2+(-2+%i*6)*s+(2-%i*8)*s^2,4+%i*8+(3-%i*9)*s+(-3+%i*12)*s^2;-3-%i*6+(-4+%i*12)*s+(4-%i*16)*s^2,6+%i*12+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(c - SP, [ 1+%i*2,%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-2+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2]);
assert_checkequal(c - SPC, [ 1+%i*2,-%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-2-%i*10; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-1-%i*6, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2]);

assert_checkequal(R - empty, []);
assert_checkequal(R - r, [-1,0;1,2]);
assert_checkequal(R - c, [-%i*2, 1-%i*2; 2-%i*2, 3-%i*2]);
assert_checkequal(R - R, [0,0;0,0]);
assert_checkequal(R - C, [-%i*2,-%i*4;-%i*6,-%i*8]);
assert_checkequal(R - e, [0,2;3,3]);
assert_checkequal(R - ec, [-4-%i, 2; 3,-1-%i]);
assert_checkequal(R - p, [-s+s^2,1-s+s^2;2-s+s^2,3-s+s^2]);
assert_checkequal(R - pc, [-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2,1-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2;2-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2,3-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2]);
assert_checkequal(R - P, [-1-2*s+2*s^2,5+3*s-3*s^2;-1-4*s+4*s^2,9+5*s-5*s^2]);
assert_checkequal(R - PC, [-1-%i*4+(-2+%i*6)*s+(2-%i*8)*s^2,5+%i*6+(3-%i*9)*s+(-3+%i*12)*s^2;-1-%i*8+(-4+%i*12)*s+(4-%i*16)*s^2,9+%i*10+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(R - i8, int8([9,10;11,12]));
assert_checkequal(R - I8, int8([9,18;27,36]));
assert_checkequal(R - ui8, uint8([249,250;251,252]));
assert_checkequal(R - UI8, uint8([249,242;235,228]));
assert_checkequal(R - i16, int16([17,18;19,20]));
assert_checkequal(R - I16, int16([17,34;51,68]));
assert_checkequal(R - ui16, uint16([65521,65522;65523,65524]));
assert_checkequal(R - UI16, uint16([65521,65506;65491,65476]));
assert_checkequal(R - i32, int32([33,34;35,36]));
assert_checkequal(R - I32, int32([33,66;99,132]));
assert_checkequal(R - ui32, uint32([4294967265,4294967266;4294967267,4294967268]));
assert_checkequal(R - UI32, uint32([4294967265,4294967234;4294967203,4294967172]));
assert_checkequal(R - sp, [0,1;2,3]);
assert_checkequal(R - spc, [0-%i,1-%i;2-%i,3-%i]);

assert_checkequal(C - empty, []);
assert_checkequal(C - r, [-1+%i*2,%i*4; 1+%i*6, 2+%i*8]);
assert_checkequal(C - c, [0, 1+%i*2; 2+%i*4, 3+%i*6]);
assert_checkequal(C - R, [%i*2,%i*4;%i*6,%i*8]);
assert_checkequal(C - C, [0,0;0,0] * %i);
assert_checkequal(C - e, [%i*2, 2+%i*4; 3+%i*6, 3+%i*8]);
assert_checkequal(C - ec, [-4+%i, 2+%i*4; 3+%i*6,-1+%i*7]);
assert_checkequal(C - p, [%i*2-s+s^2,1+%i*4-s+s^2;2+%i*6-s+s^2,3+%i*8-s+s^2]);
assert_checkequal(C - pc, [(-1+%i*3)*s+(1-%i*4)*s^2,1+%i*2+(-1+%i*3)*s+(1-%i*4)*s^2;2+%i*4+(-1+%i*3)*s+(1-%i*4)*s^2,3+%i*6+(-1+%i*3)*s+(1-%i*4)*s^2]);
assert_checkequal(C - P, [-1+%i*2-2*s+2*s^2,5+%i*4+3*s-3*s^2;-1+%i*6-4*s+4*s^2,9+%i*8+5*s-5*s^2]);
assert_checkequal(C - PC, [-1-%i*2+(-2+%i*6)*s+(2-%i*8)*s^2,5+%i*10+(3-%i*9)*s+(-3+%i*12)*s^2;-1-%i*2+(-4+%i*12)*s+(4-%i*16)*s^2,9+%i*18+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(C - sp, [2*%i,1+4*%i;2+6*%i,3+8*%i]);
assert_checkequal(C - spc, [%i,1+3*%i;2+5*%i,3+7*%i]);

assert_checkequal(e - empty, []);
assert_checkequal(e - r, -1);
assert_checkequal(e - c, -%i*2);
assert_checkequal(e - R, [0,-2;-3,-3]);
assert_checkequal(e - C, [-%i*2,-2-%i*4;-3-%i*6,-3-%i*8]);
assert_checkequal(e - e, (0)*eye());
assert_checkequal(e - ec, (-4-%i)*eye());
assert_checkequal(e - p, -s+s^2);
assert_checkequal(e - pc, -%i*2+(-1+%i*3)*s+(1-%i*4)*s^2);
assert_checkequal(e - P, [-1-2*s+2*s^2,3+3*s-3*s^2;-4-4*s+4*s^2,6+5*s-5*s^2]);
assert_checkequal(e - PC, [-1-%i*4+(-2+%i*6)*s+(2-%i*8)*s^2,3+%i*6+(3-%i*9)*s+(-3+%i*12)*s^2;-4-%i*8+(-4+%i*12)*s+(4-%i*16)*s^2,6+%i*10+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(e - SP, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[1;-1;1;1;-3;1;-2],[4,10]));
assert_checkequal(e - SPC, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 1;-1-%i*4; 1; 1;-3-%i*12; 1;-2-%i*8],[4,10]));

assert_checkequal(ec - empty, []);
assert_checkequal(ec - r,  3+%i);
assert_checkequal(ec - c,  4-%i);
assert_checkequal(ec - R, [ 4+%i,-2;-3, 1+%i]);
assert_checkequal(ec - C, [ 4-%i,-2-%i*4;-3-%i*6, 1-%i*7]);
assert_checkequal(ec - e, ( 4+%i)*eye());
assert_checkequal(ec - ec, (0*%i)*eye());
assert_checkequal(ec - p, 4+%i-s+s^2);
assert_checkequal(ec - pc, 4-%i+(-1+%i*3)*s+(1-%i*4)*s^2);
assert_checkequal(ec - P, [3+%i-2*s+2*s^2,3+3*s-3*s^2;-4-4*s+4*s^2,10+%i+5*s-5*s^2]);
assert_checkequal(ec - PC, [3-%i*3+(-2+%i*6)*s+(2-%i*8)*s^2,3+%i*6+(3-%i*9)*s+(-3+%i*12)*s^2;-4-%i*8+(-4+%i*12)*s+(4-%i*16)*s^2,10+%i*11+(5-%i*15)*s+(-5+%i*20)*s^2]);
assert_checkequal(ec - SP, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 5+%i;-1; 5+%i; 5+%i;-3; 5+%i;-2],[4,10]));
assert_checkequal(ec - SPC, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 5+%i;-1-%i*4; 5+%i; 5+%i;-3-%i*12; 5+%i;-2-%i*8],[4,10]));

assert_checkequal(p - empty, []);
assert_checkequal(p - r, -1+s-s^2);
assert_checkequal(p - c, -%i*2+s-s^2);
assert_checkequal(p - R, [s-s^2,-1+s-s^2;-2+s-s^2,-3+s-s^2]);
assert_checkequal(p - C, [-%i*2+s-s^2,-1-%i*4+s-s^2;-2-%i*6+s-s^2,-3-%i*8+s-s^2]);
assert_checkequal(p - e, s-s^2);
assert_checkequal(p - ec, -4-%i+s-s^2);
assert_checkequal(p - p, 0*s);
assert_checkequal(p - pc, -%i*2+(%i*3)*s+(-%i*4)*s^2);
assert_checkequal(p - P, [-1-s+s^2,4+4*s-4*s^2;-3-3*s+3*s^2,6+6*s-6*s^2]);
assert_checkequal(p - PC, [-1-%i*4+(-1+%i*6)*s+(1-%i*8)*s^2,4+%i*6+(4-%i*9)*s+(-4+%i*12)*s^2;-3-%i*8+(-3+%i*12)*s+(3-%i*16)*s^2,6+%i*10+(6-%i*15)*s+(-6+%i*20)*s^2]);

assert_checkequal(pc - empty, []);
assert_checkequal(pc - r, -1+%i*2+(1-%i*3)*s+(-1+%i*4)*s^2);
assert_checkequal(pc - c, (1-%i*3)*s+(-1+%i*4)*s^2);
assert_checkequal(pc - R, [%i*2+(1-%i*3)*s+(-1+%i*4)*s^2,-1+%i*2+(1-%i*3)*s+(-1+%i*4)*s^2;-2+%i*2+(1-%i*3)*s+(-1+%i*4)*s^2,-3+%i*2+(1-%i*3)*s+(-1+%i*4)*s^2]);
assert_checkequal(pc - C, [(1-%i*3)*s+(-1+%i*4)*s^2,-1-%i*2+(1-%i*3)*s+(-1+%i*4)*s^2;-2-%i*4+(1-%i*3)*s+(-1+%i*4)*s^2,-3-%i*6+(1-%i*3)*s+(-1+%i*4)*s^2]);
assert_checkequal(pc - e, %i*2+(1-%i*3)*s+(-1+%i*4)*s^2);
assert_checkequal(pc - ec, -4+%i+(1-%i*3)*s+(-1+%i*4)*s^2);
assert_checkequal(pc - p, %i*2+(-%i*3)*s+(%i*4)*s^2);
assert_checkequal(pc - pc, 0*s);
assert_checkequal(pc - P, [-1+%i*2+(-1-%i*3)*s+(1+%i*4)*s^2,4+%i*2+(4-%i*3)*s+(-4+%i*4)*s^2;-3+%i*2+(-3-%i*3)*s+(3+%i*4)*s^2,6+%i*2+(6-%i*3)*s+(-6+%i*4)*s^2]);
assert_checkequal(pc - PC, [-1-%i*2+(-1+%i*3)*s+(1-%i*4)*s^2,4+%i*8+(4-%i*12)*s+(-4+%i*16)*s^2;-3-%i*6+(-3+%i*9)*s+(3-%i*12)*s^2,6+%i*12+(6-%i*18)*s+(-6+%i*24)*s^2]);

assert_checkequal(P - empty, []);
assert_checkequal(P - r, [2*s-2*s^2,-5-3*s+3*s^2;2+4*s-4*s^2,-7-5*s+5*s^2]);
assert_checkequal(P - c, [1-%i*2+2*s-2*s^2,-4-%i*2-3*s+3*s^2;3-%i*2+4*s-4*s^2,-6-%i*2-5*s+5*s^2]);
assert_checkequal(P - R, [1+2*s-2*s^2,-5-3*s+3*s^2;1+4*s-4*s^2,-9-5*s+5*s^2]);
assert_checkequal(P - C, [1-%i*2+2*s-2*s^2,-5-%i*4-3*s+3*s^2;1-%i*6+4*s-4*s^2,-9-%i*8-5*s+5*s^2]);
assert_checkequal(P - e, [1+2*s-2*s^2,-3-3*s+3*s^2;4+4*s-4*s^2,-6-5*s+5*s^2]);
assert_checkequal(P - ec, [-3-%i+2*s-2*s^2,-3-3*s+3*s^2;4+4*s-4*s^2,-10-%i-5*s+5*s^2]);
assert_checkequal(P - p, [1+s-s^2,-4-4*s+4*s^2;3+3*s-3*s^2,-6-6*s+6*s^2]);
assert_checkequal(P - pc, [1-%i*2+(1+%i*3)*s+(-1-%i*4)*s^2,-4-%i*2+(-4+%i*3)*s+(4-%i*4)*s^2;3-%i*2+(3+%i*3)*s+(-3-%i*4)*s^2,-6-%i*2+(-6+%i*3)*s+(6-%i*4)*s^2]);
assert_checkequal(P - P, [0*s,0*s;0*s,0*s]);
assert_checkequal(P - PC, [-%i*4+(%i*6)*s+(-%i*8)*s^2,%i*6+(-%i*9)*s+(%i*12)*s^2;-%i*8+(%i*12)*s+(-%i*16)*s^2,%i*10+(-%i*15)*s+(%i*20)*s^2]);

assert_checkequal(PC - empty, []);
assert_checkequal(PC - r, [%i*4+(2-%i*6)*s+(-2+%i*8)*s^2,-5-%i*6+(-3+%i*9)*s+(3-%i*12)*s^2;2+%i*8+(4-%i*12)*s+(-4+%i*16)*s^2,-7-%i*10+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - c, [1+%i*2+(2-%i*6)*s+(-2+%i*8)*s^2,-4-%i*8+(-3+%i*9)*s+(3-%i*12)*s^2;3+%i*6+(4-%i*12)*s+(-4+%i*16)*s^2,-6-%i*12+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - R, [1+%i*4+(2-%i*6)*s+(-2+%i*8)*s^2,-5-%i*6+(-3+%i*9)*s+(3-%i*12)*s^2;1+%i*8+(4-%i*12)*s+(-4+%i*16)*s^2,-9-%i*10+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - C, [1+%i*2+(2-%i*6)*s+(-2+%i*8)*s^2,-5-%i*10+(-3+%i*9)*s+(3-%i*12)*s^2;1+%i*2+(4-%i*12)*s+(-4+%i*16)*s^2,-9-%i*18+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - e, [1+%i*4+(2-%i*6)*s+(-2+%i*8)*s^2,-3-%i*6+(-3+%i*9)*s+(3-%i*12)*s^2;4+%i*8+(4-%i*12)*s+(-4+%i*16)*s^2,-6-%i*10+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - ec, [-3+%i*3+(2-%i*6)*s+(-2+%i*8)*s^2,-3-%i*6+(-3+%i*9)*s+(3-%i*12)*s^2;4+%i*8+(4-%i*12)*s+(-4+%i*16)*s^2,-10-%i*11+(-5+%i*15)*s+(5-%i*20)*s^2]);
assert_checkequal(PC - p, [1+%i*4+(1-%i*6)*s+(-1+%i*8)*s^2,-4-%i*6+(-4+%i*9)*s+(4-%i*12)*s^2;3+%i*8+(3-%i*12)*s+(-3+%i*16)*s^2,-6-%i*10+(-6+%i*15)*s+(6-%i*20)*s^2]);
assert_checkequal(PC - pc, [1+%i*2+(1-%i*3)*s+(-1+%i*4)*s^2,-4-%i*8+(-4+%i*12)*s+(4-%i*16)*s^2;3+%i*6+(3-%i*9)*s+(-3+%i*12)*s^2,-6-%i*12+(-6+%i*18)*s+(6-%i*24)*s^2]);
assert_checkequal(PC - P, [%i*4+(-%i*6)*s+(%i*8)*s^2,-%i*6+(%i*9)*s+(-%i*12)*s^2;%i*8+(-%i*12)*s+(%i*16)*s^2,-%i*10+(%i*15)*s+(-%i*20)*s^2]);
assert_checkequal(PC - PC, [0*s,0*s;0*s,0*s]);

assert_checkequal(SP - empty, []);
assert_checkequal(SP - r, [-2,-1,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,1;-2,-2,-2,-2,0,-2,-2,-2,-2,-2]);
assert_checkequal(SP - c, [-1-%i*2,-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 2-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2]);
assert_checkequal(SP - e, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-1;1;-1;-1;3;-1;2],[4,10]));
assert_checkequal(SP - ec, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-5-%i; 1;-5-%i;-5-%i; 3;-5-%i; 2],[4,10]));
assert_checkequal(SP - SP, sparse([],[],[4,10]));
assert_checkequal(SP - SPC, sparse([1,2;3,10;4,5],[-%i*4;-%i*12;-%i*8],[4,10]));

assert_checkequal(SPC - empty, []);
assert_checkequal(SPC - r, [-2,-1+%i*4,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2, 1+%i*12;-2,-2,-2,-2,%i*8,-2,-2,-2,-2,-2]);
assert_checkequal(SPC - c, [-1-%i*2,%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 2+%i*10;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 1+%i*6,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2]);
assert_checkequal(SPC - e, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-1; 1+%i*4;-1;-1; 3+%i*12;-1; 2+%i*8],[4,10]));
assert_checkequal(SPC - ec, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-5-%i; 1+%i*4;-5-%i;-5-%i; 3+%i*12;-5-%i; 2+%i*8],[4,10]));
assert_checkequal(SPC - SP, sparse([1,2;3,10;4,5],[%i*4;%i*12;%i*8],[4,10]));
assert_checkequal(SPC - SPC, sparse([],[],[4,10]));

assert_checkequal(i8 - empty, []);
assert_checkequal(i8 - r, int8(-10));
assert_checkequal(i8 - R, int8([-9,-10;-11,-12]));
assert_checkequal(i8 - i8, int8(0));
assert_checkequal(i8 - I8, int8([0,8;16,24]));
assert_checkequal(i8 - ui8, uint8(240));
assert_checkequal(i8 - UI8, uint8([240,232;224,216]));
assert_checkequal(i8 - i16, int16(8));
assert_checkequal(i8 - I16, int16([8,24;40,56]));
assert_checkequal(i8 - ui16, uint16(65512));
assert_checkequal(i8 - UI16, uint16([65512,65496;65480,65464]));
assert_checkequal(i8 - i32, int32(24));
assert_checkequal(i8 - I32, int32([24,56;88,120]));
assert_checkequal(i8 - ui32, uint32(4294967256));
assert_checkequal(i8 - UI32, uint32([4294967256,4294967224;4294967192,4294967160]));

assert_checkequal(I8 - empty, []);
assert_checkequal(I8 - r, int8([-10,-18;-26,-34]));
assert_checkequal(I8 - R, int8([-9,-18;-27,-36]));
assert_checkequal(I8 - i8, int8([0,-8;-16,-24]));
assert_checkequal(I8 - I8, int8([0,0;0,0]));
assert_checkequal(I8 - ui8, uint8([240,232;224,216]));
assert_checkequal(I8 - UI8, uint8([240,224;208,192]));
assert_checkequal(I8 - i16, int16([8,0;-8,-16]));
assert_checkequal(I8 - I16, int16([8,16;24,32]));
assert_checkequal(I8 - ui16, uint16([65512,65504;65496,65488]));
assert_checkequal(I8 - UI16, uint16([65512,65488;65464,65440]));
assert_checkequal(I8 - i32, int32([24,16;8,0]));
assert_checkequal(I8 - I32, int32([24,48;72,96]));
assert_checkequal(I8 - ui32, uint32([4294967256,4294967248;4294967240,4294967232]));
assert_checkequal(I8 - UI32, uint32([4294967256,4294967216;4294967176,4294967136]));

assert_checkequal(ui8 - empty, []);
assert_checkequal(ui8 - r, uint8(6));
assert_checkequal(ui8 - R, uint8([7,6;5,4]));
assert_checkequal(ui8 - i8, uint8(16));
assert_checkequal(ui8 - I8, uint8([16,24;32,40]));
assert_checkequal(ui8 - ui8, uint8(0));
assert_checkequal(ui8 - UI8, uint8([0,248;240,232]));
assert_checkequal(ui8 - i16, uint16(24));
assert_checkequal(ui8 - I16, uint16([24,40;56,72]));
assert_checkequal(ui8 - ui16, uint16(65528));
assert_checkequal(ui8 - UI16, uint16([65528,65512;65496,65480]));
assert_checkequal(ui8 - i32, uint32(40));
assert_checkequal(ui8 - I32, uint32([40,72;104,136]));
assert_checkequal(ui8 - ui32, uint32(4294967272));
assert_checkequal(ui8 - UI32, uint32([4294967272,4294967240;4294967208,4294967176]));

assert_checkequal(UI8 - empty, []);
assert_checkequal(UI8 - r, uint8([6,14;22,30]));
assert_checkequal(UI8 - R, uint8([7,14;21,28]));
assert_checkequal(UI8 - i8, uint8([16,24;32,40]));
assert_checkequal(UI8 - I8, uint8([16,32;48,64]));
assert_checkequal(UI8 - ui8, uint8([0,8;16,24]));
assert_checkequal(UI8 - UI8, uint8([0,0;0,0]));
assert_checkequal(UI8 - i16, uint16([24,32;40,48]));
assert_checkequal(UI8 - I16, uint16([24,48;72,96]));
assert_checkequal(UI8 - ui16, uint16([65528,0;8,16]));
assert_checkequal(UI8 - UI16, uint16([65528,65520;65512,65504]));
assert_checkequal(UI8 - i32, uint32([40,48;56,64]));
assert_checkequal(UI8 - I32, uint32([40,80;120,160]));
assert_checkequal(UI8 - ui32, uint32([4294967272,4294967280;4294967288,0]));
assert_checkequal(UI8 - UI32, uint32([4294967272,4294967248;4294967224,4294967200]));

assert_checkequal(i16 - empty, []);
assert_checkequal(i16 - r, int16(-18));
assert_checkequal(i16 - R, int16([-17,-18;-19,-20]));
assert_checkequal(i16 - i8, int16(-8));
assert_checkequal(i16 - I8, int16([-8,0;8,16]));
assert_checkequal(i16 - ui8, uint16(65512));
assert_checkequal(i16 - UI8, uint16([65512,65504;65496,65488]));
assert_checkequal(i16 - i16, int16(0));
assert_checkequal(i16 - I16, int16([0,16;32,48]));
assert_checkequal(i16 - ui16, uint16(65504));
assert_checkequal(i16 - UI16, uint16([65504,65488;65472,65456]));
assert_checkequal(i16 - i32, int32(16));
assert_checkequal(i16 - I32, int32([16,48;80,112]));
assert_checkequal(i16 - ui32, uint32(4294967248));
assert_checkequal(i16 - UI32, uint32([4294967248,4294967216;4294967184,4294967152]));

assert_checkequal(I16 - empty, []);
assert_checkequal(I16 - r, int16([-18,-34;-50,-66]));
assert_checkequal(I16 - R, int16([-17,-34;-51,-68]));
assert_checkequal(I16 - i8, int16([-8,-24;-40,-56]));
assert_checkequal(I16 - I8, int16([-8,-16;-24,-32]));
assert_checkequal(I16 - ui8, uint16([65512,65496;65480,65464]));
assert_checkequal(I16 - UI8, uint16([65512,65488;65464,65440]));
assert_checkequal(I16 - i16, int16([0,-16;-32,-48]));
assert_checkequal(I16 - I16, int16([0,0;0,0]));
assert_checkequal(I16 - ui16, uint16([65504,65488;65472,65456]));
assert_checkequal(I16 - UI16, uint16([65504,65472;65440,65408]));
assert_checkequal(I16 - i32, int32([16,0;-16,-32]));
assert_checkequal(I16 - I32, int32([16,32;48,64]));
assert_checkequal(I16 - ui32, uint32([4294967248,4294967232;4294967216,4294967200]));
assert_checkequal(I16 - UI32, uint32([4294967248,4294967200;4294967152,4294967104]));

assert_checkequal(ui16 - empty, []);
assert_checkequal(ui16 - r, uint16(14));
assert_checkequal(ui16 - R, uint16([15,14;13,12]));
assert_checkequal(ui16 - i8, uint16(24));
assert_checkequal(ui16 - I8, uint16([24,32;40,48]));
assert_checkequal(ui16 - ui8, uint16(8));
assert_checkequal(ui16 - UI8, uint16([8,0;65528,65520]));
assert_checkequal(ui16 - i16, uint16(32));
assert_checkequal(ui16 - I16, uint16([32,48;64,80]));
assert_checkequal(ui16 - ui16, uint16(0));
assert_checkequal(ui16 - UI16, uint16([0,65520;65504,65488]));
assert_checkequal(ui16 - i32, uint32(48));
assert_checkequal(ui16 - I32, uint32([48,80;112,144]));
assert_checkequal(ui16 - ui32, uint32(4294967280));
assert_checkequal(ui16 - UI32, uint32([4294967280,4294967248;4294967216,4294967184]));

assert_checkequal(UI16 - empty, []);
assert_checkequal(UI16 - r, uint16([14,30;46,62]));
assert_checkequal(UI16 - R, uint16([15,30;45,60]));
assert_checkequal(UI16 - i8, uint16([24,40;56,72]));
assert_checkequal(UI16 - I8, uint16([24,48;72,96]));
assert_checkequal(UI16 - ui8, uint16([8,24;40,56]));
assert_checkequal(UI16 - UI8, uint16([8,16;24,32]));
assert_checkequal(UI16 - i16, uint16([32,48;64,80]));
assert_checkequal(UI16 - I16, uint16([32,64;96,128]));
assert_checkequal(UI16 - ui16, uint16([0,16;32,48]));
assert_checkequal(UI16 - UI16, uint16([0,0;0,0]));
assert_checkequal(UI16 - i32, uint32([48,64;80,96]));
assert_checkequal(UI16 - I32, uint32([48,96;144,192]));
assert_checkequal(UI16 - ui32, uint32([4294967280,0;16,32]));
assert_checkequal(UI16 - UI32, uint32([4294967280,4294967264;4294967248,4294967232]));

assert_checkequal(i32 - empty, []);
assert_checkequal(i32 - r, int32(-34));
assert_checkequal(i32 - R, int32([-33,-34;-35,-36]));
assert_checkequal(i32 - i8, int32(-24));
assert_checkequal(i32 - I8, int32([-24,-16;-8,0]));
assert_checkequal(i32 - ui8, uint32(4294967256));
assert_checkequal(i32 - UI8, uint32([4294967256,4294967248;4294967240,4294967232]));
assert_checkequal(i32 - i16, int32(-16));
assert_checkequal(i32 - I16, int32([-16,0;16,32]));
assert_checkequal(i32 - ui16, uint32(4294967248));
assert_checkequal(i32 - UI16, uint32([4294967248,4294967232;4294967216,4294967200]));
assert_checkequal(i32 - i32, int32(0));
assert_checkequal(i32 - I32, int32([0,32;64,96]));
assert_checkequal(i32 - ui32, uint32(4294967232));
assert_checkequal(i32 - UI32, uint32([4294967232,4294967200;4294967168,4294967136]));

assert_checkequal(I32 - empty, []);
assert_checkequal(I32 - r, int32([-34,-66;-98,-130]));
assert_checkequal(I32 - R, int32([-33,-66;-99,-132]));
assert_checkequal(I32 - i8, int32([-24,-56;-88,-120]));
assert_checkequal(I32 - I8, int32([-24,-48;-72,-96]));
assert_checkequal(I32 - ui8, uint32([4294967256,4294967224;4294967192,4294967160]));
assert_checkequal(I32 - UI8, uint32([4294967256,4294967216;4294967176,4294967136]));
assert_checkequal(I32 - i16, int32([-16,-48;-80,-112]));
assert_checkequal(I32 - I16, int32([-16,-32;-48,-64]));
assert_checkequal(I32 - ui16, uint32([4294967248,4294967216;4294967184,4294967152]));
assert_checkequal(I32 - UI16, uint32([4294967248,4294967200;4294967152,4294967104]));
assert_checkequal(I32 - i32, int32([0,-32;-64,-96]));
assert_checkequal(I32 - I32, int32([0,0;0,0]));
assert_checkequal(I32 - ui32, uint32([4294967232,4294967200;4294967168,4294967136]));
assert_checkequal(I32 - UI32, uint32([4294967232,4294967168;4294967104,4294967040]));

assert_checkequal(ui32 - empty, []);
assert_checkequal(ui32 - r, uint32(30));
assert_checkequal(ui32 - R, uint32([31,30;29,28]));
assert_checkequal(ui32 - i8, uint32(40));
assert_checkequal(ui32 - I8, uint32([40,48;56,64]));
assert_checkequal(ui32 - ui8, uint32(24));
assert_checkequal(ui32 - UI8, uint32([24,16;8,0]));
assert_checkequal(ui32 - i16, uint32(48));
assert_checkequal(ui32 - I16, uint32([48,64;80,96]));
assert_checkequal(ui32 - ui16, uint32(16));
assert_checkequal(ui32 - UI16, uint32([16,0;4294967280,4294967264]));
assert_checkequal(ui32 - i32, uint32(64));
assert_checkequal(ui32 - I32, uint32([64,96;128,160]));
assert_checkequal(ui32 - ui32, uint32(0));
assert_checkequal(ui32 - UI32, uint32([0,4294967264;4294967232,4294967200]));

assert_checkequal(UI32 - empty, []);
assert_checkequal(UI32 - r, uint32([30,62;94,126]));
assert_checkequal(UI32 - R, uint32([31,62;93,124]));
assert_checkequal(UI32 - i8, uint32([40,72;104,136]));
assert_checkequal(UI32 - I8, uint32([40,80;120,160]));
assert_checkequal(UI32 - ui8, uint32([24,56;88,120]));
assert_checkequal(UI32 - UI8, uint32([24,48;72,96]));
assert_checkequal(UI32 - i16, uint32([48,80;112,144]));
assert_checkequal(UI32 - I16, uint32([48,96;144,192]));
assert_checkequal(UI32 - ui16, uint32([16,48;80,112]));
assert_checkequal(UI32 - UI16, uint32([16,32;48,64]));
assert_checkequal(UI32 - i32, uint32([64,96;128,160]));
assert_checkequal(UI32 - I32, uint32([64,128;192,256]));
assert_checkequal(UI32 - ui32, uint32([0,32;64,96]));
assert_checkequal(UI32 - UI32, uint32([0,0;0,0]));



// OPPOSITE

// - []
assert_checkequal(-[], []);
// - eye
assert_checkequal(-e, eye() * -1);
// - eye complex
assert_checkequal(-ec, (-5-%i) * eye());
// - double
assert_checkequal(-r, -2);
// - double complex
assert_checkequal(-c, - 1 - 2*%i);
// - DOUBLE
assert_checkequal(-R, [-1 -2;-3 -4]);
// - DOUBLE COMPLEX
assert_checkequal(-C, [-1-2*%i -2-4*%i;-3-6*%i -4-8*%i]);
// - int
assert_checkequal(-i8, int8(8));
assert_checkequal(-i16, int16(16));
assert_checkequal(-i32, int32(32));
assert_checkequal(-ui8, uint8(-8));
assert_checkequal(-ui16, uint16(-16));
assert_checkequal(-ui32, uint32(-32));
// - INT
assert_checkequal(-I8, int8([8 16 ; 24 32]));
assert_checkequal(-I16, int16([16 32 ; 48 64]));
assert_checkequal(-I32, int32([32 64 ; 96 128]));
assert_checkequal(-UI8, uint8([-8 -16 ; -24 -32]));
assert_checkequal(-UI16, uint16([-16 -32 ; -48 -64]));
assert_checkequal(-UI32, uint32([-32 -64 ; -96 -128]));
// - SPARSE
assert_checkequal(-SP, sparse([1,2;4,5;3,10],[-1,-2,-3]));
// - SPARSE COMPLEX
assert_checkequal(-SPC, sparse([1,2;4,5;3,10],[1,2,3]) * ( -1 - 4*%i));
// - polynom
assert_checkequal(-p, -1-%s+%s**2);
// - polynom complex
assert_checkequal(-pc, - 1 - %s + %s**2 + ( - 2 + 3 * %s - 4 * %s**2 ) * %i);
// - POLYNOM
assert_checkequal(-P, [-2-2*%s+2*%s**2 3+3*%s-3*%s**2; -4-4*%s+4*%s**2, 5+5*%s-5*%s**2]);
// - POLYNOM COMPLEX
assert_checkequal(-PC, [-2-2*%s+2*%s**2 3+3*%s-3*%s**2; -4-4*%s+4*%s**2, 5+5*%s-5*%s**2] + ([-4+6*%s-8*%s**2 6-9*%s+12*%s**2; -8+12*%s-16*%s**2 10-15*%s+20*%s**2] * %i));

