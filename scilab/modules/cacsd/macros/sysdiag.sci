// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=sysdiag(varargin)
//Returns the block-diagonal system made with subsystems put in the main
//diagonal
// Syntax:
// r=sysdiag(a1,a2,...,an)
//
// ai    : subsystems (i.e. gains, or linear systems in state-space or
//                     transfer form)
//Remark:
//  At most 17 arguments...
//Example
// s=poly(0,'s')
// sysdiag(rand(2,2),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])
// sysdiag(tf2ss(1/s),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])


//!
r=varargin(1);
[m1,n1]=size(r);
for k=2:size(varargin)
  ak=varargin(k)
  [mk,nk]=size(ak);
  r=[r,0*ones(m1,nk);0*ones(mk,n1),ak]
  m1=m1+mk
  n1=n1+nk
end
endfunction
