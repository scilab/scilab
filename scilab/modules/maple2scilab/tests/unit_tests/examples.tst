// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->


// This test requires maple
// to create a dynamic library with maple
// maple exemples.mpl examples.out


// example 1
x=1

link('foof1'+getdynlibext(),'foof1');
exec('foof1.sci');
out=foof1(x)

link('fooc1'+getdynlibext(),'fooc1','c');
exec('fooc1.sci');
out=fooc1(x)

exec('foos1.sci');
out=foos1(x)

// example 2

link('foof2'+getdynlibext(),'foof2');
exec('foof2.sci');
out=foof2()

link('fooc2'+getdynlibext(),'fooc2','c');
exec('fooc2.sci');
out=fooc2()

exec('foos2.sci');
out=foos2()

// example 3
x=2

link('fooc3'+getdynlibext(),'fooc3','c');
exec('fooc3.sci');
out=fooc3(x)

link('foof3'+getdynlibext(),'foof3');
exec('foof3.sci');
out=foof3(x)

exec('foos3.sci');
out=foos3(x)

// example 4
x=1
y=2

link('foof4'+getdynlibext(),'foof4');
exec('foof4.sci');
out=foof4(x,y)

link('fooc4'+getdynlibext(),'fooc4','c');
exec('fooc4.sci');
out=fooc4(x,y)

exec('foos4.sci');
out=foos4(x,y)

// example 5
k=2
x=[1,2,3]

link('foof5'+getdynlibext(),'foof5');
exec('foof5.sci');
out=foof5(k,x)

link('fooc5'+getdynlibext(),'fooc5','c');
exec('fooc5.sci');
out=fooc5(k,x)

exec('foos5.sci');
out=foos5(k,x)

// example 6
k=2
x=[1,2,3;4,5,6]

link('foof6'+getdynlibext(),'foof6');
exec('foof6.sci');
out=foof6(k,x)

link('fooc6'+getdynlibext(),'fooc6','c');
exec('fooc6.sci');
out=fooc6(k,x)

exec('foos6.sci');
out=foos6(k,x)

// example 7
k=2
x=[1,2,3;4,5,6]

link('foof7'+getdynlibext(),'foof7');
exec('foof7.sci');
out=foof7(k,x)

link('fooc7'+getdynlibext(),'fooc7','c');
exec('fooc7.sci');
out=fooc7(k,x)

exec('foos7.sci');
out=foos7(k,x)

// example 8

link('foof8'+getdynlibext(),'foof8');
exec('foof8.sci');
out=foof8()

link('fooc8'+getdynlibext(),'fooc8','c');
exec('fooc8.sci');
out=fooc8()

exec('foos8.sci');
out=foos8()

// example 9

exec('foos9.sci');
out=foos9(1,1,1)
