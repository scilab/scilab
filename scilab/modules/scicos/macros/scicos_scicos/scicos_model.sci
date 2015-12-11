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

function model=scicos_model(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,...
    v17,v18,v19,v20,v21,v22,v23,v24,v25,v26)

    //initialisation de model scicos_new
    if exists("sim","local")==0 then sim="",end
    if exists("in","local")==0 then in=[],end
    if exists("in2","local")==0 then in2=[],end
    if exists("intyp","local")==0 then intyp=[],end
    if exists("out","local")==0 then out=[],end
    if exists("out2","local")==0 then out2=[],end
    if exists("outtyp","local")==0 then outtyp=[],end
    if exists("evtin","local")==0 then evtin=[],end
    if exists("evtout","local")==0 then evtout=[],end
    if exists("state","local")==0 then state=[],end
    if exists("dstate","local")==0 then dstate=[],end
    if exists("odstate","local")==0 then odstate=list(),end
    if exists("opar","local")==0 then opar=list(),end
    if exists("rpar","local")==0 then rpar=[],end
    if exists("ipar","local")==0 then ipar=[],end
    if exists("blocktype","local")==0 then blocktype="c",end
    if exists("firing","local")==0 then firing=[],end
    if exists("dep_ut","local")==0 then dep_ut=[%f %f],end
    if exists("label","local")==0 then label="",end
    if exists("nzcross","local")==0 then nzcross=0,end
    if exists("nmode","local")==0 then nmode=0,end
    if exists("equations","local")==0 then equations=list(),end
    if exists("uid","local")==0 then uid="",end

    model=scicos_new(["model","sim","in","in2","intyp","out","out2","outtyp",...
    "evtin","evtout","state","dstate","odstate","rpar","ipar","opar",...
    "blocktype","firing","dep_ut","label","nzcross",..
    "nmode","equations","uid"],..
    sim,in,in2,intyp,out,out2,outtyp,evtin,evtout,state,dstate,odstate,..
    rpar,ipar,opar,blocktype,firing,dep_ut,label,nzcross,nmode,equations,uid)
endfunction
