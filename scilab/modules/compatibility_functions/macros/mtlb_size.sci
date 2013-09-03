// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function varargout=mtlb_size(X,dim)
    // Emulation function for Matlab size()

    varargout=list()

    [lhs,rhs]=argn()

    // d=size(X,dim)
    if rhs==2 then
        if dim<=size(size(X),2) then
            varargout(1)=size(X,dim)
        else
            varargout(1)=1
        end
        // [d1,...,dn]=size(X)
    else
        // if n < size(size(X),2)
        if lhs<size(size(X),2) then
            // d1,...,dn-1
            for k=1:lhs
                varargout(k)=size(X,k)
            end
            // dn is equal to the product of remaining dimensions
            last=1
            for k=lhs+1:size(size(X),2)
                last=last*size(X,k)
            end
            varargout(lhs)=last
        else

            for k=1:size(size(X),2)
                varargout(k)=size(X,k)
            end

            // if n > size(size(X),2)
            if lhs>size(size(X),2)
                for k=size(size(X),2)+1:lhs
                    varargout(k)=1
                end
            end
        end
    end
endfunction
