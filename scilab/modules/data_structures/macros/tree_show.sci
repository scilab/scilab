//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2019 - Samuel GOUGEON
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

function tree_show(x, titleRoot, styles, arrayByFields)
    // x : list, cells or structs array, tlist, mlist, Xcos block
    // titleRoot: single string: label of the tree root, for instance
    //     the name of x in the calling environment
    // styles: single string = HTML 4 tag styling either the address of
    //   each data (fields names, lists indices..), or their content, or both.
    //   In the string, "$" is replaced with the address.
    //   Example: "<b>$</b>"  where $ will be replaced with the address.
    // arrayByFields: single boolean (when x is an array of structs).
    //   %T => For each field: the array of its values is displayed.
    //   %F => For each array component, all its fields are displayed.

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    if argn(2) < 1 then
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.\n")
        error(sprintf(msg, "tree_show", 1, 4));
    end

    if and(type(x)<>[15 16 17 128]) then
        msg = _("%s: Argument #%d: Container or Xcos object expected.\n")
        error(msprintf(msg, "tree_show", 1))
    end

    if ~isdef("titleRoot","l") then
        titleRoot = ""
    end

    if ~isdef("arrayByFields","l") then
        arrayByFields = %f
    elseif type(arrayByFields) <> 4
        msg = _("%s: Argument #%d: Boolean(s) expected.\n")
        error(msprintf(msg, "tree_show", 3))
    else
        arrayByFields = arrayByFields(1)
    end

    if ~isdef("styles","l") then
        styles = "<font color=""blue"">$</font>"
    elseif type(styles) <> 10
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, "tree_show", 4))
    else
        styles = styles(1)
    end

    // BUILDING THE TREE
    // -----------------
    tree = list2tree(x, titleRoot, styles, arrayByFields)

    // Displaying it:
    uiDisplayTree(tree);
endfunction
