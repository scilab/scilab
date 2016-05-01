// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [d,pt,ind]=Dist2polyline(xp,yp,pt)
    // computes minimum distance from a point to a polyline
    //d    minimum distance to polyline
    //pt   coordinate of the polyline closest point
    //ind
    //     if negative polyline closest point is a polyline corner:pt=[xp(-ind) yp(-ind)]
    //     if positive pt lies on segment [ind ind+1]

    // Copyright INRIA
    x=pt(1)
    y=pt(2)
    xp=xp(:);yp=yp(:)
    cr=4*sign((xp(1:$-1)-x).*(xp(1:$-1)-xp(2:$))+..
    (yp(1:$-1)-y).*(yp(1:$-1)-yp(2:$)))+..
    sign((xp(2:$)-x).*(xp(2:$)-xp(1:$-1))+..
    (yp(2:$)-y).*(yp(2:$)-yp(1:$-1)))

    ki=find(cr==5) // index of segments for which projection fall inside
    np=size(xp,"*")
    if ki<>[] then
        //projection on segments
        x=[xp(ki) xp(ki+1)]
        y=[yp(ki) yp(ki+1)]
        dx=x(:,2)-x(:,1)
        dy=y(:,2)-y(:,1)
        d_d=dx.^2+dy.^2
        d_x=( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d
        d_y=(-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./d_d
        xp=[xp;d_x]
        yp=[yp;d_y]
    end
    [d,k]=min(((xp-pt(1))).^2+((yp-pt(2))).^2) //distance with all points
    d=sqrt(d)
    pt(1)=xp(k)
    pt(2)=yp(k)
    if k>np then ind=ki(k-np),else ind=-k,end
endfunction
