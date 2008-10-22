// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i=modulo(n,m)
//i=modulo(n,m) returns  n modulo m.

  if size(n,'*')==1 then
    i=zeros(m);
    k=find(m==0);i(k)=n
    k=find(m~=0);i(k)=n-int(n./m(k)).*m(k)
  elseif size(m,'*')==1 then
    i=zeros(n);
    if m==0 then
      i=n
    else
      i=n-int(n./m).*m
    end
  else
    if or(size(n)<>size(m)) then 
      error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"modulo"));
    end
    i=zeros(n);
    k=find(m==0);i(k)=n(k)
    k=find(m~=0);i(k)=n(k)-int(n(k)./m(k)).*m(k)
  end
endfunction
