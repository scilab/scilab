// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
// Copyright (C) 2013 - Stanislav KROTER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 11333 -->
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Short Description -->
// paramfplot2d was failing with theta

deff('y=f(x,t)','y=t*sin(x)');
x=linspace(0,2*%pi,50);
theta=0:0.05:1;
clf;paramfplot2d(f,x,theta.');




