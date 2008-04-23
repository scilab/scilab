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

function [in,out]=get_blocks_in_rect(scs_m,ox,oy,w,h)
//
//** 11 Jan 2007 : 'Block' / 'Text' bug validation: this function is OK.
//** 
  in=[]; out=[]
  
  for k=1:lstsize(scs_m.objs)
    o = scs_m.objs(k)
    
    if typeof(o)=='Block'|typeof(o)=='Text' then //** OK 
      x = [0 1 1 0]*o.graphics.sz(1)+o.graphics.orig(1)
      y = [0 0 1 1]*o.graphics.sz(2)+o.graphics.orig(2)
      ok = %f
      
      for kk=1:4
	data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
	if data(1)<0&data(2)<0 then ok=%t;in=[in k];break;end
      end
      
      if ~ok then out=[out k], end
    
    end //** filter 'Block' OR 'Text'
    
  end //** ... of scan 
endfunction
