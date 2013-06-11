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
// See the file ./license.txt
//
mode(-1)
//this script is used to compile Modelica models of scicos implicit
//blocks
if ~isdef("genmoc_verbose") then
    genmoc_verbose=%f
end

if ~with_modelica_compiler() then quit,end
models=stripblanks(models);

// modelicac is defined in PATH environment variable

compilername = getmodelicacpath() + "modelicac";

for k=1:size(models,1)
    M=models(k)
    n=newest(M,M+"c",compilername)
    if n<>2 then
        write(%io(2),"Processing file "+M)
        if genmoc_verbose then
            ierr=unix(compilername+" -c "+M+" -o "+M+"c")
            if ierr<>0 then
                mprintf("------------------------- "+M+..
                " Compilation error detected")
                break,
            end
        else
            // error will stop unix_s in case of error
            unix_s(compilername+" -c "+M+" -o "+M+"c")
        end
    end
end
