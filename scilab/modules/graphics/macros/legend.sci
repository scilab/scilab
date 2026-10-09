// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2010 - INRIA - Serge Steer <serge.steer@inria.fr>
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout=legend(varargin)

    if size(varargin)<1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "legend", 1));
    end

    if argn(1) > 1 then
        msg = gettext("%s: Wrong number of output argument(s): at most %d expected.\n")
        error(msprintf(msg, "legend", 1));
    end

    options_names=["in_upper_right";"in_upper_left";"in_lower_left";
    "in_lower_right";"by_coordinates";"out_upper_right";
    "out_upper_left";"out_lower_left";"out_lower_right";
    "upper_caption";"lower_caption"];

    options_codes=[1;2;3;
    4;5;-1;
    -2;-3;-4;
    -5;-6];
    f        = gcf();
    vis      = f.immediate_drawing;
    vis_on   = vis == "on"; // current figure draw status (to decide if drawnow or standby)
    narg     = size(varargin);
    k0       = 1;
    H        = [];
    leg      = [];
    Acur     = gca();
    A        = Acur;
    kopt      = 1;
    with_box = %T;
    coords=[];

    if type(varargin(narg))==4 then
        with_box=varargin(narg)
        narg=narg-1
    end
    if type(varargin(narg))==1 then
        pos=varargin(narg)
        if size(pos,"*")==2 then //position given by coordinates
            coords=pos;
            narg=narg-1
        elseif size(pos,"*")==1 then //position given by code
            if or(pos==options_codes) then
                kopt=find(pos==options_codes)
                narg=narg-1
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                "legend",narg,strcat(string(options_codes),",")));
            end
        else
            error(msprintf(_("%s: Wrong size for input argument #%d."),"legend",narg));
        end
    elseif  type(varargin(narg))==10 then
        k=find(varargin(narg)==options_names)
        if ~isempty(k) then
            kopt=k
            narg=narg-1
        end
    end

    if type(varargin(k0))==9 then //a handle that could be an Axes or array of Polyline handle.
        tmpH=varargin(k0);
        if size(tmpH,"*") == 1 && tmpH.type=="Axes"
            H = getvalidchildren(tmpH)($:-1:1);
        elseif  tmpH.type == "Polyline"
            H = tmpH;
        else
            error(msprintf(_("%s: Wrong type for input argument #%d: single Axes handle or array of Polyline handles expected\n"), "legend", k0))     
        end
        k0 = k0+1;
    elseif type(varargin(k0)) <> 10
         error(msprintf(_("%s: Wrong type for input argument #%d: handle or string expected\n"), "legend", k0));
    end

    if H==[] then
        //walk subtree to get all proper children
        H = getvalidchildren(A)($:-1:1);
    end

    if H==[] then
        error(msprintf(_("%s: No ''%s'' handle found.\n"), "legend","Polyline"));
    end

    //get all labels
    for k=k0:narg
        if type(varargin(k))<>10 then
            break;
        end
        vk=varargin(k)
        leg=[leg; vk(:)]
    end
    nleg=size(leg,"*")


    //upper left coordinates
    if ~isempty(coords) then
        bnds=get(gca(),"axes_bounds");
        as=get(gcf(),"axes_size");

        [x1,y1,rect]=xchange(coords(1),coords(2),"f2i")
        x1=x1-as(1)*bnds(1)
        y1=y1-as(2)*bnds(2)
        pos=[x1/(as(1)*bnds(3)), y1/(as(2)*bnds(4))]
        kopt=5
    elseif kopt==5 then
        pos=[]
    end
    // the number of labels might be lower than the number of polylines
    nbLeg = min(size(H, "*"), size(leg, "*"));
    H = H(1:nbLeg);
    leg = leg(1:nbLeg);


    drawlater()

    c = %_legend(H,leg)
    if c == [] then
        if argn(1) == 1
            varargout(1) = c;
        end    
        // return immediately (to avoid to return a struct see issue #17275)
        return
    end
    if options_codes(kopt)<0 then
        c.background=f.background
    else
        c.background=Acur.background
    end
    if with_box then c.line_mode="on",else c.line_mode="off",end
    c.legend_location=options_names(kopt)
    if kopt==5 then
        if pos<>[] then
            c.position=pos;
        else
            if vis_on then drawnow(); end    // draw if figure status allows it (otherwise standby)
            bnds=get(gca(),"axes_bounds");
            as = get(gcf(),"axes_size");
            tmp = c.parent.parent   // Figure | uicontrol (frame)
            if tmp.type=="uicontrol"  // frame
                asf = tmp.position(3:4)
                if tmp.units=="normalized"
                    as = as .* asf
                else
                    // pixels ('points' not implemented: scale point/pixel unknown)
                    as = asf
                end
            end
            while %t
                rep=xgetmouse()
                if rep(3)<> -1 then break,end
                [x1,y1,rect]=xchange(rep(1),rep(2),"f2i")
                x1=x1-as(1)*bnds(1)
                y1=y1-as(2)*bnds(2)
                pos=[x1/(as(1)*bnds(3)), y1/(as(2)*bnds(4))]
                c.position=pos;
            end
        end
    end
    if vis_on then drawnow(); end       // draw if figure status allows it (otherwise standbye)
    if argn(1) == 1
        varargout(1) = c;
    end    
endfunction

function h=getvalidchildren(A)
    h=[]
    for k=1:size(A,"*")
        a=A(k)
        select a.type
        case "Polyline" then
            h=[h;a]
        case "Axes"
            ax=a.children
            h=[h;getvalidchildren(ax)]
        case "Compound"
            for i=1:1:size(a.children,"*")
                h=[h;getvalidchildren(a.children(i))]
            end
        end
    end
endfunction
