// This file is part Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [c,lagindex]=xcov(x,varargin)
    nv=size(varargin)
    if nv>0&type(varargin(nv))==10 then
        validemodes=["biased","unbiased","coeff","none"]
        scalemode=varargin(nv)
        if and(scalemode<>validemodes) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),...
            "xcov",nv+1,strcat(""""+validemodes+"""",",")))
        end
        nv=nv-1;
    else
        scalemode="none"
    end
    //test de validité de x
    szx=size(x)
    if type(x)<>1|and(szx>1) then
        error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
        "xcov",1))
    end
    x=x-mean(x)
    autocorr=%t
    maxlags=[]
    if nv==1 then
        if size(varargin(1),"*")==1 then //xcov(x,maxlags)
            autocorr=%t
            maxlags=int(varargin(1))
            if type( maxlags)<>1|size(maxlags,"*")>1|~isreal(maxlags)|maxlags<>int(maxlags) then
                error(msprintf(_("%s: Wrong type for argument #%d: an integer expected.\n"),...
                "xcov",2))
            end
            if maxlags<1 then
                error(msprintf(_("%s: Wrong value for argument #%d: the expected value must be greater than %d.\n"),...
                "xcov",2,1))
            end
        else //xcov(x,y)
            autocorr=%f
            y=varargin(1)
            if type(y)<>1|and(size(y)>1) then
                error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
                "xcov",2))
            end
            varargin(1)=y-mean(y)
            maxlags=[]
        end
    elseif nv==2 then //xcov(x,y,maxlag)
        autocorr=%f
        y=varargin(1)
        if type(y)<>1|and(size(y)>1) then
            error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
            "xcov",2))
        end
        if type(y)<>1|and(size(y)>1) then
            error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
            "xcov",2))
        end
        varargin(1)=y-mean(y)
        maxlags=int(varargin(2))
        if type( maxlags)<>1|size(maxlags,"*")>1|~isreal(maxlags)|maxlags<>int(maxlags) then
            error(msprintf(_("%s: Wrong type for argument #%d: an integer expected.\n"),...
            "xcov",2))
        end
        if maxlags<1 then
            error(msprintf(_("%s: Wrong value for argument #%d: the expected value must be greater than %d.\n"),...
            "xcov",2,1))
        end
    end
    [c,lagindex]=xcorr(x,varargin(:))

endfunction
