// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x_neigh = neigh_func_vfsa(x_current, T, param)
if ~isdef('param','local') then
  param = [];
end

[Delta,err] = get_param(param,'delta',0.1*ones(size(x_current,1), size(x_current,2)));
[Min,err]   = get_param(param,'min_bound',-%inf*ones(size(x_current,1),size(x_current,2)));
[Max,err]   = get_param(param,'max_bound',%inf*ones(size(x_current,1),size(x_current,2)));

y = zeros(size(x_current,1), size(x_current,2));
for i=1:length(y)
  a = rand(1,1);
  y(i) = sign(a-0.5)*T*((1+1/T)^abs(2*a-1)-1);
  x_neigh(i) = x_current(i) + y(i)*Delta(i);
end
x_neigh = max(min(x_neigh, Max),Min);
endfunction
