// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2009
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function libn = ilib_compile(lib_name,makename,files, ..
				                         ldflags, ..
				                         cflags, ..
				                         fflags, ..
				                         cc)


  [lhs,rhs] = argn(0);
  if rhs < 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"ilib_compile"));
    return
  end
  
  libn=""; //** init variable 

  if ~haveacompiler() then
  	error(_("A Fortran or C compiler is required."))
  	return;
  end

  [lhs,rhs]=argn(0);
  
  if rhs < 3 then files = []; end 
  
  if typeof(lib_name)<>'string' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"ilib_compile",1));
    return ;
  end
  
  oldpath = pwd();
  files = files(:)';
  
  managed_ext = ['.obj','.o'];
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(managed_ext == ext_f) then
      files1(i) = path_f + file_f;
    else
      files1(i) = path_f + file_f + ext_f;
    end
  end
  
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names(lib_name,makename,files);

  if path<> '';  chdir(path);  end 
  
   
  // first try to build each file step by step 
  if MSDOS then
    //** ----------- Windows section  -----------------
    msgs_make = '';
    nf = size(files,'*');
    
    for i=1:nf
      if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Compilation of ") + string(files1(i)) +'\n');
      end
    end
    
    // then the shared library 
    if ( ilib_verbose() <> 0 ) then
      mprintf(_("   Building shared library (be patient)\n"));
    end
    
    // TO DO : Check if it is always needed ...
    if ilib_verbose() > 1 then
      msg = unix_g(make_command + makename + ' all'); 
      disp(msg);
    else
      unix_s(make_command + makename + ' all'); 
    end
   
  else
    //** ---------- Linux/MacOS/Unix section ---------------------  
	
	ScilabTreeFound=%f

    // Source tree version
	// Headers are dispatched in the source tree
	if isdir(SCI+"/modules/core/includes/") then
	  defaultModulesCHeader=[ "core", "mexlib","api_scilab","output_stream","localization" ];
	  defaultModulesFHeader=[ "core" ];
	  ScilabTreeFound=%t

	  for x = defaultModulesCHeader(:)' 
	      cflags=" -I"+SCI+"/modules/"+x+"/includes/ "+cflags;
	  end
	  cflags=" -I"+SCI+"/libs/MALLOC/includes/ " + cflags;

 	  for x = defaultModulesFHeader(:)' 
	  	  fflags=" -I"+SCI+"/modules/"+x+"/includes/ " + fflags;
          end
	end

	// Binary version
	if isdir(SCI+"/../../include/scilab/") & ScilabTreeFound then
  	  cflags="-I"+SCI+"/../../include/scilab/ " + cflags
  	  fflags="-I"+SCI+"/../../include/scilab/ " + fflags
	  ScilabTreeFound=%t
	end

	// System version (ie: /usr/include/scilab/)	
	if isdir("/usr/include/scilab/") & ScilabTreeFound then
	   cflags="-I/usr/include/scilab/ "+cflags
	   fflags="-I/usr/include/scilab/ "+fflags
	   ScilabTreeFound=%t
	end

	if ( ilib_verbose() <> 0 & ScilabTreeFound <> %t) then
	   mprintf(gettext("%s: Warning: Scilab has not been able to find where the Scilab sources are. Please submit a bug report on http://bugzilla.scilab.org/\n"),"ilib_compile");	
	end

      oldPath = pwd();
	  // Switch back to the TMPDIR where the mandatory files are
	  chdir(TMPDIR);
	  cmd = "make "
	  
	  cmd = cmd + gencompilationflags_unix(ldflags, cflags, fflags, cc)

    //** BEWARE : this function can cause errors if used with "old style" Makefile inside a Scilab 5
    //**          environment where the Makefile are created from a "./configure"  
	  [msg, ierr, stderr] = unix_g(cmd) ; 

	    if ( ilib_verbose() == 2 ) then
		   mprintf(gettext("%s: Build command: %s\n"),"ilib_compile",cmd);
		   mprintf(gettext("Output: %s\n"),msg);
		   mprintf(gettext("stderr: %s\n"),stderr);
		end
	  if ierr <> 0 then
	      mprintf(gettext("%s: An error occured during the compilation:\n"),"ilib_compile");
	      lines(0);
	      disp(stderr);
		    mprintf("\n");
		    mprintf(gettext("%s: The command was:\n"),"ilib_compile");
		    mprintf(cmd);
		    mprintf("\n");
		  chdir(oldPath); // Go back to the working dir
	    return ;
	  end
	  if stderr <> "" then
	    if ( ilib_verbose() <> 0 ) then
	      mprintf(gettext("%s: Warning: No error code returned by the compilation but the error output is not empty:\n"),"ilib_compile");
	      mprintf(stderr);
	    end
	    return ;
	  end

	  // Copy the produce lib to the working path
	  copyfile(".libs/" + lib_name, oldPath);
  end
	
  libn = path + lib_name_make ; 
  chdir(oldpath);
  
endfunction
//==========================================

//==========================================
// function only defined in ilib_compile
//==========================================
function [make_command,lib_name_make,lib_name,path,makename,files] = ilib_compile_get_names(lib_name,makename,files) 
  
  managed_ext = ['.obj','.o'];
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(managed_ext == ext_f) then
      files(i) = path_f + file_f;
    else
      files(i) = path_f + file_f + ext_f;
    end
  end
    
  k = strindex(makename,['/','\']);
  
  if k~=[] then
    path = part(makename,1:k($));
    makename = part(makename,k($)+1:length(makename));
  else
     path='';
  end
  
  lib_name = lib_name+getdynlibext();
  lib_name_make = lib_name;
  
  if MSDOS then // WINDOWS
    FILES = [];
    for x = files(:)' 
      [ptmp,ftmp,fext] = fileparts(x);
      FILES = [FILES,ptmp+ftmp];
    end
    
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      makename = makename + '.mak' ; 
      make_command = 'nmake /Y /nologo /f '
    else
      // LCC-WIN32
      if findlcccompiler() then
        makename = makename + '.lcc' ; 
        make_command = 'make -f ';
      else
      // TO DO : Add another compiler here
      end
      
    end
    if FILES<>[] then 
      files = FILES + '.obj' ;
    end
  else // LINUX
  
    make_command = 'make '; 
    
    if files <> [] then 
      files = files + '.o';
    end
    
  end
  
endfunction 

//==========================================
