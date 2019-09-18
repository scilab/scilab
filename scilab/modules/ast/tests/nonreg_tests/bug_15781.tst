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
