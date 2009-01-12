// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// Basic use
computed=linspace(0,1,11);
expected=[0. 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.];
if norm(computed-expected)>10*%eps then pause, end
// With 2 arguments only : from 0. to 0.99
computed=linspace(0,1.-1/100.);
expected=zeros(1,100);
for k=1:100
  expected(1,k)=(k-1)*0.01;
end
if norm(computed-expected)>10*%eps then pause, end
// Corner case with a real value
a = (1-0.9)*50;
computed=linspace(0,1,a);
expected=[0. 0.25 0.50 0.75 1.];
if norm(computed-expected)>10*%eps then pause, end
// Basic use with 12 and 14 which also test scaling
computed=linspace(12,14,11);
expected=[12. 12.2 12.4 12.6 12.8 13. 13.2 13.4 13.6 13.8 14.];
if norm(computed-expected)>10*%eps then pause, end

