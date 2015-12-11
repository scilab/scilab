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

function state=scicos_state(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11)
    //initialisation de state scicos_new
    if exists("x","local")==0 then x=[],end
    if exists("z","local")==0 then z=[],end
    if exists("oz","local")==0 then oz=[],end
    if exists("iz","local")==0 then iz=[],end
    if exists("tevts","local")==0 then tevts=[],end
    if exists("evtspt","local")==0 then evtspt=[],end
    if exists("pointi","local")==0 then pointi=1,end
    if exists("outtb","local")==0 then outtb=list(),end
    state=scicos_new(["xcs","x","z","oz","iz","tevts","evtspt","pointi","outtb"],..
    x,z,oz,iz,tevts,evtspt,pointi,outtb)
endfunction
