// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 15260 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15260
//
// <-- Short Description -->
// sci2exp() was broken for all types of hypermatrix, simple structure or
//  arrays of structures, and simple cell or arrays of cells.

// Hypermatrix of booleans
b = rand(4,5,3,2)<0.5;
tb = sci2exp(b);
assert_checkequal(evstr(tb), b);

// Hypermatrix of encoded integers
i = int8(rand(4,5,3,2)*100);
ti = sci2exp(i);
assert_checkequal(evstr(ti), i);

// Hypermatrix of complex numbers
c = rand(4,5,3,2) + rand(4,5,3,2)*%i;
tc = sci2exp(c);
assert_checkalmostequal(evstr(tc), c, 1e-7, 1e-7);

// Hypermatrix of complex polynomials
p = (rand(4,5,3) + rand(4,5,3)*%i - %z).^fix(rand(4,5,3)*4);
tp = sci2exp(p);
z = %z;
rp = evstr(tp);
r = clean(rp-p, 1e-7);
assert_checkequal(sum(abs(r)),0*%z);

// Hypermatrix of strings
s = string(rand(4,5,3,2)*100);
ts = sci2exp(s);
assert_checkequal(evstr(ts), s);

// Array of cells
c = {1,%t,"A",%z,%i,0,list(2,"B"),"B",%f,3.1415,"abc",list(%i,"cat")};
c = matrix(c, [2 3 1 2]);
tc = sci2exp(c);
z = %z;
rc = evstr(tc);
assert_checkequal(rc,c);

// Scalar structure:
s = struct("r",1.234-5.6*%i,"b",%f,"t","Hello","i",int8([123 -76]),"l",list(4,,%i));
ss = sci2exp(s);
ssref = "struct(""r"",1.234-%i*5.6, ""b"",%f, ""t"",""Hello"", ""i"",int8([123,-76]), ""l"",list(4,,%i))";
assert_checkequal(ss, ssref);
assert_checkequal(evstr(ss), s);

// Array of structures
// =======================================================================
// There is no longer any way to generate nand initialize from scratch any
//  non-scalar array of structures.
// =======================================================================
// This part shall be rewritten when struct(dims,field1,listOfValues1, ..)
// will have been implemented (= bug http://bugzilla.scilab.org/15264 fixed)
clear s
s.r = 1.2345;
s(1,2).r = %i;
s(2,2).r = 1-%i;
s(2,1).r = 2.345;
s.t = "A";
s(1,2).t = "C";
s(2,2).t = "D";
s(2,1).t = "B";
tmpref = mlist(["st","dims","r","t"],int32([2,2]),list(1.2345,2.345,%i,1-%i),list("A","B","C","D"));
ssref = "mlist([""st"",""dims"",""r"",""t""],int32([2,2]),list(1.2345,2.345,%i,1-%i),list(""A"",""B"",""C"",""D""))";
assert_checkequal(sci2exp(s), ssref);
assert_checkequal(evstr(sci2exp(s)), tmpref);
