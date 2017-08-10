
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Q]=orth(A)
    // Orthogonal basis for the span of A.
    // Range(Q) = Range(A) and Q'*Q=eye
    //!
    [X,n]=rowcomp(A);X=X';
    Q=X(:,1:n);
endfunction

