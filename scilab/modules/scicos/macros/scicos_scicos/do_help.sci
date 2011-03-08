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

function do_help()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//**
//** 17 July 2007: this code handle the "help" for Scicos blocks and menu.   
//** Sept 2009, S Steer INRIA : Adaptation to new menu mechanism 
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
      mprintf("do_help: Cmenu=%s\n",Cmenu)
      DisableAllMenus()
      //**         Click                 Press       
      if (Cmenu<>"XcosMenuSelectLink") & (Cmenu<>"XcosMenuMoveLink") then
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

  //** Display the help 
  mprintf("do_help: %s\n",name)
  help(name);
endfunction




