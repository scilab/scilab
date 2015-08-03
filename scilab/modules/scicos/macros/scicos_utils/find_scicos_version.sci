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

function [scicos_ver]=find_scicos_version(scs_m)
    //**
    //** find_scicos_version try to retrieve a version of a
    //** scs_m
    //**
    //** 21/08/07: Alan, inital revision
    //**

    if ~exists("scicos_ver") then
        scicos_ver = "scicos2.2";
    else
        scicos_ver = scicos_ver;
    end

    if type(scs_m)==17 || type(scs_m)==128 then
        //case for diagrams >= 4.2
        if find(getfield(1,scs_m)=="version")<>[] then
            if scs_m.version<>"" then
                scicos_ver=scs_m.version
            else
                n=size(scs_m.objs);
                for j=1:n //loop on objects
                    o=scs_m.objs(j);
                    if typeof(o)=="Block" then
                        if find(getfield(1,o.model)=="equations")<>[] then
                            scicos_ver = "scicos2.7.3"
                            break;
                        else
                            //** the last version supported here is scicos2.7
                            //** other tests can be done
                            scicos_ver = "scicos2.7"
                            break;
                        end
                    end
                end //** end for
            end
            //case for diagrams < 4.2
        else
            n=size(scs_m.objs);
            for j=1:n //loop on objects
                o=scs_m.objs(j);
                if typeof(o)=="Block" then
                    if find(getfield(1,o.model)=="equations")<>[] then
                        scicos_ver = "scicos2.7.3"
                        break;
                    else
                        //** the last version supported here is scicos2.7
                        //** other tests can be done
                        scicos_ver = "scicos2.7"
                        break;
                    end
                end
            end //** end for
        end
    end

endfunction
