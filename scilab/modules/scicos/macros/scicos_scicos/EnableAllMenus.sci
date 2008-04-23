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

function EnableAllMenus()
  %ws=intersect(winsid(),[inactive_windows(2);curwin]')
  %men=menus(1)
  for %w=%ws
    In=find
    for k=2:size(%men,'*')
      // This is dangerous because we assume  Main_Scicos_window is the
      // main diagram. This main no longer be true in the future
      if Main_Scicos_window==%w | %men(k)<>'Simulate' then
	setmenu(%w,%men(k))
      end
    end  // Suppose here all windows have similar menus
  end
endfunction
