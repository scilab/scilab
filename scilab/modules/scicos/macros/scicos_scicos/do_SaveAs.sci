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

function [scs_m, edited] = do_SaveAs()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//
// Copyright INRIA

  fname=scs_m.props.title(1);
  tit = ["Use .cos extension for binary and .cosf for ascii file"];
  //The Scilab5  savefile function does not allow to give a default file name
  //fname = savefile(['*.cos*';'*.xml'],emptystr(),tit,fname)
  fname = savefile(['*.cos*';'*.xml'],emptystr(),tit)

  if fname==emptystr() then
    return ; //** EXIT point 
  end

  [path,name,ext] = splitfilepath_cos(fname)
  
  select ext
    
   case "cos" then
    ok = %t
    frmt = 'unformatted'
    
   case "cosf" then
    ok = %t
    frmt = 'formatted'
    
   case "" then
     ok = %t
     frmt = 'unformatted'
     fname=fname+".cos"
     ext='cos'

   case "xml" then
    ok = %t
    frmt = 'xmlformatted'
  else
    messagebox("Only *.cos binary or cosf ascii files allowed",'modal');
    return //** EXIT Point 
  end

  if ~super_block & ~pal_mode then
    //update %cpr data structure to make it coherent with last changes
    if needcompile==4 then
      %cpr = list()
    else
      [%cpr, %state0, needcompile, alreadyran, ok] = do_update(%cpr,%state0,needcompile); 
      if ~ok then
         return
      end
      %cpr.state=%state0
    end
  else
    %cpr=list()
  end

  // open the selected file
  if frmt=='formatted'
    [u,err] = file('open',fname,'unknown',frmt)
  else
    [u,err] = mopen(fname,'wb')
  end
  if err<>0 then
    messagebox("File or directory write access denied",'modal')
    return
  end


  scs_m;
  scs_m.props.title = [name, path] // Change the title
  if pal_mode then
    scs_m.objs(1).graphics.id=name
    scs_m.objs(1).model.rpar.props.title=[name, path] // Change the title
  end
  
  // save
  if ext=="cos" then
    save(u,scs_m,%cpr)
  elseif ext=="xml" then
    [ok,t]=cos2xml(scs_m,'',%f)
    if ~ok then 
      messagebox("Error in xml format",'modal')
      file('close',u)
      return
    end
    mputl(t,u);
  else
  
    ierr = cos2cosf(u,do_purge(scs_m));
    if ierr<>0 then
      messagebox("Directory write access denied",'modal')
      file('close',u) ;
      return 
    end
  end
  
  file('close',u)

  //** disp("... SaveAs debug"); pause 
  
  //** if the current window is list of the phisically existing Scilab windows list winsid()
  if or(curwin==winsid()) then
    drawtitle(scs_m.props)  // draw the new title
  end   

  edited = %f
  if pal_mode then
    scicos_pal = update_scicos_pal(path,scs_m.props.title(1),fname),
    scicos_pal = resume(scicos_pal)
  end
endfunction
