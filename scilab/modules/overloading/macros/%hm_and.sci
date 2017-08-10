// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// STILL CALLED in 6.0.0-b2

function x = %hm_and(m,d)
    if argn(2)==1 | d=="*" then
        x=and(m(:));
        return
    end

    dims = size(m);
    if d=="m" then
        d=find(dims>1,1)
        if d==[] then
            x=and(m(:));
            return
        end
    end

    N=size(dims,"*");
    p1=prod(dims(1:d-1));// step to build one vector on which and is applied
    p2=p1*dims(d);//step for beginning of next vectors
    ind=(0:p1:p2-1)';// selection for building one vector
    deb=(1:p1);
    I=ind*ones(deb)+ones(ind)*deb

    ind=(0:p2:prod(dims)-1);
    I=ones(ind).*.I+ind.*.ones(I)

    x=and(matrix(m(I),dims(d),-1),1);

    dims(d)=1
    while  dims($)==1 then
        dims($)=[];
    end
    if d==N then
        dims=dims(1:$);
    else
        dims(d)=1;
    end
    if dims<>[]
        x=matrix(x, dims);
    end
endfunction
