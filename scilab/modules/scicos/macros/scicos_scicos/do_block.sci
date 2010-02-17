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

function [scs_m] = do_block(%pt,scs_m)
// do_block - edit a block icon
//** win = %win; //** just for memo: '%win' is the clicked window
//
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()
//** 25/06/2009 : Serge Steer, remove links,  text and object which are
//   not in the current window out of the selection

  K=Select(find(Select(:,2)==%win),1) //look for selected blocks in the current window
				      
  if K==[] then
    K = getblock(scs_m, %pt(:))
  end
   				      
  if K==[] then
    messagebox(_("No selected block in the current Scicos window."),'error','modal')
    return
  end
   
  if size(K,'*')>1 then
    messagebox(_("Only one block can be selected in current window for this operation."),'error','modal')
    return
  end    
  o=scs_m.objs(K)
  if typeof(o)<>'Block' then
    messagebox(_("Only  blocks can be selected for this operation."),'error','modal')
    return
  end    
 
 
  gh_curwin = scf(%win) ;  gh_axes = gca(); 

  gr_i = o.graphics.gr_i ; //** isolate the graphics command string 'gr_i'

  if type(gr_i)==15 then //** ? boh
    [gr_i,coli] = gr_i(1:2)
  else
    coli=[] ;
  end

  if gr_i==[] then gr_i=' ',end

  //** Acquire the current clicked window figure and axis handles
  while %t do
    //** use a dialog box to get input
    gr_i = dialog(['Give scilab instructions to draw block';
	 	 'shape.';
		 'orig(1) : block down left corner x coordinate';
		 'orig(2) : block down left corner y coordinate';
		 'sz(1)   : block width';
		 'sz(2)   : block height'],gr_i)

    if gr_i==[] then return; end ; //** no update : EXIT

    mac = null(); deff('[]=mac()', gr_i, 'n')

    if check_mac(mac) then
      o.graphics.gr_i = list(gr_i,coli) ; //** update the graphic command string
      scs_m.objs(K) = o ; //** update the data structure

      //** Alan/Simone 13/12/06 : Use of update_gr 
      o_size = size(gh_axes.children) ;
      gr_k   = get_gri(K, o_size(1)) ; //** compute the index in the graphics data structure 
      drawlater() ;
         update_gr(gr_k, o);
         drawnow();
      break; //** exit from the while loop
    end

  end //** of the while(1)

endfunction
