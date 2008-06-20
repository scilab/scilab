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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [scs_m] = do_move(%pt, scs_m)
//** get a scicos object to move, and move it with connected objects
//**

//** 19 October 2007: I have prayed for all my sins : TXH to Alan Layec that
//**                  has show me the way to redemption.

//** 20 June 2008 : finally, the green light for a brand new revision 

//** Acquire the current window and axes handles 
  gh_curwin = scf(%win) ;
  gh_axes = gca(); 

//** get block to move
  win = %win  ; //** recover 'clicked' window
  xc  = %pt(1);
  yc  = %pt(2); //** recover mouse position at the last event

  //** look for a valid object 
  [k,wh] = getobj(scs_m,[xc;yc]) ;
  //** "k" is the index of the object in the data structure "scs_m"
  //** "wh" is a parameter valid ONLY for the links:
  //**       if negative polyline closest point is a polyline corner: pt = [xp(-ind) yp(-ind)]
  //**       if positive pt lies on segment [ind ind+1]
  //**
  //** "wh" shortcut for "where", ia an INDEX for the polyline matrix that contains
  //**      the polylines coordinate.
  //**      If "wh" is positive means that the user has clicked near a link segment:
  //**          the 'wh' INDEX point to starting point of the segment clicked ;
  //**      If "wh" is negative means that the user has clicked near a corner,
  //**          then 'wh' is the INDEX of that corner coordinates
  //**

  if k==[] then
    return
  end ; //** if NO object found -> exit

  scs_m_save = scs_m ; //** save the diagram

  //**-------------------- Block or Text ---------------------------------
  if typeof(scs_m.objs(k))=="Block" | typeof(scs_m.objs(k))=="Text" then
    needreplay = replayifnecessary() ;    //** to be removed later (obsolete)
    scs_m = moveblock(scs_m, k, xc, yc) ; //** see below in the code

  //**------------------- Link ------------------------------
  elseif typeof(scs_m.objs(k))=="Link" then
   //** filter the two possible link cases
    if wh>0 then
      // if positive 'pt' lies on segment [ind ind+1] where 'ind = wh; '
      if %scicos_debug_gr
        disp ("SmartMove:Link:-->Segment... wh= "); disp(wh) ;
      end
      
      scs_m = movelink(scs_m, k, xc, yc, wh);

    else
      //**  if 'wh' is negative, the polyline closest point is a polyline corner of
      //**  coordinate pt = [xp(-ind) yp(-ind)] ,   where 'ind = wh;'
      //**  WARNING:  the "dist2polyline" routine has some corner detection problems
      //** 
      if %scicos_debug_gr
        disp ("SmartMove:Link:-->Corner...wh= "); disp(wh) ;
      end
      scs_m = movecorner(scs_m,k,xc,yc,wh) ;
    end
   //**-------------------------------------------------------
  end

  if Cmenu=="Quit" then
       //** active window has been closed
       [%win,Cmenu] = resume(%win,Cmenu) ;
  end
  if %scicos_debug_gr
    disp ("SmartMove:...End") ;
  end

  [scs_m_save,enable_undo,edited,nc_save,needreplay] = resume(scs_m_save,%t,%t,needcompile,needreplay);

endfunction
//**---------------------------------------------------------------------------------------------------------


function scs_m = moveblock(scs_m,k,xc,yc)
// Move  block 'k' and modify connected links if any
//
//** This code handle the case of moving a block with connectected link(s)
//** The code has three section:
//** - find the link id, filter the three possible cases of link(s), prepare matrix where each
//**   column represent the (x,y) coordinate of the link corner
//** - interactive move of block (replace by an empty box to speed the moving operation)
//** - if the move is valid, update the scs_m data structure with the new object coordinates    
//**----------------------------------------------------------------------------------------------

  //** The code below is modified according the new graphics API using the standard "copy" mode 
  //** "xor" mode is now "desuete" in Scilab 5
  

//**-----------------------------------------------------------------------------------
  //** Acquire the current window and axes handles 
  gh_curwin = scf(%win) ;
  gh_axes = gca(); 

  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure 
  //** I need an equivalent index for the graphics

  o_size = size (gh_axes.children ) ; //** the size: number of all the object

  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure
  gh_k   = get_gri(k, o_size(1))
  gh_blk = gh_axes.children(gh_k);

//**-----------------------------------------------------------------------------------
//
//** Save the mouse position at the beginning of the operation
     xo_mouse = xc ;
     yo_mouse = yc ;
//
//**-----------------------------------------------------------------------------------------------
//** This code segment find the Block and the Links to move; classify link in three type 
//** type :
//**        "0" : if the Link start from the Block and end in the same Block (the "Perverted Link");
//**        "1" : if the Link start from the Block ( the "Outgoing Link")
//**       "-1" : if the Link end in the Block ( the "Ingoing Link")
//**
//** The "link type" is stored as vector in 'ii'
//**
//** Create the 'xx' 'yy' matrix that contain the coordinate of the
//**
// Move  block 'k' and modify connected links if any
// look at connected links

  connected = unique(get_connected(scs_m,k)) ; //** connected links
  o  = scs_m.objs(k) ; //** block
  
  xx = [] ;//** matrix of the 'x' coordinate of the moving link(s)
  yy = [] ;//**    "   "   "  'y'      "      "  "     "     "
  ii = [] ;//** three value vector (link type, see above)
  mx = [] ;//** ?
  my = [] ;//** ?

  // build movable segments for all connected links

  gh_objs_invisible = [] ; //** vector of handles of object(s) that I need put invisible
                           //** during the interactive move

  clr_nrm = [] ;//** vector that specify the 'color' of the normal (in & out) moving link(s)
  clr_per = [] ;//** vector that specify the 'color' of the perverted (in & out over same block) moving link(s)
                //** This variable is not used because each perverted link is handled separately

  //===============================================
  // look at connected links
  //** This loop classify the links and store the coordinates used for the move
  for i=connected
    oi = scs_m.objs(i) ; //**
    [xl, yl, ct, from, to] = (oi.xx, oi.yy, oi.ct, oi.from, oi.to)
    nl = prod(size(xl))
    if from(1)==k&to(1)==k then
    //** if the Link start from the Block and end in the same Block (the "Perverted Link")
      //move all link
      ii=[ii 0]
      clr_per = [clr_per ct(1)]; //** update the color vector

    elseif from(1)==k then
    //** if the Link start from the Block ( the "Outgoing Link")
      ii=[ii 1] ;
      clr_nrm = [clr_nrm ct(1)] ; //** update the color vector

      // build movable segments for this link
      if nl>=4 then
	x1=xl(1:4)
	y1=yl(1:4)
      elseif nl==3 then
	// 3 points link add one point at the begining
	x1=xl([1 1:3])
	y1=yl([1 1:3])
      elseif xl(1)==xl(2)|yl(1)==yl(2) then 
	// vertical or horizontal 2 points link, add a point in the middle
	x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	y1=[yl(1);yl(1)+(yl(2)-yl(1))/2;yl(1)+(yl(2)-yl(1))/2;yl(2)]
      else
	// oblique 2 points link add 2 points in the middle
	x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	y1=[yl(1);yl(1);yl(2);yl(2)]
      end
      //set allowed (x or y) move for each points of build movable segments
      if nl==3 then
	if xl(1)==xl(2) then
	  mx=[mx,[1;1;1;0]]
	  my=[my,[1;1;0;0]]
	else
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	end
      else
	if xl(1)==xl(2) then
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	else
	  mx=[mx,[1;0;0;0]]
	  my=[my,[1;1;0;0]]
	end
      end
      xx=[xx x1];yy=[yy y1]  //store  movable segments for this link

    else
    //** if the Link end in the Block ( the "Ingoing Link")
      ii=[ii -1]; //** add a '-1' element to the 'ii' vector
      clr_nrm = [clr_nrm ct(1)] ; //** update the color vector

      // build movable segments
      if nl>=4 then
	x1=xl(nl:-1:nl-3)
	y1=yl(nl:-1:nl-3)
      elseif nl==3 then 
	// 3 points link add one point at the end
	sel=[nl:-1:nl-2,nl-2]
	x1=xl([nl nl:-1:nl-2])
	y1=yl([nl nl:-1:nl-2])
      elseif xl(1)==xl(2)|yl(1)==yl(2) then
	// vertical or horizontal 2 points link add a point in the middle
	xm=xl(2)+(xl(1)-xl(2))/2
	x1= [xl(2);xm;xm;xl(1)]
	ym=yl(2)+(yl(1)-yl(2))/2;
	y1= [yl(2);ym;ym;yl(1)]
      else
	// oblique 2 points link add 2 points in the middle
	xm=xl(2)+(xl(1)-xl(2))/2
	x1=[xl(2);xm;xm;xl(1)]
	y1=[yl(2);yl(2);yl(1);yl(1)]
      end
      if nl==3 then
	if x1(2)==x1(3) then 
	  mx=[mx,[1;1;1;0]]
	  my=[my,[1;1;0;0]]
	else
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	end
      else
	if x1(1)==x1(2) then
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	else
	  mx=[mx,[1;0;0;0]]
	  my=[my,[1;1;0;0]]
	end
      end
      xx=[xx x1];yy=[yy y1] // store  movable segments for this link
    end

  end //** end of 'connected' link classification 

  // move a block and connected links
  //=================================
  [mxx,nxx] = size(xx)

  if connected<>[] then // move a block and connected links
    
    //** --------------- BLOCK WITH CONNECTED LINKS -----------------------------------
    //** 
    drawlater(); //** postpone the drawing
    
    [xmin,ymin] = getorigin(o) ;
    xc = xmin ;
    yc = ymin ;

    [xy,sz] = (o.graphics.orig,o.graphics.sz)

    //** look for the handle of the block to move 
    gr_k = get_gri(k, o_size(1)) ; //** from scs_m to gh_
    gh_block_move = gh_axes.children(gr_k);
    
    //** Put the links to "invisible"
    gh_link_invisible = [];
    for i=connected
       gr_k = get_gri(i, o_size(1)) ; //** from scs_m to gh_
       gh_link_invisible = gh_axes.children(gr_k); 
       gh_link_invisible.visible = "off"; //** put the link invisible
       gh_objs_invisible = [gh_objs_invisible gh_link_invisible] ; //** add at the list
    end 

    //**-------------------------------------------------------------------------------

    //** Interactive loop that move block and links

    //** This vector contains the handle(s) of the temporary object(s)
    //** created and used for the SmartMove; these object(s) are erased
    //** at the end of operation
    gh_interactive_move = [] ; //** initialize the object vector
   
    //** --------> perverted links <----------------------------------
    //** the 'perverted' links are handled as full link
    full_move = connected(find(ii==0))
    for i=full_move
	oi = scs_m.objs(i);
	//** xpolys(oi.xx+(xc-xmin),oi.yy+(yc-ymin),oi.ct(1));
	xpoly( oi.xx+(xc-xmin), oi.yy+(yc-ymin) ) ;
	gh_poly_perlink(i) = gce()                ;
	gh_poly_perlink(i).foreground = oi.ct(1)  ;
	draw(gh_poly_perlink(i)) ;//** draw the poly (erase all the link   )
	gh_interactive_move = [gh_interactive_move gh_poly_perlink(i)] ;
    end

    //** normal "InGoing" and "Outgoing" links 
    //** xpolys(xx + mx*(xc-xmin), yy+my*(yc-ymin),clr);
    ngxx = xx+mx*(xc-xmin) ; ngyy = yy+my*(yc-ymin) ;
    v_isopoly = size(clr_nrm) ; isopoly = v_isopoly(2) ;
    for i=1:isopoly
       ngx = ngxx(:,i) ; ngy = ngyy(:,i) ; //** isolate the column vector
       xpoly( ngx, ngy ) ; //** create the polyline
       gh_poly_stdlink(i) = gce() ; //** get the handle
       gh_poly_stdlink(i).foreground = clr_nrm(i) ; //** modify the color
       draw(gh_poly_stdlink(i)); //** draw the poly (erase the moving part of the link  )
       gh_interactive_move = [gh_interactive_move gh_poly_stdlink(i)] ; //** add the handle at the list
    end

    //**---------- Interactive Move loop ----------------------------------------

    //** These variables are used to emulate the "incremental" move in order to avoid the
    //** the "block and links postion step" at the beginning of the move
    //** This sophistication is not necessary for the link because you need to place the
    //** mouse EXACTLY on the link to move it.
    delta_x = 0 ;
    delta_y = 0 ;
    dxdy = [delta_x , delta_y] ; 
    while (1) //** move loop

      drawlater();

      //** move the block       
      dxdy = [delta_x , delta_y] ;
      move(gh_block_move, dxdy)  ;

      //** draw normal moving links
      ngxx = xx+mx*(xc-xmin) ; ngyy = yy+my*(yc-ymin) ;
      for i=1:isopoly
         ngx = ngxx(:,i) ; ngy = ngyy(:,i) ;
         gh_poly_stdlink(i).data = [ngx ngy] ;
      end

      //** draw perverted moving link
      for i=full_move
	oi = scs_m.objs(i);
	gh_poly_perlink(i).data = [oi.xx+(xc-xmin)  oi.yy+(yc-ymin) ];
      end

      drawnow(); //** update the screen 

      //**-------------------------------------------------------
      // get new position

      // rep = xgetmouse(0,[%t,%t]); (original by Serge)
      //** My version clear the event queque in order to minimize the 
      //** load and spped up the operation 
      rep = xgetmouse([%t,%t]); //** wait for user movement :)

      if or(rep(3)==[0,2,3,5,-5]) then
          break ; //** ---> EXIT point of the while
      elseif rep(3)==-1000 then //active window has been closed
	  [%win,Cmenu] = resume(curwin,'Quit')
      end

      delta_x = rep(1) - xo_mouse ; //** compute delta positions
      delta_y = rep(2) - yo_mouse ;

      xo_mouse = rep(1) ; //** acquire mouse position for the next iteration
      yo_mouse = rep(2) ;

      xc = xc + delta_x ; //** update the absolute coordinate of the object
      yc = yc + delta_y ;

      xy = [xc, yc] ;

    end //** ... end of while() loop
    //**-----------------------------------------------------------
     

    drawlater();
    
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move 
    if %scicos_debug_gr then
      disp("SmartMoveConnLink:End_Interactive_Loop"); //** pause ; //** debug only
    end

    //**----------

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-1000 then
        [%win,Cmenu] = resume(curwin,"Quit") ;
    end

    // update  block
    if and(rep(3)<>[2 5]) then
        o.graphics.orig = xy;
	scs_m.objs(k) = o ;
    end
    //** update moved links in scicos structure
    xx = xx+mx*(xc-xmin)
    yy=yy+my*(yc-ymin)
    i=0;
    for i1=1:prod(size(ii))
      oi = scs_m.objs(connected(i1))
      xl=oi.xx;
      yl=oi.yy;nl=prod(size(xl))
      if ii(i1)>0 then
	i=i+1
	if nl>=4 then
	  xl(1:4)=xx(:,i)
	  yl(1:4)=yy(:,i)
	elseif nl==3 then
	  xl=xx(2:4,i)
	  yl=yy(2:4,i)
	else
	  xl=xx(:,i)
	  yl=yy(:,i)
	end
      elseif ii(i1)<0 then
	i=i+1
	if nl>=4 then
	  xl(nl-3:nl)=xx(4:-1:1,i)
	  yl(nl-3:nl)=yy(4:-1:1,i)
	elseif nl==3 then
	  xl=xx(4:-1:2,i)
	  yl=yy(4:-1:2,i)
	else
	  xl=xx(4:-1:1,i)
	  yl=yy(4:-1:1,i)
	end
      else // full move
	xl=xl+(xc-xmin)
	yl=yl+(yc-ymin)
      end
      nl=prod(size(xl))
      // eliminate double points
      kz=find((xl(2:nl)-xl(1:nl-1))^2+(yl(2:nl)-yl(1:nl-1))^2==0)
      xl(kz)=[];yl(kz)=[]
      // store
      //** xpolys(xl,yl,oi.ct(1))// erase thin link
      if  and(rep(3)<>[2 5]) then
	oi.xx=xl;
	oi.yy=yl;
	scs_m.objs(connected(i1)) = oi;
      end
    end

    //** Clear the graphic window WITHOUT changing his paramaters ! :)
    delete(gh_axes.children) ; //** wipe out all the temp graphics object
    drawobjs(scs_m, gh_curwin) ;   //** re-draw all the graphics object
    drawnow(); //** draw the graphic object and show on screen (now included in "drawobjs()"

//**---  
  else // move an unconnected block
//**------------------- UnConnected Block : move Block only ... -------------------------
    
    if %scicos_debug_gr then
      disp("SmartMove : Move block without links"); //** debug only 
    end 

    [xy, sz] = (o.graphics.orig, o.graphics.sz)

    //**----------------------------------------------------
    //** Interactive Bloc move

    delta_move_x = 0 ; delta_move_y = 0 ; //** init 
   
    rep(3) = -1 ;
    while rep(3)==-1 , // move loop

      rep = xgetmouse([%t,%t]); //** get new position

      if rep(3)==-1000 then //** active window has been closed
	[%win,Cmenu] = resume(curwin,"Quit")
      end

      delta_x = rep(1) - xc ; delta_y = rep(2) - yc ; //** calc the differential position because
      move (gh_blk , [delta_x , delta_y]);            //** "move()" works only in differential

      delta_move_x = delta_move_x + delta_x ; //** accumulate the delta to compute the
      delta_move_y = delta_move_y + delta_y ; //** physical displacement of the block 

      xc = rep(1) ; //** store actual position for the next iteration 
      yc = rep(2) ;
      
    end // of while()
    //**----------------------------------------------------

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-1000 then
        [%win,Cmenu] = resume(curwin,'Quit') ;
    end

    // update and draw block
    if and(rep(3)<>[2 5]) then
        // xy = [xc,yc]; //** format of block's origin 
        xc = xy(1) ; yc = xy(2) ;
        xc = xc + delta_move_x  ; 
        yc = yc + delta_move_y  ;
        xy = [xc,yc]            ;
        o.graphics.orig = xy ;
        scs_m.objs(k) = o    ;
    end
  end //** of the main connected/UnConnected Block

endfunction

//**------------------------------------------------------------------------------------------

function scs_m = movelink(scs_m, k, xc, yc, wh)
//** INRIA 
//**
//** This function handle the case of a Smartmove of a single link where the use has clicked
//** in proximity of the link and not near any corner. In this case 'wh' is a positive integer
//** INDEX of the first coordinate of the link segment.
//** 'pt' (the user click) lies on segment [ind ind+1] ; where 'ind = wh';
//**
//** Actions: move the  segment 'wh' of the link 'k' and modify the other segments if necessary
//**
//** ----> This routine handle the main case of a LINK ONLY move <-----------------------------
//**
//** 20 June 2008 : "copy" mode only for Scilab 5 

  //** Acquire the current clicked window handles
  gh_curwin = scf(%win) ;
  gh_axes = gca(); 
  
  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
  //** I need an equivalent index for the graphics
  o_size = size (gh_axes.children ) ; //** the size: number of all the object
  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure
  gh_k   = get_gri(k,o_size(1));
  gh_link = gh_axes.children(gh_k);

  //** Put the link to "invisible" 
  gh_link.visible = "off"; //** put the link invisible
  
  gh_interactive_move = [] ; //** initialize the object vector used for the 
                             //** interactive operations
  o = scs_m.objs(k)
  [xx, yy, ct] = (o.xx,o.yy,o.ct)

  nl = size(o.xx,'*')  // number of link points

  //** wh=1 first segment
  //** wh>=nl-1 last segment
  //** the link has 3 (three) elements (nor the fist, nor the last

  if wh==1 then
  //**----------> Click over the first segment of the link <-----------------------
    from = o.from ;
    to   = o.to   ;
    if is_split(scs_m.objs(from(1))) & is_split(scs_m.objs(to(1))) & nl<3 then
    //** if the link has two segment AND the link start AND the link end are both split
    //**
    //** WARNING: 'movelink1()' is up to date BUT I'm not able to create an examples !
    //**  
      scs_m = movelink1(scs_m) ; //** see below for the code

    elseif ~is_split(scs_m.objs(from(1))) | nl<3 then //** ok
    //** the link start is NOT a split (is a Block output) OR the link has two segment
    //**
    //** this code create a new segment and move the new corner in the desired position
    //** I do not agree with this approach that create two "oblique" object
      p = projaff( xx(1:2), yy(1:2), [xc,yc] ) ;
      X1= [xx(1); p(1); xx(2)] ;
      Y1= [yy(1); p(2); yy(2)] ;
      x1= X1;
      y1= Y1;

      xpoly( x1, y1 ) ; //** create the moving object
      gh_poly = gce() ; //** get the handle of the moving object
      gh_poly.foreground = ct(1) ; //** set the color
      
      gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                             //** the object to be delated at the end of the operation
      //**---- Interactive loop ----------------------------- 
      rep(3)=-1
      while and(rep(3)<>[3 0 2 5]) do
 
        rep = xgetmouse([%t,%t]); //** wait for user movement :)
	
	if rep(3)==-1000 then // active window has been closed
	  [%win,Cmenu] = resume(curwin,"Quit")
	end
	
        xc1 = rep(1) ; //** acquire muse coordinate
	yc1 = rep(2) ;
	x1(2) = X1(2)-(xc-xc1)  ;
	y1(2) = Y1(2)-(yc-yc1)  ; //** update coordinate

	gh_poly.data = [x1 y1]; //** update object 
      end //** of while
      //**--- Interactive loop ends -------------------------
      
      //** accidental window closing protection 
      gh_figure = gcf();
      if gh_figure.figure_id<>curwin | rep(3)==-1000 then
	[%win,Cmenu] = resume(curwin,"Quit") ; 
      end
      
      //** update link data structure 
      xx = [xx(1);x1(2);xx(2:$)] ;
      yy = [yy(1);y1(2);yy(2:$)] ;

      if and(rep(3)<>[2 5]) then 
	o.xx = xx ;
	o.yy = yy ;
	scs_m.objs(k) = o ; //** update the scs_m 
      end

    else  //** ok
      //** link comes from a split
      scs_m = movelink2(scs_m) ; //** ok
    
      if Cmenu=="Quit" then 
        [%win,Cmenu] = resume(curwin,Cmenu)
      end
    
    end //** and of "I'm in the first segment of a link"

  //**-----------------------------------------------------------
  elseif wh>=nl-1 then
  //**---> click over the last segment of 3 or more segment link
    to = o.to ;
    if ~is_split(scs_m.objs(to(1))) | nl<3 then
    //** the start is a split  OR is a one/two segment link  
      p = projaff(xx($-1:$),yy($-1:$),[xc,yc])
      X1= [xx($-1);p(1);xx($)]
      Y1= [yy($-1);p(2);yy($)]
      x1= X1;
      y1= Y1;

      xpoly( x1, y1 ) ; //** create the moving object 
      gh_poly = gce() ; //** get the handle of the moving object 
      gh_poly.foreground = ct(1) ; //** set the color 
   
      gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                     //** the object to be delated at the end of the operation
      rep(3)=-1
      //**---- Interactive loop -----------------------------
      while and(rep(3)<>[3 0 2 5]) do
	
        rep = xgetmouse([%t,%t]); //** wait for user movement :)
	
	if rep(3)==-1000 then //active window has been closed
	  [%win,Cmenu] = resume(curwin,'Quit')
	end

	xc1 = rep(1) ; 
	yc1 = rep(2) ;
	x1(2) = X1(2)-(xc-xc1) ;
	y1(2) = Y1(2)-(yc-yc1) ;
	
	gh_poly.data = [x1 y1];
	
      end
      //**--- Interactive loop ends -------------------------

      //** window closing protection 
      gh_figure = gcf();
      if gh_figure.figure_id<>curwin | rep(3)==-1000 then
	[%win,Cmenu] = resume(curwin,"Quit") ;
      end

      //** link data structure update 
      xx = [xx(1:$-1);x1(2);xx($)]
      yy = [yy(1:$-1);y1(2);yy($)]
      
      if and(rep(3)<>[2 5]) then //** if the user has not "clered" the op
	o.xx = xx ;
	o.yy = yy ;
	scs_m.objs(k) = o ; //** update the diagram 
      end

    else // link goes to a split
         // click over the last segment of 3 or more segment link
	 //**
	 //** the user has clicked in a end segment that goes in a split
         //** 
      scs_m = movelink3(scs_m) ; //** see below in the code

      if Cmenu=="Quit" then
        [%win,Cmenu] = resume(curwin,Cmenu)
      end

    end
  //**----------------------------------------------------------------

  elseif nl<4 then
  //**---> the link has 3 (three) elements (nor the first, nor the last)
  //**     is a click in the intemediate segment (wh=2 allways so is not necessary :)
  //**     check it )
    p  = projaff(xx(wh:wh+1),yy(wh:wh+1),[xc,yc]) ;
    X1 = [xx(wh);p(1);xx(wh+1)] 
    Y1 = [yy(wh);p(2);yy(wh+1)]
    x1 = X1 ;
    y1 = Y1 ;

    rep(3)=-1
    //**---- Interactive loop -----------------------------
    while and(rep(3)<>[3 0 2 5]) do
      
      rep = xgetmouse([%t,%t]);

      if rep(3)==-1000 then // active window has been closed
	[%win,Cmenu]=resume(curwin,'Quit')
      end

      xc1 = rep(1) ;
      yc1 = rep(2) ; 
      x1(2) = X1(2)-(xc-xc1)
      y1(2) = Y1(2)-(yc-yc1)

      gh_poly.data = [x1 y1];

    end
    //**--- Interactive loop ends -------------------------

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-1000 then
	 [%win,Cmenu] = resume(curwin,"Quit") ;
    end

    //** update data structure 
    xx=[xx(1:wh);x1(2);xx(wh+1:$)]
    yy=[yy(1:wh);y1(2);yy(wh+1:$)]

    if and(rep(3)<>[2 5]) then
      o.xx = xx ;
      o.yy = yy ;
      scs_m.objs(k) = o
    end

  else
  //**---> the user has cliked over at least 4 segment long link, NOR at the start NOR at the end

    scs_m = movelink4(scs_m) ; //** see below in the code

    if Cmenu=="Quit" then
      [%win,Cmenu] = resume(curwin,Cmenu)
    end

  end

  //** Clear the graphic window WHITOUT changing his parameters AND recreate all the good graphics object
  drawlater() ;
     delete(gh_axes.children) ; //** wipe out all the temp graphics object

     gh_curwin.background = options.Background(1) ; //** "options" is sub structure of scs_m
     gh_axes.background   = options.Background(1) ;

     drawobjs(scs_m, gh_curwin) ;   
  drawnow(); 

endfunction

//**---------------------------------------------------------------------------------

function scs_m = movelink4(scs_m)
//**---> the user has cliked over at least 4 segment long link, NOR at the start NOR at the end

  o; //** local copy of an external variable "o"
  e = [min(yy(wh:wh+1))-max(yy(wh:wh+1)),min(xx(wh:wh+1))-max(xx(wh:wh+1))];
  e = e/norm(e) ;     //** matrix normalization
  X1 = xx(wh-1:wh+2);
  Y1 = yy(wh-1:wh+2);
  x1 = X1;
  y1 = Y1;

  gh_interactive_move = [];

  //** xpolys(x1,y1,ct(1)) //erase moving part of the link
  xpoly(x1, y1) ; //** create the moving object
  gh_poly = gce() ; //** get the handle of the moving object
  gh_poly.foreground = ct(1) ; //** set the color

  gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                                 //** the object to be deleted at the end of the operation

  rep(3)=-1
  //**---- Interactive loop -----------------------------
  while and(rep(3)<>[3 0 2 5]) do

    rep = xgetmouse([%t,%t]); //** wait for user movement :)

    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu] = resume(curwin,'Quit')
    end

    xc1 = rep(1); yc1 = rep(2)
    x1(2:3) = X1(2:3)+e(1)*(xc-xc1) ;
    y1(2:3) = Y1(2:3)+e(2)*(yc-yc1) ;

    gh_poly.data = [x1 y1];

   end
  //**--- Interactive loop ends -------------------------

  if gh_interactive_move <> []
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move
  end

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-1000 then
      [%win,Cmenu] = resume(curwin,'Quit') ;
  end


  if and(rep(3)<>[2 5]) then
    o.xx(wh-1:wh+2) = x1 ;
    o.yy(wh-1:wh+2) = y1 ;
    scs_m.objs(k) = o    ; 
  end

 endfunction

//**-----------------------------------------------------------------------------------


function scs_m = movelink1(scs_m)
  // link between to splits

    //**
    //** WARNING: 'movelink1()' is up to date BUT I'm not able to create an examples !
    //**

//** As Serge correcty point out is the case of a "short" (one or two segment link )
//** between two split

  if %debug_scicos_gr then
    disp ("SmartMove::movelink1");
  end

  o; //** local copy of an external variable "o" 
  e = [min(yy)-max(yy), min(xx)-max(xx)];
  e = e/norm(e) ;

  gh_interactive_move = [];

  xpoly(xx, yy) ; //** create the moving object
  gh_poly = gce() ; //** get the handle of the moving object
  gh_poly.foreground = ct(1) ; //** set the color

  gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                                 //** the object to be deleted at the end of the operation
  X1 = xx ;
  Y1 = yy ;

  rep(3)=-1
  //**---- Interactive loop ----------------------------- 
  while and(rep(3)<>[3 0 2 5]) do

    rep = xgetmouse([%t,%t]); //** wait for user movement :)

    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end

    xc1 = rep(1); yc1 = rep(2) ; //** acquire muse coordinate
    xx = X1 + e(1)*(xc-xc1) ;
    yy = Y1 + e(2)*(yc-yc1) ; //** update coordinate

    gh_poly.data = [xx yy];

  end
  //**--- Interactive loop ends -------------------------

  if gh_interactive_move <> [] 
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move
  end

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-1000 then
      [%win,Cmenu] = resume(curwin,"Quit") ;
  end

  if and(rep(3)<>[2 5]) then
      o.xx = xx;
      o.yy = yy;
  end

  scs_m.objs(k) = o ;

  if or(rep(3)==[2 5]) then
     return ; 
  end

  // move split block and update other connected links
  connected = [get_connected(scs_m,from(1)),get_connected(scs_m,to(1))] ; 

  // change links
  if connected(1)<>k then
    //update links coordinates
    o = scs_m.objs(connected(1));

    if size(o.xx,'*')>2 then
      if o.xx($)==o.xx($-1) then
	o.xx($-1:$)=o.xx($-1:$)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      elseif o.yy($)==o.yy($-1) then
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($-1:$)=o.yy($-1:$)+e(2)*(yc-yc1);
      else
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      end
    else
      o.xx($)=o.xx($)+e(1)*(xc-xc1);
      o.yy($)=o.yy($)+e(2)*(yc-yc1);
    end

    scs_m.objs(connected(1)) = o;
   
  end

  for kk=2:size(connected,'*')
    if connected(kk)<>k then
      //update links coordinates
      o=scs_m.objs(connected(kk))

      if size(o.xx,'*')>2 then
	if o.xx(1)==o.xx(2) then
	  o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	elseif o.yy(1)==o.yy(2) then
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
	else
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	end
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end

      scs_m.objs(connected(kk)) = o;
    end

  end

  // update split coordinates
  o = scs_m.objs(from(1)) ; 
  o.graphics.orig(1) = o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2) = o.graphics.orig(2)+e(2)*(yc-yc1);

  scs_m.objs(from(1)) = o ;
  o = scs_m.objs(to(1));
  o.graphics.orig(1) = o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2) = o.graphics.orig(2)+e(2)*(yc-yc1);
  scs_m.objs(to(1))  = o ; 

endfunction

//**-------------------------------------------------------------------------------------

function scs_m = movelink2(scs_m)
//** Move the first segment 'wh==1' of a link that start from a split 
//** In the first part, the first segment link that start from the plist is interactively moved
//** if the move is valid, also the 'split' object is moved accordly at the link starting point
//** it is nice and it's works ! 

  o   ; //** local copy of an external variable "o" 
  e = [min(yy(1:2))-max(yy(1:2)), min(xx(1:2))-max(xx(1:2))];
  e = e/norm(e) ;
  X1 = xx(1:3) ;
  Y1 = yy(1:3) ;
  x1 = X1 ;
  y1 = Y1 ;

  gh_interactive_move = [];

  xpoly( x1, y1 ) ; //** create the moving object
  gh_poly = gce() ; //** get the handle of the moving object
  gh_poly.foreground = ct(1) ; //** set the color
 
  gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                     //** the object to be delated at the end of the operation

  rep(3)=-1
  //**---- Interactive loop -----------------------------
  while and(rep(3)<>[3 0 2 5]) do
    
    rep = xgetmouse([%t,%t]); //** wait for user movement :)

    if rep(3)==-1000 then //active window has been closed
	[%win,Cmenu] = resume(curwin,"Quit") ;
    end
    
    xc1 = rep(1); yc1 = rep(2); //** acquire muse coordinate
    x1(1:2) = X1(1:2)+e(1)*(xc-xc1) ; //** update coordinate
    y1(1:2) = Y1(1:2)+e(2)*(yc-yc1) ;
    gh_poly.data = [x1 y1];

  end
  //**--- Interactive loop ends -------------------------

  if gh_interactive_move <> [] 
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move
  end

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-1000 then
      [%win,Cmenu] = resume(curwin,"Quit") ;
  end

  if and(rep(3)<>[2 5]) then //** if valid move
    o.xx(1:3) = x1 ;
    o.yy(1:3) = y1 ;         //** update the object 
  end

  scs_m.objs(k) = o ;        //** update the diagam 

  if or(rep(3)==[2 5]) then return, end

  //move split block and update other connected links
  connected = get_connected(scs_m,from(1))

  // change links
  if connected(1)<>k then
    //update links coordinates
    o = scs_m.objs(connected(1));
    if size(o.xx,'*')>2 then

      if o.xx($)==o.xx($-1) then
	o.xx($-1:$)=o.xx($-1:$)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      elseif o.yy($)==o.yy($-1) then
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($-1:$)=o.yy($-1:$)+e(2)*(yc-yc1);
      else
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      end

    else
      o.xx($)=o.xx($)+e(1)*(xc-xc1);
      o.yy($)=o.yy($)+e(2)*(yc-yc1);
    end

    scs_m.objs(connected(1))=o;
    //** drawobj(o) //redraw link
  end

  for kk=2:size(connected,'*')
    if connected(kk)<>k then
      //update links coordinates
      o = scs_m.objs(connected(kk))
      if size(o.xx,'*')>2 then
	if o.xx(1)==o.xx(2) then
	  o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	elseif o.yy(1)==o.yy(2) then
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
	else
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	end
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end
      scs_m.objs(connected(kk))=o;
      //** drawobj(o)//redraw link
    end
  end

  //update split coordinates
  o = scs_m.objs(from(1))
  o.graphics.orig(1) = o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2) = o.graphics.orig(2)+e(2)*(yc-yc1);

  scs_m.objs(from(1)) = o ; //** update diagram 


endfunction
//**---------- End of movelink2 ---------------------------------------------------

//**--------------------------------------------------------------------------------

function scs_m = movelink3(scs_m)
// link goes to a split 
// click over the last segment of 3 or more segment link
//**
 //** the user has clicked in a end segment that goes in a split
//**
  o ; //** local copy of an external variable "o"
  e = [min(yy($-1:$))-max(yy($-1:$)),min(xx($-1:$))-max(xx($-1:$))];
  e = e/norm(e) ;
  X1 = xx($-2:$);
  Y1 = yy($-2:$);
  x1 = X1 ;
  y1 = Y1 ;
  gh_interactive_move = [];
  xpoly(x1, y1) ; //** create the moving object
  gh_poly = gce() ; //** get the handle of the moving object
  gh_poly.foreground = ct(1) ; //** set the color
  
  gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                                 //** the object to be deleted at the end of the operation
  rep(3)=-1
  //**---- Interactive loop -----------------------------
  while and(rep(3)<>[3 0 2 5]) do
  
    rep = xgetmouse([%t,%t]); //** wait for user movement :)

    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end
  
    xc1 = rep(1); yc1 = rep(2)
    x1($-1:$) = X1($-1:$)+e(1)*(xc-xc1) ;
    y1($-1:$) = Y1($-1:$)+e(2)*(yc-yc1) ;

    gh_poly.data = [x1 y1]; 
  end
  //**--- Interactive loop ends ------------------------

  if gh_interactive_move <> [] 
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move
  end

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-1000 then
      [%win,Cmenu] = resume(curwin,'Quit') ;
  end

  //** xpolys(xx(1:$-2),yy(1:$-2),ct(1))//erase rest of the link

  if and(rep(3)<>[2 5])  then
    o.xx($-2:$) = x1  ;
    o.yy($-2:$) = y1  ;
    scs_m.objs(k) = o ;
  end

  if or(rep(3)==[2 5]) then
    return
  end

  //move split block and update other connected links
  connected = get_connected(scs_m,to(1))

  for kk=2:size(connected,'*')
    //update links coordinates
    o = scs_m.objs(connected(kk))
    if size(o.xx,'*')>2 then
      if o.xx(1)==o.xx(2) then
	o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1) 
      elseif o.yy(1)==o.yy(2) then 
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end
    else
      o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
      o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
    end
    scs_m.objs(connected(kk))=o;
    //** drawobj(o) // redraw connected links
  end //** of for loop
  
  o = scs_m.objs(to(1))
  o.graphics.orig(1) = o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2) = o.graphics.orig(2)+e(2)*(yc-yc1);

  scs_m.objs(to(1))=o

endfunction

//**--------------------------------------------------------------------------------

function scs_m = movecorner(scs_m,k,xc,yc,wh)
//** the user has clicked near a link corner (link segment joint)
  o = scs_m.objs(k) ; //** get the object
  [xx,yy,ct] = (o.xx,o.yy,o.ct)

  if wh==-1|wh==-size(xx,'*') then // link endpoint is choosen
    scs_m = movelink(scs_m,k,xc,yc,-wh)
    return
  end

  wh = [-wh-1 -wh] ; //** ??? Ask Serge

  wh = [wh wh($)+1] ; //** ??? Ask Serge

  X1 = xx(wh) ;
  Y1 = yy(wh) ;
  x1 = X1 ;
  y1 = Y1 ;
  gh_interactive_move = [];
  //** xpolys(x1,y1,ct(1)) //erase moving part of the link
  xpoly( x1, y1 ) ; //** create the moving object
  gh_poly = gce() ; //** get the handle of the moving object
  gh_poly.foreground = ct(1) ; //** set the color

  gh_interactive_move = [gh_interactive_move gh_poly] ; //** add the object at the list of
                     //** the object to be delated at the end of the operation

  rep(3)=-1
  //**---- Interactive loop -----------------------------
  while and(rep(3)<>[3 0 2 5]) do

    rep = xgetmouse([%t,%t]); //** wait for user movement :)

    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end

    xc1 = rep(1); yc1 = rep(2) ;
    x1(2) = X1(2)-(xc-xc1) ;
    y1(2) = Y1(2)-(yc-yc1) ;
    gh_poly.data = [x1 y1];

  end
  //**--- Interactive loop ends ------------------------
  if gh_interactive_move <> []
    delete(gh_interactive_move) ; //** delete all the object used during the interactive move
  end
  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-1000 then
      [%win,Cmenu] = resume(curwin,'Quit') ;
  end

  eps = 16 ;

  if abs(x1(1)-x1(2))<eps then
    x1(2) = x1(1) 
  elseif abs(x1(2)-x1(3))<eps then
    x1(2) = x1(3)
  end

  if abs(y1(1)-y1(2))<eps then
    y1(2) = y1(1)
  elseif abs(y1(2)-y1(3))<eps then
    y1(2) = y1(3)
  end

  d = projaff([x1(1);x1(3)],[y1(1);y1(3)],[x1(2);y1(2)]);

  if norm(d(:)-[x1(2);y1(2)])<eps then
    xx(wh) = x1
    yy(wh) = y1
    xx(wh(2)) = []
    yy(wh(2)) = []
    x1(2) = [] ;
    y1(2) = [] ;
  else
    xx(wh) = x1
    yy(wh) = y1
  end

  if and(rep(3)<>[2 5]) then
    o.xx = xx ;
    o.yy = yy ;
    scs_m.objs(k) = o ; 
  end

endfunction
