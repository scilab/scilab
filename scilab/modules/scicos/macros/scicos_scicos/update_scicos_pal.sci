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

function scicos_pal=update_scicos_pal(path,name,fname)
  scicos_pal;

  inde=find(scicos_pal(:,1)==name);
  if size(inde,'*')>=2 then 
    message(['More than one palette named '+name;
	     'This is not allowed, do an Pal Editor to correct'])
    return
  end
  if inde<>[] then
    if message(['The palette '+name+' already exists';
		   'Do you want to replace it?'],['Yes','No'])==2 then 
      return;
    else
      scicos_pal(inde,2)=fname
      if MSDOS then 
	instr='del '+TMPDIR+'\'+name+'.pal'
      else
	instr='\rm -f '+TMPDIR+'/'+name+'.pal'
      end
      if execstr('unix_s(instr)','errcatch')<>0 then
	x_message(['I was not able to delete '+name+'.pal';
		   'in '+TMPDIR+'. You must do it now!'])
      end
    end
  else
    scicos_pal=[scicos_pal;[name,fname]]
  end
  if execstr('save(''.scicos_pal'',scicos_pal)','errcatch')<>0 then
    x_message(['I was not able to write in .scicos_pal:';lasterror()])
  end
endfunction
