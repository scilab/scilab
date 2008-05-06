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

function PlaceinDiagram_()
  Cmenu=[]
  global scs_m_palettes
  %pa_ = TCL_GetVar('blko');
  
  //** pause

  %pa_ = part(%pa_,6:length(%pa_));
  execstr('%pa_=list('+%pa_+')');
  
  if size(%pa_)==1 then
    global ClipboardPal
    ClipboardPal = %pa_(1)
    Cmenu = 'Palettes'
  else
    scs_m = do_placeindiagram(scs_m,scs_m_palettes(scs_full_path(%pa_)))
  end
endfunction
