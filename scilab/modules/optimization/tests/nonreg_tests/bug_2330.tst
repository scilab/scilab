// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2330 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2330
//
// <-- Short Description -->
//    datafit does not work equivalently on WinXP and Linux. With a given dataset and
//    same routines centered on datafit function, it works perfectly on Linux and
//    partially (some data are fitted some others not) on WinXP.

//build the data to fit
//---------------------
function Xcalc=biexp(p,t)
  Xcalc=p(1).*exp(-p(2).*t)+p(3).*exp(-p(4).*t)+p(5);
endfunction;
t=(0:100:36000)'; 
p=[0.1;0.0001;0.2;0.0002;0.3];
X=biexp(p,t);

//try to fit the data
//-------------------
//the error function
function e=myerf(p,X,t),e=X-biexp(p,t),endfunction
// the initial point
p0=[0.01;0.001;0.01;0.001;0.1];
//call datafit
[pr,err]=datafit(list(myerf,t),X,p0);
if err>=5d-4 then pause,end
