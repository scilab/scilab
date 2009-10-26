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

function [%ll,%ierr] = script2var(%txt, %ll)
//** [%scicos_context, ierr] = script2var(context, %scicos_context)

//**
//** 10 Jan 2006 
  %ierr = 0 ; //** init 
  %mm = getfield(1,%ll)
  for %mi=%mm(3:$)
    if execstr(%mi+'=%ll(%mi)','errcatch')<>0 then
      disp(lasterror())
      %ierr=1
      return
    end
  end
  
  [%ll,%ierr] = getvardef(%txt,%ll)
  if %ierr<>0 then 
    return,
  end
endfunction

//**--------------------------------------------------------------------------
function [%ll,%ierr]=getvardef(%txt,%ll)
  %nww='';%ierr=0;  // to make sure %ww does not enter the difference
  %nww=size(who('get'),'*')
  if isempty(%txt) then
    %ierr = 0;
  else
    %ierr=execstr(%txt,'errcatch')
  end
  if %ierr<>0 then 
    return,
  end
  %mm=who('get')
  %nww2=size(%mm,'*')
  %mm=%mm(1:%nww2-%nww)
  for %mi=%mm(:)'
    if type(evstr(%mi)) <> 13 then
      if %mi=="scs_m" then
	disp('the name scs_m is reseved; it cannot be used as block"+...
	     " parameter')
      else
	%ll(%mi)=evstr(%mi)
      end
    end
  end
endfunction
