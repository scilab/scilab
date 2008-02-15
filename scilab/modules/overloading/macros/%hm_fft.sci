// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=%hm_fft(x,flag)
rhs=argn(2)
if rhs == 1 then
  flag = -1
end
dims=matrix(x.dims,-1,1)
v=matrix(x.entries,-1,1);
n=size(dims,'*')
incr=1
for k=1:n
  dk=double(dims(k));
  v=fft(v ,flag,dk,incr)
  incr=incr*dk
end
x.entries=v
endfunction
