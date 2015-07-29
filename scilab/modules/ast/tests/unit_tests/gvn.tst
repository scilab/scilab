// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//

out = testGVN("a=1;b=1");
assert_checkequal(out.a, out.b);

out = testGVN("a=-1-5+4-3;b=7-12");
assert_checkequal(out.a, out.b);

out = testGVN("a=n+1;b=1+n");
assert_checkequal(out.a, out.b);

out = testGVN("a=n*3;b=3*n");
assert_checkequal(out.a, out.b);

out = testGVN("a=2*n+1;b=1+n;c=b+n");
assert_checkequal(out.a, out.c);

out = testGVN("a=x+2;b=x+x*x+5+2*x-1+x;c=a*a;d=a^2;e=(x+2)*(2+x)");
assert_checkequal(out.b, out.c);
assert_checkequal(out.b, out.d);
assert_checkequal(out.b, out.e);

out = testGVN("a=b+c;d=a^3;e=(b+c)*(b+c)^2;f=b^3+3*b^2*c+3*b*c^2+c^3");
assert_checkequal(out.d, out.e);
assert_checkequal(out.d, out.f);

out = testGVN("a=b+c;d=a*(e+f);g=d^2;h=b^2*e^2+b^2*f^2+c^2*e^2+c^2*f^2+2*b^2*e*f+2*b*c*e^2+4*b*e*c*f+2*b*c*f^2+2*c^2*e*f");
assert_checkequal(out.g, out.h);

out = testGVN("a=x-3;b=a^3;c=-18/2*x^2+(32-5)*x-(168/6-1)+x^2*x");
assert_checkequal(out.b, out.c);

out = testGVN("a=n+1;b=a+n;c=2*b;d=c+b;e=d+a-1;A=n+1;B=2*n+1;C=4*n+2;D=6*n+3;E=7*n+3");
assert_checkequal(out.a, out.A);
assert_checkequal(out.b, out.B);
assert_checkequal(out.c, out.C);
assert_checkequal(out.d, out.D);
assert_checkequal(out.e, out.E);

out = testGVN("a=x^10+y*x+1;b=a(x=d+1,y=d+e);c=(d+1)^10+(d+e)*(d+1)+1");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^5+x^3+x^2+y*x+1;b=a(x=d+1,y=d+e);c=(d+1)^5+(d+1)^3+(d+1)^2+(d+e)*(d+1)+1");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^10+y*x+1;b=a(x=2,y=5);c=1035");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^10+y*x+1;b=a(x=y);c=y^10+y^2+1");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^10+y*x+1;b=a(x=2);c=1025+2*y");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^10+y*x+1;b=a(y=3);c=x^10+3*x+1");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^5+x^3+x^2+y*x+1;b=a(x=2);c=45+2*y");
assert_checkequal(out.b, out.c);

out = testGVN("a=x^5+x^3+x^2+y*x+1;b=a(y=x);c=x^5+x^3+2*x^2+1");
assert_checkequal(out.b, out.c);