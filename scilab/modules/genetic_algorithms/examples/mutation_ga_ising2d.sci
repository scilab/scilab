// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Mut_Indiv = mutation_ga_ising2d(Indiv,param)
if ~isdef('param','local') then
  param = [];
end
// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Proba = get_param(param,'proba',0.1);
ChangeMatrix = rand(size(Indiv,1),size(Indiv,2));
Index = find(ChangeMatrix<0.1);
Mut_Indiv = Indiv;
Mut_Indiv(Index) = Mut_Indiv(Index)*(-1);
endfunction
