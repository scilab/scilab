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

function genmoc(path,force,verbose)
// make moc files in the specified directory

  [lhs,rhs] = argn(0)
  if rhs <= 2 then force   = %f ; end
  if rhs <= 3 then verbose = %f ; end

  //++ Check that modelica compiler is available
  //++ Otherwise, give some feedback and quit
  if ~with_modelica_compiler() then
    error(sprintf(gettext("%s: Error: Modelica compiler (MODELICAC) is unavailable."), "genmoc"))
    return
  end

  compilerpath = getmodelicacpath() + "modelicac"; //** thanks to automatic detection of MODELICAC

  // with env subsitution
  path = pathconvert(path, %t, %t);

  // list the mo files
  files = listfiles(path + '*.mo', %f)

  if files == [] | files == "" then
    error(sprintf(gettext("%s: Cannot find any MO files in the following location: %s.\n"), "genmoc", path))
    return
  end

  names = basename(files, %f);

  if force == %t then
    for i=1:size(files,'*')  // loop on .mo files
      mof = files(i);
      if verbose then
        mprintf(gettext(" %s.mo compilation forced\n"), names(i))
      end
      mocf = strsubst(mof, ".mo", ".moc")
      unix_s(compilerpath + " -c " + mof + " -o " + mocf)
    end
  else
    for i=1:size(files,'*')  // loop on .mo files
      mof       = files(i);
      mocf      = strsubst(mof, ".mo", ".moc")
      mocf_info = fileinfo(mocf);
      recompile = %f ;
      if mocf_info == [] then
        recompile = %t ;
      else
        mof_info = fileinfo(mof);
        if ( mof_info(6) > mocf_info(6)) then
          recompile = %t ;
        end
      end
      if recompile == %t then
        if verbose then
          mprintf(gettext(" %s.mo must be recompiled\n"), names(i))
        end
        if execstr('unix_s(compilerpath + '' -c '' + mof + '' -o '' + mocf)', 'errcatch') <> 0 then
          mprintf(gettext(" %s.mo cannot be compiled\n"), names(i))
        end
      end
    end
  end

  // list the c files (extern functions)
  files = listfiles(path + '*.c', %f)
  names = basename(files, %f);
  if files == [] | files == "" then
    return
  end
  libname = 'lib' + basename(part(path,1:length(path)-1))
  make = ilib_gen_Make(libname,['',''], names, [], path + 'Makelib', %f, '', '', '')
  make = strsubst(make, '.mak', '')
  if execstr('ilib_compile(libname, make, names);', 'errcatch') <> 0 then
    mprintf(gettext("Problem while building library %s\n"), libname)
  end

endfunction

