//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 8349 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8349
//
// <-- Short Description -->
// write_csv() failed to support writting complexes with %nan or %inf parts

m = [7,-2,9;1,-8,2;1,-6,0];
m = m*(1+%i);
filename=TMPDIR+"/test.txt";
write_csv(m,filename);
b=mgetl(filename);
assert_checkequal(b, ["7+7i,-2-2i,9+9i";"1+1i,-8-8i,2+2i";"1+1i,-6-6i,0"]);
b=csvRead(filename);
assert_checkequal(b,m);

write_csv([ 1  %nan*(1+%i)],filename);
b=mgetl(filename);
assert_checkequal(b,"1,Nan+Nani");
b=csvRead(filename);
assert_checkequal(b, [1  %nan*(1+%i)]);

write_csv([ 1.1  %nan*(1+%i)],filename);
b=mgetl(filename);
assert_checkequal(b,"1.1000000000000001,Nan+Nani");
b=csvRead(filename);
assert_checkequal(b, [1.1  %nan*(1+%i)]);


write_csv([ 1  %inf*(1+%i)],filename)
b=mgetl(filename);
assert_checkequal(b,"1,Inf+Infi");
b=csvRead(filename);
assert_checkequal(b, [1  %inf*(1+%i)]);

write_csv([ 1.1  %inf*(1+%i)],filename)
b=mgetl(filename);
assert_checkequal(b,"1.1000000000000001,Inf+Infi");
b=csvRead(filename);
assert_checkequal(b, [1.1  %inf*(1+%i)]);

write_csv([ 1.1  1+%i*%inf],filename)
b=mgetl(filename);
assert_checkequal(b,"1.1000000000000001,Nan+Infi");
b=csvRead(filename);
assert_checkequal(b, [ 1.1  1+%i*%inf]);
