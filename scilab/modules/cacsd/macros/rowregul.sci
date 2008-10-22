// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Stmp,Ws]=rowregul(Sl,Alfa,Beta);
//[Stmp,Ws]=regul(Sl) computes a polynomial-state-space postfilter 
//Ws such that Stmp=Ws*Sl is proper and has D full rank
//Poles at infinity of Sl are moved to -Alfa;
//Zeros at infinity of Sl are moved to -Beta;
//Sl is asummed right invertible i.e. ss2tf(Sl) full row rank
//!

[Stmp,Ws]=colregul(Sl',Alfa,Beta);
Stmp=Stmp';Ws=Ws';
endfunction
