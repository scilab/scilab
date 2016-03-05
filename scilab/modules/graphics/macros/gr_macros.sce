// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=Delete(sd)
    //destruction d'un objet
    xx=locate(1);eps=0.2
    mm=clearmode();
    //recherche de l'objet contenant le point
    for ko=2:ksd;
        obj=sd(ko);
        to="rien";if size(obj)<>0 then to=obj(1);end,
        select to
        case "ligne" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    sd(ko)=ligne(obj,"del");
                end,
            end,
        case "fligne" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    sd(ko)=fligne(obj,"del");
                end,
            end,
        case "rect" then
            x1=obj(2);x2=obj(3);y1=obj(4);y2=obj(5);
            z=[x1,x1,x2,x2,x1 ; y1,y2,y2,y1,y1];
            [nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    sd(ko)=rect(obj,"del");
                end,
            end,
        case "frect" then
            x1=obj(2);x2=obj(3);y1=obj(4);y2=obj(5);
            z=[x1,x1,x2,x2,x1 ; y1,y2,y2,y1,y1];
            [nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    sd(ko)=frect(obj,"del");
                end,
            end,
        case "points" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    sd(ko)=points(obj,"del");
                end,
            end,
        case "cercle" then
            dist=norm(obj(2)-xx,2);
            if abs(dist-obj(3))<eps then sd(ko)=cerc(obj,"del");end,
        case "fcercle" then
            dist=norm(obj(2)-xx,2);
            if abs(dist-obj(3))<eps then sd(ko)=fcerc(obj,"del");end,
        case "fleche" then
            o1=obj(2);o2=obj(3);p1=[o1(1);o2(1)];p2=[o1(2);o2(2)];
            e=norm(xx-p1,2)+norm(xx-p2,2)
            if abs(e-norm(p2-p1))< eps then sd(ko)=fleche(obj,"del");end,
        case "comm" then
            xxr=xstringl(0,0,obj(3))
            hx=xxr(3);
            hy=xxr(4);
            crit=norm(obj(2)-xx)+norm(obj(2)+[hx;hy]-xx)
            if crit<hx+hy then sd(ko)=comment(obj,"del");end
        end, //fin selec to
    end; //fin for ko ...
    modeback(mm);
    sd=resume(sd)
endfunction

function sd=Move(sd)
    //deplacement d'un objet
    // Copyright INRIA
    xx=locate(1);eps=0.03
    //mm=clearmode();
    //recherche de l'objet contenant le point
    for ko=2:ksd;
        obj=sd(ko);
        to="rien";if size(obj)<>0 then to=obj(1);end,
        select to
        case "ligne" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    new=ligne(obj,"mov");
                    if new<>list() then
                        sd(ko)=new
                    end
                    return;
                end,
            end,
        case "fligne" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    new=fligne(obj,"mov");
                    if new<>list() then
                        sd(ko)=new
                    end
                    return;
                end,
            end,
        case "rect" then
            x1=obj(2);x2=obj(3);y1=obj(4);y2=obj(5);
            z=[x1,x1,x2,x2,x1 ; y1,y2,y2,y1,y1];
            [nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    new=rect(obj,"mov");
                    if new<>list() then
                        sd(ko)=new
                    end
                    return;
                end,
            end,
        case "frect" then
            x1=obj(2);x2=obj(3);y1=obj(4);y2=obj(5);
            z=[x1,x1,x2,x2,x1 ; y1,y2,y2,y1,y1];
            [nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    new=frect(obj,"mov");
                    if new<>list() then
                        sd(ko)=new
                    end
                    return;
                end,
            end,
        case "points" then
            z=obj(2),[nw,npt]=size(z),
            for kpt=2:npt
                e=norm(xx-z(:,kpt),2)+norm(xx-z(:,kpt-1),2)
                if abs(e-norm(z(:,kpt)-z(:,kpt-1),2))< eps then
                    new=points(obj,"mov");
                    if new<>list() then
                        sd(ko)=new
                    end
                    return;
                end,
            end,
        case "cercle" then
            dist=norm(obj(2)-xx,2);
            if abs(dist-obj(3))<eps then
                new=cerc(obj,"mov");
                if new<>list() then
                    sd(ko)=new
                end
                return;
            end
        case "fcercle" then
            dist=norm(obj(2)-xx,2);
            if abs(dist-obj(3))<eps then
                new=fcerc(obj,"mov");
                if new<>list() then
                    sd(ko)=new
                end
                return;
            end
        case "fleche" then
            o1=obj(2);o2=obj(3);p1=[o1(1);o2(1)];p2=[o1(2);o2(2)];
            e=norm(xx-p1,2)+norm(xx-p2,2)
            if abs(e-norm(p2-p1))< eps then
                new=fleche(obj,"mov");
                if new<>list() then
                    sd(ko)=new
                end
                return;
            end
        case "comm" then
            xxr=xstringl(0,0,obj(3))
            hx=xxr(3);
            hy=xxr(4);
            crit=norm(obj(2)-xx)+norm(obj(2)+[hx;hy]-xx)
            if crit<hx+hy then
                new=comment(obj,"mov");
                if new<>list() then
                    sd(ko)=new
                end
                return;
            end
        end, //fin selec to
    end; //fin for ko ...
endfunction

function [sd1]=symbs(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then
        c=xget("mark")
        n1=c(1);dime=c(2)
        sd1=list("symbs",c(1),c(2));
    else
        n1=sd(2);dime=sd(3)
    end
    xset("mark",n1,dime);
endfunction

function [sd1]=dashs(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then
        n1=x_choose(dash,"Choose a dash style");
        if n1==[] then
            sd1=list()
        else
            sd1=list("dashs",n1);
        end
    else
        n1=sd(2)
    end
    xset("dashes",n1);
endfunction

function [sd1]=patts(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then
        n1=getcolor("Choose a pattern ",0)
        if n1==[] then
            sd1=list()
        else
            sd1=list("patts",n1);
        end
    else

        n1=sd(2)
    end
    xset("pattern",n1);
endfunction

function [sd1]=Thick(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then

        T=string(1:15)
        ll=list()
        t=xget("thickness")
        ll(1)=list("Thickness",t,T);
        n1=x_choices("Choose a Thickness",ll);
        if n1==[] then
            sd1=list()
        else
            sd1=list("thick",n1);
        end
    else
        n1=sd(2)
    end
    xset("thickness",n1);
endfunction

function sd1 =rect(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then //get
        [x1,y1,x2,y2,but]=xgetm(d_xrect)
        if but==2 then sd1=list();return,end
        sd1=list("rect",x1,x2,y1,y2);
        d_xrect(x1,y1,x2,y2);
    elseif rhs==1 then //draw
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5);
        d_xrect(x1,y1,x2,y2);
    elseif del=="del" then //erase
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5);
        d_xrect(x1,y1,x2,y2);
    elseif del=="mov" then //move
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5)
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("d_xrect(x1-(x0-xo),y1-(y0-yo),x2-(x0-xo),y2-(y0-yo))",x0,y0);
        sd1=sd;
        sd1(2)=sd(2)-(x0-xo)
        sd1(3)=sd(3)-(x0-xo)
        sd1(4)=sd(4)-(y0-yo)
        sd1(5)=sd(5)-(y0-yo)
    end
endfunction

function sd1=frect(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then // get
        [x1,y1,x2,y2,but]=xgetm(d_xrect)
        if but==2 then sd1=list();return,end
        sd1=list("frect",x1,x2,y1,y2);
        d_xfrect(x1,y1,x2,y2);
    elseif rhs==1 then //draw
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5)
        d_xfrect(x1,y1,x2,y2);
    elseif del=="del" then //erase
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5)
        d_xfrect(x1,y1,x2,y2);
    elseif del=="mov" then //move
        x1=sd(2);x2=sd(3),y1=sd(4),y2=sd(5)
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("d_xfrect(x1-(x0-xo),y1-(y0-yo),x2-(x0-xo),y2-(y0-yo))",x0,y0);
        sd1=sd
        sd1(2)=sd(2)-(x0-xo)
        sd1(3)=sd(3)-(x0-xo)
        sd1(4)=sd(4)-(y0-yo)
        sd1(5)=sd(5)-(y0-yo)
    end
endfunction

function sd1=cerc(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then // get
        [c1,c2,x1,x2,but]=xgetm(d_circle);
        if but==2 then sd1=list();return,end
        x=[x1;x2],c=[c1;c2];r=norm(x-c,2);
        sd1=list("cercle",c,r);
        d_circle(c,r);
    elseif rhs==1 then //draw
        c=sd(2);r=sd(3);
        d_circle(c,r);
    elseif del=="del" then //erase
        c=sd(2);r=sd(3);
        d_circle(c,r);
    elseif del=="mov" then //move
        c=sd(2);r=sd(3)
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("d_circle(c-[x0-xo;y0-yo],r)",x0,y0);
        sd(2)=sd(2)-[x0-xo;y0-yo]
        sd1=sd
    end;
endfunction

function sd1=fcerc(sd,del)
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then // get
        [c1,c2,x1,x2,but]=xgetm(d_circle);
        if but==2 then sd1=list();return,end
        x=[x1;x2],c=[c1;c2];r=norm(x-c,2);
        sd1=list("fcercle",c,r);
        d_fcircle(c,r);
    elseif rhs==1 then //draw
        c=sd(2);r=sd(3)
        d_fcircle(c,r);
    elseif del=="del" then //erase
        c=sd(2);r=sd(3)
        d_fcircle(c,r);
    elseif del=="mov" then //move
        c=sd(2);r=sd(3)
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("d_fcircle(c-[x0-xo;y0-yo],r)",x0,y0);
        sd(2)=sd(2)-[x0-xo;y0-yo]
        sd1=sd
    end;
endfunction

function [sd1]=fleche(sd,del)
    [lhs,rhs]=argn(0);sd1=[]
    if rhs<=0 then // get
        [oi1,oi2,of1,of2,but]=xgetm(d_arrow);
        if but==2 then sd1=list();return,end
        o1=[oi1;of1],o2=[oi2;of2];
        [r1,r2]=xgetech()
        sz=1/(40*min(abs(r2(3)-r2(1)),abs(r2(4)-r2(2))))
        sd1=list("fleche",o1,o2,sz);
        d_arrow(o1,o2,sz);
    elseif rhs==1 then //draw
        o1=sd(2),o2=sd(3),
        sz=-1
        if size(sd)>=4 then sz=sd(4),end
        d_arrow(o1,o2,sz);
    elseif del=="del" then //erase
        o1=sd(2),o2=sd(3),
        sz=-1
        if size(sd)>=4 then sz=sd(4),end
        d_arrow(o1,o2,sz);
    elseif del=="mov" then //move
        o1=sd(2),o2=sd(3),
        sz=-1
        if size(sd)>=4 then sz=sd(4),end
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("d_arrow(o1-(x0-xo),o2-(y0-yo),sz)",x0,y0);
        sd(2)=sd(2)-(x0-xo)
        sd(3)=sd(3)-(y0-yo)
        sd1=sd
    end
endfunction

function [sd1]=comment(sd,del)
    [lhs,rhs]=argn(0),sd1=[];
    if rhs<=0 then // get
        [i,z1,z2]=xclick(0);z=[z1;z2];
        com=x_dialog("Enter string"," ");
        if com<>[] then
            sd1=list("comm",z,com),
            xstring(z(1),z(2),com,0,0);
        end
    elseif rhs==1 then //draw
        z=sd(2);com=sd(3);
        xstring(z(1),z(2),com,0,0);
    elseif del=="del" then //erase
        z=sd(2);com=sd(3);
        xstring(z(1),z(2),com,0,0);
    elseif del=="mov" then //move
        z=sd(2);com=sd(3);
        [xo,yo]=move_object("xstring(xo,yo,com,0,0)",z(1),z(2));
        sd1=sd;sd1(2)(1)=xo;sd1(2)(2)=yo;
    end;
endfunction

function [sd1]=ligne(sd,del)
    // polyline
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then // get
        z=xgetpoly(d_seg);
        if z==[], return;end;
        sd1=list("ligne",z);
        xpoly(z(1,:)',z(2,:)',"lines")
    elseif rhs==1 then //draw
        z=sd(2);
        xpoly(z(1,:)',z(2,:)',"lines")
    elseif del=="del" then //erase
        z=sd(2);
        xpoly(z(1,:)',z(2,:)',"lines")
    elseif del=="mov" then //move
        z=sd(2);
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("xpoly(z(1,:)''-(x0-xo),z(2,:)''-(y0-yo),""lines"")",x0,y0);
        sd(2)=[z(1,:)-(x0-xo);z(2,:)-(y0-yo)]
        sd1=sd
    end;
endfunction

function [sd1]=fligne(sd,del)
    // filled polyline
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then // get
        z=xgetpoly(d_seg);
        if z==[], return;end;
        sd1=list("fligne",z);
        xfpoly(z(1,:),z(2,:),1);
    elseif rhs==1 then //draw
        z=sd(2);
        xfpoly(z(1,:),z(2,:),1);
    elseif del=="del" then //erase
        z=sd(2);
        xfpoly(z(1,:),z(2,:),1)
    elseif del=="mov" then //move
        z=sd(2);
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("xfpoly(z(1,:)-(x0-xo),z(2,:)-(y0-yo),1)",x0,y0);
        sd(2)=[z(1,:)-(x0-xo);z(2,:)-(y0-yo)]
        sd1=sd
    end;
endfunction

function [sd1]=curve(sd,del)
    // smoothed curve
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then ,//get
        z=xgetpoly(d_seg);
        if z==[], return;end
        mm=clearmode();xpoly(z(1,:)',z(2,:)',"lines");modeback(mm)
        [x1,k1]=gsort(z(1,:));y1=z(2,k1);z=[x1;y1];
        [n1,n2]=size(z);z=smooth(z(:,n2:-1:1));
        sd1=list("ligne",z);
    else
        z=sd(2);
    end;
    xpoly(z(1,:)',z(2,:)',"lines");

endfunction

function [sd1]=points(sd,del)
    // polymark
    [lhs,rhs]=argn(0);sd1=[];
    if rhs<=0 then //get
        z=xgetpoly(d_point);
        if z==[], return;end;
        sd1=list("point",z);
        xpoly(z(1,:)',z(2,:)',"marks");
    elseif rhs==1 then //draw
        z=sd(2);
        xpoly(z(1,:)',z(2,:)',"marks");
    elseif del=="del" then //erase
        z=sd(2);
        xpoly(z(1,:)',z(2,:)',"marks");
    elseif del=="mov" then //move
        z=sd(2);
        x0=xx(1);y0=xx(2);
        [xo,yo]=move_object("xpoly(z(1,:)''-(x0-xo),z(2,:)''-(y0-yo),""marks"")",x0,y0);
        sd(2)=[z(1,:)-(x0-xo);z(2,:)-(y0-yo)]
        sd1=sd
    end;
endfunction

function [sd1]=grclipoff(sd,del)
    [lhs,rhs]=argn(0),sd1=[];
    if rhs<=0 then ,
        sd1=list("clipoff")
    end;
    // disable clipping
    axes = gca();
    axes.clip_state = "off";
endfunction

function [sd1]=grclipon(sd,del)
    [lhs,rhs]=argn(0),sd1=[];
    if rhs<=0 then ,
        sd1=list("clipon")
    end;
    // axes clip
    axes = gca();
    axes.clip_state = "clipgrf";
endfunction

function []=redraw(sd,s_t)
    ksd=size(sd)
    plot2d(0,0,[-1],s_t," ",sd(2));
    xset("clipgrf");
    for k=3:ksd,
        obj=sd(k);
        if size(obj)<>0 then
            to=obj(1)
            select to,
            case "rect"    then rect(obj);
            case "frect"   then frect(obj);
            case "cercle"  then cerc(obj);
            case "fcercle" then fcerc(obj);
            case "fleche"  then fleche(obj);
            case "comm"    then comment(obj);
            case "ligne"   then ligne(obj);
            case "fligne"  then fligne(obj);
            case "point"   then points(obj);
            case "symbs"   then symbs(obj);
            case "dashs"   then dashs(obj);
            case "patts"   then patts(obj);
            case "clipon"  then grclipon(obj);
            case "clipoff" then grclipoff(obj);
            end
        end
    end
endfunction

function [x0,y0,x,y,ibutton]=xgetm(m_m)
    // Object aquisition
    kpd=driver();
    driver("X11");
    alu=xget("alufunction")
    xset("alufunction",6);
    // attente du click
    [ii,x0,y0]=xclick()
    x=x0;y=y0;
    // suivit de la souris en attendant le button release
    ibutton=-1
    while ( ibutton<0)
        // dessin
        m_m(x0,y0,x,y);
        rep=xgetmouse(0);
        ibutton = rep(3)
        m_m(x0,y0,x,y)
        x=rep(1);y=rep(2);
    end
    xset("alufunction",alu);
    //m_m(x0,y0,x,y)
    driver(kpd);
endfunction

function []=d_xrect(x0,yy0,x,y)
    xi=min(x0,x);
    w=abs(x0-x);
    yi=max(yy0,y);
    h=abs(yy0-y);
    xrect(xi,yi,w,h);
endfunction

function []=d_xfrect(x0,yy0,x,y)
    xi=min(x0,x);
    w=abs(x0-x);
    yi=max(yy0,y);
    h=abs(yy0-y);
    xrects([xi,yi,w,h]',xget("pattern"));
endfunction

function []=d_circle(c1,c2,x1,x2)
    [lhs,rhs]=argn(0);
    if rhs==2 then
        r=c2;c2=c1(2);c1=c1(1);
        xarc(c1-r,c2+r,2*r,2*r,0,64*360);

    else
        r=norm([x1-c1;x2-c2],2);
        xarc(c1-r,c2+r,2*r,2*r,0,64*360);
    end
endfunction

function []=d_fcircle(c1,c2,x1,x2)
    [lhs,rhs]=argn(0);
    if rhs==2 then r=c2;c2=c1(2);c1=c1(1);
    else
        r=norm([x1-c1;x2-c2],2);
    end
    xfarc(c1-r,c2+r,2*r,2*r,0,64*360);
endfunction

function d_arrow(c1,c2,x1,x2)
    [lhs,rhs]=argn(0);
    if rhs<>4 then
        sz=x1;x1=c1(2);c1=c1(1);x2=c2(2);c2=c2(1);
    else
        [r1,r2]=xgetech()
        sz=1/(40*min(abs(r2(3)-r2(1)),abs(r2(4)-r2(2))))
    end
    xarrows([c1;x1],[c2;x2],sz,-1);
endfunction

function [z]=xgetpoly(m_m)
    // interactive polyline aquisition m_m is
    // used to draw between aquisitions
    kpd=driver();
    //driver("X11");
    // attente du click
    [ii,x0,y0]=xclick(0)
    x=x0;y=y0;
    z=[x0;y0];
    ibutton=1
    alu=xget("alufunction")
    xset("alufunction",6);
    while and(ibutton<>[0 3 10])
        ibutton=-1
        while ibutton==-1
            // dessin
            m_m(x0,y0,x,y);
            rep=xgetmouse(0);
            ibutton = rep(3)
            m_m(x0,y0,x,y)
            x=rep(1);y=rep(2);
        end
        if and(ibutton<>[0 3 10])  then
            m_m(x0,y0,x,y)
            z=[z,[x;y]]
            x0=x;y0=y;
        end
    end
    xset("alufunction",alu);

    [nn,ll]=size(z);
    if ll==1 then z=[];end
    driver(kpd);
endfunction

function []=d_seg(x1,y1,x2,y2)
    xpoly([x1,x2],[y1,y2],"lines");
endfunction

function []=d_point(x1,y1,x2,y2)
    xpoly([x1,x2],[y1,y2],"marks");

endfunction

function [xo,yo]=move_object(inst,xo,yo)
    // Object aquisition
    xos=xo;yos=yo
    kpd=driver();
    alu=xget("alufunction")
    xset("alufunction",6);
    execstr(inst) //erase
    driver("X11");
    // suivi de la souris en attendant le button release
    rep=[0 0 -1];
    while rep(3)<0
        execstr(inst) //draw
        rep=xgetmouse(0);
        execstr(inst) //erase
        xo=rep(1);yo=rep(2);
    end
    if or(rep(3)==[2 5 12]) then
        xo=xos;yo=yos, //move canceled
    else
        xo=rep(1);yo=rep(2);
    end
    xset("alufunction",alu);
    driver(kpd);
    execstr(inst) //draw
endfunction
