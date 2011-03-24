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

function ok = do_save(scs_m, filenamepath)   
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// saves scicos data structures scs_m and %cpr on a binary file
// filename is an optional to indicate the name of the file (name of
// the diagram is not affected)
//!
  if pal_mode then 
    scs_m = do_purge(scs_m)
  end
  
  //file path
  if size(scs_m.props.title,'*')<2 then 
    path = './';
  else
    path = scs_m.props.title(2);
  end

  // open file
  if ~super_block & ~pal_mode then
    // update %cpr data structure to make it coherent with last changes
    if needcompile==4 then
      %cpr = list()
    else
      [%cpr,%state0,needcompile,alreadyran,ok] = do_update(%cpr,%state0,needcompile); 
      if ~ok then
         return; 
      end
      %cpr.state = %state0; 
    end
  else
    %cpr = list() ; 
  end

  if argn(2)>1 then
    fname = filenamepath ; 
  else
    fname = path + scs_m.props.title(1)+'.cos' ;
  end

  [u,err] = mopen(fname,'wb') ;

  if err<>0 then
    messagebox("Directory write access denied",'modal') ;
    ok = %f;
    return ;
  end

  // save
  if execstr('save(u,scs_m,%cpr)','errcatch')<>0 then
    messagebox(["Save error:";lasterror()],'modal') ;
    ok = %f   ;
    mclose(u) ;
    return    ;
  end

  mclose(u); 

  if pal_mode then 
    update_scicos_pal(path,scs_m.props.title(1),fname)
  end

  ok = %t ; 

endfunction
