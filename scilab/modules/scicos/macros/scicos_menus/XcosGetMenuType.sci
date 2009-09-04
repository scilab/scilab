//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <Serge.Steer@inria.fr>
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
function [itype, mess] = XcosGetMenuType(Cmenu)
//** look inside "%CmenuTypeOneVector" if the command is type 1 (need both Cmenu and %pt)
  k = find (Cmenu == %CmenuTypeOneVector(:,1));
  if k==[] then //** if is not type 1 (empty k)
    itype = 0 ; //** type zero
    mess=''   ; //** set message to empty
    return    ; //** --> EXIT point : return back
  end
  nm= size(k,'*')
  if nm>1 then //** if found more than one command
    messagebox(msprintf(_("Warning %d menus have identical name %s\n"),nm,Cmenu),"modal");
    k = k(1); //** extract the index
  end

  itype = 1 ; //** type one

  mess = %CmenuTypeOneVector(k,2) ; //** use the index to recover the message

endfunction
