// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// unit tests for sylm()
// =====================
// https://en.wikipedia.org/wiki/Sylvester_matrix
// http://bugzilla.scilab.org/15744

assert_checkequal(sylm(0*%s, 0*%s), []);
assert_checkequal(sylm(1+0*%s, 0*%s), []);
assert_checkequal(sylm(0*%s, 2+0*%s), []);
assert_checkequal(sylm(3+0*%s, 2+0*%s), []);

assert_checkequal(sylm(1+0*%s, %s), 1);
assert_checkequal(sylm(3+0*%s, %s^2), eye(2,2)*3);
assert_checkequal(sylm(%i+0*%s, (1-%s)^3), eye(3,3)*%i);

for j = 1:10    // 10 random examples
    d1 = grand(1,1,"uin",1,5);
    p1 = poly(grand(1,d1+1,"uin",-9,9),"s","coeff");
    if degree(p1) < d1 then
        p1 = p1 + %s^d1;
    end

    d2 = grand(1,1,"uin",1,5);
    p2 = poly(grand(1,d2+1,"uin",-9,9),"s","coeff");
    if degree(p2) < d2 then
        p2 = p2 + %s^d2;
    end
    xy  = diophant([p1 p2],1);
    [x,y] = (xy(1), xy(2));         // p1*x1 + p2*x2 = 1;
    // assert_checkalmostequal(clean(p1*x + p2*y, 1e-15), 1+0*%s, 1e-15)
    S = sylm(p1, p2)

    ref = coeff(p1*x + p2*y).'
    d = degree(x) + degree(y)
    if length(ref) < d+2
        ref = [ref ; zeros(d+2-length(ref), 1)]
    end
    assert_checkalmostequal(S*[coeff(x) coeff(y)].', ref, 1e-14, 1e-14);
end
