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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function cmd=get_errorcmd(path,scs_m_in,title_err,mess_err)
//** get_errorcmd : return a Scicos_commands strings
//** to select/hilite and display error messages for block
//** defined by his main scs_m path.
//** If the block is included in a super block, the editor
//** will open the correspondig windows by the use of the
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
    if typeof(scs_m_in)=='diagram' then
      scs_m=scs_m_in
      mess_err=[]
    elseif typeof(scs_m_in)=='string' then
      title_err=scs_m_in
      mess_err=[]
    end
  elseif (rhs == 3) then
    if typeof(scs_m_in)=='diagram' then
      scs_m=scs_m_in
    elseif typeof(scs_m_in)=='string' then
      mess_err=title_err
      title_err=scs_m_in
    end
  end

  //** **************
  //** modelica block
  //** **************
  if type(path)==15 then

    spec_err='The modelica block returns the error :';
    //** create cmd
    cmd=['message(['''+title_err+''';'+...
            ''''+spec_err+''';'+...
            strcat(''''+mess_err+'''',";")+']);']

  //** ************************
  //** all other type of blocks
  //** ************************
  else
    obj_path=path(1:$-1)
    spec_err='block'
    blk=path($)
    scs_m_n=scs_m;
    //** check if we can open a window
    //** Alan: we can improve that piece of code
    //**       to also returns the name of the comput. func.
    for i=1:size(path,'*')
      if scs_m_n.objs(path(i)).model.sim=='super' then
         scs_m_n=scs_m_n.objs(path(i)).model.rpar;
      elseif scs_m_n.objs(path(i)).model.sim=='csuper' then
        obj_path=path(1:i-1);
        blk=path(i);
        //spec_err='csuper block (block '+string(path(i+1))+')'
        spec_err='csuper block'
        break;
      end
    end

    if spec_err=='csuper block' then
        //** update spec_err
        spec_err='The hilited '+spec_err+' returns the error :';
        //**
        //scf(curwin)
        //** call bad_connection
        bad_connection(path,...
                      [title_err;spec_err;mess_err],0,1,0,-1,0,1)
        //** create cmd
        cmd=['%diagram_path_objective='+sci2exp(obj_path)+';%scicos_navig=1;'
             'Select=['+string(blk)+',curwin];'+...
             'xcosShowBlockWarning('+string(blk)+');'+...
             'xcosClearBlockWarning('+string(blk)+');']
    else
      //** update spec_err
      spec_err='The hilited '+spec_err+' returns the error :';
      //** create cmd
      cmd=['%diagram_path_objective='+sci2exp(obj_path)+';%scicos_navig=1;'
           'Select=['+string(blk)+',curwin];'+...
           'xcosShowBlockWarning('+string(blk)+');'+...
           'message(['''+title_err+''';'+...
              ''''+spec_err+''';'+...
              strcat(''''+mess_err+'''',";")+']);'+...
           'xcosClearBlockWarning('+string(blk)+');']
    end

  end

 //execude cmd error ( bug 5437 )
  for i = 1 : size(cmd, "*")
      execstr(cmd(i))
  end
  cmd = [];
endfunction
