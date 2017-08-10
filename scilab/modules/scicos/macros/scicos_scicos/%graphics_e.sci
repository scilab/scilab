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

function varargout=%graphics_e(i,o)
    //function used only for backward compatibility of scicos blocks gui
    if i == "flip" || i == "theta" then
        warning(msprintf(_("%s: property ""%s"" obsolete, please use ""%s"" instead.\n"), "graphics", i, "style"));
    else
        warning("Obsolete use of graphics(i) in this scicos block")
    end
    varargout=list()
    for k=1:size(i,"*")
        varargout($+1)=getfield(i(k)+1,o)
    end
endfunction
