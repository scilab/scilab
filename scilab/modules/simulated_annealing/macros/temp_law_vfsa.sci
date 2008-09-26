// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function T = temp_law_vfsa(T, step_mean, step_var, temp_stage, n, param)
if (~isdef('param','local')) then
  param = []; // First create the empty param var
end

[c,err]         = get_param(param,'c',0.01);
[dimension,err] = get_param(param,'dimension',3);

T = T * (n/(n+1))^(1/dimension);
endfunction
