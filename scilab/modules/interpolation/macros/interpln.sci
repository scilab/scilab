//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1991 - INRIA - Serge STEER
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
function [y]=interpln(xyd,x)
    //Given a set of points in the plane with coordinates xyd and a set of
    //x points
    // [y]=interpln(xyd,x) allows to compute a set of y obtained by linear interpolation.
    //% parameters
    //  xyd :  coordinates of points (2 rows matrix)
    //        first row : x
    //        second row : y
    //  x   : vector of x's for which y is wanted
    //  y   : vector of y computed.

    [md,nd]=size(xyd);
    if md<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Two rows expected.\n"),"interpln",1));
    end
    //

    n=size(x,"*");
    d=xyd(:,1:nd-1)-xyd(:,2:nd);
    k=find(abs(d(1,1:nd-1))==0)
    if k<>[] then
        d(1,k)=%eps*ones(k)
        xyd(1,k+ones(k))=xyd(1,k)+%eps*ones(k);
    end
    d=d(2,:)./d(1,:);
    for pt=1:n,
        xpt=x(pt);
        k=find(xpt>xyd(1,1:nd-1)&xpt<=xyd(1,2:nd));
        if k==[] then
            if xpt<=xyd(1,1) then k=1,else k=nd-1,end
        else
            k=k(size(k,"*"));
        end
        y(1,pt)=xyd(2,k)+(xpt-xyd(1,k))*d(k)
    end
endfunction
