function do_help()
// Copyright INRIA
//**
//** 17 July 2007: this code handle the "help" for Scicos blocks and  menu.   
//** 
//** BEWARE: this code use an ot of standard modalily because can recall cosclick();
//** 
//** nm = 0 ; -> the help is relative to a block 
//**
//** nm = 1 ; -> the help is realative to a menu: this operating mode is not yet implemented 
//**
  
//**--------------------------------------------------------------------------

cwin = %win ;
  
sel_items = size(Select)   ; 

obj_selected = sel_items(1) ; 

if obj_selected==0 then 
  //** ---------------- No object selected ---------------------------------   
  while %t do //** infinite while 
    EnableAllMenus()
    [btn, %pt, cwin, Cmenu] = cosclick();
    DisableAllMenus()
    //**         Click                 Press       
    if (Cmenu<>"SelectLink") & (Cmenu<>"MoveLink") then
      name = Cmenu ;
      nm = 1 ;
      break ; //** exit point-> go to visualize  
    //else
	// cwin = curwin ;
    end
    
    //** in the current window 
    if cwin==curwin then
      // disp("cn");
      xc = %pt(1); yc = %pt(2); %pt=[]
      k = getobj(scs_m,[xc;yc])
      if k<>[] then
	o = scs_m.objs(k)
        if typeof(o)=="Block" then 
	  name = o.gui ; //** recover the block's name 
	  nm = 0       ; //** help on block 
	else
	  return; //** EXIT: it is not a block
	end
	
	break ; //** exit point-> go to visualize 
      
      end
    
    //** in a palette window     
    elseif or(windows(find(windows(:,1)<0),2)==cwin) then
      // disp("pn");
      kwin = find(windows(:,2)==cwin)
      pal  = palettes(-windows(kwin,1))
      xc = %pt(1); yc = %pt(2); %pt=[]
      k  = getobj(pal,[xc;yc])
      if k<>[] then
	o = pal.objs(k);
        if typeof(o)=="Block" then 
	  name = o.gui ; //** recover the block's name 
	  nm = 0       ; //** help on block 
	else
	  return; //** EXIT: it is not a block
	end
	
	break ; //** exit point-> go to visualize 
      
      end
    
    //** in other cases     
    else
      return; //** EXIT  
    end
    
  end //** of the infinite while loop 
  //**--------------------------------------------------------------------------

else //** at least an object is selected

    //**
    cwin=Select(1,2)
    //** in the current window 
    if cwin==curwin then
      // disp("cs");
      k = Select(1,1)
      o = scs_m.objs(k)
      if typeof(o)=="Block" then 
	 name = o.gui ; //** recover the block's name 
	 nm = 0       ; //** help on block 
      else
	  return; //** EXIT: it is not a block
      end 
      
    
    //** in a palette window     
    elseif or(windows(find(windows(:,1)<0),2)==cwin) then
      // disp("ps");
      kwin = find(windows(:,2)==cwin)   ;
      pal  = palettes(-windows(kwin,1)) ;
      k = Select(1,1) ;
      o = pal.objs(k);
      if typeof(o)=="Block" then 
	name = o.gui ; //** recover the block's name 
	nm = 0       ; //** help on block 
      else
	  return; //** EXIT: it is not a block
      end 
         
    //** in other cases     
    else
      return; //** EXIT  
    end

end //** of the No Selection / Selection case
//**-------------------------------------------------------------

  //** Display the help 
  if nm==0 then
    
    help(name);
    
  else //** nm==1 ; 

    if execstr('mess = %scicos_help.menu(name)' , 'errcatch') <>0 then
      if execstr('mess = %scicos_help(name)' , 'errcatch') <>0 then
        mess = "No help available on this topic. Sorry.";
      end
    end
  
    message(mess); //** show the message   
  
  end 
  
endfunction




