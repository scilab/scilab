
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function T = temp_law_default(T, step_mean, step_var, temp_stage, n,param)
if (~isdef('param','local')) then
  param = []; // First create the empty var param
end
if is_param(param,'beta') then
  Beta = get_param(param,'beta');
else
  Beta = 0;
end

if (is_param(param,'alpha')) then
  _alpha = get_param(param,'alpha');
else
  _alpha = 0.9;
end
T = _alpha*T;
endfunction
