// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) -2011 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout=%hm_find(Mb,nmax)
    lhs=argn(1)
    if argn(2)==2 then
        I=find(Mb.entries,nmax)
    else
        I=find(Mb.entries)
    end
    if lhs==1 then
        varargout=list(I)
    else
        dims=size(Mb);
        nd=size(dims,"*")
        if nd>lhs then
            dims=[dims(1:lhs-1) prod(dims(lhs:$))]
        else
            dims(nd+1:lhs)=1;
        end
        varargout=list()
        I=ind2sub(dims,I)
        for k=1:size(I,2)
            varargout(k)=I(:,k)'
        end
    end
endfunction
