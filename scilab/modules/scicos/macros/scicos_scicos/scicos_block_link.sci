//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - Clément DAVID <clement.david@scilab.org>
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

function ok = scicos_block_link(funam, txt, flag, libs)
    // Link a new funame block with txt implementation.
    //
    // Parameters:
    //  funam: the name of the function
    //  txt: the code
    //  flag: language type ('c' or 'f')
    //  libs: the libs to link with

    // pre-condition
    if flag <> "c" & flag <> "f" then
        ok = %f;
        messagebox(gettext("Sorry: Only C or FORTRAN languages are supported","modal"));
        return;
    end
    if stripblanks(funam)==emptystr() then
        ok = %f;
        messagebox(gettext("Sorry file name not defined in "+flag+" block","modal"));
        return;
    end
    if ~exists("libs", "l") then
        libs = "";
    end

    // put the txt to a temp file
    cur_wd = pwd();
    chdir(TMPDIR);
    mputl(txt, funam + "." + flag);

    // call the standard block link function
    ok = buildnewblock(funam, funam + "." + flag, "", "", libs, TMPDIR, "", "");

    // post
    chdir(cur_wd);
    ok = %t;
endfunction

