// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2009
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function libn = ilib_compile(lib_name, ..
                             makename, ..
                             files, ..
                             ldflags, ..
                             cflags, ..
                             fflags, ..
                             cc)

  [lhs,rhs] = argn(0);
  if rhs < 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"ilib_compile"));
    return
  end

  // The name of the library starts by "lib", strip it
  lib_name_orig = strsubst(lib_name,"/^lib/","","r");

  libn=""; //** init variable
  if getos() == "Windows" & ~haveacompiler() then
    error(_("A Fortran or C compiler is required."))
    return;
  end

  [lhs,rhs]=argn(0);

  if rhs < 3 then
    files = [];
  else
    if ~isempty(files) & (or(fileext(files)==".o") | or(fileext(files)==".obj")) then
      error(999, msprintf(_("%s: A managed file extension for input argument #%d expected."), "ilib_compile", 3));
    end
  end

  if typeof(lib_name)<>"string" then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"ilib_compile",1));
    return ;
  end

  oldpath = pwd();
  files = files(:)';

  [make_command, lib_name_make, lib_name, path, makename, files]= ...
      ilib_compile_get_names(lib_name, files);

  if isdir(path) then
    chdir(path);
  end

  if getos() == "Windows" then
    //** ----------- Windows section  -----------------
    
    // Load dynamic_link Internal lib if it"s not already loaded
    if ~ exists("dynamic_linkwindowslib") then
      load("SCI/modules/dynamic_link/macros/windows/lib");
    end
    
    dlwCompile(files, make_command, makename);
    
  else
    //** ---------- Linux/MacOS/Unix section ---------------------

    ScilabTreeFound=%f;

    // Source tree version
    // Headers are dispatched in the source tree
    if isdir(SCI+"/modules/core/includes/") then
      defaultModulesCHeader=[ "core", "mexlib","api_scilab","output_stream","localization" ];
      defaultModulesFHeader=[ "core" ];
      ScilabTreeFound=%t

      for x = defaultModulesCHeader(:)';
          cflags=" -I"+SCI+"/modules/"+x+"/includes/ "+cflags;
      end

      for x = defaultModulesFHeader(:)';
          fflags=" -I"+SCI+"/modules/"+x+"/includes/ " + fflags;
          end
    end

    // Binary version
    if isdir(SCI+"/../../include/scilab/") & ~ScilabTreeFound then
      cflags="-I"+SCI+"/../../include/scilab/ " + cflags
      fflags="-I"+SCI+"/../../include/scilab/ " + fflags
      ScilabTreeFound=%t
    end

    // System version (ie: /usr/include/scilab/)
    if isdir("/usr/include/scilab/") & ~ScilabTreeFound then
       cflags="-I/usr/include/scilab/ "+cflags
       fflags="-I/usr/include/scilab/ "+fflags
       ScilabTreeFound=%t
    end

    if ( ilib_verbose() <> 0 & ScilabTreeFound <> %t) then
       mprintf(gettext("%s: Warning: Scilab has not been able to find where the Scilab sources are. Please submit a bug report on http://bugzilla.scilab.org/\n"),"ilib_compile");
    end

    oldPath = pwd();

    // Switch back to the TMPDIR where the mandatory files are

    chdir(TMPDIR+"/"+lib_name_orig);

   // Detect the actual path to the libstdc++ library. For the dynamic link
   // build, we want to use the same lib as the compiler installed.
   // CF bug #7887 for more information.
   // Note that, for the configure, the setup is done by compilerDetection.sh
    cmdGCC="if test -x ""$(which gcc 2>/dev/null)""; then echo $(LC_ALL=C gcc -print-search-dirs|awk ''$1==""install:""{print $2}''); fi";
    [GCClibpath, ierr, stderr] = unix_g(cmdGCC);
    if (GCClibpath <> "" & ierr == 0 & grep(getenv("LD_LIBRARY_PATH"),GCClibpath) == []) then
        setenv("LD_LIBRARY_PATH",GCClibpath+"/../../../:"+getenv("LD_LIBRARY_PATH"));
    end

    cmd = "make "

    cmd = cmd + gencompilationflags_unix(ldflags, cflags, fflags, cc, "build")

    //** BEWARE : this function can cause errors if used with "old style" Makefile inside a Scilab 5
    //**          environment where the Makefile are created from a "./configure"
    [msg, ierr, stderr] = unix_g(cmd) ;

    if ( ilib_verbose() == 2 ) then
      mprintf(gettext("%s: Build command: %s\n"),"ilib_compile",cmd);
      mprintf(gettext("Output: %s\n"),msg);
      mprintf(gettext("stderr: %s\n"),stderr);
    end

    if ierr <> 0 then
      errMsg = sprintf(gettext("%s: An error occurred during the compilation:\n"), "ilib_compile");
      errMsg = [errMsg ; stderr];
      errMsg = [errMsg ; sprintf(gettext("%s: The command was:\n"), "ilib_compile")];
      errMsg = [errMsg ; cmd];
      chdir(oldPath); // Go back to the working dir
      error(errMsg);
      return ;
    end

    if stderr <> "" then
      if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("%s: Warning: No error code returned by the compilation but the error output is not empty:\n"),"ilib_compile");
        mprintf(stderr);
      end
      return ;
    end

    generatedLibrary=".libs/" + lib_name;
    // Copy the produce lib to the working path
    if ~isfile(generatedLibrary) then
      error(msprintf(gettext("%s: Could not find the built library ''%s''.\n"),"ilib_compile",generatedLibrary));
    end
    copyfile(generatedLibrary, oldPath);
    
  end
  
  libn = path + lib_name_make ;
  chdir(oldpath);

endfunction
//=============================================================================
// function only defined in ilib_compile
//=============================================================================
function [make_command, lib_name_make, lib_name,path, makename, files] = ..
             ilib_compile_get_names(lib_name, files)

  if getos() <> "Windows" then
    path = "";

    lib_name = lib_name + getdynlibext();
    lib_name_make = lib_name;

    make_command = "make ";
    if files <> [] then
      files = files + ".o";
    end

    makename = 'Makefile';

  else // Windows
    // Load dynamic_link Internal lib if it"s not already loaded
    if ~ exists("dynamic_linkwindowslib") then
      load("SCI/modules/dynamic_link/macros/windows/lib");
    end

    [make_command, lib_name_make, lib_name, path, makename, files] = ..
         dlwGetParamsIlibCompil(lib_name, files);
  end

endfunction
//=============================================================================

