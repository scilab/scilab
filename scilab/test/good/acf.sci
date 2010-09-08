// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ac,Mean]=acf(x,n,minim,maxim)
//function acp(x,n,[minim,maxim])
// Autocorrelation for one-deimensional process

[lhs,rhs]=argn(0)
if rhs <= 1 ; n=prod(size(x))/4;end
if rhs <= 2 ; minim=-1.0;end
if rhs <= 3 ; maxim= 1.0;end
[cov,Mean]=corr(x,n+1);
ac=cov'/cov(1);
plot2d3("onn",(0:n)',ac,[1],"011"," ",[0,minim,n,maxim]);
//stde=sqrt((1+2*ac(2:n+1)'*ac(2:n+1))*1/prod(size(x)))
stde=2*sqrt(1/prod(size(x)));
plot2d( [0,0,0;n,n,n],[0,stde,-stde;0,stde,-stde],[1,2,2],"000")
xtitle(gettext("Autocorrelation Function"));
endfunction
