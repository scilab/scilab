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

function CustomizeMask_()
Cmenu=[];%pt=[];
if size(Select,1)<>1 | curwin<>Select(1,2) then
   return
end
i=Select(1)
o=scs_m.objs(i)
if typeof(o)=='Block' then
   if o.model.sim=='csuper' & o.model.ipar==1 then 
      items=o.graphics.exprs(2)(1)
      result=tk_mdialog(['Customize block GUI:';'Modify title and menu labels.'],..
      ['Title of the GUI';items],[o.graphics.exprs(2)(2);items])

      if result<>[] & ~isequal(items,result) then
         o.graphics.exprs(2)(2)=result;
         scs_m.objs(i)=o;
         edited=%t
      end
   else
      message('This block is not masked.')
   end
else
  message('Select a block.')
end
endfunction
