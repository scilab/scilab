// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x_neigh = neigh_func_ising2d(x_current, T, param)
    if ~isdef("param","local")
        param = [];
    end

    Index_x = ceil(rand()*(size(x_current,1)-%eps)+%eps);
    Index_y = ceil(rand()*(size(x_current,1)-%eps)+%eps);

    x_neigh = x_current;
    x_neigh(Index_x,Index_y) = x_neigh(Index_x,Index_y)*(-1);

endfunction

