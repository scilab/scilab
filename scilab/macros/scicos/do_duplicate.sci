function [scs_m,needcompile,Select] = do_duplicate(%pt,scs_m,needcompile,Select)
// Copyright INRIA
//**   
//** If you double click an object in a palettes windows, a call at "Duplicate_" is generated ->
//** then the "do_duplicate":  - this very function - is is executed : Welcome to the Real Mess :)
//**
  
  win = %win; //** win contains the windows id where you do the double click
  xc = %pt(1); yc = %pt(2); //** acquire the last mouse position
  kc = find(win==windows(:,2)); //** find the window in the active Scicos windows  
  
    if kc==[] then
    //** -----> It is NOT a Scicos window
      message("This window is not an active palette"); 
      k=[]; //** this means that not valid objet was found; used as flag for the rest of the code

    //** -----> It is a PALETTE window
    //** the windows identificator is NEGATIVE : this mean that you are in a valid palette
    elseif windows(kc, 1) < 0 then //** the click was inside a Palette windows (negative index) 
      kpal = -windows(kc,1)   ;    //** identify the palette
      palette = palettes(kpal);    //** select the right palette datastructure
      k = getblocktext(palette,[xc;yc])   //** ?? get the block (object) name_string ??
       if k<>[] then
        o = disconnect_ports(palette.objs(k)) ; 
       end

    //** the double click is the  main window
    elseif win==curwin then //click dans la fenetre courante
      k=getblocktext(scs_m,[xc;yc])
      if k<>[] then
          o=disconnect_ports(scs_m.objs(k)) // mark ports disconnected
      end

    //** double click in a superblock ?
    elseif slevel>1 then
       execstr('k=getblocktext(scs_m_'+string(windows(kc,1))+',[xc;yc])')
       if k<>[] then
         execstr('o=scs_m_'+string(windows(kc,1))+'.objs(k)')
         o=disconnect_ports(o)//mark ports disconnected
       end

    else // in all the other cases 
      message("This window is not an active palette")
      k=[]; 
    
    end //** end of the filter on windows  
   

    // if a valid obect is found 
    if k<>[] then 
      //** if the proper flag is set, the duplicate function is active
      //** the duplicate function move an empty box until the user
      //** choose a destination for the block 

      xinfo("Click where you want object to be placed (right-click to cancel)");

      [xy, sz] = (o.graphics.orig, o.graphics.sz) //** origin and size
      %xc = xy(1);  %yc = xy(2) ; //** default start position
      drawlater() ;
      gh_blk = drawobj(o); //** draw the block (in the buffer) - using the corresponding Interface Function
                          //** Scilab Language - of the specific block (blk) and get back the graphic handler
                          //** to handle the block as a single entity
      draw(gh_blk.parent)
      show_pixmap();

      //**--------------------------------------------------------------------------
      //** ---> main loop that move the empty box until you click
      gh_cw = gcf();
      rep(3)=-1 ; //** mouse move only : no pressing button events
      drawlater();
      while 1 then //** infinite move loop
        //** Exit from the loop condition 
        if or(rep(3)==[0,2,3,5,-5]) then break, end ; //** exit point

        // get new position
        rep = xgetmouse(0,[%t,%t])

        //** Protection from window closing
        if rep(3)==-100 then //active window has been closed
          [%win,Cmenu] = resume(curwin,"Quit")
        end

        xm = rep(1) ; ym = rep(2) ;
        dx = xm - %xc ; dy = ym - %yc ;
        move (gh_blk , [dx dy]);
        draw(gh_blk.parent);
        show_pixmap();
        %xc = xm ; %yc = ym ; //** position update

      end //** of the infinite while loop 
      //**---------------------------------------------------------------------------

    //** someone has closed the active windows
    if get(gh_cw,"figure_id") <> curwin then
      //** active window has been closed
      [%win,Cmenu] = resume(curwin,"Quit") ; 
    end

    //** the user has click or press the mouse right button (ESC)
    if or(rep(3)==[2 5]) then
      delete(gh_blk) ; //** delete the elements from the graphics datastructure
                       //** in order to mantain the coherency
      gh_a = gca();
      draw(gh_a)  ;
      show_pixmap();
      return ;  //**   the program came back with no action
    end

    xinfo(" "); 

    xy = [%xc, %yc]       ;
    o.graphics.orig = xy  ;
    scs_m_save = scs_m    ; 
    nc_save = needcompile ;
    m_obj_pos = size(scs_m.objs) ; //** the return parameter is a matrix
    obj_pos = m_obj_pos(1) + 1 ;
    scs_m.objs($+1) = o ; //** add the object to the data structure
    needcompile = 4 ; 

    Select = [size(scs_m.objs), get(gh_current_window,"figure_id")];

    [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save,nc_save,%t,%t) ; //** 

  end // end of valid object is found 

endfunction

