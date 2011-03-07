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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//
function tk_message_modeless(strings)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// Copyright INRIA
//** tk_message_modeless : overlad function for x_message_modeless
//**                  inside scicos
//**
//** Alan, 02/04/08 : initial rev

  //** check lhs/rhs arg
  [lhs,rhs]=argn(0)

  if rhs<>1 then
    error(77)
  end

  if type(strings)<>10 then
    error(207,1)
  end

  if rhs>2 then
    error(77)
  end

  strings=strings(:);

  //** create tcl/tk box txt
  display_message_box(strings)

endfunction

//** display_message_box : create txt of the Tcl/Tk box
//**
//** Alan, 02/04/08 : initial rev
function display_message_box(str_in)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //** check lhs/rhs arg
  [lhs,rhs]=argn(0)

  //## input param of editor
  if getos() == 'Windows' then
    InFile = strsubst(TMPDIR,'/','\')+'\TTMPin';
    InFile = strsubst(InFile,'\','\\\\')';
  else
    InFile = TMPDIR+'/TTMPin';
  end

  //## compute max_l/max_h
  max_h = size(str_in,1)
  max_l = max(length(str_in))
  if max_h<15 then max_h=15, end
  if max_h>40 then max_h=40, end
  max_h=max_h
  if max_l<30 then max_l=30, end
  if max_l>80 then max_l=80, end
  max_l=max_l+2

  //## store input txt in file buffer
  mputl(str_in,InFile);

  //## Init TCL/TK sciGUI interf
  sciGUI_init();
  TCL_EvalFile(SCI+'/macros/scicos/scstxtedit.tcl');

  //** retrieve current postion of the last dialog box
  //** potential TCL global variables numx/numy
  if TCL_ExistVar('numx') then
    numx=TCL_GetVar('numx')
  else
    numx=-1
  end
  if TCL_ExistVar('numy') then
    numy=TCL_GetVar('numy')
  else
    numy=-1
  end

  //## run get info
  TCL_EvalStr('GetInfo '+InFile+' '+...
              string(max_l)+' '+string(max_h)+' '+...
              string(numx)+' '+string(numy));
endfunction 
 
