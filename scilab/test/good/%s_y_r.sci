// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%s_y_r(a,b)
// a./.b

[ma,na]=size(a)
[mb,nb]=size(b)
r=zeros(ma*mb,na*nb)
k=0
for j=1:na
  l=0
  for i=1:ma
    r(l+(1:mb),k+(1:nb))=a(i,j)./b
    l=l+mb
  end
  k=k+nb
end
endfunction
