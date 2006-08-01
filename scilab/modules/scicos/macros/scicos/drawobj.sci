function gh_blk = drawobj(o)
//
// Copyright INRIA
//!
//** Comments by Simone Mannori 
//
//** 22 May 2006 : this function IS substantially different from the standard
//**               SCICOS because came back
//**               with the graphic handler of the object drawn 
//
//** 07 June 2006: adding of an empty box for the Select / HighLith operations 
//**
//** the code below is modified according the new graphics API
//  
  gh_blk = [] ; //** default value 
  gh_curwin = gh_current_window ; 
  
  if typeof(o)=='Block' then //** Block draw 
    //** ---------------------- Block -----------------------------
  
    o_size = size ( gh_curwin.children.children ) ; //** initial size
 
      //** Block drawing works throught call (execstr) the block function 
      ierr = execstr(o.gui+'(''plot'',o);','errcatch')
    
     [orig, sz, orient] = (o.graphics.orig, o.graphics.sz, o.graphics.flip) ;
     
     //** orig = [0,0]
     //** sz = [40,40]
     sel_x = orig(1) ; sel_y = orig(2)+sz(2) ;
     sel_w = sz(1)   ; sel_h = sz(2)   ;
       
      xrect(sel_x, sel_y, sel_w, sel_h);
      gh_e = gce();
      gh_e.mark_background = -1 ;
      gh_e.mark_mode = "on"     ;
      gh_e.mark_style = 11      ;
      gh_e.line_mode="off"      ;
      gh_e.visible = "off"      ;  

    p_size = size ( gh_curwin.children.children ) ; //** size after the draw 
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1) ;
    gh_blk = glue( gh_curwin.children.children(d_size:-1:1) ) ; 
 
    if ierr<>0 then 
      message(['Block '+o.gui+ ' not defined'; 'You must leave scicos and define it now.']) ;
      gh_blk = [];
    end
    
  //** ---------- Link -------------------------------    
  elseif typeof(o)=='Link' then //** Link draw 
    
    //**  ------ Put the new graphics here --------------------------------
    //**
    o_size = size ( gh_curwin.children.children ) ; //** initial size
      
        xpoly(o.xx, o.yy,'lines') ;
        gh_e = gce();
        gh_e.thickness = maxi( o.thick(1) , 1) * maxi(o.thick(2), 1) ; //** thickness
        gh_e.foreground = o.ct(1);         //** color
	gh_e.mark_style = 11       ;  
        gh_e.mark_mode = "off"     ;
    p_size = size ( gh_curwin.children.children ) ; //** size after the draw 
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1) ;
    gh_blk = glue( gh_curwin.children.children(d_size:-1:1) ) ; 
     
  //** ---------- Deleted ----- CAUTION: also "Deleted object MUST be draw ! ----    
  elseif typeof(o)=='Deleted' then //** Dummy "deleted" draw     
    
    //** draw a dummy object 
    
    xpoly ([0;1],[0;1]) ; //** just a dummy object 
    gh_blk = glue( gh_curwin.children.children(1) ) ; //** create the Compound macro object 
    gh_blk.visible = "off" ; //** set to invisible :)
     
  //** ---------- Text -------------------------------  
  elseif typeof(o)=='Text' then //** Text draw 
    
    //**  ------ Put the new graphics here -----------
    //**
    execstr(o.gui+'(''plot'',o)')
  
  end //** of the main if  
  
  //** disp("gh_DataStructure --> "); //** debug only
  //** disp( gh_curwin.children.children);
  
endfunction
