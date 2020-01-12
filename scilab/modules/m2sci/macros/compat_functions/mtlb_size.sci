// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
