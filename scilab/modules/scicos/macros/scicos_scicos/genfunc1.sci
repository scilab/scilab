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

function [ok,tt,dep_ut]=genfunc1(tt,inp,out,nci,nco,nx,nz,nrp,type_)
    // manages dialog to get  definition (with scilab instruction) of a new scicos
    // block
    //!
    ni=size(inp,"*")
    no=size(out,"*")

    mac=[];ok=%f,dep_ut=[]
    if size(tt)<>7 then
        [txt1,txt0,txt2,txt3,txt4,txt5,txt6]=(" "," "," "," "," "," "," ")
    else
        [txt1,txt0,txt2,txt3,txt4,txt5,txt6]=tt(1:7)
    end
    dep_u=%f;dep_t=%f
    depp="t";deqq="t";

    u=emptystr(),
    for k=1:ni,u=u+"u"+string(k)+",",end
    dep=["t,","x,","z,",u,"n_evi,","rpar"]



    if nx==0 then dep(2)=emptystr(),end
    if nz==0 then dep(3)=emptystr(),end
    //if nci==0 then dep(5)=emptystr(),end
    if nrp==0 then dep(6)=emptystr(),end


    //flag = 1
    if no>0 then
        depp=strcat(dep([1:5,6]))
        w=[]
        for k=1:no,w=[w;"y"+string(k)+" (size: "+string(out(k))+")"],end
        while %t do
            txt1=dialog(["Define function which computes the output";
            " "
            "Enter Scilab instructions defining";
            w;
            "as a functions of "+depp],txt1)
            if txt1==[] then return,end
            // check if txt defines y from u
            mac=null();
            if execstr("deff(""[]=mac()"",txt1)", "errcatch") <> 0 then
                messagebox(["Incorrect syntax: ";lasterror()],"modal")
            else
                vars=macrovar(mac)
                for k=1:ni
                    if or(vars(3)=="u"+string(k)) then dep_u=%t,end
                end
                if or(vars(3)=="t") then dep_t=%t,end
                w=[];w(no)=%f;
                for k=1:no,if or(vars(5)=="y"+string(k)) then w(k)=%t,end,end
                if ~and(w) then
                    k1=find(~w)
                    w=[];for k=1:size(k1,"*"),w=[w;"y"+string(k)+" (size: "+string(out(k))+")"],end
                    messagebox("You did not define "+strcat(w,",")+" !","modal")
                else
                    break
                end
            end
        end
    else
        txt1=[]
    end

    //flag = 2
    if nx>0 then
        // xdot
        depp=strcat(dep([1:4,6]))
        while %t do
            if txt0==[] then txt0=" ",end
            txt0=dialog(["Define continuous states evolution";
            " "
            "Enter Scilab instructions defining:";
            "derivative of continuous state xd (size:"+string(nx)+")"
            "as  function(s) of "+depp],txt0)

            if txt0==[] then return,end
            mac=null();
            if execstr("deff(""[]=mac()"",txt0)", "errcatch") <> 0 then
                messagebox(["Incorrect syntax: ";lasterror()],"modal")
            else
                vars=macrovar(mac)
                if or(vars(5)=="xd") then
                    break,
                else
                    messagebox("You did not define xd !","modal")
                end
            end
        end
    else
        txt0="xd=[]"
    end
    if (nci>0&(nx>0|nz>0))|nz>0 then // x+ z+
        depp=strcat(dep([1:5,6]))
        while %t do
            if txt2==[] then txt2=" ",end
            t1=[]
            if nx>0 then
                t1=[t1;"-new continuous state x (size:"+string(nx)+")"]
            end
            if nz>0 then
                t1=[t1;"-new discrete state z (size:"+string(nz)+")"]
            end

            txt2=dialog(["You may define:";
            t1
            "at event time, as functions of "+depp],txt2)
            if txt2==[] then return,end
            mac=null();
            if execstr("deff(""[]=mac()"",txt2)", "errcatch") <> 0 then
                messagebox(["Incorrect syntax: ";lasterror()],"modal")
            else
                vars=macrovar(mac)
                if ~or(vars(5)=="x") then txt3=[txt3;"x=[]"];end
                if ~or(vars(5)=="z") then txt3=[txt3;"z=[]"];end
                break;
            end
        end
    else
        txt2=" "
    end // end of x+ z+

    //flag = 3
    if nci>0&nco>0 then
        depp=strcat(dep)

        while %t do
            if txt3==[] then txt3=" ",end
            txt3=dialog(["Using "+depp+",you may set "
            "vector of output time events t_evo (size:"+string(nco)+")"
            "at event time. "],txt3)
            if txt3==[] then return,end
            mac=null();
            if execstr("deff(""[]=mac()"",txt3)", "errcatch") <> 0 then
                messagebox(["Incorrect syntax: ";lasterror()],"modal")
            else
                vars=macrovar(mac)
                if ~or(vars(5)=="t_evo") then txt3=[txt3;"t_evo=[]"];end
                break;
            end
        end
    else
        txt3=" "
    end

    //flag = 4
    depp=strcat(dep([2 3 6]))
    t1=[]
    if nx>0 then
        t1=[t1;"- continuous state x (size:"+string(nx)+")"]
    end
    if nz>0 then
        t1=[t1;"- discrete state z (size:"+string(nz)+")"]
    end
    if t1<>[] then
        t1=["You may also reinitialize:";
        t1]
    end
    while %t do
        if txt4==[] then txt4=" ",end
        txt4=dialog(["You may do whatever needed for initialization :"
        "File or graphic opening...,"
        t1
        "as  function(s) of "+depp],txt4)
        if txt4==[] then return,end
        mac=null();
        if execstr("deff(""[]=mac()"",txt4)", "errcatch") <> 0 then
            messagebox(["Incorrect syntax: ";lasterror()],"modal")
        else
            break
        end
    end

    //flag = 5
    depp=strcat(dep([2 3 6]))
    t1=[]
    if nx>0 then
        t1=[t1;"- continuous state x (size:"+string(nx)+")"]
    end
    if nz>0 then
        t1=[t1;"- discrete state z (size:"+string(nz)+")"]
    end
    if t1<>[] then
        t1=["You may also change final value of:";t1]
    end
    while %t do
        if txt5==[] then txt5=" ",end
        txt5=dialog(["You may do whatever needed to finish :"
        "File or graphic closing...,"
        t1
        "as  function(s) of "+depp],txt5)
        if txt5==[] then return,end
        mac=null();
        if execstr("deff(""[]=mac()"",txt5)", "errcatch") <> 0 then
            messagebox(["Incorrect syntax: ";lasterror()],"modal")
        else
            break
        end
    end

    //flag = 6
    if nx>0|nz>0|no>0 then
        t1=[]
        if nx>0 then
            t1=[t1;"- state x (size:"+string(nx)+")"]
        end
        if nz>0 then
            t1=[t1;"- state z (size:"+string(nz)+")"]
        end
        w=[]
        for k=1:no
            w=[w;"- output y"+string(k)+" (size : "+string(out(k))+")"]
        end
        t1=[t1;w]

        depp=strcat(dep([2:4,6]))

        while %t do
            txt6=dialog(["You may define here functions imposing constraints";
            "on initial inputs, states and outputs";
            "Note: these functions may be called more than once";
            " ";
            "Enter Scilab instructions defining:"
            t1;
            "as a function of "+depp],txt6)
            if txt6==[] then return,end

            mac=null();
            if execstr("deff(""[]=mac()"",txt6)", "errcatch") <> 0 then
                messagebox(["Incorrect syntax: ";lasterror()],"modal")
            else
                vars=macrovar(mac)
                for k=1:no
                    if and(vars(5)<>"y"+string(k)) then txt6=[txt6;"y"+string(k)+"=[]"],end
                end
                break,
            end
        end
    else
        txt6=[]
    end

    ok=%t
    tt=list(txt1,txt0,txt2,txt3,txt4,txt5,txt6)


    dep_ut=[dep_u dep_t]
endfunction
