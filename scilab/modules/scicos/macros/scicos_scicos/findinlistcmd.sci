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

function [path]=findinlistcmd(L,v,oper,path)
    //recherche si un element de valeur v existe dans la liste L
    global paths
    if and(type(L)<>(15:17)) then error("First argument should be a list"),end
    firstlevel=argn(2)<4
    if firstlevel then paths=list(),path=[];end
    for k=1:size(L)
        l=L(k)
        if or(type(l)==(15:17)) then
            findinlistcmd(l,v,oper,[path,k])
        else
            if oper=="=" then
                if and(l(:)==v) then
                    paths($+1)=[path k]
                end
            elseif oper==">" then
                if or(l(:) > v) then
                    paths($+1)=[path k]
                end
            elseif oper=="<" then
                if or(l(:) < v) then
                    paths($+1)=[path k]
                end
            else
            end
        end
    end
    if firstlevel then path=paths,clearglobal paths,end
endfunction

