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

function r=%s_y_p(a,b)
    // a./.b

    [ma,na]=size(a)
    [mb,nb]=size(b)
    r=zeros(ma*mb,na*nb)
    k=0
    for j=1:na
        l=0
        for i=1:ma
            r(l+(1:mb),k+(1:nb))=a(i,j)./b
            l=l+mb
        end
        k=k+nb
    end
endfunction
