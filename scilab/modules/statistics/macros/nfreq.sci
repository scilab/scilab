// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [m]=nfreq(x)
//
//Frequence of the values in a real or complex vector or a  real
//or complex matrix x.
//
//For a real or complex vector or a  real or complex matrix x,
//m=freq(x) returns in the first column of the size(x,'*')x2 matrix
//m the values of x and in the second column of this matrix the
//frequences of the corresponding values.
//
//
  lxnozero=size(x,'*');
  xx=matrix(x,lxnozero,1);
  m=[]
  while lxnozero>0 do
    xnozero=find(xx-xx(1)<>0)',
    l=lxnozero,
    lxnozero=size(xnozero,'*'),
    m=[m ; xx(1) l-lxnozero],
    xx=xx(xnozero),
  end
endfunction
