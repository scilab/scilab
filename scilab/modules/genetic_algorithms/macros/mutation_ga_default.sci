// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Mut_Indiv = mutation_ga_default(Indiv,param)
if ~isdef('param','local') then
  param = [];
end
// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
[Delta,err]     = get_param(param,'delta',0.1);
[MinBounds,err] = get_param(param,'minbound',-2*ones(size(Indiv,1),size(Indiv,2)));
[MaxBounds,err] = get_param(param,'maxbound', 2*ones(size(Indiv,1),size(Indiv,2)));

Mut_Indiv = Indiv + 2*Delta*rand(size(Indiv,1),size(Indiv,2)) - Delta*ones(size(Indiv,1),size(Indiv,2));

Mut_Indiv = max(min(Mut_Indiv, MaxBounds),MinBounds);
endfunction
