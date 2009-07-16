// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x_neigh = neigh_func_csa(x_current, T, param)
if ~isdef('param','local') then
  param = [];
end

[Sigma,err] = get_param(param,'sigma',ones(size(x_current,1), size(x_current,2)));
[Min,err]   = get_param(param,'min_bound',-%inf*ones(size(x_current,1),size(x_current,2)));
[Max,err]   = get_param(param,'max_bound',%inf*ones(size(x_current,1),size(x_current,2)));

x_neigh = x_current + Sigma.*sqrt(2)*T*rand(1,1,'norm');
x_neigh = max(min(x_neigh, Max),Min);
endfunction
