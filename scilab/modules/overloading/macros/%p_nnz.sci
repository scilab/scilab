// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,

function n = %p_nnz(p)
    n = size(p,"*") - nnz(isinf(degree(p)))
endfunction
