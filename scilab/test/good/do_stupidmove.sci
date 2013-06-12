function [scs_m] = do_stupidmove(%pt,scs_m)
    // Copyright INRIA
    // get a scicos object to move, and move it with connected objects
    //!
    //**
    //** 28 Jun 2006 : restart :(
    //**
    //**
    // get block to move
    rela = .1   ;
    win  = %win ;
    xc =%pt(1); yc = %pt(2) ; //** recover mouse position at the last event

    //** look for a valid object
    [k,wh,scs_m] = stupid_getobj(scs_m,[xc;yc]) ;

    //** "k" is the object index in the data structure "scs_m"
    if k==[] then return , end ;//** if NO object found -> exit

    scs_m_save = scs_m ; //** make a backup of the data structure

    //**-------------------------------------------------------------------
    if typeof(scs_m.objs(k))=="Block"| typeof(scs_m.objs(k))=="Text" then
        //**-------------------- Block --------------------------------------
        needreplay = replayifnecessary()         ;
        scs_m = stupid_moveblock(scs_m,k,xc,yc)  ;

    elseif typeof(scs_m.objs(k))=="Link" then
        //**------------------- Link ------------------------------
        scs_m=stupid_movecorner(scs_m,k,xc,yc,wh)  ;

    elseif typeof(scs_m.objs(k))=="Text" then
        //**------------------- Text ------------------------------
        //**.... do nothing ...
    end
    //**------------------------------------------------------------------

    if Cmenu=="Quit" then
        //active window has been closed
        [%win,Cmenu]=resume(%win,Cmenu)
    end

    [scs_m_save,enable_undo,edited,nc_save,needreplay] = resume(scs_m_save,%t,%t,needcompile,needreplay)
endfunction
//**-----------------------------------------------------------------------------------------------------------
//**
//*************************************************************************************************************
//
//       ---------------------------- Move Blocks and connected Link(s) ---------------------------------------
//
// Move  block k and modify connected links if any
// look at connected links
//
function scs_m = stupid_moveblock(scs_m,k,xc,yc)
    // Move  block k and modify connected links if any
    // look at connected links

    //**----------------------------------------------------------------------------------
    //** the code below is modified according the new graphics API
    gh_curwin = gh_current_window ;
    gh_curwin.pixmap ="on"; //set the pixmap mode

    //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
    //** I need an equivalent index for the graphics

    o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object

    //** "k" is the object index in the data structure "scs_m"
    //** compute the equivalent "gh_k" for the graphics datastructure
    gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
    disp (gh_k);
    gh_blk = gh_curwin.children.children(gh_k);

    //**-----------------------------------------------------------------------------------

    o = scs_m.objs(k) ;

    xx=[];yy=[];ii=[];clr=[];mx=[];my=[]

    // build movable segments for all connected links
    //===============================================
    xm = []; ym = []; jj=0 ; //** init ...

    gh_link_i = []; gh_link_mod = [];

    connected = unique(get_connected(scs_m,k))//** look for connected link(s)

    //** ----------------------------------------------------------------------

    //** look for all the connected link(s) and build "impiling" the two data structures
    //** [xm , ym] for the links data points
    //** gh_link_i fro the associated graphic handles
    for l=1:length(connected)

        i  = connected(l) ;

        oi = scs_m.objs(i) ;

        gh_i = o_size(1) - i + 1 ; //** calc the handler of all the connected link(s)

        gh_link_i = [ gh_link_i gh_curwin.children.children(gh_i) ]; //**

        [xl , yl,ct,from,to] = (oi.xx,oi.yy,oi.ct,oi.from,oi.to)

        nl = prod(size(xl)) ;

        if from(1)==k then
            xm = [xm,[xl(2);xl(1)]];
            ym = [ym,[yl(2);yl(1)]];
            // draw_link_seg(oi,1:2) //erase link
            // clr=[clr ct(1)]
        end

        if to(1)==k then
            xm = [xm,xl($-1:$)];
            ym = [ym,yl($-1:$)];
            // draw_link_seg(oi,$-1:$) //erase link
            // clr=[clr ct(1)]
        end

    end //** end of the for() loop
    //** ----------------------------------------------------------------------


    xmt = xm ; ymt = ym ; //** init ...

    // move a block and connected links
    //=================================


    if size(connected,2)>0 then // move a block and connected links

        //** ------------- MOVE BLOCK AND CONNECTED LINKS LINKS -----------------
        disp("do_stupidmove.sci : Move block WITH connected links ");

        [xmin,ymin] = getorigin(o) ;

        xco = xc; yco = yc;

        [xy,sz] = (o.graphics.orig,o.graphics.sz)

        dx = xc - xmin ; dy = yc - ymin ;

        //**-------------------------------------------------------------------
        //** rep(3) = -1 ;
        gh_link_mod = [] ;
        tmp_data = [] ;
        t_xmt = [] ; t_ymt  = [];
        while 1 do //** interactive move loop

            rep = xgetmouse(0,[%t,%t]);

            if or(rep(3)==[0,2,3,5,-5,-100]) then
                break ; //** ---> EXIT point of the while
            end

            //** rep = xgetmouse(0,[%t,%t]);

            if xget("window")<>curwin|rep(3)==-100 then
                [%win,Cmenu]=resume(curwin,"Quit")
            end

            delta_x = rep(1) - xc ; delta_y = rep(2) - yc ; //** calc the differential position ...

            drawlater();

            move (gh_blk , [delta_x , delta_y]);  //** ..because "move()" works only in differential

            xc = rep(1); yc = rep(2)

            xmt(2,:) = xm(2,:) - xco + xc ; //** update datas of links
            ymt(2,:) = ym(2,:) - yco + yc ;

            j = 0 ; //** init
            //**---------------------------------------
            for l=1:length(connected) // ... for all the connected links
                i  = connected(l)  ; // from the progressive index "l" to the scs_m index "i"
                oi = scs_m.objs(i) ; // get the "i"th link
                [xl,from,to] = (oi.xx,oi.from,oi.to); // extract the proprieties from the link

                gh_link_mod = gh_link_i(l) ; // get the link graphics data structure

                if from(1)==k then
                    tmp_data = gh_link_mod.children.data ; // extract the vectors that define the link
                    // the first two points
                    j = j + 1 ; // update the [x,y]mt pointer
                    t_xmt = xmt([2,1],j) ;  t_ymt = ymt([2,1],j) ; // estract the element
                    //** update the graphics datastructure
                    gh_link_mod.children.data = [ [t_xmt(1) , t_ymt(1)] ; tmp_data(2:$ , 1:$) ]  ;

                end
                //** see the obove comments :)
                if to(1)==k then
                    tmp_data = gh_link_mod.children.data ;
                    // the last two points
                    j = j + 1 ;
                    gh_link_mod.children.data = [ tmp_data(1:$-2 , 1:$) ; [xmt(:,j) , ymt(:,j)] ]  ;
                end

            end // ... of for() link loop

            drawnow();
            show_pixmap();

        end //** ...  of while
        //**-------------------------------------------------------------------

        if xget("window")<>curwin|rep(3)==-100 then
            [%win,Cmenu]=resume(curwin,"Quit")
        end

        xy = [xc-dx,yc-dy];

        // update and draw block

        o.graphics.orig = xy;
        scs_m.objs(k) = o; //update block coordinates

        j = 0 ; //** init
        //**---------------------------------------
        for l=1:length(connected)
            i  = connected(l)  ;
            oi = scs_m.objs(i) ;
            [xl,from,to] = (oi.xx,oi.from,oi.to);

            if from(1)==k then
                j = j + 1 ;
                oi.xx(1:2) = xmt([2,1],j)
                oi.yy(1:2) = ymt([2,1],j)
                // draw_link_seg(oi,1:2) //draw link
            end

            if to(1)==k then
                j = j+1
                oi.xx($-1:$) = xmt(:,j)
                oi.yy($-1:$) = ymt(:,j)
                // draw_link_seg(oi,$-1:$) //draw link
            end

            scs_m.objs(i) = oi ;

        end //... for loop
        //**----------------------------------------

        //** ---------------------------- MOVE BLOCK W/O LINKS ------------------------
    else // move an unconnected block
        disp("Move block without links");

        [xy,sz] = (o.graphics.orig,o.graphics.sz)

        //**----------------------------------------------------------------------
        %xc = xy(1); %yc = xy(2); //** default start position
        while 1 do

            rep = xgetmouse(0,[%t,%t]); // get new position

            if or(rep(3)==[0, 2, 3, 5, -5, -100]) then break ; end //** ---> EXIT point of the while

            xc = rep(1) ; yc = rep(2) ;

            dx = xc - %xc ; dy = yc - %yc ;

            drawlater();

            move (gh_blk , [dx dy]);

            drawnow();
            show_pixmap();

            %xc = xc ; %yc = yc ; //** block position absolute position update

        end
        //**---------------------------------------------------------------------

        if xget("window")<>curwin|rep(3)==-100 then
            //   active window has been closed
            [%win,Cmenu]=resume(curwin,"Quit")
        end

        // update and draw block
        if and(rep(3)<>[2 5]) then
            // update and draw block
            xy = [%xc, %yc] ; //** compute the new position
            o.graphics.orig = xy ; //** update the local data strucure
            scs_m.objs(k) = o    ; //** update the global data structure
        end

    end // end of Move Block with / without Link(s)

endfunction
//**--------------------------------------------------------------------------

function scs_m = stupid_movecorner(scs_m,k,xc,yc,wh)

    o = scs_m.objs(k) ;

    [xx,yy,ct] = (o.xx,o.yy,o.ct) ;

    //** dr=driver()

    seg = [-wh-1:-wh+1] ;

    //** if dr=='Rec' then driver('X11'),end

    xpolys(xx(seg),yy(seg),ct(1)) //draw thin link
    //** if pixmap then xset('wshow'),end

    X1 = xx(seg)
    Y1 = yy(seg)
    x1 = X1 ; y1 = Y1;

    xpolys(x1,y1,ct(1)) //erase moving part of the link
    rep(3)=-1

    while 1 do

        if or(rep(3)==[0,2,3,5,-5,-100]) then break,end

        xpolys(x1,y1,ct(1))//draw moving part of the link

        rep=xgetmouse(0,[%t,%t]);

        if xget("window")<>curwin|rep(3)==-100 then
            //active window has been closed
            // driver(dr);
            [%win,Cmenu]=resume(curwin,"Quit")
        end

        //** if pixmap then xset('wshow'),end

        xpolys(x1,y1,ct(1)) //erase moving part of the link
        xc1 = rep(1); yc1 = rep(2)
        x1(2)=X1(2)-(xc-xc1)
        y1(2)=Y1(2)-(yc-yc1)
    end //** of the while

    if xget("window")<>curwin|rep(3)==-100 then
        //active window has been closed
        //** driver(dr);
        [%win,Cmenu]=resume(curwin,"Quit")
    end

    if and(rep(3)<>[2 5]) then
        if abs(x1(1)-x1(2))<rela*abs(y1(1)-y1(2)) then
            x1(2)=x1(1)
        elseif abs(x1(2)-x1(3))<rela*abs(y1(2)-y1(3)) then
            x1(2)=x1(3)
        end
        if abs(y1(1)-y1(2))<rela*abs(x1(1)-x1(2)) then
            y1(2)=y1(1)
        elseif abs(y1(2)-y1(3))<rela*abs(x1(2)-x1(3)) then
            y1(2)=y1(3)
        end
        d=projaff([x1(1);x1(3)],[y1(1);y1(3)],[x1(2);y1(2)])
        if norm(d(:)-[x1(2);y1(2)])<..
            rela*max(norm(d(:)-[x1(3);y1(3)]),norm(d(:)-[x1(1);y1(1)])) then
            xx(seg)=x1
            yy(seg)=y1
            xx(seg(2))=[]
            yy(seg(2))=[]
            x1(2)=[];y1(2)=[];seg(3)=[]
        else
            xx(seg)=x1
            yy(seg)=y1
        end
        o.xx=xx;o.yy=yy
        scs_m.objs(k)=o
    end
    //** driver(dr)
    draw_link_seg(o,seg)
    //** if pixmap then xset('wshow'),end
endfunction

//**-----------------------------------------------------------------------------------------------

function [k,wh,scs_m] = stupid_getobj(scs_m,pt)
    n=lstsize(scs_m.objs)
    wh=[];
    x=pt(1);y=pt(2)
    data=[]
    k=[]
    for i=1:n //loop on objects
        o=scs_m.objs(i)
        if typeof(o)=="Block" then
            graphics=o.graphics
            [orig,sz]=(graphics.orig,graphics.sz)
            data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
            if data(1)<0&data(2)<0 then k=i,break,end
        elseif typeof(o)=="Link" then
            [frect1,frect]=xgetech();
            eps=3
            xx=o.xx;yy=o.yy;
            [d,ptp,ind]=stupid_dist2polyline(xx,yy,pt,.85)
            if d<eps then
                if ind==-1 then
                    k=o.from(1),break,
                elseif ind==-size(xx,1) then
                    k=o.to(1),break,
                elseif ind>0 then
                    draw_link_seg(o,[ind,ind+1])
                    o.xx=[xx(1:ind);ptp(1);xx(ind+1:$)];
                    o.yy=[yy(1:ind);ptp(2);yy(ind+1:$)];
                    scs_m.objs(i)=o
                    k=i,wh=-ind-1,break,
                else k=i,wh=ind,draw_link_seg(o,[-ind-1:-ind+1]);break,end
            end
        elseif typeof(o)=="Text" then
            graphics=o.graphics
            [orig,sz]=(graphics.orig,graphics.sz)
            data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
            if data(1)<0&data(2)<0 then k=i,break,end
        end
    end
endfunction

//**---------------------------------------------------------------------------------------
function [d,pt,ind]=stupid_dist2polyline(xp,yp,pt,pereps)
    // computes minimum distance from a point to a polyline
    //d    minimum distance to polyline
    //pt   coordinate of the polyline closest point
    //ind
    //     if negative polyline closest point is a polyline corner:
    //        pt=[xp(-ind) yp(-ind)]
    //     if positive pt lies on segment [ind ind+1]
    //
    x=pt(1)
    y=pt(2)
    xp=xp(:);yp=yp(:)
    cr=4*sign((xp(1:$-1)-x).*(xp(1:$-1)-xp(2:$))+..
    (yp(1:$-1)-y).*(yp(1:$-1)-yp(2:$)))+..
    sign((xp(2:$)-x).*(xp(2:$)-xp(1:$-1))+..
    (yp(2:$)-y).*(yp(2:$)-yp(1:$-1)))

    ki=find(cr==5) // index of segments for which projection fall inside
    np=size(xp,"*")
    if ki<>[] then
        //projection on segments
        x=[xp(ki) xp(ki+1)]
        y=[yp(ki) yp(ki+1)]
        dx=x(:,2)-x(:,1)
        dy=y(:,2)-y(:,1)
        d_d=dx.^2+dy.^2
        d_x=( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d
        d_y=(-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./d_d
        xp=[xp;d_x]
        yp=[yp;d_y]
    end

    zzz=[ones(np,1);zeros(size(ki,"*"),1)]*eps
    zz=[ones(np,1)*pereps;ones(size(ki,"*"),1)]
    [d,k]=min(sqrt((xp-pt(1)).^2+(yp-pt(2)).^2).*zz-zzz)
    pt(1)=xp(k)
    pt(2)=yp(k)
    if k>np then ind=ki(k-np),else ind=-k,end
endfunction

//**-----------------------------------------------------------------------------------------

function draw_link_seg(o,seg)
    if o.thick(2)>=0 then
        d=xget("dashes");thick=xget("thickness")
        t=maxi(o.thick(1),1)*maxi(o.thick(2),1)
        xset("thickness",t);xset("dashes",o.ct(1))
        xpoly(o.xx(seg),o.yy(seg),"lines")
        xset("dashes",d);xset("thickness",thick)
    end
endfunction

