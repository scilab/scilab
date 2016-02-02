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

function [hzt,zt,gt]=trans(pd,zd,gd,tr_type,frq)
    //hzt=trans(pd,zd,gd,tr_type,frq)
    //macro for transforming standardized low-pass filter into
    //one of the following filters:
    //     low-pass, high-pass, band-pass, stop-band.
    // hz      :input polynomial
    // tr_type :type of transformation
    // frq     :frequency values
    // hzt     :output polynomial
    //!
    if and(argn(1)<>[1 3]) then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d or %d expected.\n"),"trans",1,3))
    end
    select argn(2)
    case 3 then //trans(hz,tr_type,frq): filter given by a siso tranfer function
        hz=pd
        if typeof(hz)<>"rational" then
            error(msprintf(_("%s: Wrong type for input argument #%d: rational fraction array expected.\n"),"trans",1))
        end
        if size(hz,"*")<>1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A single input, single output system expected.\n"),"trans",1))
        end
        tr_type=zd;tr_pos=2;
        frq=gd;frq_pos=3;
        pd=roots(hz.den)
        zd=roots(hz.num)
        gd=coeff(hz.num,degree(hz.num))/coeff(hz.den,degree(hz.den))
    case 5 then //trans(pd,zd,gd,tr_type,frq):  filter given by zeros,poles and gain
        if type(pd)<>1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"trans",1))
        end
        if type(zd)<>1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"trans",2))
        end
        if type(gd)<>1|size(gd,"*")<>1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"trans",3))
        end
        tr_pos=4;
        frq_pos=5;
    else
        error(msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"),"trans",3,5))
    end
    if and(tr_type<>["lp","hp","bp","sb"]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"trans",tr_pos,"''lp'',''hp'',''bp'',''sb''"))
    end
    if type(frq)<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"trans",frq_pos))
    end
    if or(tr_type==["lp","hp"]) then
        if and(size(frq,"*")<>[1,2]) then
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"trans",frq_pos))
        end
        fu=frq(1)
    else
        if size(frq,"*")<>2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A %d elements array expected.\n"),"trans",frq_pos,2))
        end
        fu=frq(1);fl=frq(2);
    end

    z=poly(0,"z");fc=.25;
    //make filter type using all-pass change of variables
    select tr_type
    case "lp" then
        alpha=sin(%pi*(fc-fu))/sin(%pi*(fc+fu));
        num=z-alpha;
        den=1-alpha*z;
    case "hp" then
        alpha=-cos(%pi*(fc-fu))/cos(%pi*(fc+fu));
        num=-(1+alpha*z);
        den=z+alpha;
    case "bp" then
        k=tan(%pi*fc)/tan(%pi*(fu-fl));
        alpha=cos(%pi*(fu+fl))/cos(%pi*(fu-fl));
        num=-((k+1)-2*alpha*k*z+(k-1)*z^2);
        den=(k+1)*z^2-2*alpha*k*z+(k-1);
    case "sb" then
        k=tan(%pi*fc)*tan(%pi*(fu-fl));
        alpha=cos(%pi*(fu+fl))/cos(%pi*(fu-fl));
        num=(k+1)-2*alpha*z+(1-k)*z^2;
        den=(k+1)*z^2-2*alpha*z+(1-k);
    end
    if tr_type == "lp" & fu == 0.25 then //no change of variables is necessary if user wants a low pass filter with cut off frequency of 0.25Hz.
        pt=pd;
        zt=zd;
        gt=gd;
    else
        [pt,zt,gt]=bilt(pd,zd,gd,num,den);
    end
    if argn(1)==1 then
        hzt=rlist(gt*real(poly(zt,"z")),real(poly(pt,"z")),"d");
    else
        hzt=pt(:)
        zt=zt(:)
    end
endfunction
