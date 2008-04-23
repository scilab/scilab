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

function [%vv_list,%ierr_vec]=context_evstr(%str,%scicos_context)
// Evaluate Scicos Context utility function
 %mm=getfield(1,%scicos_context)
 for %mi=%mm(3:$)
   if execstr(%mi+'=%scicos_context(%mi)','errcatch')<>0 then
     disp(lasterror())
     %ok=%f
     return
   end
 end
 %nn=prod(size(%str))
 %ierr_vec=zeros(%nn,1)
 %vv_list=list()
 for %kk=1:%nn
   [%vv_list(%kk),%ierr_vec(%kk)]=evstr(%str(%kk))
 end
endfunction
