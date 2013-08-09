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
function param=scicos_txtedit(v1,v2,v3,v4,v5)
    // Copyright INRIA
    // Scicos text editor param structure
    if exists("typ","local")==0 then typ="", end
    if exists("ttitle","local")==0 then ttitle="", end
    if exists("head","local")==0 then head=[], end
    if exists("clos","local")==0 then clos=0, end

    param=mlist(["TxtEdit","typ","title","head","clos"],typ,ttitle,head,clos)
endfunction
