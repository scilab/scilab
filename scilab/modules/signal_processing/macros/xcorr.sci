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
function [c,lagindex]=xcorr(x,varargin)
    nv=size(varargin)
    if nv>0&type(varargin(nv))==10 then
        validemodes=["biased","unbiased","coeff","none"]
        scalemode=varargin(nv)
        if and(scalemode<>validemodes) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),...
            "xcorr",nv+1,strcat(""""+validemodes+"""",",")))
        end
        nv=nv-1;
    else
        scalemode="none"
    end
    //test de validité de x
    szx=size(x)
    if type(x)<>1|and(szx>1) then
        error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
        "xcorr",1))
    end

    autocorr=%t
    maxlags=[]
    if nv==1 then
        if size(varargin(1),"*")==1 then //xcorr(x,maxlags)
            autocorr=%t
            maxlags=int(varargin(1))
            if type( maxlags)<>1|size(maxlags,"*")>1|~isreal(maxlags)|maxlags<>int(maxlags) then
                error(msprintf(_("%s: Wrong type for argument #%d: an integer expected.\n"),...
                "xcorr",2))
            end
            if maxlags<1 then
                error(msprintf(_("%s: Wrong value for argument #%d: the expected value must be greater than %d.\n"),...
                "xcorr",2,1))
            end
        else //xcorr(x,y)
            autocorr=%f
            y=varargin(1)
            if type(y)<>1|and(size(y)>1) then
                error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
                "xcorr",2))
            end
            maxlags=[]
        end
    elseif nv==2 then //xcorr(x,y,maxlag)
        autocorr=%f
        y=varargin(1)
        if type(y)<>1|and(size(y)>1) then
            error(msprintf(_("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"),...
            "xcorr",2))
        end
        maxlags=int(varargin(2))
        if type( maxlags)<>1|size(maxlags,"*")>1|~isreal(maxlags)|maxlags<>int(maxlags) then
            error(msprintf(_("%s: Wrong type for argument #%d: an integer expected.\n"),...
            "xcorr",2))
        end
        if maxlags<1 then
            error(msprintf(_("%s: Wrong value for argument #%d: the expected value must be greater than %d.\n"),...
            "xcorr",2,1))
        end
    end

    if autocorr then //auto correlation
        x=matrix(x,-1,1);n=size(x,"*")
        if maxlags==[] then maxlags=n-1,end
        npad=2^nextpow2(2*n-1)
        x(npad)=0;
        t=fft(x);
        c=ifft(real(t.*conj(t)))
        if isreal(x) then c=real(c);end
    else //cross correlation
        x=matrix(x,-1,1);nx=size(x,1)
        xx=sum(abs(x).^2)
        y=matrix(y,-1,1);ny=size(y,1)
        yy=sum(abs(y).^2)
        if nx<ny then
            x(ny)=0;
        elseif ny<nx then
            y(nx)=0;
        end
        n=max(nx,ny)
        if maxlags==[] then maxlags=n-1,end
        npad=2^nextpow2(2*n-1)
        x(npad)=0;
        y(npad)=0;
        c=ifft(fft(x).*conj(fft(y)))
        if isreal(x)&isreal(y) then c=real(c),end
    end
    //extract requested lags
    padding=zeros(maxlags-n+1,1)
    if maxlags<n then
        c=[c($-maxlags+1:$);c(1:maxlags+1);]
    else
        padding=zeros(maxlags-n+1,1)
        c = [padding;
        c($-n+2:$);
        c(1:n);
        padding];
    end
    //normalization
    select scalemode
    case "biased" then
        c=c/n
    case "unbiased" then
        scale=n-abs(-maxlags:maxlags)
        scale(scale==0)=1;
        c=c./scale'
    case "coeff" then
        if autocorr then
            c=c/c(maxlags+1)
        else
            c=c/sqrt(xx*yy)
        end
    end
    //give result same orientation as x
    if szx(1)==1 then c=matrix(c,1,-1),end
    if argn(1)==2 then lagindex=-maxlags:maxlags,end
endfunction
