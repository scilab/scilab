//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2011 - INRIA - Serge Steer
//  Copyright (C) 2018, 2020 - Samuel GOUGEON
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

function [%ll,%ierr] = script2var(%txt, %ll)
    //** [%scicos_context, ierr] = script2var(context, %scicos_context)
    //** %txt is the scs_m.props.context (string array) associated with the current level
    //** %ll  is a struct containing the values defined by the
    //    calling contexts
    //**
    //** 10 Jan 2006
    //local variable names are prefixed with a %  to limit conflicts with
    //variables  defined in %txt instructions
    %ierr = 0 ; //** init
    [tmp,mc] = where()
    errInMsgbox = (getscilabmode()=="STD" & mc($)=="script2var"); // call from Java SetContextDialog
    //next lines checks if variable defined in %ll struct can be evaluated

    %mm = fieldnames(%ll)';
    for %mi=%mm
        if execstr(%mi+"=%ll(%mi)","errcatch")<>0 then
            if errInMsgbox
                messagebox(lasterror(), _("Set Xcos context"), "warning", "modal")
            else
                mprintf("%s\n",lasterror())
            end
            %ierr=1
            return
        end
    end
    [%ll,%ierr] = getvardef(%txt,%ll)
endfunction

//**--------------------------------------------------------------------------
function [%ll,%ierr]=getvardef(%txt,%ll)
    //extend and modify the %scicos_context variable (%ll) with the variable
    //defined in the current level scs_m.props.context (%txt) instructions

    //local variable names are prefixed with a %  to limit conflicts with
    //variables  defined in %txt instructions

    if isempty(%txt) then return,end

    global %_old_   // safe location in case of clear() in the context
    %_ierr_ = 0;    // to make sure %ierr does not enter the difference
    %_old_ = who("scope")
    %_old_ = %_old_(isdef(%_old_,"l"))
    %_ierr_ = execstr(%txt,"errcatch");
    if %_ierr_<>0 then
        if errInMsgbox
            messagebox(lasterror(), _("Set Xcos context"), "warning", "modal")
        else
            mprintf("%s\n",lasterror())
        end
        return
    end
    global %_old_
    %_new_ = who("scope");
    %_new_ = setdiff(%_new_, %_old_)
    if %_new_ <> [] then
        %_new_ = %_new_(isdef(%_new_,"l"))
    end
    clearglobal %_old_

    %_msg = []
    %_tmp = _("The variable name %s cannot be used as block parameter: ignored\n")
    %_msg0 = msprintf(%_tmp, "scs_m")
    %_forbid = ["scs_m"]
    %_ignore = ["ans"]
    for %mi = matrix(%_new_, 1, -1)
        if or(%mi==%_forbid) then
            %_msg = [%_msg ; %_msg0]
            continue
        elseif or(%mi==%_ignore) then
            continue
        end

        clear %v
        %v = evstr(%mi);

        if typeof(%v)=="scs_m" then
            %_msg = [%_msg ; %_msg0]
            continue
        elseif or(type(%v)==[13 14]) then
            continue
        end

        %ll(%mi)=%v;
        clear %v
    end
    if %_msg ~=[] then
        if errInMsgbox
            messagebox(%_msg, _("Set Xcos context"), "warning", "modal")
        else
            mprintf("%s\n", %_msg)
        end
    end
endfunction
