//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function standard_draw (o, frame, draw_ports, up)
    //
    //
    //** This function is used to draw the object from the most of (but NOT all) the interface functions
    //**
    //** 21 Nov 2006 : some save/restore patch moved to upper level "drawobj.sci"
    //** 22 Jun 2009  (Serge Steer INRIA)
    //   -  gh_curwin.children use replaced by gh_axes to get the
    //      axes children because uimenu are now children of the figure
    //   -  use gh_blk for argument of dig_bound_compound
    //   -  use of gh_text to set text properties of the indentifier (instead
    //      of modifying  the correponding axes properties

    xf = 60 ; yf = 40 ;

    [lhs,rhs] = argn(0)

    if rhs==1 then
        frame=%t
    end

    if rhs<3 then
        draw_ports = standard_draw_ports     //** left right ports
        //** the function 'standard_draw_ports' it's copied in 'draw_ports'
    elseif rhs==4 then                       //** otherwise
        draw_ports = standard_draw_ports_up  //** up / down ports
    end                                      //** the function 'standard_draw_ports_up' it's copied in 'draw_ports'

    nin   = size(o.model.in,1);
    nout  = size(o.model.out,1);
    clkin = size(o.model.evtin,1);
    clkout= size(o.model.evtout,1);
    [orig,sz] = (o.graphics.orig,o.graphics.sz)
    orient=%t;
    style=graphics.style;
    subStr=strstr(style,"flip=");
    if subStr<>"" then
        orient=%f;
    end

    //** Default values -------------------
    thick = 1              ; //** patch Simone
    e = 4 ;
    With3D = options("3D")(1)
    //** ----------------------------------

    gr_i = o.graphics.gr_i

    if type(gr_i) == 15 then
        [gr_i,coli] = gr_i(1:2) ;
    else
        coli = [] ;
    end

    //** --------------------------------------- draw box ------------------------------------------------
    if frame then
        if With3D then  //** this is the code relative to the block's "window dressing"
            //**---------- 3D Mode ON -----------------------------------------------------------------------
            #Color3D = options("3D")(2)
            // xpoly([orig(1)+e;orig(1)+sz(1);orig(1)+sz(1)],[orig(2)+sz(2);orig(2)+sz(2);orig(2)+e],'lines')
            xrect( orig(1)+e, orig(2)+sz(2), sz(1)-e, sz(2)-e) ;
            gh_e = gce(); //** new graphics :)
            gh_e.thickness = 0 ;
            gh_e.foreground = #Color3D ;

            if coli<>[] then
                gh_e.fill_mode = "on"  ;
                gh_e.background = coli ;
            end

            xx = [ orig(1)   , orig(1)
            orig(1)   , orig(1)+sz(1)-e
            orig(1)+e , orig(1)+sz(1)
            orig(1)+e , orig(1)+e] ;

            yy = [orig(2)         , orig(2)
            orig(2)+sz(2)-e , orig(2)
            orig(2)+sz(2)   , orig(2)+e
            orig(2)+e       , orig(2)+e];

            xfpolys(xx,yy,-[1,1]*#Color3D); //** fill a set of polygons
            gh_e = gce()              ;
            gh_c = gh_e.children(1:2) ;
            gh_c.foreground = default_color(0) ;
            gh_c.thickness = 2 ;

        else //** not in 3D mode
            //**----------3D Mode OFF -------------------------------------------------------------------------
            e = 0 ;
            xrect(orig(1),orig(2)+sz(2),sz(1),sz(2)) ;
            gh_e = gce()       ;
            gh_e.thickness = 2 ;

            if coli<>[] then
                gh_e.fill_mode = "on"  ;
                gh_e.background = coli ;
            end

        end //** of 3D mode ON/OFF
    end //** of the frame mode
    draw_ports(o) ; //** 'standard_draw_ports' or 'standard_draw_ports_up'
    //** --------------------------------------------------------------------------------------------------------------

    //** ---- scs_m , %cpr Indexes Show including information on internal superblocks ---------------------------------
    if exists("%scicos_with_grid") then
        if %scicos_with_grid then
            scs_m_i=[]; cpr_i=[] ; sflag=[] ;
            win_id = gh_current_window.figure_id

            kc = find(win_id==windows(:,2))

            pal = windows(kc,1);

            if pal > 0 then
                if ~exists("scs_m_index") then scs_m_index=-1, end
                [scs_m_i, cpr_i, sflag] = get2index(o, scs_m_index);

                txt_index = [];
                if scs_m_i<>[] & cpr_i<>[] then
                    if sflag<>[] then
                        txt_index = string(scs_m_i)+"s, "+cpr_i+"c";
                    else
                        txt_index = string(scs_m_i)+"s, "+string(cpr_i)+"c";
                    end
                end

                gh_axes = gca(); //** get the Axes proprieties

                // draw indexes
                if txt_index<>[] then

                    //@@ compute bbox
                    rect = stringbox(txt_index, orig(1), orig(2), 0,...
                    options.ID(1)(1), options.ID(1)(2));

                    w = (rect(1,3)-rect(1,2)) * %zoom;
                    w = max(w, sz(1));
                    h = (rect(2,2)-rect(2,4)) * 1.3 * %zoom/1.4;

                    //@@ fill txt_index in a box
                    xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) + sz(2), txt_index , w, h,"fill") ;
                    gh_txt = gce();
                    gh_txt.font_style = options.ID(1)(1) ;
                    gh_txt.font_size  = options.ID(1)(2) ;

                end
            end
        end
    end //** of on/off control
    //** --------------------------------------------------------------------------------------------------------------

    //** --------------------------------- draw gr_i ---------------------------------
    o_size=size(gh_axes.children)
    deff("c=scs_color(c)"," ") ; //** on line function definition
    gr_i = [gr_i]; //** simple but not simpler :)
    model = o.model
    if With3D & frame then
        orig = orig + e ;
        sz   = sz - e   ;
    end
    //@@ Alan,28/11/08 : protect variables of standard_draw
    deff("ierr=exec_gr_i(gr_i)","ierr=execstr(gr_i,''errcatch'');") ; //** on line function definition
    //** Check of the graphics object has generated any error
    if exec_gr_i(gr_i)<>0 then
        messagebox(["Error in Icon defintion"; lasterror()],"modal")
    end
    //** -----------------------------------------------------------------------------

    //** -------------------------------- Identification --------------------------------------------------------------

    ident = o.graphics.id

    // draw Identification
    if ident<>[] & ident<>"" then


        //@@ compute bbox of gr object

        p_size=size(gh_axes.children);
        if p_size(1)-o_size(1)<>0 then
            gh_blk=glue(gh_axes.children(1:(p_size(1)-o_size(1))));
            rect=dig_bound_compound(gh_blk)
            //@@ Alan,28/11/08 : I redraw all here because glue/unglue op change
            //@@                 some properties of objects in the compound
            //unglue(gh_blk)
            delete(gh_blk)
            exec_gr_i(gr_i) //@@ no test here : this have been done previously
        else
            rect=[orig(1) orig(2) orig(1)+sz(1) orig(2)+sz(2)]
        end
        if With3D & frame then
            rect(1,1)=rect(1,1)-e
            rect(1,2)=rect(1,2)-e
            orig = orig - e ;
            sz   = sz + e   ;
        end
        //rect(1,1)=min(rect(1,1),orig(1))
        rect(1,2)=min(rect(1,2),orig(2))
        //rect(1,3)=max(rect(1,3),orig(1)+sz(1))
        rect(1,4)=max(rect(1,4),orig(2)+sz(2))

        //@@ compute bbox
        rectstr = stringbox(ident, rect(1,1), rect(1,2), 0,...
        options.ID(1)(1), options.ID(1)(2));
        w=(rectstr(1,3)-rectstr(1,2)) * %zoom;
        h=(rectstr(2,2)-rectstr(2,4)) * %zoom;

        //@@ fill ident in a box
        xstringb(orig(1) + sz(1) / 2 - w / 2, rect(1,2) - (h*1.1) , ident , ...
        w, h,"fill") ;
        //@@ Serge 22 Jun 2009
        gh_text=gce();
        gh_text.font_style = options.ID(1)(1) ;
        gh_text.font_size  = options.ID(1)(2) ;
        gh_text.font_foreground = options.ID(1)(3) ; //@@

        //@@ set visible
        if gh_axes.children(1).type == "Text" then
            if options.ID(1)(4)==0 then
                gh_axes.children(1).visible="off"
            else
                gh_axes.children(1).visible="on"
            end
        else //@@compound
            for i=1:size(gh_axes.children(1).children,1)
                if options.ID(1)(4)==0 then
                    gh_axes.children(1).children(i).visible="off"
                else
                    gh_axes.children(1).children(i).visible="on"
                end
            end
        end
    end
    //** --------------------------- Identification End --------------------------------------------------------------

endfunction
