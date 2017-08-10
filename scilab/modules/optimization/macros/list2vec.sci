// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
function [bigVector,varsizes]=list2vec(li)
    //li=list(X1,...Xk) is a list of matrices
    //bigVector: big vector [X1(:);...;Xk(:)] (stacking of matrices in li)
    //varsizes: k x 2 matrix, with varsiz(i,:)=size(Xi)
    bigVector=[];varsizes=[];
    li=aplat(li)
    for mati=li
        sm=size(mati);
        varsizes=[varsizes;sm];
        bigVector=[bigVector;matrix(mati,-1,1)];

    end
endfunction
