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

function [scs_m,needcompile,Select] = do_duplicate(%pt,scs_m,needcompile,Select)
//**   
//** If you double click an object in a palettes windows, a call at "Duplicate_" is generated ->
//** then the "do_duplicate":  - this very function - is executed : Welcome to the Real Mess :)
//**

  win = %win; //** win contains the windows id where you have activated this function
  xc = %pt(1); yc = %pt(2); //** acquire the last mouse position
  kc = find(win==windows(:,2)); //** find the window in the active Scicos windows  
  
  selected=[]; //will contain object to be duplicated
  if kc==[] then
    //** -----> It is NOT a Scicos window
    messagebox(_("This window is not an active palette","error","modal")); 
    k=[]; //** this means that not valid objet was found; used as flag for the rest of the code

    //** -----> It is a PALETTE window
    //** the windows identificator is NEGATIVE : this mean that you are in a valid palette
  elseif windows(kc, 1) < 0 then //** the click was inside a Palette windows (negative index) 
    kpal = -windows(kc,1)   ;    //** identify the palette
    palette = palettes(kpal);    //** select the right palette datastructure
    k = getblocktext(palette,%pt(:))   //** ?? get the block (object) name_string ??
    if k<>[] then
      o = disconnect_ports(palette.objs(k)) ; 
      if o.gui=='PAL_f' then
	[palettes,windows] = do_load_as_palette(palettes,windows,o.model.rpar)
	[palettes,windows] = resume(palettes,windows)
      end
      selected=o
    end

    //** the  click is the  main window
  elseif win==curwin then 
    K=find(Select(:,2)==curwin)
    if K==[] then
      k = getblocktext(scs_m,%pt(:)); 
      if k<>[] then
	selected = disconnect_ports(scs_m.objs(k)) //** mark ports as disconnected
      end
    elseif size(K,'*')==1 then
      k=K
    else //multiple selection
      selected = get_multiple_selection(Select(K,:))
    end
    //** in a superblock ?
  elseif slevel>1 then //I was not able to reach this case! S. Steer
    execstr('k=getblocktext(scs_m_'+string(windows(kc,1))+',%pt(:))')
    if k<>[] then
      execstr('o=scs_m_'+string(windows(kc,1))+'.objs(k)')
      selected=disconnect_ports(o)//mark ports disconnected
    end

  else // in all the other cases 
    messagebox(_("This window is not an active palette"),"error","modal")
    k=[]; 
    
  end //** end of the filter on windows  
   
  if isequal(selected,[]) then return,end

  // a valid selection is found 

  //** the duplicate function move an empty box until the user
  //** choose a destination for the block 

  xinfo(_("Click where you want object to be placed (right-click to cancel)"));
  if typeof(selected)<>"diagram" then //simple selection
    o=selected
    selected=scicos_diagram()
    selected.objs(1)=o
  end
  //compute initial position

  o=selected.objs(1)
  [xy, sz] = (o.graphics.orig, o.graphics.sz) //** origin and size
  %xc = xy(1);  %yc = xy(2) ; //** default start position
  drawlater() ;
  //** draw the block(s) (in the buffer)  and get back the graphic handle(s)
  H=[];  for o=selected.objs, H=[H,drawobj(o)];end

  drawnow(); 

  //**--------------------------------------------------------------------------
  //** ---> main loop that move the objects until you click
  gh_cw = gcf();
  rep(3)=-1 ; //** mouse move only : no pressing button events
  while 1 then //** infinite move loop
      
    //** Exit from the loop condition 
    if or(rep(3)==[0,2,3,5,-5]) then
      break ; //** exit point
    end ; 

    //** get new position
    [rep, m_win] = xgetmouse([%t,%t]); //** 
      
    //** Protection from window closing
    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu] = resume(curwin,"XcosMenuQuit")
    end

    if m_win==curwin then //** if the mouse is in the current window 
      xm = rep(1) ; ym = rep(2) ;
      dx = xm - %xc ; dy = ym - %yc ;

      //** move the object(s)
      drawlater();
      for k=1:size(H,'*'),move (H(k) , [dx dy]); end
      drawnow();
	
      %xc = xm ; %yc = ym ; //** position update
    end 
      


    end //** of the infinite while loop 
    
    //**---------------------------------------------------------------------------
      
    //** someone has closed the active windows
    if get(gh_cw,"figure_id") <> curwin then
      //** active window has been closed
      [%win,Cmenu] = resume(curwin,"XcosMenuQuit") ; 
    end

    //** the user has click or press the mouse right button (ESC)
    if or(rep(3)==[2 5]) then
      drawlater(); 
      //** delete the elements from the graphics datastructure 
      //   in order to mantain the coherency
      for k=1:size(H,'*'),delete(H(k)); end
      drawnow();
      xinfo(" "); 
      return ;  //**   the program came back with no action
    end

    xinfo(" "); 
    scs_m_save = scs_m    ; 
    //place the duplicated object in the scs_m data structure, taking
    //care of the new position and new numbering
    dx=%xc-xy(1);dy=%yc-xy(2);
    m_obj = size(scs_m.objs) ; 
    k=m_obj;
    for o=selected.objs
      if or(typeof(o)==["Block","Text"]) then
	o.graphics.orig=o.graphics.orig+[dx dy]
	if o.graphics.pin<>[] then
	  nz=find(o.graphics.pin<>0)
	  o.graphics.pin(nz)=o.graphics.pin(nz)+m_obj;
	end
	if o.graphics.pout<>[] then
	  nz=find(o.graphics.pout<>0)
	  o.graphics.pout(nz)=o.graphics.pout(nz)+m_obj;
	end
	if o.graphics.pein<>[] then
	  nz=find(o.graphics.pein<>0)
	  o.graphics.pein(nz)=o.graphics.pein(nz)+m_obj;
	end
	if o.graphics.peout<>[] then
	  nz=find(o.graphics.peout<>0)
	  o.graphics.peout(nz)=o.graphics.peout(nz)+m_obj;
	end
	scs_m.objs($+1) = o ;
    elseif typeof(o)=="Link" then
	o.xx=o.xx+dx;
	o.yy=o.yy+dy
	if o.from<>[] then o.from(:,1)=o.from(:,1)+m_obj;end
	if o.to<>[] then o.to(:,1)=o.to(:,1)+m_obj;end
	scs_m.objs($+1) = o ;
      end
      //fill the selection with this new created objects
      Select=[];
      for k=m_obj+1:size(scs_m.objs)
	Select=[Select;k curwin]
      end
    end
    
    nc_save = needcompile ;
    needcompile = 4 ; 
    [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save,nc_save,%t,%t) ; //** 
endfunction

