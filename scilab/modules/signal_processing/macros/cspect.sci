// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [sm,cwp]=cspect(nlags,ntp,wtype,x,y,wpar)
    //[sm,cwp]=cspect(nlags,ntp,wtype,x,y,wpar)
    //Spectral estimation using the correlation method.
    //Cross-spectral estimate of x and y is calculated when both
    //x and y are given.  Auto-spectral estimate of x is calculated
    //if y is not given.
    //  x        :data if vector, amount of input data if scalar
    //  y        :data if vector, amount of input data if scalar
    //  nlags    :number of correlation lags (pos. integer)
    //  ntp      :number of transform points (pos. integer)
    //  wtype    :window type ('re','tr','hm','hn','kr','ch')
    //  wpar     :optional window parameters
    //           :         for wtype='kr', wpar>0
    //           :         for wtype='ch', 0<wpar(1)<.5, wpar(2)>0
    //  sm       :power spectral estimate in the interval [0,1]
    //  cwp      :calculated value of unspecified Chebyshev
    //           :window parameter
    //
    //!

    [lhs,rhs]=argn(0);

    if and(wtype<>["re","tr","hm","hn","kr","ch"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "cspect",3,"''re'',''tr'',''hm'',''hn'',''kr'',''ch''"));
    end

    //Analyze syntax and construct window
    if rhs==4 then,
        //cspect(nlags,ntp,wtype,x)
        [w,cwp]=window(wtype,2*nlags-1);
        crossFlag=%f; //autocorrelation
    elseif rhs==5 then
        //cspect(nlags,ntp,wtype,x,wpar) or cspect(nlags,ntp,wtype,x,y)
        if wtype=="kr" then //cspect(nlags,ntp,'kr',x,wpar)
            wpar=y;
            [w,cwp]=window(wtype,2*nlags-1,wpar);
            crossFlag=%f; //autocorrelation
        elseif wtype=="ch" then  //cspect(nlags,ntp,'ch',x,wpar)
            wpar=y;
            [w,cwp]=window(wtype,2*nlags-1,wpar);
            crossFlag=%f; //autocorrelation
        else,//cspect(nlags,ntp,wtype,x,y)
            crossFlag=%t;//cross correlation
            [w,cwp]=window(wtype,2*nlags-1);
        end,
    else,//cspect(nlags,ntp,wtype,x,y,wpar)
        [w,cwp]=window(wtype,2*nlags-1,wpar);
        crossFlag=%t;//cross correlation
    end,


    //Make x and y row vectors
    x=matrix(x,1,-1);
    if crossFlag then
        y=matrix(y,1,-1);
        if size(x,"*")<>size(y,"*") then
            error(msprintf(gettext("%s: Arguments #%d and #%d must have the same sizes.\n"),"pspect",4,5));
        end
    end

    //Estimate correlations
    if size(x,"*")==1 then  //Batch processing of x and y data
        nsects=int(x/(3*nlags));
        xlen=int(x/nsects);
        ss=zeros(1,2*nlags);
        if crossFlag then,
            for k=1:nsects
                xk=getx(xlen,1+(k-1)*xlen);
                yk=gety(xlen,1+(k-1)*xlen);
                ss=corr("update",xk,yk,ss);
            end
            re=fft(ss,1)/x;
            re=[re(nlags:-1:1) re(2:nlags)];
        else
            for k=1:nsects
                xk=getx(xlen,1+(k-1)*xlen);
                ss=corr("update",xk,ss);
            end
            re=fft(ss,1)/x;
            re=[re(nlags:-1:1) re(2:nlags)];
        end
    else // Signal data given in x and y if cross-correlation
        if crossFlag then
            [re1,me]=corr(x,y,nlags);
            [re2,me]=corr(y,x,nlags);
            re=[re1(nlags:-1:1) re2(2:nlags)];
        else
            [re,me]=corr(x,nlags);
            re=[re(nlags:-1:1) re(2:nlags)];
        end
    end

    //Window correlation estimate
    wre=w.*re;

    //Fourier transform to obtain spectral estimate
    wree=[wre zeros(1,ntp-2*nlags+1)];
    sm=fft(wree,-1)
    //sm=abs(fft(wree,-1));
endfunction
