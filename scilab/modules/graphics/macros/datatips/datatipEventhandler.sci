// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatipEventhandler(win,x,y,ibut)
    //datatip utility function
    //The event handler which rules the datatips interactive edition;
    fig=get_figure_handle(win);
    global datatipAngles zoom_box;
    if ibut<0 then
        if ibut==-1000 then
            global datatipGUIwin
            if datatipGUIwin<>[]&or(datatipGUIwin==winsid()) then //the gui window isopened
                delete(get_figure_handle(datatipGUIwin))
                clearglobal datatipGUIHandles datatipGUICurve datatipGUIwin
            end
        end
        //     ax=getAxes([x,y],fig);
        //     if or(datatipAngles<>ax.rotation_angles)|or(zoom_box<>ax.zoom_box) then
        //       datatipAngles=ax.rotation_angles;
        //       zoom_box=ax.zoom_box;
        //     end
        return,
    end

    fig.event_handler_enable = "off";
    ax=getAxes([x,y],fig);
    sca(ax);
    //change the pixel coordinates to user coordinates

    [x,y]=xchange(x,y,"i2f");pt=[x,y]
    if or(ibut==[127 100]) then //delete current
        curve_handles=datatipGetEntities(ax);
        datatipDeleteSelected(curve_handles)
    elseif or(ibut==[37 39]) then //left and right arrows
        curve_handles=datatipGetEntities(ax);
        [curve,ind]=datatipGetSelected(curve_handles)
        if ind<>[] then
            ud=datatipGetStruct(curve)
            tip_handle=ud.tips.children(ind)
            point_handle=tip_handle.children(1)

            if ibut==39 then
                k=point_handle.user_data(2)+1
                if k<=size(curve.data,1) then
                    datatipSetTipPosition(ud,tip_handle,curve.data(k,:),k)
                end
            else
                k=point_handle.user_data(2)-1
                if k>=1 then
                    datatipSetTipPosition(ud,tip_handle,curve.data(k,:),k)
                end
            end
        end
    elseif or(ibut==[0 3]) then //select a point on a curve

        //find  curves in the current axes
        curve_handles=datatipGetEntities(ax);
        if curve_handles==[] then;
            return,
        end
        //The datatip text box origins are given in user coordinates but their
        //computation takes into account the pixel dimensions of the text
        //boxes, so rotations and zoom requires a recomputation of the
        //origins. There is currently
        if datatipAngles==[] then
            datatipAngles=ax.rotation_angles;
            zoom_box=ax.zoom_box;
        else
            if or(datatipAngles<>ax.rotation_angles)|or(zoom_box<>ax.zoom_box) then
                datatipAngles=ax.rotation_angles;
                zoom_box=ax.zoom_box;
            end
        end
        //check for a text box present near the selected point
        [k,l]=datatipLookfor(curve_handles,pt);
        if k<>[] then;
            datatipSetSelected(curve_handles,[k,l])
            if ibut==0 then
                ud=datatipGetStruct(curve_handles(k));
                tip_handle=ud.tips.children(l);
                datatipMove(tip_handle);
            end
            fig.event_handler_enable = "on";
            return,
        end
        [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,curve_handles);
        if dmin<10 then;
            //small minimal distance, create a text box at this point (or move
            //an existing tip at this point
            ud=datatipGetStruct(curve);// the curve datatips data structure
            if typeof(ud)<>"datatips" then;
                ud=datatipGetStruct(curve);
            end
            if ud.replace&ud.tips.children<>[] then
                tip_handle=ud.tips.children($)
                if ud.interpolate then
                    datatipSetTipPosition(ud,tip_handle,ptmin,l)
                else
                    datatipSetTipPosition(ud,tip_handle,curve.data(l,:),l)
                end
            else
                if ud.interpolate then
                    datatipCreate(curve,ptmin);
                else
                    datatipCreate(curve,l); //
                end
            end
        else
            //      unselect all
            [curve,ind]=datatipGetSelected(curve_handles)
            if ind<>[] then
                ud=datatipGetStruct(curve);
                tip_handle=ud.tips.children(ind);
                datatipHilite(tip_handle) //unhilite
                ud.selected=0
                datatipSetStruct(curve,ud);
            end
        end
    elseif or(ibut==5) then
        datatipContextMenu(ax)

    elseif or(ibut==[1 4]) then //middle button
        curve_handles=datatipGetEntities(ax);
        [k,l]=datatipLookfor(curve_handles,pt);
        if k<>[] then;
            ud=datatipGetStruct(curve_handles(k))// the curve datatips data structure
            tip_handle=ud.tips.children(l);
            orient=["automatic" "upper left" "upper right", "lower left", "lower right"];
            orientations=[_("automatic") _("upper left") _("upper right"), _("lower left"), _("lower right")];
            r=x_choose(orientations,_("Select tip orientation"));
            if r<>0 then
                datatipSetOrientation(tip_handle,orient(r));
            end
        end
    elseif ibut==1004 then //Ctrl-D
        curve_handles=datatipGetEntities(ax);
        datatipDeleteSelected(curve_handles)
    end
    fig.event_handler_enable = "on";
endfunction

function [curve_index,tip_index]=datatipLookfor(curve_handles,pt)
    //looks for a datatip in the neighborhood of a given point
    //curve_handles:  a vector of curves which are supposed to have datatips
    //pt           :  a 2D point in pixels
    //curve_index  :  the index of corresponding curve in curve_handles
    //tip_index    :  the datatip index for the curve;
    for curve_index=1:size(curve_handles,"*")
        ud=datatipGetStruct(curve_handles(curve_index));
        if typeof(ud)=="datatips" then
            tips=ud.tips.children;
            for tip_index=1:size(tips,"*")
                data=tips(tip_index).children(1).data(1,:);
                if size(data,"*")==3 then
                    [xx,yy]=geom3d(data(1),data(2),data(3));
                    d=pixDist([xx,yy],pt)/2;
                else
                    d=pixDist(data,pt);
                end
                if d<10 then return, end
            end
        end
    end
    curve_index=[];
    tip_index=[];
endfunction

function [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,ax)
    //looks for the nearest entity from a given point
    //ax     : either a handle on a xes or a vector of handles on curves
    //curve  : the handle on the nearest entity
    //dmin   : euclidean distance in pixel from the given point to the
    //         nearest curve
    //ptmin  : the orthogonal projection of the point on the curve
    //l      : index of the segment containing the projection

    dmin=%inf;
    l=0;
    curve=[];
    ptmin=[];
    if argn(2)==1 then
        //only a point given look into the axes where the point lies
        ax=getAxes(pt);
    end
    if size(ax,"*")==0 then return,end
    if size(ax,"*")==1&ax.type=="Axes" then
        curves=datatipGetEntities(ax);
    else
        curves=ax;
        if or(curves.type<>"Polyline") then
            error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
            "datatipGetNearestEntity",2,"Polyline"))
        end
    end
    kmin=[]
    for k=1:size(curves,"*")
        ck=curves(k)
        ax=ck.parent;

        while ax.type<>"Axes" then ax=ax.parent,end
        if ck.type=="Polyline" then
            //find the polylines point with realizes the minimal euclidean
            //distance with pt
            if ax.view=="3d" then
                [xx,yy]=geom3d(ck.data(:,1),ck.data(:,2),ck.data(:,3))
                [d,ptp,ind,c]=orthProj([xx,yy],pt)

            else
                [d,ptp,ind]=orthProj(ck.data,pt)
            end
            if ind<>[] then
                d= pixDist(ptp,pt);
                if d<dmin then
                    dmin=d
                    l=ind
                    kmin=k,
                    if ax.view=="3d" then
                        ptmin=ck.data(l,:)+c*(ck.data(l+1,:)-ck.data(l,:))
                    else
                        ptmin=ptp
                    end
                end
            end
        elseif ck.type=="Plot3d" then
            [m,n]=size(ck.data.z);
            [xx,yy]=geom3d(ck.data.x*ones(1,n),ones(m,1)*ck.data.y',ck.data.z);
            [xx,yy]=xchange(xx,yy,"f2i");
            [xp,yp]=xchange(pt(1),pt(2),"f2i");
            [d,ind]=min(abs(xx-xp)+abs(yy-yp));
            if d<dmin then
                dmin=d
                l=ind
                kmin=k,
                ptmin=[ck.data.x(l(1)) ck.data.y(l(2))  ck.data.z(l(1),l(2))]
            end
        end
    end
    curve=curves(kmin)
endfunction

function datatipSelectFunction(curve)
    p=uigetfile("*.sci");
    if p=="" then return,end
    nold=size(who("get"),"*");
    if execstr("exec(p,-1);","errcatch")<>0 then
        messagebox([_("The selected file cannot be executed");lasterror()],"modal")
        return
    end
    new=who("get");
    new=new(1:$-nold-1);
    if size(new,"*")<>1 then
        messagebox(_("The selected file defines more than one function"))
        return
    end
    datatipSetDisplay(curve,evstr(new));
endfunction

function datatipSetReplaceMode(curve_handle,m)
    //changes the tips creation modes for a given curve
    // curve_handle : a handle on a polyline
    // m            : %t (interpolation active) or %f
    if argn(2)==1 then m=%f,end
    if type(curve_handle)<>9|or(curve_handle.type<>"Polyline") then
        error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
        "datatipCreate",1,"Polyline"))
    end

    ud=datatipGetStruct(curve_handle);
    if typeof(ud)<>"datatips" then;
        ud=datatipGetStruct(curve_handle);
    end
    ud.replace=m;
    datatipSetStruct(curve_handle,ud);
endfunction

function ax_handle=getAxes(pt,fig)
    //Returns the axes handle where a point given in pixel lies in the
    //current (or given) figure
    // pt        : the vector or coordinates in pixel in the figure
    // fig       : optional handle on the figure
    // ax_handle : selected axes handle
    if argn(2)==1 then fig=gcf(),end
    axes=get(fig,"children");
    axes(axes.type=="uimenu")=[];
    if size(axes,"*")==1 then ax_handle=axes;return,end
    sz=fig.axes_size;
    for k=1:size(axes,"*")
        if axes(k).type <> "Axes" then // uicontrol
            continue
        end
        ax_handle=axes(k);
        xbounds=ax_handle.axes_bounds(:,1)*sz(1);
        ybounds=ax_handle.axes_bounds(:,2)*sz(2);
        if prod(xbounds-pt(1))<0&prod(ybounds-pt(2))<0 then break,end
    end
endfunction


