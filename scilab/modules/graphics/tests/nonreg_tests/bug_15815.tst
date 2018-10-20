// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15815 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15815
//
// <-- Short Description -->
// After polarplot(), reversing x|y axes shifted all labels

q1 = 0:0.01:%pi/2;
q2 = %pi/2:0.01:%pi;
q3 = %pi:0.01:3*%pi/2;
q4 = 3*%pi/2:0.01:2*%pi;
q12 = 0:0.01:%pi;
q34 = %pi:0.01:2*%pi;
q14 = -%pi/2:0.01:%pi/2;
q1234 = 0:0.01:2*%pi;

clf
gcf().axes_size = [1920 926];
Q = list(q1, q2, q3, q4, q12, q34, q14, q1234);
drawlater
for i = [0 8]
    for j = 1:8
        subplot(2,8,i+j), polarplot(Q(j), -1 + sin(Q(j)));
        if i>0
            gca().axes_reverse(2) = "on";
        end
    end
end
drawnow

// TOP ROW    : Before reversing axes
// BOTTOM ROW : After reversing axes
// => Check that labels ar enot shifted as they were
//    as shown there http://bugzilla.scilab.org/attachment.cgi?id=4819
