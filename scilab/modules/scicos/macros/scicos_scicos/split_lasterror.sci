//  Scicos
//
//  Copyright (C) INRIA - Alan Layec <alan.layec@inria.fr>
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

function str_out=split_lasterror(str_in)
//** Fix the unreadable error messages
//** returned by the simulator.
//** Alan, 11/10/07 : Initial rev
  str_out=str_in;
  ind_bl=strindex(str_in,' ');
  if find(ind_bl>50)<>[] then
    ind_bl2=ind_bl; nind=[];
    for i=1:size(ind_bl,2)
       if ind_bl2(i)>50 then
         nind=[nind;ind_bl(i)];
         ind_bl2=ind_bl2-ind_bl2(i);
       end
    end
    str_out=strsplit(str_in,nind)
  end
endfunction
