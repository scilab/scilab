//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

funcprot(0);

// f(x,y)=y-x^2

function res=fx(x,y)
    res=-2*x;
endfunction

function res=fy(x,y)
    res=1;
endfunction

funcprot(1);
