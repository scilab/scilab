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

function XcosMenuRemoveMask()
  
  K=find(Select(:,2)==%win)
  if K==[] then
    K = getblock(scs_m, %pt(:))
  else
    K=Select(K,1)
  end
  Cmenu=[];%pt=[];

  if K==[] then
    messagebox(_("No selected block in the current Scicos window."),'error','modal')
    return
  end  	
  if size(K,'*')>1 then
    messagebox(_("Only one block can be selected in current window for this operation."),'error','modal')
    return
  end 
  i=K
  o=scs_m.objs(i)
  if typeof(o)=='Block' then
    if o.model.sim=='csuper' & isequal(o.model.ipar,1)  then  
      o.model.sim='super'
      o.model.ipar=[] 
      o.gui='SUPER_f'
      o.graphics.exprs=[]      
      scs_m_save = scs_m    ;
      scs_m.objs(i)=o;
      nc_save = needcompile ;
      needcompile=4  // this is perhaps too conservative
      enable_undo = %t
      edited=%t
    else
      messagebox('This block is not masked.','modal')
    end
  else
    messagebox('Select a block.','modal')
  end
endfunction
