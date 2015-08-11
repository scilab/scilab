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

function params=scicos_params(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
    if exists("wpar","local")==0 then wpar=[600,450,0,0,600,450],end
    if exists("title","local")==0 & exists("Title","local")==0 then
        titlex="Untitled"
    else
        if exists("title","local")==1 then
            titlex=title
        elseif exists("Title","local")==1 then
            titlex=Title
        end
    end
    if exists("tf","local")==0 then tf=100000,end
    if exists("tol","local")==0 then tol=[1.d-6,1.d-6,1.d-10,tf+1,0,1,0],end
    if exists("context","local")==0 then context=[],end
    void1=[]
    if exists("options","local")==0 then options=default_options(),end
    void2=[]
    void3=[]
    if exists("doc","local")==0 then doc=list(),end

    params=scicos_new(["params","wpar","title","tol","tf","context",..
    "void1","options","void2","void3","doc"],..
    wpar,titlex,tol,tf,context,void1,options,void2,..
    void3,doc)

endfunction

