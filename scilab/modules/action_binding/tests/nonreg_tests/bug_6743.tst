// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6743 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6743
//
// <-- Short Description -->
// ismenu function is slow

function r=inside(x,y); if((x*x+y*y)<1.) r=1.; else r=0.; end; endfunction

function p=monte_carlo_pi(n)
x=rand(1,n);
y=rand(1,n);
 p=0.;
  for i=1:n
      p=p+inside(x(i),y(i));
    end
p=p/n*4.;
endfunction

tic();monte_carlo_pi(1000000);t = toc();
disp(t)
if t > 20 then pause,end

// on a Core 2 Quad 2.4 Ghz, it must be something as 9 s
