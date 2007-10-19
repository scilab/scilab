function [scs_m] = do_stupidMultimove(%pt, Select, scs_m)
// Copyright INRIA
//** ---------------------------------   M U L T I     M O V E   -----------------------------------------

have_moved=%f

// Acquire the current window
// NB : the MultiMove works ONLY in the current window    
  gh_curwin = gh_current_window ;

  xc = %pt(1) ; //** recover mouse position of the last event
  yc = %pt(2) ;

 //** Select : matrix of selected object
 //** Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
 //**                                 and "win_id"    is the Scilab window id.
 //**  Multiple selection is permitted: each object is a line of the matrix.
  
  scs_m_save = scs_m ; //** make a backup of the data structure
  
  needreplay = replayifnecessary() ; //** to be removed later (obsolete)
  
  //**------------------------------------------------------------------
  
  //** scs_m , Select, xc yc (mouse coordinate of the last valid event)
  [scs_m,have_moved] = stupid_MultiMoveObject(scs_m, Select, xc, yc)  ; //** see below in the code

  //**------------------------------------------------------------------


  if Cmenu=='Quit' then
    //active window has been closed
    [%win,Cmenu] = resume(%win,Cmenu)
  end

  if have_moved then
    [scs_m_save,enable_undo,edited,nc_save,needreplay] = resume(scs_m_save,%t,%t,needcompile,needreplay)
  end
endfunction


//  ---------------------------- Move Blocks and connected Link(s) ----------------------------


function [scs_m,have_moved] = stupid_MultiMoveObject(scs_m, Select, xc, yc)
  // Move Selected Blocks/Texts and Links and modify connected (external) links if any
  
  //** scs_m  : the local level diagram 
  //** Select : matrix [object_id win_id] of selected object  
  //** xc ,yc : mouse coodinate of the last valid LEFT BUTTON PRESS
  //**
  //** Select : matrix of selected object
  //**          Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
  //**                                          and "win_id"    is the Scilab window id.
  //**          Multiple selection is permitted: each object is a line of the matrix.     
  //**----------------------------------------------------------------------------------
  //**
  //** the code below is modified according the new graphics API
  gh_curwin = gh_current_window ; //** acqiore the current window handle

  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
  //** I need an equivalent index for the graphics

  //** This variable is fundamental because at the end of the move the number of graphics 
  //** object MUST BE the same 
  o_size = size (gh_curwin.children.children ) ; //** o_size(1) number of "Compound" objects 

  //**-----------------------------------------------------------------------------------------------
  //** Acquire axes physical limits (visible limits are smaller) to avoid "off window" move
  figure_axes_size = gh_curwin.axes_size ; //** size in pixel
  x_f = figure_axes_size(1) ;
  y_f = figure_axes_size(2) ;

  [x1_f, y1_f, rect_f] = xchange([0, x_f],[0, y_f],"i2f"); //** convert to local coordinate

  x_min = x1_f(1) ; x_max = x1_f(2) ; //** hor. limits
  y_min = y1_f(2) ; y_max = y1_f(1) ; //** ver. limits (inverted because the upper left corner effect)
  //**-------------------------------------------------------------------------------------------------

  //** Initialization  
  gh_link_i   = [];
  gh_link_mod = [];
   
  //**----------------------------------------------------------------------------------  
  diagram_links = [] ; //** ALL the LINKs of the diagram 
  diagram_size = size(scs_m.objs);
  if diagram_size<>0
     
     for k=1:diagram_size //** scan ALL the diagram and look for 'Link'
          if typeof(scs_m.objs(k))=="Link" then    
	     diagram_links = [diagram_links k];
          end 
     end 
  
  end      
  //**----------------------------------------------------------------------------------
  //** Classification of selected object
  sel_block = []; //** blocks selected by the user 
  sel_link  = []; //** links     "     "   "    "
  sel_text  = []; //** text      "     "        "
  
  SelectObject_id = Select(:,1)'  ; //** select all the object in the current window

  if SelectObject_id == [] then
    k=getblocktext(scs_m,[xc;yc])
    if k==[] then return, end
    SelectObject_id = k
  end

  for k = SelectObject_id                    //** scan all the selected object
   
     if typeof(scs_m.objs(k))=='Block' then  //** look for selected BLOCK
       sel_block = [sel_block k] ; //**
     end
     
     if typeof(scs_m.objs(k))=='Link' then   //** look for selected LINK
       sel_link = [sel_link k]; //**
     end
     
     if typeof(scs_m.objs(k))=='Text' then   //** look for selected TEXT 
       sel_text = [sel_text k]; //**
     end
  
  end //** end of scan      
  
  //**----------------------------------------------------------------------------------
  
  //**----------------------------------------------------------------------------------        
  int_link = []; //** link(s) involved in the move operation  

  for l = diagram_links                   //** scan all links and look for external link 
     from_block = scs_m.objs(l).from(1) ; //** link proprieties 
       to_block = scs_m.objs(l).to(1)   ;
     //** "from" and "to" are relatives to selected blocks 
      if (or(from_block==sel_block)) & (or(to_block==sel_block)) then 
           int_link = [int_link l]; //** pile up   
      end
  end //** end of the link scan 
  //**-----------------------------------------------------------------------------------
  
  //**----------------------------------------------------------------------------------
  connected = []; //** ALL the Links that from/to the supercompound
  ext_block = []; //** ALL the selected blocks that have a links from/to the supercompound       
  
  for k = sel_block //** Scan ALL the selected block and look for external link
        
     sig_in = scs_m.objs(k).graphics.pin' ; //** signal input 
     for l = sig_in //** scan all the input 
	  if (~(or(l==int_link ))) & (or(l==diagram_links)) then //** the link is not internal 
            connected = [connected l]; //** add to the list of link to move
	    ext_block = [ext_block k];    
	  end  
     end 
      
     sig_out = scs_m.objs(k).graphics.pout' ; //** signal output 
     for l = sig_out //** scan all the output 
	  if (~(or(l==int_link ))) & (or(l==diagram_links)) then // ext link
            connected = [connected l]; //** add to the list of link to move
	    ext_block = [ext_block k];    
	  end  
     end 
      
     ev_in = scs_m.objs(k).graphics.pein' ;
     for l = ev_in //** scan all the output 
	  if (~(or(l==int_link ))) & (or(l==diagram_links)) then // ext link
            connected = [connected l]; //** add to the list of link to move
	    ext_block = [ext_block k];    
	  end  
     end    
	
     ev_out = scs_m.objs(k).graphics.peout' ; 
     for l = ev_out //** scan all the output 
	  if (~(or(l==int_link ))) & (or(l==diagram_links)) then // ext link
	    connected = [connected l]; //** add to the list of link to move
	    ext_block = [ext_block k];    
	  end
     end

  end //** end of scan
  //**-----------------------------------------------------------------------------------

  //** look for all the connected link(s) and build "impiling" the two data structures
  //** [xm , ym] for the links data points
  //** gh_link_i is a vector of the associated graphic handles

  xm = []; //** init 
  ym = [];
  if connected<>[] then //** check if external link are present
      for l=1:length(connected) //** scan all the connected links
        i  = connected(l)  ;
        oi = scs_m.objs(i) ;
        gh_i = get_gri(i,o_size(1)); //** calc the handle of all the connected link(s)
        gh_link_i = [ gh_link_i gh_curwin.children.children(gh_i) ]; //** vector of handles
        [xl, yl, ct, from, to] = (oi.xx, oi.yy, oi.ct, oi.from, oi.to)
          //**------------------------------------------
          if from(1)==ext_block(l) then 
              xm = [xm, [xl(2);xl(1)] ];
              ym = [ym, [yl(2);yl(1)] ];
          end

          if to(1)==ext_block(l) then
              xm = [xm, xl($-1:$) ];
              ym = [ym, yl($-1:$) ];
          end
          //**------------------------------------------
       end //** end of the for() loop
  end //** end of if 
  //** ----------------------------------------------------------------------

//**----------------------------------------------------------------------------------
  //** Supposing that all the selected object are in the current window 
  //** create a new compund that include ALL the selected object 

  SuperCompound_id = [sel_block int_link sel_text] ;

  //** -----------------------------------------------------------------------
  xmt = xm ;
  ymt = ym ; //** init ...

  //** --------------------------------- MOVE BLOCK WITH CONNECTED LINKS ------------

  xco = xc;
  yco = yc;

  move_x = 0 ;
  move_y = 0 ;

  //**-------------------------------------------------------------------
  gh_link_mod = [] ;
  tmp_data = [] ;
  t_xmt = [] ; t_ymt  = [];

  drawlater();


    //** ------------------------------- INTERACTIVE MOVEMENT LOOP ------------------------------

    moved_dist=0
    
    if with_gtk() then queue_state=[],end // GTK case
    
    while 1 do //** interactive move loop
      if with_gtk() then // GTK case
	rep = xgetmouse(queue_state=[],[%t,%t]); 
      else // Normal case
	rep = xgetmouse(0,[%t,%t]); //** the event queue is NOT cleared
	                             //** "getmotion" AND "getrelease" active because the mode is made with
				     //** the left button pressed
      end
      //** left button release, right button (press, click) 
      if with_gtk() then   // GTK case
	queue_state=1,
	if or(rep(3)==[-5, 2, 3, 5, 10]) then //** put the end exit from the loop condition here 
	  if rep(3)==10 then
	    global scicos_dblclk
	    scicos_dblclk=[rep(1),rep(2),curwin]
	  end
	  
	  break ; //** ---> EXIT point of the while
	end
      else  // Normal case
	if or(rep(3)==[-5, 2, 3, 5]) then //** put the end exit from the loop condition here 
          break ; //** ---> EXIT point of the while
	end
      end

      //** Window change and window closure protection
      gh_figure = gcf();
      if gh_figure.figure_id<>curwin | rep(3)==-100 then
	[%win,Cmenu] = resume(curwin,'Quit') ;
      end



      //**------------------------------------------------------------------
      //** Mouse movement limitation: to avoid go off the screen ;) 
       if rep(1)>x_min & rep(1)<x_max
           delta_x = rep(1) - xc ; //** calc the differential position ...
	   xc = rep(1);
       else
           delta_x = 0.0 ;
       end 

       if rep(2)>y_min & rep(2)<y_max
           delta_y = rep(2) - yc ; //** calc the differential position ...
	   yc = rep(2) 
       else
           delta_y = 0.0 ;
       end
      //**------------------------------------------------------------------

      //** Integrate the movements
      move_x = move_x +  delta_x ;
      move_y = move_y +  delta_y ;

      moved_dist=moved_dist+abs(delta_x)+abs(delta_y)
      // under window clicking on a block in a different window causes a move
      if moved_dist>.001 then have_moved=%t,end

      //** Move the SuperCompound
      for k = SuperCompound_id 
          gh_k = get_gri(k,o_size(1)); //** calc the handle
          gh_ToBeMoved = gh_curwin.children.children(gh_k) ;
	  move (gh_ToBeMoved, [delta_x , delta_y]);  //** ..because "move()" works only in differential
      end 

      //**---------------------------------------------------------------------------------------------
      if connected<>[] then 
          //** Move the links 
          xmt(2,:) = xm(2,:) - xco + xc ; //** update datas of links
          ymt(2,:) = ym(2,:) - yco + yc ;
          j = 0 ; //** init
          //** Move all the connected links 
          for l=1:length(connected) // ... for all the connected links
             i  = connected(l)  ; // from the progressive index "l" to the scs_m index "i"
             oi = scs_m.objs(i) ; // get the "i"th link
             [xl,from,to] = (oi.xx,oi.from,oi.to); // extract the proprieties from the link
	     gh_link_mod = gh_link_i(l); //** get the link graphics data structure

             if from(1)==ext_block(l) then
                 tmp_data = gh_link_mod.children.data ; // extract the vectors that define the link
 	                                                // the first two points
                 j = j + 1 ; // update the [x,y]mt pointer
	         t_xmt = xmt([2,1],j) ;
	         t_ymt = ymt([2,1],j) ; // exstract the element
	         //** update the graphics datastructure
	         gh_link_mod.children.data = [ [t_xmt(1) , t_ymt(1)] ; tmp_data(2:$ , 1:$) ]  ;
             end

             //** see the above comments :)
             if to(1)==ext_block(l) then
 	         tmp_data = gh_link_mod.children.data ;
	         // the last two points
	         j = j + 1 ;
                 gh_link_mod.children.data = [ tmp_data(1:$-2 , 1:$) ; [xmt(:,j) , ymt(:,j)] ]  ;
	     end

           end //** scan the connected links
       end//** end of the connected links
      //**---------------------------------------------------------------------------------------------

    draw(gh_curwin.children); //** draw ALL the moving objects 
    show_pixmap();

    end //** ... of while Interactive move LOOP --------------------------------------------------------------
    //**--------------------------------------------------------------------------------------------------------

    //**-----------------------------------------------
    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-100 then
         [%win,Cmenu] = resume(curwin,'Quit') ;
    end
    //**-----------------------------------------------

    //** OK If update and block and links position in scs_m

    //** if the exit condition is NOT a right button press OR click
    if and(rep(3)<>[2 5]) then //** update the data structure

      //**---------------------------------------------------
      //** Rigid SuperCompund Elements
      block=[];
      for k = sel_block
           block = scs_m.objs(k)    ;
	   xy_block = block.graphics.orig ;
	   xy_block(1) = xy_block(1) + move_x ;
	   xy_block(2) = xy_block(2) + move_y ;
	   block.graphics.orig = xy_block ;
	   scs_m.objs(k) = block; //update block coordinates
      end

      text=[]
      for k = sel_text
           text = scs_m.objs(k)
	   xy_text = text.graphics.orig ;
           xy_text(1) = xy_text(1) + move_x ;
	   xy_text(2) = xy_text(2) + move_y ;
	   text.graphics.orig = xy_text;
	   scs_m.objs(k) = text; //update block coordinates
      end

      link_=[]
      for l = int_link
           link_= scs_m.objs(l)
           [xl, yl] = (link_.xx, link_.yy)
	   xl = xl + move_x ;
	   yl = yl + move_y ;
	   link_.xx = xl ; link_.yy = yl ;
	   scs_m.objs(l) = link_ ; 
      end 
      //**---------------------------------------------------

      //**---------------------------------------------------
      //** Flexible Link elements 
      if connected<>[] then 
          j = 0 ; 
          for l=1:length(connected)
             i  = connected(l)  ;
             oi = scs_m.objs(i) ;
             [xl,from,to] = (oi.xx,oi.from,oi.to);

             if from(1)==ext_block(l) then
               j = j + 1 ;
               oi.xx(1:2) = xmt([2,1],j) ;
               oi.yy(1:2) = ymt([2,1],j) ;
             end

             if to(1)==ext_block(l) then
               j = j + 1 ;
               oi.xx($-1:$) = xmt(:,j) ;
               oi.yy($-1:$) = ymt(:,j) ;
             end
              scs_m.objs(i) = oi ; //** update the datastructure 
           end //... for loop
      end //** of if
      //**---------------------------------------------------

    //**=---> If the user abort the operation
    else //** restore original position of block and links in figure
         //** in this case: [scs_m] is not modified !
      drawlater();

        //** Move back the SuperCompound
        for k = SuperCompound_id 
          gh_k = get_gri(k,o_size(1)); //** calc the handle 
          gh_ToBeMoved = gh_curwin.children.children(gh_k) ;
	  move (gh_ToBeMoved, [-move_x , -move_y]);  //** ..because "move()" works only in differential
        end

	//**-------------------------------------------------------
        if connected<>[] then 
	    xmt(2,:) = xm(2,:);  //** original datas of links
            ymt(2,:) = ym(2,:);
            j = 0 ; //** init
            for l=1:length(connected)
               i  = connected(l)  ;
               oi = scs_m.objs(i) ;
               [xl,from,to] = (oi.xx,oi.from,oi.to);
               gh_link_mod = gh_link_i(l) ; // get the link graphics data structure

               if from(1)==ext_block(l) then
                 tmp_data = gh_link_mod.children.data ;
                 j = j + 1 ;
                 t_xmt = xmt([2,1],j) ;  t_ymt = ymt([2,1],j) ;
                 gh_link_mod.children.data = [ [t_xmt(1) , t_ymt(1)] ; tmp_data(2:$ , 1:$) ];
               end

               if to(1)==ext_block(l) then
                 tmp_data = gh_link_mod.children.data ;
                 j = j +  1 ;
                 gh_link_mod.children.data = [ tmp_data(1:$-2 , 1:$) ; [xmt(:,j) , ymt(:,j)] ];
               end
             end //... for loop
          end //** of if 
         //**------------------------------------------------------
	 
      draw(gh_curwin.children);
      show_pixmap();

    end //**----------------------------------------

endfunction
//**--------------------------------------------------------------------------

