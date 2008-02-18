//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//


// f(x,y)=y-x^2-cos(omega*x)/3

function res=fx(x,y)
res=-2*x+omega*sin(omega*x)/3;

function res=fy(x,y)
res=1;
