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

function XcosMenuGrid()
  Cmenu = [] ;
  
  if exists('%scicos_with_grid') then
    gh_window = scf(%win);
    gh_axes=gca()

    %scicos_with_grid = ~%scicos_with_grid;
    if %scicos_with_grid then
      if ~exists('%scs_wgrid') then
	gh_axes.grid=ones(1,2)*12
      else
	gh_axes.grid=ones(1,2)*%scs_wgrid(3)
      end
    else
      gh_axes.grid=-ones(1,2);
    end
  end

  drawnow();
endfunction
