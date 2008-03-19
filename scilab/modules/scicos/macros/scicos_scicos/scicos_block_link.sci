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

function ok=scicos_block_link(funam,txt,flag)
  if flag=='c' then mflag='C', else mflag='fortran',end
  if stripblanks(funam)==emptystr() then 
    ok=%f;
    x_message('sorry file name not defined in '+flag+' block');return
  end
  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(txt,funam+'.'+flag);
  ilib_for_link=ilib_for_link;//load ilib_for_link and its subfunctions
  ilib_compile=ilib_compile;//load ilib_for_link and its subfunctions
  ilib_link_gen_loader(funam,flag,'loader.sce',[],"");
  ilib_link_gen_Make(funam,funam+'.o',[],'Makelib',"",...
		     "","","","",flag);
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names('lib'+funam,'Makelib',funam+'.o')
  ierr= execstr('unix_s(make_command+makename + '' ''+ files)',..
		'errcatch');
  if ierr<> 0 then 
    chdir(cur_wd)
    x_message('Sorry compilation problem');ok=%f;return;
  end
  [a,b]=c_link(funam); while a;  ulink(b);[a,b]=c_link(funam);end
  ierr= execstr('unix_s(make_command+makename + '' ''+ lib_name)', ...
		'errcatch');
  if ierr<> 0 then 
    chdir(cur_wd)
    x_message('Sorry shared lib cannot be created');ok=%f;return;
  end
  ierr= exec('loader.sce','errcatch')
  if ierr<> 0 then     
    chdir(cur_wd)
    x_message('Sorry link problem');ok=%f;return;
  end
  chdir(cur_wd)
  ok=%t
endfunction

