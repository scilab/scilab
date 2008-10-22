//  Scipad - programmer's editor and debugger for Scilab
//
//  Copyright (C) 2002 -      INRIA, Matthieu Philippe
//  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
//  Copyright (C) 2004-2008 - Francois Vogel
//
//  Localization files ( in tcl/msg_files/) are copyright of the 
//  individual authors, listed in the header of each file
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
// See the file scipad/license.txt
//

function answ=edit_error(clearerror)
  if argn(2)==0 then clearerror=%f; end
  [str,n,line,func]=lasterror(clearerror)
  if n>0 then
    if func=="" then
      answ=gettext('the last error did not occur inside a library function.');
    else
      p=get_function_path(func);
      if p<>[] then
        scipad(p);
        TCL_EvalStr("blinkline "+msprintf("%d",line)+" "+func,"scipad");
        answ=gettext('opening file ')+p+gettext(' on line ')+msprintf("%d",line)+gettext(' of the source file of the function ')+func;
      else
        answ=gettext('the source file of the function ')+func+ ..
              gettext(' was not found in any library path');
      end
    end
  else
    answ=gettext(' was not found in any library path');
  end
endfunction

