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
function [libss,ok,cancel]=get_dynamic_lib_dir(tt,funam,flag)
    // Copyright INRIA
    cancel=%f
    cur_wd = pwd();
    chdir(TMPDIR);
    mputl(tt,funam+"."+flag);
    libss="";
    label="";
    ok=%f
    while ~ok then
        [ok,libss,label]=scicos_getvalue("Linking the "+funam+" function","External libraries (if any)",list("str",1),label);
        if ~ok then chdir(cur_wd);cancel=%t,return;end
        // for multiple libraries
        if strindex(libss,"''")<>[] | strindex(libss,"""")<>[] then
            ierr=execstr("libss=evstr(libss)","errcatch")
            if ierr<>0  then
                messagebox(["Can''t solve other files to link"],"modal")
                chdir(cur_wd);
                ok=%f;
            end
        else
            libss=tokens(libss,[" ",";"])
        end
        for i=1:size(libss,"*")
            lib_dll=libss(i) + getdynlibext();
            ifexst=fileinfo(lib_dll)
            if ifexst==[] then messagebox ("the library "+lib_dll+" doesn''t exists","modal");ok=%f;end
        end
    end
    chdir(cur_wd);
endfunction
