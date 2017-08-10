//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function cmd=get_errorcmd(path,scs_m_in,title_err,mess_err)
    //** get_errorcmd : return a Scicos_commands strings
    //** to select/hilite and display error messages for block
    //** defined by his main scs_m path.
    //** If the block is included in a super block, the editor
    //** will open the corresponding windows by the use of the
    //** scicos global variable %diagram_path_objective and
    //** %scicos_navig.
    //**
    //** exemple of use :
    //**       path=corinv(kfun);
    //**       global Scicos_commands;
    //**       Scicos_commands=get_errorcmd(path,'my Title error','error message');
    //**
    //** inputs : path : the path of the object which have
    //**                 generated an error in a scs_m
    //**
    //**          scs_m_in : a scicos diagram data structure
    //**                 (if any. if not scs_m is semi global)
    //**
    //**          title_err : the title of the error box message
    //**                      (if any)
    //**
    //**          mess_err : the message of the error box message
    //**                      (if any)
    //**
    //** nb : the string message will be formated as this :
    //**      str_err=[title;
    //**               specific message for type of block;
    //**               mess_err]
    //**
    //** output : cmd  : the Scicos_commands strings
    //**
    //** Alan, 11/10/07 : Initial rev
    //Copyright INRIA

    //** first generate an empty cmd
    cmd=[]

    //** generte empty spec_err
    spec_err=[]

    //** check number of rhs arg
    rhs = argn(2) ;
    if (rhs == 1) then
        title_err=[]
        mess_err=[]
    elseif (rhs == 2) then
        if typeof(scs_m_in)=="diagram" then
            scs_m=scs_m_in
            mess_err=[]
        elseif typeof(scs_m_in)=="string" then
            title_err=scs_m_in
            mess_err=[]
        end
    elseif (rhs == 3) then
        if typeof(scs_m_in)=="diagram" then
            scs_m=scs_m_in
        elseif typeof(scs_m_in)=="string" then
            mess_err=title_err
            title_err=scs_m_in
        end
    end

    //** **************
    //** modelica block
    //** **************
    if type(path)==15 then

        spec_err="The modelica block returns the error :";
        //** create cmd
        cmd=["messagebox([''"+title_err+"'';"+...
        "''"+spec_err+"'';"+...
        strcat("''"+mess_err+"''",";")+"],''modal'');"]

        //** ************************
        //** all other type of blocks
        //** ************************
    else
        msg = "<html><body>" + title_err + ": <br/>";
        msg = msg + strcat(mess_err, "<br/>");
        msg = msg + "</body></html>";
        hilite_path(path, msg);
    end

endfunction
