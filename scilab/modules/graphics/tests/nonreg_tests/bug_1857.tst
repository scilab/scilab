// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NOT FIXED -->

// <-- Non-regression test for bug 1857 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1857
//
// <-- Short Description -->
// Problems with mtlb_semilogx, mtlb_semilogy and mtlb_loglog.
// They does not seems to set log_flags accordingly

// mtlb_semilogx
x=[0.001 0.01 0.1 1.0 10. 100];y=[20 30 40 50 60 70];
mtlb_semilogx(x,y)

// check that current axes has log flags "ln";
axes = gca();
if (axes.log_flags <> "lnn") then pause; end

// mtlb_semilogy
x=[1:0.1:10];y=2*x^2;
mtlb_semilogy(x,y)

// check that current axes has log flags "nl";
axes = gca();
if (axes.log_flags <> "nln") then pause; end

// mtlb_loglog
x=[1:0.1:10];y=2*x^2;
mtlb_loglog(x,y)

// check that current axes has log flags "ll";
axes = gca();
if (axes.log_flags <> "lln") then pause; end




