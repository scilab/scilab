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

function x_neigh = neigh_func_ising2d(x_current, T, param)
    if ~isdef("param","local")
        param = [];
    end

    Index_x = ceil(rand()*(size(x_current,1)-%eps)+%eps);
    Index_y = ceil(rand()*(size(x_current,1)-%eps)+%eps);

    x_neigh = x_current;
    x_neigh(Index_x,Index_y) = x_neigh(Index_x,Index_y)*(-1);

endfunction

