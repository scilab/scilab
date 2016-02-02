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

function []=m_circle(gain)
    //this function is obsolete, superseeded by hallchart.
    [lhr,rhs]=argn(0)
    //
    d36=11;
    if rhs<1 then
        gain=[-12 -8 -6 -5 -4 -3 -2 -1.4 -1 -.5 0.25 0.5 0.7 1 1.4 ..
        2 2.3 3 4 5 6 8 12]
    else
        gain=matrix(gain,1,prod(size(gain)))
    end
    //
    titre="isogain contours for y/(1+y)"
    l10=log(10);
    //
    lambda=exp(l10*gain/20)
    rayon=lambda./(lambda.*lambda-ones(lambda))
    centre=-lambda.*rayon
    rayon=abs(rayon)
    rect=[min(centre-rayon),min(-rayon),max(centre+rayon),max(rayon)];
    //
    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    ax=gca();
    llrect=xstringl(0,0,"1")
    hx=llrect(3);
    //
    for i=1:prod(size(gain))
        sgn=1;if 2*int(i/2)==i then sgn=-1,end
        g=string(gain(i)),ng=length(g)
        if gain(i)<0 then
            w=0:0.03:%pi;
            xx=centre(i)+rayon(i)*sin(%pi/3)+hx
            xy=sgn*cos(%pi/3)*rayon(i)
        else
            xx=centre(i)-sin(%pi/3)*rayon(i)-hx*ng
            xy=sgn*cos(-%pi/3)*rayon(i)
            w=-%pi:0.03:0;
        end;
        n=prod(size(w))
        rf=centre(i)*ones(w)+rayon(i)*exp(%i*w);
        xpoly([real(rf) real(rf($:-1:1))],[imag(rf)  -imag(rf($:-1:1))])
        e=gce();e.foreground=3;e.clip_state="clipgrf"
    end;
    fig.immediate_drawing=immediate_drawing;

endfunction
