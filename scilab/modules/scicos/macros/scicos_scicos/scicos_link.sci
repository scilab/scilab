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

function lnk=scicos_link(v1,v2,v3,v4,v5,v6,v7)
    //initialisation de link scicos_new
    if exists("xx","local")==0 then xx=[],end
    if exists("yy","local")==0 then yy=[],end
    if exists("id","local")==0 then id="",end
    if exists("thick","local")==0 then thick=[0,0],end
    if exists("ct","local")==0 then ct=[1,1],end
    if exists("from","local")==0 then from=[],end
    if exists("to","local")==0 then to=[],end

    lnk=scicos_new(["Link","xx","yy","id","thick","ct","from","to"],..
    xx,yy,id,thick,ct,from,to)
endfunction
