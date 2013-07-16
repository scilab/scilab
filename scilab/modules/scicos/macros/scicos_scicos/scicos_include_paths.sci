//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - 2010 - Allan CORNET
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

function p = scicos_include_paths(modules)

    if argn(2) < 1 then
        modules = ["scicos_blocks", "dynamic_link", "scicos", "core"];
    end

    p = [];

    if getos() == "Windows" then
        // Windows
        p= """" + SCI + "/modules/" + modules + "/includes" + """";
    else
        //Unix
        if isdir(SCI+"/modules/core/includes/") then
            // source version
            p = SCI + "/modules/" + modules + "/includes";
        elseif isdir(SCI+"/../../include/scilab/core/") then
            // binary version
            p = SCI + "/../../include/scilab/" + modules;
        elseif isdir("/usr/include/scilab/") then
            // packaged version
            p = [];
        end
    end

endfunction
