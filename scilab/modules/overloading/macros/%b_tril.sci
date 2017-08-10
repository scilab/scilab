// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Bruno Pincon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [d]=%b_tril(a,k)
    // Copyright INRIA (modified by bruno, June 10 2004)
    [lhs,rhs] = argn()
    if rhs==1 then k=0,end
    [m,n] = size(a)
    d(m,n) = %f  // create an m x n boolean mat with %f elts
    i = find(tril(ones(a),k))
    d(i) = a(i)
endfunction
