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

function MoveLink_()

//** This function is activated by PRESSING the LEFT mouse buttom 
 
  
  //** -------------- NAVIGATOR -------------------------------------------- 
  //** the press is inside a navigator window
  if windows(find(%win==windows(:,2)),1)==100000 then //** Navigator window:
    Cmenu=[] ; %pt=[] ; //** clear the variable 
    return; //** and exit 
    
  //** ------------------ NOT in the current window ------------------------
  elseif %win<>curwin then //** the press is not in the current window 
    
    kc = find(%win==windows(:,2)); 
    if kc==[] then //** no valid block has be found 
      Cmenu=[]; %pt=[]; return
    elseif windows(kc,1)<0 then //** the press is inside a palette 
      kpal = -windows(kc,1)    ; 
      palette = palettes(kpal) ;
      %kk = getobj(palette,%pt) ; //** get the obj inside the palette 
    elseif slevel>1 then //** the press is over a block inside a superblock window
      execstr('%kk=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    end
    
    if %kk<>[] then //** press over a valid block 
        Cmenu="Duplicate"
        Select=[%kk,%win]
    else           //** press in the void   
        Cmenu="SelectRegion"
        Select=[];
    end
  
  //** The PRESS is in the CURRENT window -----------------------------------    
  else //** ... the press is in the current window 

    //** look for an object 
    %kk = getobj(scs_m,%pt)
    
    //** if an object id found 
    if %kk<>[] then
      ObjSel = size (Select) ; //** [row, col]
      ObjSel = ObjSel(1) ; //**  row 
			   //**--------------------------------------------------------------
      if ObjSel<=1 then //** with zero or one object already selected 
        
	Cmenu = check_edge(scs_m.objs(%kk),"Move",%pt);
	//** N.B. if the click is over an output port [Cmenu = "Link"]       
        
	if Cmenu=="Link" then
	  Select = []; //** Execute "Link" : deselct any previously selected object
	else 
	  Select = [%kk, %win]; //** Execute "Move" with the object selected 
	end 
	
      else //** more than one object is selected 
        SelectedObjs = Select(:,1)'; //** isolate the object column and put in a row 
	if or(%kk==SelectedObjs) then //** check if the user want to move the aggregate
	  Cmenu = "Move";
	  //** Select information is preserved    
	else
	  Cmenu  = "Move";
	  Select = [%kk, %win]; //** user want to move only the object in the focus
	end 
	
      end    
      //**---------------------------------------------------------------
      
    else //** if the press is in the void of the current window 

      Cmenu = "SelectRegion" ; //** "SelectRegion" will be called later 
      %ppt = []; Select = [] ; 

    end
  
  end

endfunction


