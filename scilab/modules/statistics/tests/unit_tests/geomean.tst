// =======================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =======================================================================
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// No specific expected console output to be checked against:
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//

x = grand(50,40,"uin", 1, 1000);
y = geomean(x);
yi = geomean(1 ./ x);
assert_checkequal(y*yi, 1);
yc = geomean([x, 1../x], "c");
assert_checkalmostequal(yc, ones(x(:,1)), 10*%eps);
yr = geomean([x ; 1../x], "r");
assert_checkalmostequal(yr, ones(x(1,:)), 10*%eps);

// With a special spreading value somewhere
sv = [0 %inf -%inf %nan];
for v = sv
    xs = x;
    xs(20,30) = v;
    if v==-%inf
        r = %inf*(1+%i);
    else
        r = v;
    end
    assert_checkequal(geomean(xs), r);
    ysc = geomean(xs, "c");
    ysc(20) = r;
    assert_checkalmostequal(geomean(xs, "c"), ysc, 10*%eps);
    ysr = geomean(xs, "r");
    ysr(30) = r;
    assert_checkalmostequal(geomean(xs, "r"), ysr, 10*%eps);
end

// With an homogeneous matrix, possibly negative or complex or huge value
sv = [1 2 -2 %i -%i 1.e234];
for v = sv
    xs = ones(50, 40)*v;
    assert_checkalmostequal(clean(geomean(xs)), v, 1e-11);
    assert_checkalmostequal(clean(geomean(xs, "c")), ones(x(:,1))*v, 1e-11);
    assert_checkalmostequal(clean(geomean(xs, "r")), ones(x(1,:))*v, 1e-11);
end

// Hypermatrices
x = grand(30,20,10, "uin", 1, 1000);
y = geomean(x);
yi = geomean(1 ./ x);
assert_checkequal(y*yi, 1);
yc = geomean([x, 1../x], "c");
assert_checkalmostequal(yc, ones(x(:,1,:)), 10*%eps);
yr = geomean([x ; 1../x], "r");
assert_checkalmostequal(yr, ones(x(1,:,:)), 10*%eps);
xe = x;
xe(:,:,11:20) = 1../x;
y3 = geomean(xe, 3);
assert_checkalmostequal(y3, ones(x(:,:,1)), 10*%eps);

// Forbidden data types
assert_checkfalse(execstr("geomean(x>500)", "errcatch")==0);
assert_checkfalse(execstr("geomean(int16(x))", "errcatch")==0);
assert_checkfalse(execstr("geomean(uint16(x))", "errcatch")==0);
assert_checkfalse(execstr("geomean(x+0*%z)", "errcatch")==0);
assert_checkfalse(execstr("geomean(sparse(x))", "errcatch")==0);
// Forbidden orientations
assert_checkfalse(execstr("geomean(x,''g'')", "errcatch")==0);
assert_checkfalse(execstr("geomean(x,[''c'' ''r''])", "errcatch")==0);
assert_checkfalse(execstr("geomean(x,[1 2])", "errcatch")==0);
assert_checkfalse(execstr("geomean(x,-2)", "errcatch")==0);
assert_checkfalse(execstr("geomean(x,0)", "errcatch")==0);
assert_checkfalse(execstr("geomean(x,4)", "errcatch")==0);
