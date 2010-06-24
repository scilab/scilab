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

//** 27 May 2008 : updated for Scilab 5 by Simone Mannori 

function ok = scicos_block_link(funam, txt, flag)

//** typ. usage is "ok = scicos_block_link(funam,txt,'c'); "
//** funam : (string) function name
//** txt   : (string vector) text, e.g. "C" source code   
 
  if (flag<>"c")&(flag<>"f") then
     ok = %f;
     messagebox(gettext("Sorry: Only C or FORTRAN languages are supported",'modal'));
     return; 
  end
  
  if stripblanks(funam)==emptystr() then 
    ok = %f;
    messagebox(gettext("Sorry file name not defined in "+flag+" block",'modal'));
    return; 
  end

  cur_wd = pwd(); //** get and save current working directory 

  chdir(TMPDIR); //** change to TMPDIR 

  //**---------------------------------------------------------------------

  mputl(txt, funam+'.'+flag); //** writes strings in an ascii file
                              //** with the appropriate extension 

  entry_names = [funam]; 
  file_names = funam + "."+flag  ;
  libs  = ""  ; //** CBLOCKs does not support external libraries 
  //** flag is already defined  
  makename = [] ;
  loadername = "loader.sce" ;
  libname = "" ;
  ldflags = "" ;
  cflags = strcat("-I"+scicos_include_paths(),' ')
  fflags = ""; //** no Fortran 
  cc = ""; //** default "C" compiler 
  
  disp("Compile and link Scicos C BLOCK code");

  libn = ilib_for_link(entry_names, file_names, libs, flag, makename, loadername, libname, ldflags, cflags, fflags, cc); 

  exec("loader.sce"); //** load the shared library

  //**--------------------------------------------------------------------- 
  
  chdir(cur_wd); //** go back to the original working directory 
  
  ok = %t ; //** signal the correct execution 


endfunction

