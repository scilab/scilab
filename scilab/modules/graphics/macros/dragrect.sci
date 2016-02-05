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

function [rects,btn]=dragrect(varargin)

    // Check number of input argument
    if size(varargin)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "dragrect", 1));
    end
    rects=varargin(1);

    if size(rects,1)==1 then rects=rects(:),end
    n=size(rects,2)
    f=gcf();
    a=gca();
    db = a.data_bounds;
    xrects(rects)
    a.data_bounds = db;
    R=gce(); //Compound of rectangles
    rep=[rects(1),rects(2),-1]
    while rep(3)==-1 then
        repn=xgetmouse()
        if repn(3)==-100  then //window has been closed
            btn=repn(3)
            return
        end
        rects(1:2,:)=rects(1:2,:)+(repn(1:2)-rep(1:2))'*ones(1,n);
        move(R,repn(1:2)-rep(1:2))
        rep=repn
    end
    delete(R)
    btn=rep(3)
endfunction
