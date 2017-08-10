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

function [sm,cwp]=pspect(sec_step,sec_leng,wtype,x,y,wpar)
    //[sm [,cwp] ]=pspect(sec_step,sec_leng,wtype,x [,y] [,wpar])
    //Cross-spectral estimate between x and y if both are given
    //and auto-spectral estimate of x otherwise.
    //Spectral estimate obtained using the modified periodogram method
    // x        :
    //            - if vector, the first signal data
    //            - amount of input data if scalar. In this case the first signal
    //              is read by batch by a   user defined function xbatch=getx(n,offset).)
    // y        : optional, if present cross correlation is made else auto correlation
    //            - if vector, the second  signal data .It must have the same number of element than x
    //            - if scalar, the second signal
    //              is read by batch by a  user defined function ybatch=gety(n,offset).)
    // sec_step : offset of each data window.
    //            if sec_step==sec_leng/2 50% overlap is made
    // sec_leng : Number of points of the window
    // wtype    : window type ('re','tr','hm','hn','kr','ch')
    // wpar     : optional window parameters
    //          :         for wtype='kr', wpar>0
    //          :         for wtype='ch', 0<wpar(1)<.5, wpar(2)>0
    // sm       : power spectral estimate in the interval [0,1]
    // cwp      : unspecified Chebyshev window parameter
    //!

    [lhs,rhs]=argn(0);


    if sec_step>=sec_leng then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Argument #%d expected to be less than argument #%d.\n"),"pspect",1,2,1,2));
    end
    if and(wtype<>["re","tr","hm","hn","kr","ch"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "pspect",3,"''re'',''tr'',''hm'',''hn'',''kr'',''ch''"));
    end

    //Analyze syntax and construct window
    if rhs==4 then,
        //pspect(sec_step,sec_leng,wtype,x)
        w=window(wtype,sec_leng);
        crossFlag=%f; //autocorrelation
    elseif rhs==5 then,
        //pspect(sec_step,sec_leng,wtype,x,y) or pspect(sec_step,sec_leng,wtype,x,wpar)
        if wtype=="kr" then,//pspect(sec_step,sec_leng,'kr',x,wpar)
            wpar=y;
            w=window(wtype,sec_leng,wpar);cwp=[];
            crossFlag=%f; //autocorrelation
        elseif wtype=="ch" then,//pspect(sec_step,sec_leng,'ch',x,wpar)
            wpar=y;
            [w,cwp]=window(wtype,sec_leng,wpar);
            crossFlag=%f; //autocorrelation
        else,//pspect(sec_step,sec_leng,wtype,x,y)
            w=window(wtype,sec_leng);cwp=[];
            crossFlag=%t; //cross-correlation
        end,
    else //pspect(sec_step,sec_leng,wtype,x,y,wpar)
        [w,cwp]=window(wtype,sec_leng,wpar);
        crossFlag=%t; //cross-correlation
    end,
    wpower=w*w';//the window energy

    //Make x and y row vectors
    x=matrix(x,1,-1);
    if crossFlag then
        y=matrix(y,1,-1);
        if size(x,"*")<>size(y,"*") then
            error(msprintf(gettext("%s: Arguments #%d and #%d must have the same sizes.\n"),"pspect",4,5));
        end
    end

    //Average periodograms
    sm=0*w;
    if size(x,"*")==1 then  //Batch processing of x and y data

        //get number of sections to be used
        nsecs=int((x-sec_leng+sec_step)/sec_step);//x  contains the number of points of the signals
        ovrlp=sec_leng-sec_step;
        xd=[0*ones(1,sec_step) getx(ovrlp,1)];
        if crossFlag then
            yd=[0*ones(1,sec_step) gety(ovrlp,1)];
        end
        for k=1:nsecs
            xd(1:ovrlp)=xd(sec_step+1:sec_leng);
            xd(ovrlp+1:sec_leng)=getx(sec_step,sec_leng+(k-1)*sec_step+1);
            xw=w.*(xd-(sum(xd)/sec_leng));
            fx=fft(xw,-1);
            if crossFlag then
                yd(1:ovrlp)=yd(sec_step+1:sec_leng);
                yd(ovrlp+1:sec_leng)=gety(sec_step,sec_leng+(k-1)*sec_step+1);
                yw=w.*(yd-(sum(yd)/sec_leng));
                sm=sm+fx.*conj(fft(yw,-1));
            else
                sm=sm+real(fx.*conj(fx));
            end
        end
    else // Signal data given in x and y if cross-correlation

        //get number of sections to be used
        nsecs=int((size(x,"*")-sec_leng+sec_step)/sec_step);
        ind=(1:sec_leng);
        for k=1:nsecs
            indd=ind+(k-1)*sec_step*ones(1:sec_leng);
            xd=x(indd);
            xe=w.*(xd-(sum(xd)/sec_leng));
            fx=fft(xe,-1);
            if crossFlag then
                yd=y(indd);
                ye=w.*(yd-(sum(yd)/sec_leng));
                sm=sm+fx.*conj(fft(ye,-1));
            else
                sm=sm+real(fx.*conj(fx));
            end
        end
    end

    //Normalization
    sm=sm/(nsecs*wpower);
endfunction
