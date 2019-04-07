// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017, 2019 - Samuel GOUGEON
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

// Structures:
// ----------
// Scalar:
s = struct("r",1.234-5.6*%i,"b",%f,"t","Hello","i",int8([123 -76]),"l",list(4,,%i));
ss = sci2exp(s);
ssref = "struct(""r"",1.234-%i*5.6, ""b"",%f, ""t"",""Hello"", ""i"",int8([123,-76]), ""l"",list(4,,%i))";
assert_checkequal(ss, ssref);
assert_checkequal(evstr(ss), s);
// Array:
s = struct("à", {3.1415,%t,"abc";%z,1-1/%s,%i}, "é",{2.7183,3,"test"; %z^2,list(,7),-%i});
ssref = "matrix(struct(""à"",{3.1415,%z,%t,rlist(-1+%s,%s),""abc"",%i}, ""é"",{2.7183,%z^2,3,list(,7),""test"",-%i}),[2 3])";
assert_checkequal(sci2exp(s), ssref);
assert_checkequal(evstr(ssref), s);
