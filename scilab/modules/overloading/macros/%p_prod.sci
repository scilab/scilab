// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%p_prod(a,flag)

r=[]
[m,n]=size(a);
[m,n]=size(a);
if flag=='m'|flag==-1 then
  flag=find([m,n]>1,1)
  if flag==[] then a=prod(a),return,end
end

if flag==1|flag=='r' then
  for k=1:n,r(1,k)=prod(a(:,k)),end
end
if flag==2|flag=='c' then
  for k=1:m,r(k,1)=prod(a(k,:)),end
end
endfunction
