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
function p=tk_savefile_scicos(file_mask,path,Title,ini)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('file_mask','local')==1 then
    fmask="";
    for i=1:size(file_mask,'*')
      a=file_mask(i);
      a=strsubst(a,'.','');a=strsubst(a,'*','');
      if and(a<>[[],'']) then a=convstr(a,'u');
      else a='All';
      end
      if a=='COS' then a='Scicos'; end
      fmask=fmask+"{"""+a+" Files"" {"+file_mask(i)+"}} ";
    end
    TCL_SetVar("ftypes",fmask)
    arg = arg+" -filetypes $ftypes"
  end
  if exists('path','local')==1 then arg=arg+' -initialdir ""'+path+'""',end
  if exists('title','local')==1 then Title=title,end
  if exists('Title','local')==1 then arg=arg+' -title ""'+Title+'""',end
  if exists('ini','local')==1 then arg=arg+' -initialfile ""'+ini+'.cos""',end
  TCL_EvalStr('set scifilepath [tk_getSaveFile'+arg+']')
  p=TCL_GetVar('scifilepath')
endfunction
 
 
