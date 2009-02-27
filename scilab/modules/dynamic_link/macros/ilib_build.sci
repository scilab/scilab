// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC/INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//==========================================
function ilib_build(ilib_name,table,files,libs,makename,ldflags,cflags,fflags,ismex,cc)

  if ~haveacompiler() then
    error(msprintf(gettext("%s: A Fortran or C compiler is required.\n"),'ilib_build'));
  	return;
  end
  
  [lhs,rhs] = argn(0);
  
  if ~MSDOS & strncpy(ilib_name,3) <> "lib" then
	// We add a leading lib under Linux/Unix because it is the way
	  ilib_name="lib" + ilib_name;
  end
  
  if rhs <= 4 then makename = 'Makelib';end
  if rhs <= 5 then ldflags = ''; end 
  if rhs <= 6 then cflags  = ''; end 
  if rhs <= 7 then fflags  = ''; end 
  if rhs <= 8 then ismex  = %f; end 
  if rhs <= 9 then cc  = ''; end 
  // generate the gateway file
  if ( ilib_verbose() <> 0 ) then
    mprintf(_("   Generate a gateway file\n"));
  end    
  ilib_gen_gateway(ilib_name,table);
  
  // generate a loader file
  if ( ilib_verbose() <> 0 ) then
    mprintf(_("   Generate a loader file\n"));
  end
  if ~ismex then
    ilib_gen_loader(ilib_name,table,libs);
  else
    ilib_gen_loader(ilib_name,table);
  end
  
  // generate a Makefile
  if ( ilib_verbose() <> 0 ) then
	  if MSDOS
	    mprintf(_("   Generate a Makefile: %s\n"),'Makelib');
	  else
	    mprintf(_("   Generate a Makefile\n"));
	  end
  end
  
  if ~MSDOS then // Needs to copy the libfoo.c which contains important stuff
    files = [files,ilib_name + '.c'];
  end

  ilib_gen_Make(ilib_name,table,files,libs,makename,%t,ldflags,cflags,fflags,cc);
  
  // we call make
  if ( ilib_verbose() <> 0 ) then
    mprintf(_("   Running the makefile\n"));
  end
  
  ilib_compile(ilib_name,makename,files);
  
endfunction
//==========================================
