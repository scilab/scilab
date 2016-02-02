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

function [win_l,cwp]=window(wtype,n,par)
    //[win_l,cwp]=window(wtype,n,par)
    //macro which calculates symmetric window
    // wtype :window type (re,tr,hn,hm,kr,ch)
    // n     :window length
    // par   :parameter 2-vector (kaiser window: par(1)=beta>0)
    //       :                   (chebyshev window:par=[dp,df])
    //       :                   dp=main lobe width (0<dp<.5)
    //       :                   df=side lobe height (df>0)
    // win   :window
    // cwp   :unspecified Chebyshev window parameter
    //!
    WT=["re","tr","hm","hn","kr","ch"]

    if and(wtype<>WT) then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"window",1,strcat(WT,",")))
    end
    if type(n)<>1|size(n,"*")<>1|~isreal(n)|size(n,"*")<>1|int(n)<>n|n<1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A positive integer expected.\n"),"window",2))
    end

    if or(wtype==["kr","ch"]) then
        if type(par)<>1|~isreal(par) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A %d-by-%d real vector expected.\n"),"window",3,1,2))
        end
        if wtype=="kr" then
            if size(par,"*")==0| size(par,"*")>2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A %d-by-%d real vector expected.\n"),"window",3,1,2))
            end
            Beta=par(1)
            if Beta<0 then
                error(msprintf(_("%s: Input argument #%d must be strictly positive.\n"),"window",3))
            end
        else //chebychev
            if size(par,"*")<>2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A %d-by-%d real vector expected.\n"),"window",3,1,2))
            end
            dp=par(1);df=par(2)
            if dp>0 then
                if df>0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: incorrect element #%d\n"),"window",3,2))
                end
                if dp>=0.5 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: incorrect element #%d\n"),"window",3,1))
                end
                unknown="df";
            else
                if df<=0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: incorrect element #%d\n"),"window",3,2))
                end
                unknown="dp";
            end
        end
    end
    cwp=-1;

    //Pre-calculations

    no2=(n-1)/2;
    xt=(-no2:no2);
    un=ones(1,n);

    //Select the window type

    select wtype
    case "re" then           //Rectangular window.
        win_l=un
    case "tr" then           //Triangular window.
        win_l=un-2*abs(xt)/(n+1);
    case "hm" then           //Hamming window.
        win_l=.54*un+.46*cos(2*%pi*xt/(n-1));
    case "hn" then           //Hanning window.
        win_l=.5*un+.5*cos(2*%pi*xt/(n-1));
    case "kr" then           //Kaiser window with parameter beta (n,beta)
        //http://en.wikipedia.org/wiki/Kaiser_window
        win_l = besseli(0,Beta*sqrt(1-(2*(0:n-1)/(n-1)-1).^2))/besseli(0,Beta);
    case "ch" then           //Chebyshev window
        m=(n-1)/2;
        select unknown
        case "dp" then,
            dp=1/cosh((n-1)*acosh(1/cos(%pi*df)));
            cwp=dp;
        case "df" then
            df=real(acos(1/cosh(acosh((1+dp)/dp)/(n-1)))/%pi)
            cwp=df;
        end

        //Pre-calculation

        np1=int((n+1)/2);
        odd=modulo(n,2)==1

        x0=(3-cos(2*%pi*df))/(1+cos(2*%pi*df));
        alpha=(x0+1)/2;
        Beta=(x0-1)/2;
        c2=(n-1)/2;

        //Obtain the frequency values of the Chebyshev window
        f=(0:n-1)/n;
        xarg=alpha*cos(2*%pi*f)+Beta;

        //Evaluate Chebyshev polynomials
        //           / cos(n acos(x),    |x| <= 1
        //    p(x) = |
        //           \ cosh(n acosh(x),  |x| > 1
        pr=zeros(1,n);//real part
        pi=zeros(1,n); //imaginary part
        ind = find(xarg<=1); pr(ind)=dp*cos (c2*acos (xarg(ind)));
        ind = find(xarg>1);  pr(ind)=dp*cosh(c2*acosh(xarg(ind)));
        pr=real(pr);

        if ~odd then
            pr=pr.*cos(%pi*f);
            pi=-pr.*sin(%pi*f);
            antisym=[1*ones(1:int(n/2)+1),-1*ones(int(n/2)+2:n)];
            pr=pr.*antisym;
            pi=pi.*antisym;
        end

        //Calculate the window coefficients using the inverse DFT
        twn=2*%pi/n;
        xj=(0:n-1);
        for i=1:np1;
            arg=twn*(i-1)*xj
            w(i)=sum(pr.*cos(arg)+pi.*sin(arg));
        end,
        c1=w(1);
        w=w/c1;
        if odd then
            win_l(np1:n)=w(1:np1);
            win_l(1:np1-1)=w(np1-1:-1:1);
        else,
            win_l(np1+1:n)=w(1:np1);
            win_l(1:np1)=w(np1:-1:1);
        end
        win_l=real(win_l');
    end

endfunction
