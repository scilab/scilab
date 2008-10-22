// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ind=vectorfind(m,v,job)

  if min(size(v))<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"),"vectorfind",2));
  end

  if convstr(part(job,1))=='r' then
    if size(v,'*')<>size(m,2) then
      error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
    end
    ind=1:size(m,1)
    for k=1:size(m,2)
      ind=ind(find(m(ind,k)==v(k)));
      if ind==[] then break,end
    end
  elseif convstr(part(job,1))=='c' then
    if size(v,'*')<>size(m,1) then
      error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
    end
    ind=1:size(m,2)
    for k=1:size(m,1)
      ind=ind(find(m(k,ind)==v(k)))
      if ind==[] then break,end
    end
  else
    error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"vectorfind",3,"r[ow]","c[olumn]"));
  end
endfunction
