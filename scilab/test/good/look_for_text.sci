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
// Alan
// Copyright INRIA
function [ok]=look_for_text(gh_compound)
    ok=%f;
    for i=1:size(gh_compound.children,1);
        if gh_compound.children(i).type == "Text" then
            ok=%t;
        elseif gh_compound.children(i).type == "Compound" then
            [ok]=look_for_text(gh_compound.children(i))
        end
        if ok then return, end
    end
endfunction
