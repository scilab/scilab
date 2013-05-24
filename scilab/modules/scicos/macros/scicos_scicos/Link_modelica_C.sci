//  Scicos
//
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
// -----------------------------------------------------------------------------
function ok = Link_modelica_C(Cfile)
    mlibs = pathconvert(modelica_libs, %t, %t);
    Cfile = pathconvert(Cfile, %f, %t)
    name = basename(Cfile);
    path = fileparts(Cfile, "path");

    //  build the list of external functions libraries
    // remove repreated directories from mlibs
    rep = [];
    for k = 1:size(mlibs, "*")
        for j = k + 1:size(mlibs, "*")
            if stripblanks(mlibs(k)) == stripblanks(mlibs(j)) then
                rep = [rep, j];
            end
        end
    end
    mlibs(rep) = [];

    // add dynamic libraries in same directory that .mo
    // compatibility feature with 4.x.x
    libs = [];
    ext = getdynlibext();
    for k = 1:size(mlibs, "*")
        fileSearched = findfiles(mlibs(k), "*" + ext);
        for j = 1:size(fileSearched, "*")
            [pathx, fnamex, extensionx] = fileparts(fileSearched(j));
            libsname = fullfile(pathx, fnamex);
            if getos() == "Windows" then
                libsname = strsubst(libsname, "\", "/");
            end
            libs = [libs; libsname];
        end
    end

    // add modelica_libs to the list of directories to be searched for *.h
    IncludePaths = "";
    extToSearch = ".h";

    for k = 1:size(mlibs, "*")
        pathSearch = mlibs(k);
        pathSearch = strsubst(pathSearch, "\", "/");
        filesFounded = findfiles(pathSearch, "*" + ext);
        if filesFounded <> [] then
            IncludePaths = IncludePaths + "  -I""" + pathSearch + """";
        end
    end

    //** build shared library with the C code
    files = name;
    ok = buildnewblock(name, files, "", "", libs, TMPDIR, "", IncludePaths);

endfunction
// -----------------------------------------------------------------------------
