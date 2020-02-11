// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

// normal behavior with default epsA=1e-10, epsR=1e-10
big = 1e30
small = 1;
a1 = [small big];
b1 = [0 big];
a2 = [complex(small,small) big];
b2 = [complex(0,0) big];
a3 = [complex(0,big) small complex(big,small)];
b3 = [complex(0,big) 0 big];
//double
assert_checkequal(clean(a1),b1);
assert_checktrue(clean(a2) == b2); // b2 is complex with zero realpart
assert_checktrue(clean(a3) == b3); // b3 is complex with zero realpart
//polynomial
assert_checkequal(clean(poly(a1,"x","coeff")),poly(b1,"x","coeff"));
assert_checkequal(clean(poly(a2,"x","coeff")),poly(b2,"x","coeff"));
assert_checkequal(clean(poly(a3,"x","coeff")),poly(b3,"x","coeff"));
//sparse
assert_checkequal(clean(sparse(a1)),sparse(b1));
assert_checkequal(clean(sparse(a2)),sparse(b2));
assert_checkequal(clean(sparse(a3)),sparse(b3));

// normal behavior, with epsA or/and epsR
big = 1
small = 1e-3;
epsA = 1e-4;
epsR = 1e-2;
a1 = [big small];
b1 = [big 0];
a2 = [big complex(small,small)];
b2 = [big complex(0,0)];
a3 = [complex(0,big) small complex(big,small)];
b3 = [complex(0,big) 0 big];
//double
assert_checkequal(clean(a1,epsA),a1);
assert_checkequal(clean(a2,epsA),a2);
assert_checkequal(clean(a3,epsA),a3);
assert_checkequal(clean(a1,epsA,epsR),b1);
assert_checktrue(clean(a2,epsA,epsR) == b2);
assert_checktrue(clean(a3,epsA,epsR) == b3);
//polynomial
assert_checkequal(clean(poly(a1,"x","coeff"),epsA),poly(a1,"x","coeff"));
assert_checkequal(clean(poly(a2,"x","coeff"),epsA),poly(a2,"x","coeff"));
assert_checkequal(clean(poly(a3,"x","coeff"),epsA),poly(a3,"x","coeff"));
assert_checkequal(clean(poly(a1,"x","coeff"),epsA,epsR),poly(b1,"x","coeff"));
assert_checkequal(clean(poly(a2,"x","coeff"),epsA,epsR),poly(b2,"x","coeff"));
assert_checkequal(clean(poly(a3,"x","coeff"),epsA,epsR),poly(b3,"x","coeff"));
//sparse
assert_checkequal(clean(sparse(a1),epsA),sparse(a1));
assert_checkequal(clean(sparse(a2),epsA),sparse(a2));
assert_checkequal(clean(sparse(a3),epsA),sparse(a3));
assert_checkequal(clean(sparse(a1),epsA,epsR),sparse(b1));
assert_checkequal(clean(sparse(a2),epsA,epsR), sparse(b2));
assert_checkequal(clean(sparse(a3),epsA,epsR),sparse(b3));


