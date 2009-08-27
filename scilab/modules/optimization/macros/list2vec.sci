// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [bigVector,varsizes]=list2vec(li)
//li=list(X1,...Xk) is a list of matrices
//bigVector: big vector [X1(:);...;Xk(:)] (stacking of matrices in li)
//varsizes: k x 2 matrix, with varsiz(i,:)=size(Xi)
  bigVector=[];varsizes=[];
  li=aplat(li)
  for mati=li
    sm=size(mati);
    varsizes=[varsizes;sm];
    bigVector=[bigVector;matrix(mati,-1,1)];

  end
endfunction
