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

function add_entity_menu(win, emen)
    if argn(2)<1 then
        win = get(gcf(),"figure_id")
        scf(win)
    end
    if argn(2)<2 then emen="Edit",end
    old = gcf()
    EntityMenus=["Browser","Copy","Paste","Move","Delete","Insert","Ok","Cancel"];
    //delmenu(win,'Entities')

    addmenu(win,emen,EntityMenus,list(2,"entity_menu"));
    //seteventhandler('entity_handler')
    scf(old)
endfunction



function entity_menu(k,win)
    old=gcf();fig=scf(win)
    seteventhandler("")
    select k
    case 1 then //browser
        ged(5,win)
    case 2 then //copy
        copy_entity()
    case 3 then //past
        paste_entity()
    case 4 then //move
        move_entity()
    case 5 then //Delete Entity
        delete_entity()
    case 6 then //New entity
        new_entity()
    case 7 then //ok
        fig.user_data="ok"
    case 8 then //cancel
        fig.user_data="cancel"
        //txt=gen_code()
        //mputl(txt,TMPDIR+'/foo.txt')
    end
    seteventhandler("ged_eventhandler")
    scf(old)
endfunction

function new_entity()
    entities=["Rectangle","Segment","Polyline","Text","Circle"]
    sel=x_choose(entities,"Select the Entity type")
    f=gcf();
    rep(3)=-1
    select sel
    case 1 then //Rectangle
        [btn,xc,yc]=xclick()
        xrect(xc,yc,0,0)
        r=gce();r.foreground=-1;
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data=[min(xc,rep(1)),max(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2))]
        end

    case 2 then //Segment
        [btn,xc,yc]=xclick()
        xsegs([xc;xc],[yc;yc])
        r=gce();r.foreground=-1;
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data=[xc,yc;rep(1),rep(2)]
        end

    case 3 then //Polyline
        [btn,xc,yc]=xclick()
        xpoly([xc;xc],[yc;yc])
        r=gce();r.foreground=-1;
        while %t
            while rep(3)==-1 do
                rep=xgetmouse()
                r.data($,:)=[rep(1),rep(2)]
            end
            if rep(3)==2 then break,end
            rep(3)=-1;
            r.data=[r.data;r.data($,:)]
        end
    case 4 then //Text
        text = x_dialog("Enter the new text here.","") ;
        if ( text <> [] & text <> "" ) then
            // do nothing if cancel button has been pressed or no text entered
            // get the position of the text
            xstring(0, 0, text ) ;
            r=gce();
            if r.type=="Text" then
                while rep(3)==-1 do
                    rep=xgetmouse()
                    r.data=[rep(1),rep(2)]
                end
            else //compound
                y0=0
                while rep(3)==-1 do
                    rep=xgetmouse()
                    y0=r.children($).data(2)
                    for k=1:size(r.children,"*")
                        rk=r.children(k)
                        rk.data=[rep(1),rk.data(2)+rep(2)-y0]
                    end
                    y0=rep(2)
                end
            end
        end
    case 5 then //Circle
        [btn,xc,yc]=xclick()
        xarc(xc,yc,0,0,0,64*360)
        r=gce();r.foreground=-1;
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data=[min(xc,rep(1)),max(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2)),0,360]
        end

    end
endfunction

function delete_entity()
    gedx=ged
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    h=ged_getobject([xc,yc])
    if h<>[] then delete(h),end
endfunction

function move_entity()
    gedx=ged
    [btn,xc,yc]=xclick()
    pos=[xc,yc]
    [xc,yc]=xchange(xc,yc,"f2i")
    r=ged_getobject([xc,yc])
    if r==[] return,end
    mprintf("debut move\n")
    rep(3)=-1
    select r.type
    case "Rectangle" then
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
        end
    case "Segs" then //Segment
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data=r.data+ones(r.data(:,1))*(rep(1:2)-pos)
            pos=rep(1:2)
        end
    case "Polyline" then //Polyline
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data=r.data+ones(r.data(:,1))*(rep(1:2)-pos)
            pos=rep(1:2)
        end
    case "Text" then //Text
        if r.parent.type=="Axes" then
            while rep(3)==-1 do
                rep=xgetmouse()
                r.data=r.data+(rep(1:2)-pos)
                pos=rep(1:2)
            end
        else
            r=r.parent
            while rep(3)==-1 do
                rep=xgetmouse()
                for k=1:size(r.children,"*")
                    rk=r.children(k)
                    rk.data=rk.data+(rep(1:2)-pos)
                end
                pos=rep(1:2)
            end
        end
    case "Arc" then //Circle
        while rep(3)==-1 do
            rep=xgetmouse()
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
        end

    end
    mprintf("fin move\n")
endfunction


function copy_entity()
    gedx=ged;
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    r=ged_getobject([xc,yc])
    if r==[] return,end
    twinkle(r,1);
    save(TMPDIR+"/G_Clipboard",r)
endfunction


function paste_entity()
    // check the file
    [info,err] = fileinfo(TMPDIR + "/G_Clipboard" ) ;

    if err <> 0 then
        return ;
    end

    // create the saved object
    load(TMPDIR+"/G_Clipboard") ;
endfunction

function txt=gen_code(win)
    old=gcf()
    if argn(2)>=1 then scf(win);end
    ax=gca()
    txt=[]
    for k=size(ax.children,"*"):-1:1
        txt=[txt;
        graphical_object_code(ax.children(k))]

    end
    if txt==[] then txt=" ",end
    scf(old)
endfunction

function txt=graphical_object_code(rk)
    txt=[]
    select rk.type
    case "Rectangle" then
        dx="orig(1)+sz(1)*"+num2string(rk.data(1))
        dy="orig(2)+sz(2)*"+num2string(rk.data(2))
        sx="sz(1)*"+num2string(rk.data(3))
        sy="sz(2)*"+num2string(rk.data(4))
        txt=[txt;
        "xrect("+strcat([dx,dy,sx,sy],",")+");"
        rect_opt(rk)]

    case "Arc" then
        dx="orig(1)+sz(1)*"+num2string(rk.data(1))
        dy="orig(2)+sz(2)*"+num2string(rk.data(2))
        sx="sz(1)*"+num2string(rk.data(3))
        sy="sz(2)*"+num2string(rk.data(4))
        a1=num2string(rk.data(5)*64)
        a2=num2string(rk.data(6)*64)

        txt=[txt;
        "xarc("+strcat([dx,dy,sx,sy,a1,a2],",")+");"
        arc_opt(rk)]

    case "Segs" then
        dx="orig(1)+sz(1)*["+strcat(num2string(rk.data(:,1)),";")+"]"
        dy="orig(2)+sz(2)*["+strcat(num2string(rk.data(:,2)),";")+"]"
        col=sci2exp(rk.segs_color,0)
        txt=[txt;
        "xsegs("+dx+","+dy+","+col+");"
        seg_opt(rk)]

    case "Polyline" then
        dx="orig(1)+sz(1)*["+strcat(num2string(rk.data(:,1)),";")+"]"
        dy="orig(2)+sz(2)*["+strcat(num2string(rk.data(:,2)),";")+"]"
        txt=[txt;
        "xpoly("+dx+","+dy+");"
        poly_opt(rk)]

    case "Text" then
        dx="orig(1)+sz(1)*"+num2string(rk.data(1))
        dy="orig(2)+sz(2)*"+num2string(rk.data(2))
        t=sci2exp(rk.text,0)
        t="["+strsubst(t,char(10),""";""")+"]"
        if rk.text_box_mode <> "filled" then
            txt=[txt;
            "xstring("+dx+","+dy+","+t+");"
            text_opt(rk)]
        else //patch because of xstringb patch in scicos
            w="sz(1)*"+num2string(rk.text_box(1))
            h="sz(2)*"+num2string(rk.text_box(2))

            txt=[txt;
            "xstringb("+dx+","+dy+","+t+","+w+","+h+",''fill'');"
            textb_opt(rk)]
        end
    case "Compound" then //multiple line string
        n=size(rk.children,"*")
        if n==1 then
            txt=[txt;
            graphical_object_code(rk.children)];
        else
            for i=n:-1:1
                txt=[txt;
                graphical_object_code(rk.children(i));
                "e"+string(i)+"=e;"]
            end
            txt=[txt;
            "glue(["+strcat("e"+string(1:n),",")+"]);"]
        end
    else
        mprintf("Graphical object of type "+rk.type+" not yet handled\n")
    end
endfunction

function opts=rect_opt(e)
    opts=[mark_opt(e);line_opt(e);fill_opt(e);clip_opt(e)];
    if opts<>[] then
        opts=["e=gce()";opts]
        opts=strcat(opts,";")+";"
    end
endfunction

function opts=arc_opt(e)
    opts=[line_opt(e);fill_opt(e);clip_opt(e)];
    if opts<>[] then
        opts=["e=gce()";opts]
        opts=strcat(opts,";")+";"
    end
endfunction

function opts=seg_opt(e)
    markopt=mark_opt(e)
    lineopt=line_opt(e)
    if rk.arrow_size>0 then
        arrowopt="e.arrow_size="+string(e.arrow_size)
    else
        arrowopt=[]
    end
    opts=[markopt;lineopt;arrowopt];
    if opts<>[] then
        opts=["e=gce()";opts]
        opts=strcat(opts,";")+";"
    end
endfunction

function opts=poly_opt(e)
    markopt=mark_opt(e)
    lineopt=line_opt(e)
    fillopt=fill_opt(e)
    arrowopt=arrow_opt(e)

    opts=[markopt;lineopt;fillopt;arrowopt];
    if opts<>[] then
        opts=["e=gce()";opts]
        opts=strcat(opts,";")+";"
    end
endfunction

function opts=text_opt(e)
    fontopt=font_opt(e)
    boxopt=box_opt(e)
    lineopt=[];
    if rk.fill_mode == "on" then
        lineopt="e.fill_mode="+sci2exp(e.fill_mode)
        if e.foreground<>-1 then
            lineopt=[lineopt
            "e.foreground="+sci2exp(e.foreground)]
        end
    end
    opts=[fontopt;boxopt;lineopt]
    if opts<>[] then
        opts=["e=gce()";opts]
        opts= strcat(opts,";")+";"
    end
endfunction

function opts=textb_opt(e)
    fontopt=font_opt(e)
    lineopt=[];
    if rk.fill_mode == "on" then
        lineopt="e.fill_mode="+sci2exp(e.fill_mode)
        if e.foreground<>-1 then
            lineopt=[lineopt
            "e.foreground="+sci2exp(e.foreground)]
        end
    end
    opts=[fontopt;lineopt]
    if opts<>[] then
        opts=["e=gce()";opts]
        opts= strcat(opts,";")+";"
    end
endfunction

function markopt=mark_opt(e)
    markopt=[]
    if e.mark_mode=="on" then
        markopt=[markopt;
        "e.mark_mode=''on''"]
        if e.mark_style<>0 then
            markopt=[markopt;
            "e.mark_style="+string(e.mark_style)]
        end
        if e.mark_size<>0 then
            markopt=[markopt;
            "e.mark_size="+string(e.mark_size)]
        end
        if e.mark_foreground<>-1 then
            markopt=[markopt;
            "e.mark_foreground="+string(e.mark_foreground)]
        end
        if e.mark_background<>-1 then
            markopt=[markopt;
            "e.mark_background="+string(e.mark_background)]
        end
    end
endfunction
function lineopt=line_opt(e)
    lineopt=[]
    t=e.type
    if e.line_mode=="on" then
        lineopt=[lineopt;
        "e.line_mode="+sci2exp(e.line_mode)]
        if e.line_style<>0 then
            lineopt=[lineopt;
            "e.line_style="+string(e.line_style)]
        end
        if e.thickness>1 then
            lineopt=[lineopt;
            "e.thickness="+string(e.thickness)]
        end
        if t<>"Segs"&e.foreground<>-1 then
            lineopt=[lineopt;
            "e.foreground="+string(e.foreground)]
        end
    end
endfunction

function fillopt=fill_opt(e)
    fillopt=[]
    if e.fill_mode=="on" then
        fillopt=[fillopt;
        "e.fill_mode="+sci2exp(e.fill_mode)]
        if e.background<>-2 then
            fillopt=[fillopt;
            "e.background="+string(e.background)]
        end
    end
endfunction
function arrowopt=arrow_opt(e)
    arrowopt=[]
    if e.polyline_style<>1 then
        arrowopt=[arrowopt;
        "e.polyline_style="+sci2exp(e.polyline_style)
        "e.arrow_size_factor="+string(e.arrow_size_factor)]
    end
endfunction
function fontopt=font_opt(e)
    fontopt=[]
    if e.font_foreground<>-1 then
        fontopt=[fontopt;
        "e.font_foreground="+string(e.font_foreground)]
    end
    if e.font_style<>6 then
        fontopt=[fontopt;
        "e.font_style="+string(e.font_style)]
    end
    if e.font_size<>1 then
        fontopt=[fontopt;
        "e.font_size="+string(e.font_size)]
    end
    if e.font_angle<>0 then
        fontopt=[fontopt;
        "e.font_angle="+string(e.font_angle)]
    end
endfunction

function boxopt=box_opt(e)
    boxopt=[]
    if e.text_box_mode<>"off" then
        boxopt=[boxopt;
        "e.text_box_mode="+sci2exp(e.text_box_mode)]
    end
    if or(e.text_box<>[0 0]) then
        boxopt=[boxopt;
        "e.text_box="+sci2exp(e.text_box)]
    end
    if e.box<>"off" then
        boxopt=[boxopt;
        "e.box="+sci2exp(e.box)]
    end
    if e.background<>-2 then
        boxopt=[boxopt;
        "e.background="+sci2exp(e.background)]
    end
    if e.fill_mode<>"off" then
        boxopt=[boxopt;
        "e.fill_mode="+sci2exp(e.fill_mode)];

    end
endfunction

function clipopt=clip_opt(e)
    clipopt=[]
    if e.clip_state<>"off" then
        clipopt=[clipopt;
        "e.clip_state="+sci2exp(e.clip_state)
        "e.clip_box="+sci2exp(e.clip_box)]
    end
endfunction


function s=num2string(a)
    s=matrix(msprintf("%.2f\n",a(:)),size(a))
    s=strsubst(s,".00","")
endfunction


