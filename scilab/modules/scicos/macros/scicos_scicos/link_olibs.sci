//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>   - 2007
//                      - Allan CORNET - 2008
//                      - Rachid Djenidi
//                      - Simone Mannori
//
//  Copyright (C) DIGITEO - Allan CORNET - 2010
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
//**
//------------------------------------------------------------------------
//** link_olibs   : links otherlibs in scilab
//                  for scicos C generated block
//
// Input : libs   : a matrix of string containing path + name
//                 of the libraries
//
//         rpat   : a target directory for temporary generated files
//
// Output : ok    : a boolean variable to say if job has succed
//          libs  : a matrix of string containing path + name
//                  of the libraries
//          for_link : a vector of strings with link cmd
//                     for exec or for loader.sce
//
function [ok, libs, for_link] = link_olibs(libs,rpat)

    //** get lhs,rhs nb paramaters
    [lhs,rhs] = argn(0);

    //** declare  and set local variables
    ok = %t;
    xlibs = [];
    for_link = [],

    //** get out from this function if
    //   there is nothing to do
    if ( (libs == "") | (libs == []) ) then return, end

    for i = 1:size(libs, "*")
        [_path, _name , _ext] = fileparts(libs(i));
        dynlibname = _path + _name + getdynlibext();
        if isfile(dynlibname) then
            for_link = [for_link; dynlibname];
            ierr = execstr("link(for_link($))","errcatch");
            if ierr <> 0 then
                messagebox(_("Cannot load dynamic library: ") + for_link($) ,"modal","error");
                libs = [];
                for_link = [];
                ok = %f;
                return
            else
                if getos() == "Windows" then
                    xlibs = [xlibs; _path + _name + ".lib"];
                else
                    xlibs = [xlibs; libs(i)];
                end
            end
        else
            messagebox([_("Can''t include ") + dynlibname; _("This file does not exist"); lasterror()], "modal", "error");
            libs = [];
            for_link = [];
            ok = %f;
            return
        end
    end

    //** add double quote for include in
    //   Makefile
    libs = xlibs;
    if getos() == "Windows" then
        libs = """" + libs + """";
    else
        libs = "''" + libs + "''";
    end

    //** return link cmd for for_link
    if (for_link <> []) then
        for_link = "link(""" + for_link + """);";
    end

    //** concatenate libs for Makefile
    if (size(libs,1) <> 1) then
        libs = strcat(libs," ");
    end

endfunction
