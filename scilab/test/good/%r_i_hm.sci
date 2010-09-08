// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function N=%r_i_hm(varargin)
  //author Serge Steer INRIA
  M=varargin($);
  N=varargin($-1);//inserted matrix
  //  M(i,...)=N,  M hypermatrix; N rational matrix
  Num=M;
  Den=ones(M);
  Num(varargin(1:$-2))=N.num
  Den(varargin(1:$-2))=N.den
 
  N.num=Num;
  N.den=Den;
endfunction
