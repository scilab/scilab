// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->

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
SP = sparse([1,2;4,5;3,10],[1,2,3]);
SPC = sparse([1,2;4,5;3,10],[1,2,3]) * ( 1 + 4*%i);

i8 = int8(-8);
i16 = int16(-16);
i32 = int32(-32);
i64 = int64(-64);
ui8 = uint8(8);
ui16 = uint16(16);
ui32 = uint32(32);
ui64 = uint64(64);

I8 = int8([-8 -16 ; -24 -32]);
I16 = int16([-16 -32 ; -48 -64]);
I32 = int32([-32 -64 ; -96 -128]);
I64 = int64([-64 -128 ; -192 -256]);
UI8 = uint8([8 16 ; 24 32]);
UI16 = uint16([16 32 ; 48 64]);
UI32 = uint32([32 64 ; 96 128]);
UI64 = uint64([64 128 ; 192 256]);

//Empty
assert_checkequal([] - [], []);
assert_checkequal([] - r, -r);
assert_checkequal([] - R, -R);
assert_checkequal([] - R3, -R3);
assert_checkequal([] - c, -c);
assert_checkequal([] - C, -C);
assert_checkequal([] - C3, -C3);
assert_checkequal([] - e, -e);
assert_checkequal([] - ec, -ec);
assert_checkequal([] - p, -p);
assert_checkequal([] - pc, -pc);
assert_checkequal([] - P, -P);
assert_checkequal([] - PC, -PC);
assert_checkequal([] - SP, -SP);
assert_checkequal([] - SPC, -SPC);
assert_checkequal([] - i8, -i8);
assert_checkequal([] - I8, -I8);
assert_checkequal([] - ui8, -ui8);
assert_checkequal([] - UI8, -UI8);
assert_checkequal([] - ui16, -ui16);
assert_checkequal([] - UI16, -UI16);
assert_checkequal([] - ui16, -ui16);
assert_checkequal([] - UI16, -UI16);
assert_checkequal([] - ui32, -ui32);
assert_checkequal([] - UI32, -UI32);
assert_checkequal([] - ui32, -ui32);
assert_checkequal([] - UI32, -UI32);
assert_checkequal([] - ui64, -ui64);
assert_checkequal([] - UI64, -UI64);
assert_checkequal([] - ui64, -ui64);
assert_checkequal([] - UI64, -UI64);

//Identity
assert_checkequal(e - [], e);
assert_checkequal(ec - [], ec);
assert_checkequal(e - r, -1);
assert_checkequal(ec - r, 3+%i);
assert_checkequal(e - R, [0 -2;-3 -3]);
assert_checkequal(ec - R, [4+%i -2;-3 1+%i]);
assert_checkequal(e - c, -2*%i);
assert_checkequal(ec - c, 4 - %i);
assert_checkequal(e - C, [-2*%i -2-4*%i;-3-6*%i -3-8*%i]);
assert_checkequal(ec - C, [4-%i -2-4*%i;-3-6*%i 1-7*%i]);
assert_checkequal(e - e, eye() * 0);
assert_checkequal(ec - e, eye() * (4+%i));
assert_checkequal(e - ec, eye() * (-4-%i));
assert_checkequal(ec - ec, eye() * 0 * %i);
assert_checkequal(e - p, -%s+%s**2);
assert_checkequal(ec - p, 4-%s+%s**2 + %i);
assert_checkequal(e - pc, -%s+%s**2 + (-2+3*%s-4*%s**2)*%i);
assert_checkequal(ec - pc, 4-%s+%s**2 + (-1+3*%s-4*%s**2)*%i);
assert_checkequal(e - P, [-1-2*%s+2*%s**2 3+3*%s-3*%s**2;-4-4*%s+4*%s**2 6+5*%s-5*%s**2]);
assert_checkequal(ec - P, [3+%i-2*%s+2*%s^2,3+3*%s-3*%s^2;-4-4*%s+4*%s^2,10+%i+5*%s-5*%s^2]);
assert_checkequal(e - PC, [-1-%i*4+(-2+%i*6)*%s+(2-%i*8)*%s^2,3+%i*6+(3-%i*9)*%s+(-3+%i*12)*%s^2;-4-%i*8+(-4+%i*12)*%s+(4-%i*16)*%s^2,6+%i*10+(5-%i*15)*%s+(-5+%i*20)*%s^2]);
assert_checkequal(ec - PC, [3-%i*3+(-2+%i*6)*%s+(2-%i*8)*%s^2,3+%i*6+(3-%i*9)*%s+(-3+%i*12)*%s^2;-4-%i*8+(-4+%i*12)*%s+(4-%i*16)*%s^2,10+%i*11+(5-%i*15)*%s+(-5+%i*20)*%s^2]);
assert_checkequal(e - SP, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[1;-1;1;1;-3;1;-2],[4,10]));
assert_checkequal(ec - SP, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 5+%i;-1; 5+%i; 5+%i;-3; 5+%i;-2],[4,10]));
assert_checkequal(e - SPC, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 1;-1-%i*4; 1; 1;-3-%i*12; 1;-2-%i*8],[4,10]));
assert_checkequal(ec - SPC, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[ 5+%i;-1-%i*4; 5+%i; 5+%i;-3-%i*12; 5+%i;-2-%i*8],[4,10]));
assert_checkequal(e - i8, int8(9));
assert_checkequal(e - I8, int8([9 16;24 33]));
assert_checkequal(e - ui8, uint8(249));
assert_checkequal(e - UI8, uint8([249 240;232 225]));
assert_checkequal(e - i16, int16(17));
assert_checkequal(e - I16, int16([17,32;48,65]));
assert_checkequal(e - ui16, uint16(65521));
assert_checkequal(e - UI16, uint16([65521,65504;65488,65473]));
assert_checkequal(e - i32, int32(33));
assert_checkequal(e - I32, int32([33,64;96,129]));
assert_checkequal(e - ui32, uint32(4294967265));
assert_checkequal(e - UI32, uint32([4294967265,4294967232;4294967200,4294967169]));
assert_checkequal(e - i64, int64(65));
assert_checkequal(e - I64, int64([65,128;192,257]));

// double
assert_checkequal(r - [], r);
assert_checkequal(r - e, 1);
assert_checkequal(r - ec, -3-%i);
assert_checkequal(c - [], c);
assert_checkequal(c - e, 2*%i);
assert_checkequal(c - ec, -4+%i);
assert_checkequal(R - [], R);
assert_checkequal(R - e, [0 2; 3 3]);
assert_checkequal(R - ec, [-4-%i, 2; 3,-1-%i]);
assert_checkequal(C - [], C);
assert_checkequal(C - e, [%i*2, 2+%i*4; 3+%i*6, 3+%i*8]);
assert_checkequal(C - ec, [-4+%i, 2+%i*4; 3+%i*6,-1+%i*7]);

assert_checkequal(r - r, 0);
assert_checkequal(r - c, 1-%i*2);
assert_checkequal(c - r, -1+%i*2);
assert_checkequal(c - c, 0*%i);

assert_checkequal(r - R, [1,0;-1,-2]);
rR3ref(:,:,1) = [1,0;-1,-2];
rR3ref(:,:,2) = [1,-1;0,-2] ;
assert_checkequal(r - R3, rR3ref);

assert_checkequal(r - C, [ 1-%i*2,-%i*4;-1-%i*6,-2-%i*8]);
rC3ref(:,:,1) = [ 1-%i*2,-%i*4;-1-%i*6,-2-%i*8];
rC3ref(:,:,2) = [ 1+%i*2,-1+%i*6;%i*4,-2+%i*8];
assert_checkequal(r - C3, rC3ref);

assert_checkequal(c - R, [%i*2,-1+%i*2;-2+%i*2,-3+%i*2]);
cR3ref(:,:,1) = [%i*2,-1+%i*2;-2+%i*2,-3+%i*2];
cR3ref(:,:,2) = [%i*2,-2+%i*2;-1+%i*2,-3+%i*2];
assert_checkequal(c - R3, cR3ref);

assert_checkequal(c - C, [0,-1-%i*2;-2-%i*4,-3-%i*6]);
cC3ref(:,:,1) = [0,-1-%i*2;-2-%i*4,-3-%i*6];
cC3ref(:,:,2) = [%i*4,-2+%i*8;-1+%i*6,-3+%i*10];
assert_checkequal(c - C3, cC3ref);

assert_checkequal(R - r, [-1,0;1,2]);
R3rref(:,:,1) = [-1,0;1,2];
R3rref(:,:,2) = [-1,1;0,2];
assert_checkequal(R3 - r, R3rref);

assert_checkequal(R - c, [-%i*2, 1-%i*2; 2-%i*2, 3-%i*2]);
R3cref(:,:,1) = [-%i*2, 1-%i*2; 2-%i*2, 3-%i*2];
R3cref(:,:,2) = [-%i*2, 2-%i*2; 1-%i*2, 3-%i*2];
assert_checkequal(R3 - c, R3cref);

assert_checkequal(C - r, [-1+%i*2,%i*4; 1+%i*6, 2+%i*8]);
C3rref(:,:,1) = [-1+%i*2,%i*4; 1+%i*6, 2+%i*8];
C3rref(:,:,2) = [-1-%i*2, 1-%i*6;-%i*4, 2-%i*8];
assert_checkequal(C3 - r, C3rref);

assert_checkequal(C - c, [0, 1+%i*2; 2+%i*4, 3+%i*6]);
C3cref(:,:,1) = [0, 1+%i*2; 2+%i*4, 3+%i*6];
C3cref(:,:,2) = [-%i*4, 2-%i*8; 1-%i*6, 3-%i*10];
assert_checkequal(C3 - c, C3cref);

assert_checkequal(R - R, [0,0;0,0]);
R3R3ref(:,:,1) = [0,0;0,0];
R3R3ref(:,:,2) = [0,0;0,0];
assert_checkequal(R3 - R3, R3R3ref);

assert_checkequal(R - C, [-%i*2,-%i*4;-%i*6,-%i*8]);
R3C3ref(:,:,1) = [-%i*2,-%i*4;-%i*6,-%i*8];
R3C3ref(:,:,2) = [%i*2,%i*6;%i*4,%i*8];
assert_checkequal(R3 - C3, R3C3ref);

assert_checkequal(C - R, [%i*2,%i*4;%i*6,%i*8]);
C3R3ref(:,:,1) = [%i*2,%i*4;%i*6,%i*8];
C3R3ref(:,:,2) = [-%i*2,-%i*6;-%i*4,-%i*8];
assert_checkequal(C3 - R3, C3R3ref);

assert_checkequal(C - C, [0,0;0,0]*%i);
C3C3ref(:,:,1) = [0,0;0,0];
C3C3ref(:,:,2) = [0,0;0,0];
assert_checkequal(C3 - C3, C3C3ref*%i);

assert_checkequal(r - SP, [2,1,2,2,2,2,2,2,2,2;2,2,2,2,2,2,2,2,2,2;2,2,2,2,2,2,2,2,2,-1;2,2,2,2,0,2,2,2,2,2] );
assert_checkequal(r - SPC, [ 2, 1-%i*4, 2, 2, 2, 2, 2, 2, 2, 2; 2, 2, 2, 2, 2, 2, 2, 2, 2, 2; 2, 2, 2, 2, 2, 2, 2, 2, 2,-1-%i*12; 2, 2, 2, 2,-%i*8, 2, 2, 2, 2, 2]);
assert_checkequal(c - SP, [ 1+%i*2,%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+ %i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-2+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2]);
assert_checkequal(c - SPC, [ 1+%i*2,-%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2;1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-2-%i*10; 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2,-1-%i*6, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2, 1+%i*2]);


// polynom
assert_checkequal(p - [], p);
assert_checkequal(p - e, %s - %s**2);
assert_checkequal(p - ec, -4-%i+%s-%s**2);
assert_checkequal(p - r, -1+%s-%s**2);
assert_checkequal(p - c, -%i*2+%s-%s**2);
assert_checkequal(p - R, [%s-%s**2,-1+%s-%s**2;-2+%s-%s**2,-3+%s-%s**2]);
assert_checkequal(p - C, [-%i*2+%s-%s**2,-1-%i*4+%s-%s**2;-2-%i*6+%s-%s**2,-3-%i*8+%s-%s**2]);

assert_checkequal(pc - [], pc);
assert_checkequal(pc - e, %i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2);
assert_checkequal(pc - ec, -4+%i+(1-%i*3)*%s+(-1+%i*4)*%s**2);
assert_checkequal(pc - r, -1+%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2);
assert_checkequal(pc - c, (1-%i*3)*%s+(-1+%i*4)*%s**2);
assert_checkequal(pc - R, [%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2,-1+%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2;-2+%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2,-3+%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2]);
assert_checkequal(pc - C, [(1-%i*3)*%s+(-1+%i*4)*%s**2,-1-%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2;-2-%i*4+(1-%i*3)*%s+(-1+%i*4)*%s**2,-3-%i*6+(1-%i*3)*%s+(-1+%i*4)*%s**2]);

assert_checkequal(P - [], P);
assert_checkequal(P - e, [1+2*%s-2*%s**2,-3-3*%s+3*%s**2;4+4*%s-4*%s**2,-6-5*%s+5*%s**2] );
assert_checkequal(P - ec, [-3-%i+2*%s-2*%s**2,-3-3*%s+3*%s**2;4+4*%s-4*%s**2,-10-%i-5*%s+5*%s**2]);
assert_checkequal(P - r, [2*%s-2*%s**2,-5-3*%s+3*%s**2;2+4*%s-4*%s**2,-7-5*%s+5*%s**2]);
assert_checkequal(P - c, [1-%i*2+2*%s-2*%s**2,-4-%i*2-3*%s+3*%s**2;3-%i*2+4*%s-4*%s**2,-6-%i*2-5*%s+5*%s**2]);
assert_checkequal(P - R, [1+2*%s-2*%s**2,-5-3*%s+3*%s**2;1+4*%s-4*%s**2,-9-5*%s+5*%s**2] );
assert_checkequal(P - C, [1-%i*2+2*%s-2*%s**2,-5-%i*4-3*%s+3*%s**2;1-%i*6+4*%s-4*%s**2,-9-%i*8-5*%s+5*%s**2]);

assert_checkequal(PC - [], PC);
assert_checkequal(PC - e, [1+%i*4+(2-%i*6)*%s+(-2+%i*8)*%s**2,-3-%i*6+(-3+%i*9)*%s+(3-%i*12)*%s**2;4+%i*8+(4-%i*12)*%s+(-4+%i*16)*%s**2,-6-%i*10+(-5+%i*15)*%s+(5-%i*20)*%s**2]);
assert_checkequal(PC - ec, [-3+%i*3+(2-%i*6)*%s+(-2+%i*8)*%s**2,-3-%i*6+(-3+%i*9)*%s+(3-%i*12)*%s**2;4+%i*8+(4-%i*12)*%s+(-4+%i*16)*%s**2,-10-%i*11+(-5+%i*15)*%s+(5-%i*20)*%s**2]);
assert_checkequal(PC - r, [%i*4+(2-%i*6)*%s+(-2+%i*8)*%s**2,-5-%i*6+(-3+%i*9)*%s+(3-%i*12)*%s**2;2+%i*8+(4-%i*12)*%s+(-4+%i*16)*%s**2,-7-%i*10+(-5+%i*15)*%s+(5-%i*20)*%s**2]);
assert_checkequal(PC - c, [1+%i*2+(2-%i*6)*%s+(-2+%i*8)*%s**2,-4-%i*8+(-3+%i*9)*%s+(3-%i*12)*%s**2;3+%i*6+(4-%i*12)*%s+(-4+%i*16)*%s**2,-6-%i*12+(-5+%i*15)*%s+(5-%i*20)*%s**2]);
assert_checkequal(PC - R, [1+%i*4+(2-%i*6)*%s+(-2+%i*8)*%s**2,-5-%i*6+(-3+%i*9)*%s+(3-%i*12)*%s**2;1+%i*8+(4-%i*12)*%s+(-4+%i*16)*%s**2,-9-%i*10+(-5+%i*15)*%s+(5-%i*20)*%s**2]);
assert_checkequal(PC - C, [1+%i*2+(2-%i*6)*%s+(-2+%i*8)*%s**2,-5-%i*10+(-3+%i*9)*%s+(3-%i*12)*%s**2;1+%i*2+(4-%i*12)*%s+(-4+%i*16)*%s**2,-9-%i*18+(-5+%i*15)*%s+(5-%i*20)*%s**2]);

assert_checkequal(p - p, 0*%s);
assert_checkequal(p - pc, -%i*2+(%i*3)*%s+(-%i*4)*%s**2);
assert_checkequal(p - P, [-1-%s+%s**2,4+4*%s-4*%s**2;-3-3*%s+3*%s**2,6+6*%s-6*%s**2]);
assert_checkequal(p - PC, [-1-%i*4+(-1+%i*6)*%s+(1-%i*8)*%s**2,4+%i*6+(4-%i*9)*%s+(-4+%i*12)*%s**2;-3-%i*8+(-3+%i*12)*%s+(3-%i*16)*%s**2,6+%i*10+(6-%i*15)*%s+(-6+%i*20)*%s**2]);
assert_checkequal(P - p, [1+%s-%s**2,-4-4*%s+4*%s**2;3+3*%s-3*%s**2,-6-6*%s+6*%s**2]);
assert_checkequal(P - pc, [1-%i*2+(1+%i*3)*%s+(-1-%i*4)*%s**2,-4-%i*2+(-4+%i*3)*%s+(4-%i*4)*%s**2;3-%i*2+(3+%i*3)*%s+(-3-%i*4)*%s**2,-6-%i*2+(-6+%i*3)*%s+(6-%i*4)*%s**2]);
assert_checkequal(P - P, [0*%s,0*%s;0*%s,0*%s]);
assert_checkequal(P - PC, [-%i*4+(%i*6)*%s+(-%i*8)*%s**2,%i*6+(-%i*9)*%s+(%i*12)*%s**2;-%i*8+(%i*12)*%s+(-%i*16)*%s**2,%i*10+(-%i*15)*%s+(%i*20)*%s**2]);
assert_checkequal(pc - p, %i*2+(-%i*3)*%s+(%i*4)*%s**2);
assert_checkequal(pc - pc, 0*%s);
assert_checkequal(pc - P, [-1+%i*2+(-1-%i*3)*%s+(1+%i*4)*%s**2,4+%i*2+(4-%i*3)*%s+(-4+%i*4)*%s**2;-3+%i*2+(-3-%i*3)*%s+(3+%i*4)*%s**2,6+%i*2+(6-%i*3)*%s+(-6+%i*4)*%s**2]);
assert_checkequal(pc - PC, [-1-%i*2+(-1+%i*3)*%s+(1-%i*4)*%s**2,4+%i*8+(4-%i*12)*%s+(-4+%i*16)*%s**2;-3-%i*6+(-3+%i*9)*%s+(3-%i*12)*%s**2,6+%i*12+(6-%i*18)*%s+(-6+%i*24)*%s**2]);
assert_checkequal(PC - p, [1+%i*4+(1-%i*6)*%s+(-1+%i*8)*%s**2,-4-%i*6+(-4+%i*9)*%s+(4-%i*12)*%s**2;3+%i*8+(3-%i*12)*%s+(-3+%i*16)*%s**2,-6-%i*10+(-6+%i*15)*%s+(6-%i*20)*%s**2]);
assert_checkequal(PC - pc, [1+%i*2+(1-%i*3)*%s+(-1+%i*4)*%s**2,-4-%i*8+(-4+%i*12)*%s+(4-%i*16)*%s**2;3+%i*6+(3-%i*9)*%s+(-3+%i*12)*%s**2,-6-%i*12+(-6+%i*18)*%s+(6-%i*24)*%s**2]);
assert_checkequal(PC - P, [%i*4+(-%i*6)*%s+(%i*8)*%s**2,-%i*6+(%i*9)*%s+(-%i*12)*%s**2;%i*8+(-%i*12)*%s+(%i*16)*%s**2,-%i*10+(%i*15)*%s+(-%i*20)*%s**2]);
assert_checkequal(PC - PC, [0*%s,0*%s;0*%s,0*%s]);

assert_checkequal(SP - [], SP);
assert_checkequal(SPC - [], SPC);
assert_checkequal(SP - e, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-1;1;-1;-1;3;-1;2],[4,10]));
assert_checkequal(SP - ec, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-5-%i; 1;-5-%i;-5-%i; 3;-5-%i; 2],[4,10]));
assert_checkequal(SPC - e, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-1; 1+%i*4;-1;-1; 3+%i*12;-1; 2+%i*8],[4,10]));
assert_checkequal(SPC - ec, sparse([1,1;1,2;2,2;3,3;3,10;4,4;4,5],[-5-%i; 1+%i*4;-5-%i;-5-%i; 3+%i*12;-5-%i; 2+%i*8],[4,10]));
assert_checkequal(SP - r, [-2,-1,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,1;-2,-2,-2,-2,0,-2,-2,-2,-2,-2]);
assert_checkequal(SP - c, [-1-%i*2,-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 2-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2]);
assert_checkequal(SPC - r, [-2,-1+%i*4,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2,-2;-2,-2,-2,-2,-2,-2,-2,-2,-2, 1+%i*12;-2,-2,-2,-2,%i*8,-2,-2,-2,-2,-2]);
assert_checkequal(SPC - c, [-1-%i*2,%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 2+%i*10;-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2, 1+%i*6,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2,-1-%i*2]);



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
assert_checkequal(-i64, int64(64));
assert_checkequal(-ui8, uint8(-8));
assert_checkequal(-ui16, uint16(-16));
assert_checkequal(-ui32, uint32(-32));
assert_checkequal(-ui64, uint64(-64));
// - INT
assert_checkequal(-I8, int8([8 16 ; 24 32]));
assert_checkequal(-I16, int16([16 32 ; 48 64]));
assert_checkequal(-I32, int32([32 64 ; 96 128]));
assert_checkequal(-I64, int64([64 128 ; 192 256]));
assert_checkequal(-UI8, uint8([-8 -16 ; -24 -32]));
assert_checkequal(-UI16, uint16([-16 -32 ; -48 -64]));
assert_checkequal(-UI32, uint32([-32 -64 ; -96 -128]));
assert_checkequal(-UI64, uint64([-64 -128 ; -192 -256]));
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

