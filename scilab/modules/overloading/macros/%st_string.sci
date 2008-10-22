// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=%st_string(s)
f=getfield(1,s)
ll=lines()
t=[]
for k=3:size(f,'*')
  var=f(k)
  value=s(var)
  tp=typeof(value)
  if tp=="ce" then
    t=[t;%ce_string(value)]
  elseif tp=="st" then
    t=[t;%st_string(value)]
  else
    str=sci2exp(value,ll(1))
    txt='   '+var+': '
    if size(str,'*')==1 then
      txt=txt+str
    else
      tp=typeof(value)
      txt=txt+'{'+strcat(string(size(value)),'x')+' '+tp+'}'
    end
    t=[t;txt]
  end
end
endfunction
