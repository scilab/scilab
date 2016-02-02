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

function [hn]=eqfir(nf,bedge,des,wate)
    //<hn>=eqfir(nf,bedge,des,wate)
    //Minimax approximation of multi-band, linear phase, FIR filter
    //  nf    :Number of output filter points desired
    //  bedge :Mx2 matrix giving a pair of edges for each band
    //  des   :M-vector giving desired magnitude for each band
    //  wate  :M-vector giving relative weight of error in each band
    //  hn    :Output of linear-phase FIR filter coefficients
    //!

    //get number of cosines

    nc=int(nf/2);
    if nf-2*nc<>0 then,
        flag=0;
        nc=nc+1;
    else,
        flag=1;
    end,

    //make frequency grid, desired function, and weight function

    [nb,c2]=size(bedge);
    ngp=nc*16;
    b1=bedge(:,1);
    b2=bedge(:,2);
    sb=sum(b2-b1);
    delf=sb/ngp;
    bp=round((b2-b1)/delf);
    bsum=0;
    for k=1:nb,
        bpk=bp(k);
        et=b2(k)-b1(k);
        fg(bsum+1:bsum+bpk)=b1(k)*ones(1:bpk)+(0:bpk-1)*et/(bpk-1);
        ds(bsum+1:bsum+bpk)=des(k)*ones(1:bpk);
        wt(bsum+1:bsum+bpk)=wate(k)*ones(1:bpk);
        bsum=bsum+bpk;
    end,

    //adjust values of ds and wt if filter is of even length

    if flag==1 then,
        fgs=max(size(fg));
        if fg(fgs)>.5-%eps then,
            fg=fg(1:fgs-1);
            ds=ds(1:fgs-1);
            wt=wt(1:fgs-1);
        end,
        cf=cos(%pi*fg);
        ds=ds./cf;
        wt=wt.*cf;
    end,

    //call remez

    [an]=remezb(nc,fg,ds,wt);

    //obtain other half of filter coefficients (by symmetry)

    if flag==1 then,
        hn(1)=.25*an(nc);
        hn(2:nc-1)=.25*(an(nc:-1:3)+an(nc-1:-1:2));
        hn(nc)=.5*an(1)+.25*an(2);
        hn(nc+1:2*nc)=hn(nc:-1:1);
    else,
        hn=an(nc:-1:2)/2;
        hn(nc)=an(1);
        hn(nc+1:2*nc-1)=hn(nc-1:-1:1);
    end,
endfunction
