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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function loadpallibs()
    //to be called by exec(loadpallibs)
    if ~(exists("scicos_pal_libs")) then
        [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();
    end
    for d = scicos_pal_libs
        execstr(["if exists(''scs"+convstr(d)+"lib'')==0 then"
        "  load(''SCI/modules/scicos_blocks/macros/"+d+"/lib'');"
        "end"]);
    end
    clear d
endfunction
