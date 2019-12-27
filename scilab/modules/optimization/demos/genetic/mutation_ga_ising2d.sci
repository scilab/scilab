// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Mut_Indiv = mutation_ga_ising2d(Indiv,param)
    if ~isdef("param","local") then
        param = [];
    end
    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    Proba = get_param(param,"proba",0.1);
    ChangeMatrix = rand(size(Indiv,1),size(Indiv,2));
    Index = find(ChangeMatrix<0.1);
    Mut_Indiv = Indiv;
    Mut_Indiv(Index) = Mut_Indiv(Index)*(-1);
endfunction
