// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15781 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15781
//
// <-- Short Description -->
// display of complex matrix is ugly.

n=4

a=testmatrix("magic",4);

for f = ["v","e"]
    format(f)
    x=complex(cos(a),sin(a));
    x(3,3)=%nan;
    x(4,4)=%inf;
    x(2,1:2)=1+%i;
    x(:,1)=0;
    disp(x)

    for i=1:n
        disp(x(i,:))
        disp(" ")
    end

    for i=1:n
        disp(x(:,i))
        disp(" ")
    end

    x=x-x;

    disp(x)
    disp(" ")

    acoth(-2:0.5:2).'
end

format("v")

a= [0,0,%i*0.6795197,0.7161007+%i*0.3997826,0
0,%i*0.977002,0,0,0
0,0.2629713,0,0,0.4505416
0,%i*0.8173032,0.529842,0,%i*0.8558751
0,0,0,0.7481516,%i*0.2598704
0,0.3402197+%i*0.3631865,0,0,0
0,%i*0.8686947,%i*0.0844358,0.9883794,0
0,0,0,0,0
0,0.6540791,0.6892145+%i*0.3462334,0,0
0,0,0,0,0.7204935]