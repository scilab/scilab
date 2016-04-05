// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13121 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=13121
//
// <-- Short Description -->
// ode "rk" option crashed Scilab when the user derivative function failed.

t0=0;
x0=0; y0=0;

// снаряд
c = 0.2;
s = 0.07;
m0 = 15;
tT=-1; mT=m0;
v0 = 50; teta0 = 0.6;
// ракета
c = 0.2;
s = 0.25;
m0 = 500;
tT = 1; mT = 250;
v0 = 50; teta0 = 0.6;
T0=10000;

r = 1.29; g = 9.8;

function y=m(t)
    if t <= tT then
        y = m0*(1-t/tT) + mT*t/tT;
    else
        y = mT;
    end
endfunction

function y=der_m(t)
    if t <= tT then
        y = 1 - (m0 - mT)/tT;
    else
        y = 0;
    end
endfunction

function y=T(t)
    if t <= tT then
        y = T0(1-t/tT);
    else
        y = 0;
    end
endfunction

function ydot=right(t,pl)
    x = pl;
    x(1) = max(x(1),0);
    x(2) = max(x(2),0);
    //if x(2) <= 0 & t > 0 then
    //    disp(t,"tf");
    //end;
    ydot(1) = x(3) * cos(x(4));
    ydot(2) = x(3) * sin(x(4));
    ydot(3) = 1/m(t)*(T(t)-0.5*c*r*s*x(3)*x(3)) - der_m(t)/m(t)*x(3) - g*sin(x(4));
    ydot(4) = -g/x(3)*cos(x(4));
    //    end
endfunction

t=0:0.01:40;

refMsg = [msprintf(_("Invalid index."));
msprintf(_("%s: An error occured in ''%s'' subroutine.\n"), "ode", "lsrgk")];
assert_checkerror("res=ode(""rk"",[x0;y0;v0;teta0;],t0,t,right)", refMsg);
