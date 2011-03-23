//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2011 - INRIA - Serge Steer

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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [%ll,%ierr] = script2var(%txt, %ll)
//** [%scicos_context, ierr] = script2var(context, %scicos_context)
//** context is the scs_m.props.context (string array) associated with the current level
//** %scicos_context  is a struct containing the values defined by the
//    calling contexts
//**
//** 10 Jan 2006
//local variable names are prefixed with a %  to limit conflicts with
//variables  defined in %txt instructions
  %ierr = 0 ; //** init

  //next lines checks if variable defined in %ll struct can be evaluated
  //why ???
  %mm = fieldnames(%ll)';
  for %mi=%mm
    if execstr(%mi+'=%ll(%mi)','errcatch')<>0 then
      mprintf("%s\n",lasterror())
      %ierr=1
      return
    end
  end
  [%ll,%ierr] = getvardef(%txt,%ll)
  if %ierr<>0 then return, end
endfunction

//**--------------------------------------------------------------------------
function [%ll,%ierr]=getvardef(%txt,%ll)
//extend and modify the %scicos_context variable (%ll) with the variable
//defined in the current level scs_m.props.context (%txt) instructions

//local variable names are prefixed with a %  to limit conflicts with
//variables  defined in %txt instructions

  %nww='';%ierr=0;  // to make sure %nww and %ierr does not enter the difference
  if isempty(%txt) then return,end
  %nww=size(who('get'),'*')

  %ierr=execstr(%txt,'errcatch')
  if %ierr<>0 then return,end

  %mm=who('get')
  %mm=%mm(1:size(%mm,'*')-%nww)
  //%mm contains the list of the variables defined by execstr(%txt,'errcatch')
  for %mi=%mm(:)'
    if %mi=="scs_m" then
      mprintf(_("The variable name %s cannot be used as block parameter: ignored"),"scs_m")
    else
      %ll(%mi)=evstr(%mi)
    end
  end
endfunction

