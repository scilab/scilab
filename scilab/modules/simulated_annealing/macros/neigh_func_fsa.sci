
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x_neigh = neigh_func_fsa(x_current, T, param)
if ~isdef('param','local') then
  param = [];
end

if is_param(param,'sigma') then
  Sigma = get_param(param,'sigma');
else
  Sigma = ones(size(x_current,1),size(x_current,2));
end
if is_param(param,'min_bound') then
  Min = get_param(param,'min_bound');
else
  Min = -%inf*ones(size(x_current,1),size(x_current,2));
end

x_neigh = x_current + T*Sigma.*tan(%pi*(rand(size(x_current,1),size(x_current,2)) - 0.5));
x_neigh = max(min(x_neigh, Max),Min);
endfunction
