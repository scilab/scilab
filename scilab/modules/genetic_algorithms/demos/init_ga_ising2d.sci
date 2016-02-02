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

function Pop_init = init_ga_ising2d(popsize,param)
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    Dim = get_param(param,"dimension",10);

    // Pop_init must be a list()
    Pop_init = list();
    for i=1:popsize
        Pop_init(i) = init_ising2d(Dim,rand());
    end
endfunction
