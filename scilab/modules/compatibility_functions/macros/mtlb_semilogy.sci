// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function mtlb_semilogy(varargin)
    global mtlb_log_mod
    mtlb_log_mod="nl"
    mtlb_plot(varargin(:))
    mtlb_log_mod=[]
endfunction
