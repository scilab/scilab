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

function [%pt, scs_m] = do_resize(%pt, scs_m)
//
//**
//** Change the size (w,h) of a Block
//** 02/12/06-@l@n- : use of objects permutation in
//**                  gh_curwin.children.children()
//** 8 Aug 2007 : improved version 

  win = %win;

  //** Fiter out the multiple object selected / not in current window cases
  if Select==[] then //** if no object is already selected
    xc  = %pt(1); //** acquire the last "cosclick" position and
    yc  = %pt(2);
    %pt = []    ; //** look for a object
    K = getblocklink(scs_m,[xc;yc]) ;
  else
    K   = Select(:,1)';
    %pt = [] ;
    if size(K,'*')>1 | %win<>Select(1,2) then
      message("Only one block can be selected in current window for this operation.")
      Cmenu=[]; %pt=[]; return ; //** ---> EXIT
    end
  end

  //** filter the object type
  if K<>[] then

    if typeof(scs_m.objs(K))=="Block" then
    //** ----- Block ------------------------------------------
      
      //** save the diagram for  
      scs_m_save = scs_m ; //** ... for undo ...
    
      path = list('objs', K)      ; //** acquire the index in the "global" diagram

      o = scs_m.objs(K) ;  //** scs_m
      
      o_n  = scs_m.objs(K) ; //** old object is a copy of the new one 
      
      graphics = o.graphics    ;
      sz       = graphics.sz   ;
      orig     = graphics.orig ;

      [ok,w,h] = getvalue('Set Block sizes',['width';'height'],..
	  	           list('vec',1,'vec',1),string(sz(:)))
      //** in case of valid (w,h)
      if ok  then
         //**--------- scs_m object manipulation -------------------  
         graphics.sz   = [w;h] ;
         graphics.orig = orig  ;
         o_n.graphics  = graphics ;
  
         scs_m = changeports(scs_m, path, o_n); 
		 
      end //** of ok

    elseif typeof(scs_m.objs(K))=="Link" then
    //** ----- Link ------------------------------------------
      //** extract some link parameter
      [pos,ct] = (scs_m.objs(K).thick, scs_m.objs(K).ct) ;
      Thick = pos(1) ;
      Type  = pos(2) ;
      //** open a dialog box to edit
      [ok,Thick,Type] = getvalue('Link parameters',['Thickness';'Type'],..
			          list('vec','1','vec',1),[string(Thick);string(Type)])
      if ok then
	edited = or(scs_m.objs(K).thick<>[Thick,Type]) ; //** set flag if the parm are edited
	scs_m.objs(K).thick = [Thick, Type]            ; //** update the 'scs_m' data structure
        //** update for new graphics (Scilab 5)
        drawlater() ;
          gh_curwin = scf(%win) ;
          gh_axes   = gca(); 
          o_size = size(gh_axes.children); 
          gr_k = get_gri(K, o_size(1)) ; 
          gh_axes.children(gr_k).children(1).thickness = maxi(scs_m.objs(K).thick(1) , 1) * maxi(scs_m.objs(K).thick(2), 1) ;
          //** draw(gh_curwin.children);
        drawnow(); 
        //** show_pixmap() ; //** not useful on Scilab 5
      end

    else
    //** nor Block nor Link
      message("Resize is allowed only for Blocks or Links.")
    end

  else
    //** no valid object is found then ...
    Cmenu=[];

  end //** of "I found an object"

endfunction
