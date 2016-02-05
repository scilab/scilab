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

function hilite_path(path,mess,with_intermediates)
    //hilite a block included in a superblock hierarchy and displays a
    //message.   If required all the intermediate superblocs are displayed
    //first entry of the path must be a bloc of currently opened diagram

    if argn(2)<2 then mess=" ",end

    // retrieve the root diagram
    if isdef("scs_m_s") then
        diagram = scs_m_s;
    elseif isdef("scs_m") then
        diagram = scs_m;
    else
        error("scs_m variable does not exist");
    end

    uid = []; // empty uid path

    // construct the uid path from the index path
    for i = 1:length(path)
        index = path(i);
        if typeof(diagram.objs(index)) == "Block" then
            o = diagram.objs(index);

            // masked superblock case
            if isempty(o.model.uid) then
                // we abort because blocks do not have uuid inside
                // masked superblocks
                break;
            end

            // normal case
            uid($+1) = diagram.objs(index).model.uid + "";

            if diagram.objs(index).model.sim == "super" then
                diagram = diagram.objs(index).model.rpar;
            else
                break;
            end
        end
    end

    if uid <> [] then
        warnBlockByUID(uid, strcat(mess, " "));
    end
endfunction

