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


function [nArgOut,vectInput]=check2dFun(funName,func,X,current_figure,cur_draw_mode)

    // In this function we determine whether the function 'func'
    // has the syntax [y]=func(x) or [x,y]=func(t).
    // We also test if the function accepts vector input
    // arguments.
    //
    // This allow to understand in a 2d plot function (plot)
    // if the user has requested a 2d plot of a parametric or
    // non-parametric curve.

    // First We get the two strings in and out containing the
    // names of input and output arguments :

    [out,in,text]=string(func);

    nArgOut=max(size(out));
    nArgIn=max(size(in))

    if nArgIn~=1
        warning(sprintf("%s : function must accept two input arguments",funName));
        ResetFigureDDM(current_figure, cur_draw_mode)
        return;
    end

    if nArgOut~=1 & nArgOut~=2
        warning(sprintf("%s : function must have 1 or 2 output arguments",funName));
        ResetFigureDDM(current_figure, cur_draw_mode)
        return;
    end

    // Now we test if func accepts vector inputs (we test with the X,Y
    // pair provided by the user)

    if nArgOut==1;

        ierr=execstr("yf=func(X)","errcatch");
        if ierr==0
            vectInput=(and(size(yf)==size(X)));
        else
            vectInput=%F;
        end

    elseif nArgOut==2

        // now we test if func accepts vector inputs (as above)

        xf=[];yf=[];
        ierr=execstr("[xf,yf]=func(X)","errcatch");

        vectInput=(ierr==0 & and([size(xf)==size(yf) ...
        size(yf)==size(X)]));
    end

endfunction
