// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=phaseplot(varargin)
    rhs=size(varargin)
    if type(varargin($))==10 then
        comments=varargin($);
        rhs=rhs-1;
    else
        comments=[];
    end
    fname="phaseplot";//for error messages

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
            [frq,d,phi]=varargin(1:rhs)
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
    phi=phi';
    [n,mn]=size(phi);
    if and(size(comments,"*")<>[0 mn]) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
        fname,refdim,rhs+1))
    end

    //
    fig=gcf();
    id=fig.immediate_drawing;
    fig.immediate_drawing="off";

    axes = gca() ;
    if size(axes.children,"*")==0 then
        axes.data_bounds=[min(frq),min(phi);max(frq),max(phi)]
        axes.x_label.text=_("Frequency (Hz)")
        axes.y_label.text=_("Phase (Deg)")

    else
        axes.data_bounds=[min([min(frq),min(phi)],axes.data_bounds(1,:));
        max([max(frq),max(phi)],axes.data_bounds(2,:))];
    end
    axes.axes_visible="on";
    axes.log_flags = "lnn" ;
    axes.grid=color("lightgrey")*ones(1,3);

    if size(phi,2)>1&size(frq,2)==1 then
        xpolys(frq(:,ones(1,mn)),phi,1:mn)
        e=gce();
    else
        xpolys(frq,phi,1:mn)
        e=gce();
    end
    for i=1:size(e.children,"*")
        e.children(i).display_function = "formatPhaseplotTip";
    end
    if comments<>[] then
        legend(comments)
    end
    fig.immediate_drawing=id;
endfunction
