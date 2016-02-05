// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=gainplot(varargin)
    rhs=size(varargin)

    if rhs == 0 then
        s=poly(0,"s");
        h1=syslin("c",(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
        h2=syslin("c",(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225));
        gainplot([h1;h2],0.01,100,..
        ["$\frac{s^2+18 s+100}{s^2+6.06 s+102.1}$";
        "$\frac{s^2+3.02 s+228.01}{s^2+27 s+225}$"]);
        title("Gainplot");
        return;
    end

    if type(varargin($))==10 then
        comments=varargin($);
        rhs=rhs-1;
    else
        comments=[];
    end
    fname="gainplot";//for error messages

    fmax=[];
    if or(typeof(varargin(1))==["state-space" "rational"]) then
        //sys,fmin,fmax [,pas] or sys,frq
        refdim=1 //for error message
        if rhs==1 then
            [frq,repf]=repfreq(varargin(1),1d-3,1d3);
        elseif rhs==2 then //sys,frq
            if size(varargin(2),2)<2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                fname,2,1));
            end
            [frq,repf]=repfreq(varargin(1:rhs));
        elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
            [frq,repf]=repfreq(varargin(1:rhs));
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,1,5))
        end
        [phi,d]=phasemag(repf);
    elseif  type(varargin(1))==1 then
        //frq,db,phi [,comments] or frq, repf [,comments]
        refdim=2
        select rhs
        case 2 then //frq,repf
            frq=varargin(1);
            if size(frq,2)<2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                fname,1,1))
            end
            if size(frq,2)<>size(varargin(2),2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                fname,1,2))
            end

            [phi,d]=phasemag(varargin(2))
        case 3 then  //frq,db,phi
            [frq,d]=varargin(1:rhs-1)
            if size(frq,2)<>size(d,2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                fname,1,2))
            end
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
    end;

    frq=frq';
    d=d';
    [n,mn]=size(d);
    if and(size(comments,"*")<>[0 mn]) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
        fname,refdim,rhs+1))
    end

    //
    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    axes = gca() ;
    if size(axes.children,"*")==0 then
        axes.data_bounds=[min(frq),min(d);max(frq),max(d)]
        axes.x_label.text=_("Frequency (Hz)")
        axes.y_label.text=_("Magnitude (dB)")

    else
        axes.data_bounds=[min([min(frq),min(d)],axes.data_bounds(1,:));
        max([max(frq),max(d)],axes.data_bounds(2,:))];
    end
    axes.axes_visible="on";
    axes.log_flags = "lnn" ;
    axes.grid=color("lightgrey")*ones(1,3);

    if size(d,2)>1&size(frq,2)==1 then
        xpolys(frq(:,ones(1,mn)),d,1:mn)
        e=gce();
    else
        xpolys(frq,d,1:mn)
        e=gce();
    end
    for i=1:size(e.children,"*")
        e.children(i).display_function = "formatGainplotTip";
    end
    if comments<>[] then
        legend(comments)
    end
    fig.immediate_drawing=immediate_drawing;
endfunction
