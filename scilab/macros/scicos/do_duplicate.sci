function [scs_m,needcompile] = do_duplicate(%pt,scs_m,needcompile)
// Copyright INRIA
//** Comments by Simone Mannori
//** If you double click an object in a palettes windows, a call at "Duplicate_" is generated ->
//** then the "do_duplicate":  - this very function - is executed : welcome to the real mess :) 
//**

  xinfo('Click where you want object to be placed (right-click to cancel)');
  
  win = %win; //** win contains the windows id where you do the double click
  
  xc = %pt(1); yc = %pt(2); //** acquire the last mouse position 
  
  kc = find(win==windows(:,2))
  
  if kc==[] then 
    //** -----> It is NOT a palette window
      message('This window is not an active palette')
      k=[]; //** used as flag for the rest of the code 

    //** -----> It is a PALETTE window
    //** the windows identificator is NEGATIVE : this mean that you are in a valid palette 
    elseif windows(kc, 1) < 0 then //click dans une palette
      kpal = -windows(kc,1)   ; //** identify the palette   
      palette = palettes(kpal); //** select the right palette datastructure 
      k = getblocktext(palette,[xc;yc])   //** ?? get the block (object) name_string ?? 
       if k<>[] then 
        o = disconnect_ports(palette.objs(k)),
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
    
   else
      message('This window is not an active palette')
      k=[]
  end
  
  
  if k<>[] then
  //** if the proper flag is set, the duplicate function is active
  //** the duplicate function move an empty box until the user 
    
    //** xset('window',curwin); //** put the focus 
    //** dr=driver()
    //** if dr=='Rec' then driver('X11'),end
    
    [xy, sz] = (o.graphics.orig, o.graphics.sz) //** origin and size
    
    %xc = xy(1);  %yc = xy(2) ; //** default start position
    
    // %xc = %pt(1); %yc = %pt(2); //** default start position 
    
    gh_blk = drawobj(o); //** draw the block (in the buffer) - using the corresponding Interface Function
                         //** Scilab Language - of the specific block (blk) and get back the graphic handler
	                 //** to handle the block as a single entity 

//**--------------------------------------------------------------------------
//** ---> main loop that move the empty box until you click        
    gh_cw = gcf(); gh_cw.pixmap='on';
    rep(3)=-1 ; //** mouse move only : no pressing button events 
    while rep(3)==-1 then //move loop
        
	// get new position
        rep = xgetmouse(0,[%t,%t])
  
        //** Protection from window closing
        if rep(3)==-100 then //active window has been closed
          [%win,Cmenu] = resume(curwin,'Quit')
        end
  
        xm = rep(1) ; ym = rep(2) ;
     
        dx = xm - %xc ; dy = ym - %yc ;
     
        drawlater();
  
          move (gh_blk , [dx dy]);
     
        drawnow();
     
        show_pixmap();
   
        %xc = xm ; %yc = ym ; //** position update 

	// draw block shape
        // xrect(xc,yc+sz(2),sz(1),sz(2)) //** the empty box 
        // if pixmap then xset('wshow'),end
        // get new position
        // rep=xgetmouse(0)
        // if rep(3)==-100 then //active window has been closed
	//           driver(dr);
        //    [%win,Cmenu]=resume(curwin,'Quit')
        //end
        // 
        //// clear block shape
        //xrect(xc,yc+sz(2),sz(1),sz(2))
        //xc=rep(1);yc=rep(2)
        //
        //xy=[xc,yc]; //** update the position with the current coordinate  
    end
//**---------------------------------------------------------------------------    

    //** someone has closed the active windows 
    if xget('window')<>curwin then
      //active window has been closed
      [%win,Cmenu] = resume(curwin,'Quit')
    end
    
    //** the user has click or press the mouse right button (ESC)   
    if or(rep(3)==[2 5]) then
       drawnow(); show_pixmap();
      // if pixmap then xset('wshow'),end
      return ;       //**   the program came back with no action 
    end
    
    xinfo(' ')
    
    xy = [%xc, %yc]      ;
    o.graphics.orig = xy ; 
    
    scs_m_save = scs_m,nc_save=needcompile ;
    
    m_obj_pos = size(scs_m.objs) ; //** the return parameter is a matrix
    
    obj_pos = m_obj_pos(1) + 1   ;
    
    scs_m.objs($+1) = o ; //** add the object to the data structure
    
    needcompile = 4     ; 
    
    [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save,nc_save,%t,%t) ; //** ? ? ? 
    
  
  end

endfunction

