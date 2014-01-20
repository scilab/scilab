// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1751 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/1751
//
// <-- Short Description -->
// Margins were not computed according to contents

clf();
plot([1:2]',[3:4;5:6;7:8;9:10]');
// margins should be resized to let axis labels fit them
xtitle("$\scalebox{12}{Title}$","$\scalebox{12}{1}$","$\scalebox{12}{2}$")

// Now we make six subplots with titles, labels and legends
// It must not have object overlapping another one
clf();
M=3;
N=2;
for i=1:M
    for j=1:N
        k=i+(j-1)*M;
        subplot(M,N,k);
        plot([1:2]',[3:4;5:6;7:8;9:10]');
        xtitle("title " + string(k),"x-axis " + string(k),"y-axis " + string(k));
        legend(["AAAA", "BBBB", "CCCC", "DDDD"],"out_upper_right");
    end
end
