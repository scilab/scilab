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

function CreateMask_()
Cmenu=[];%pt=[];
if size(Select,1)<>1 | curwin<>Select(1,2) then
   return
end
i=Select(1)
o=scs_m.objs(i)
if typeof(o)=="Block" then
   model=o.model
   graphics=o.graphics;
   if model.sim=='super' then  //
      [params,param_types]=FindSBParams(model.rpar,[])
      bname=model.rpar.props.title(1)
      model.sim='csuper'
      model.ipar=1 ;  // specifies the type of csuper (mask)
      graphics.exprs=list(params,list(params,..
                    ["Set block parameters";params],param_types));     
      graphics.gr_i=list('xstringb(orig(1),orig(2),'"'+..
        bname+''",sz(1),sz(2),''fill'');',8)
      o.model=model;
      o.graphics=graphics;
      o.gui='DSUPER';
      scs_m_save = scs_m    ;
      scs_m.objs(i)=o;
      nc_save = needcompile ;
      needcompile=4  // this is perhaps too conservative
      enable_undo = %t
      edited=%t
      o_size = size(gh_current_window.children.children);
      gh_k=get_gri(i,o_size(1))
      drawlater();
        update_gr(gh_k,o)
        //** draw(gh_current_window.children);
        drawnow(); 
        //** show_pixmap() ; //** not useful on Scilab 5 
   else
      message("Mask can only be created for Super Blocks.")
   end
else
  message("Mask can only be created for Super Blocks.")
end
endfunction
