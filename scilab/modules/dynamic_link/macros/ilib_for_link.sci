// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA/ENPC
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//==========================================
// Generate a shared library which can be used by link command. 
function libn = ilib_for_link(names, ..
                              files, ..
                              libs, ..
                              flag, ..
                              makename, ..
                              loadername, ..
                              libname, ..
                              ldflags, ..
                              cflags, ..
                              fflags, ..
                              cc)

  [lhs,rhs] = argn(0);
  if rhs < 4 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"ilib_for_link"));
    return
  end
  
  if rhs <= 4 then makename = "Makelib";end
  if rhs <= 5 then loadername = "loader.sce";end
  if rhs <= 6 then libname = ""; end
  if rhs <= 7 then ldflags = ""; end 
  if rhs <= 8 then cflags  = ""; end 
  if rhs <= 9 then fflags  = ""; end 
  if rhs <= 10 then cc  = ""; end 
  
  // generate a loader file
  if ( ilib_verbose() <> 0 ) then
    mprintf(gettext("   Generate a loader file\n"));
  end
  
  ilib_gen_loader(names,flag,loadername,libs,libname);
  
  // bug 4515 - unlink previous function with same name
  n = size(names,'*');
  for i = 1:n
    execstr("[bOK,ilib] = c_link(''" + names(i) + "'');if (bOK) then ulink(ilib),end");
  end
  
  // generate a Makefile
  if ( ilib_verbose() <> 0 ) then
    mprintf(gettext("   Generate a Makefile\n"));
  end

  ilib_link_gen_Make(names, ..
                     files, ..
                     libs, ..
                     makename, ..
                     libname, ..
		                 ldflags, ..
		                 cflags, ..
		                 fflags, ..
		                 cc, ..
		                 flag);
		                 
  // we call make
  if ( ilib_verbose() <> 0 ) then
    mprintf(gettext("   Running the Makefile\n"));
  end
  if (libname == "") then libname = names(1);end
  libn = ilib_compile('lib' + libname, makename, files);
  
  if ( ilib_verbose() <> 0 ) then
    mprintf(_("   Generate a cleaner file\n"));
  end
  ilib_gen_cleaner(makename,loadername,libn);

  
endfunction
//==========================================
function ilib_link_gen_Make(names, ..
                            files, ..
                            libs, ..
                            makename, ..
                            libname, ..
                            ldflags, ..
                            cflags, ..
                            fflags, ..
                            cc, ..
                            flag)

  // generate a Makefile for gateway
  [lhs,rhs] = argn(0);
  if rhs <= 2 then libs = [];end
  if rhs <= 3 then makename = 'Makelib';end
  if rhs <= 4 then libname = "";end
  if rhs <= 5 then ldflags = ""; end 
  if rhs <= 6 then cflags  = ""; end 
  if rhs <= 7 then fflags  = ""; end 
  if rhs <= 8 then cc  = ""; end 
  if rhs <= 9 then flag  = "c"; end 
  
  if MSDOS then // Windows
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      Makename = makename+'.mak';
      ilib_link_gen_Make_msvc(names, ..
                              files, ..
                              libs, ..
                              Makename, ..
                              libname, ..
			                        ldflags, ..
			                        cflags, ..
			                        fflags, ..
			                        cc);
    else
      // LCC-WIN32
      if findlcccompiler() then
        Makename = makename+'.lcc';
  	    ilib_link_gen_Make_lcc(names, ..
  	                           files, ..
  	                           libs, ..
  	                           Makename, ..
  	                           libname, ..
  	                           ldflags, ..
  	                           cflags, ..
  	                           fflags, ..
  	                           cc);
      else
      // TO DO : Add another compiler here
      end
    end
  
  else // LINUX
   Makename = makename;

   ilib_gen_Make_unix(names, ..
                           files, ..
                           libs, ..
                           libname, ..
                           ldflags, ..
                           cflags, ..
                           fflags, ..
                           cc);
  end
  
endfunction
//==========================================
function ilib_link_gen_Make_msvc(names, ..
                                  files, ..
                                  libs, ..
                                  Makename, ..
                                  libname, ..
                                  ldflags, ..
                                  cflags, ..
                                  fflags, ..
                                  cc)

  managed_ext = ['.cxx', '.cpp', '.c', '.f90', '.f'];
  obj_ext = ['.o', '.obj', ''];

  SCIDIR = SCI;
  SCIDIR1 = pathconvert(SCI,%f,%f,'w');
  LIBRARY = '';
  FILES_SRC = '';
  OBJS = '';
  OBJS_WITH_PATH = '';
  CPP_RUNTIME = '';
  FORTRAN_RUNTIME = '';
  OTHERLIBS = '';
  CC = cc;
  CFLAGS = cflags;
  MEXCFLAGS = '';
  FFLAGS = fflags;
  MEXFFLAGS = '';
  LDFLAGS = ldflags;

  FILES_SRC_MATRIX = [];  
  
  [path_Make, file_Make, ext_Make] = fileparts(Makename);
  
  for i=1:size(files,'*') 
    [path_f, file_f, ext_f] = fileparts(files(i));

    if or(obj_ext == ext_f) then
      FILENAME = [];
      FILE_FOUNDED = %f;
      for y = managed_ext(:)'
        if (FILE_FOUNDED == %f) then
          if (fileinfo(path_f + file_f + y) <> []) | (fileinfo(path_Make + file_f + y) <> []) then
            FILENAME = path_f + file_f + y;
            FILE_FOUNDED = %t;
          end
        end
      end
    else
      FILENAME = files(i);
    end
    FILES_SRC_MATRIX = [FILES_SRC_MATRIX , FILENAME];
  end

  FILES_SRC = strcat(FILES_SRC_MATRIX,' ');
  
  OBJ_DEST_PATH = '';
  if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
    OBJ_DEST_PATH = "Release/";
  else
    OBJ_DEST_PATH = "Debug/";
  end
  
  OBJS_MATRIX = [];
  OBJS_WITH_PATH_MATRIX = [];
  
  for y = 1:size(FILES_SRC_MATRIX,'*')
    [path_f, file_f, ext_f] = fileparts(FILES_SRC_MATRIX(y));
    OBJS_MATRIX = [OBJS_MATRIX, path_f + file_f + '.obj'];
    OBJS_WITH_PATH_MATRIX = [OBJS_WITH_PATH_MATRIX, OBJ_DEST_PATH + path_f + file_f + '.obj'];
  end
  
  if ( or(fileext(FILES_SRC_MATRIX) == '.cpp') | or(fileext(FILES_SRC_MATRIX) == '.cxx') ) then
    if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
      CPP_RUNTIME = 'LIBCPMT.LIB';
    else
      CPP_RUNTIME = 'LIBCPMTD.LIB';
    end
  end

  if ( or(fileext(FILES_SRC_MATRIX) == '.f90') | or(fileext(FILES_SRC_MATRIX) == '.f') ) then
    if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
      FORTRAN_RUNTIME = 'libifcoremd.lib libmmd.lib';
    else
      FORTRAN_RUNTIME = 'libifcoremdd.lib libmmdd.lib';
    end
  end
  
  OBJS = strcat(OBJS_MATRIX, ' ');
  OBJS_WITH_PATH =  strcat(OBJS_WITH_PATH_MATRIX, ' ');

  if (libname == '') then
    LIBRARY = 'lib' + names(1);
  else
    LIBRARY = 'lib' + libname;
  end

  if (libs <> []) then
    for x = libs(:)'
      if x <> '' then
        OTHERLIBS = OTHERLIBS + ' ' + x + '.lib';
      end
    end
  end

  try
    MAKEFILE_VC = mgetl(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC');
  catch
    MAKEFILE_VC = '';
  end
  
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCI__", SCIDIR);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCIDIR1__", SCIDIR1);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LIBNAME__", LIBRARY);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FILES_SRC__", FILES_SRC);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS__", OBJS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS_WITH_PATH__", OBJS_WITH_PATH);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CPP_RUNTIME__", CPP_RUNTIME);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FORTRAN_RUNTIME__", FORTRAN_RUNTIME);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OTHERSLIBS__", OTHERLIBS);
  
  if CC <> '' then
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CC__",CC);
  else
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "CC = __CC__","#CC = ");
  end
  
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CFLAGS__", CFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXCFLAGS__", MEXCFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FFLAGS__", FFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXFFLAGS__", MEXFFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LDFLAGS__", LDFLAGS);

  if ( MAKEFILE_VC <> '') then
    fd = mopen(Makename, "wt");
    mputl(MAKEFILE_VC, fd);
    mclose(fd);
    if ilib_verbose() > 1 then
      disp(MAKEFILE_VC);
    end
  else
    // TEMPLATE_MAKEFILE.VC not found
    warning(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC'+ _('not found.') );
  end

endfunction
//==========================================    
function ilib_link_gen_Make_lcc(names, ..
                                files, ..
                                libs, ..
                                Makename, ..
                                libname, ..
                                ldflags, ..
                                cflags, ..
                                fflags, ..
                                cc)
                                
  managed_ext = ['.c', '.f'];
  obj_ext = ['.o', '.obj', ''];
  
  CFLAGS = cflags;
  MEXCFLAGS = '';
  FFLAGS = fflags;
  MEXFFLAGS = '';
  LDFLAGS = ldflags;
  SCIDIR = SCI;
  SCIDIR1 = pathconvert(SCI,%f,%f,'w');
  LCCLIBDIR = SCIHOME + filesep() + 'lcclib';
  LIBRARY = '';
  FILES_SRC = '';
  OBJS = '';
  OTHERLIBS = '';
  
  FILES_SRC_MATRIX = [];
  
  [path_Make, file_Make, ext_Make] = fileparts(Makename);
  
  for i=1:size(files,'*') 
    [path_f, file_f, ext_f] = fileparts(files(i));
    
    if or(obj_ext == ext_f) then
      FILENAME = [];
      FILE_FOUNDED = %f;
      for y = managed_ext(:)'
        if (FILE_FOUNDED == %f) then
          if (fileinfo(path_f + file_f + y) <> []) | (fileinfo(path_Make + file_f + y) <> []) then
            FILENAME = path_f + file_f + y;
            FILE_FOUNDED = %t;
          end
        end
      end
    else
      FILENAME = files(i);
    end
    FILES_SRC_MATRIX = [FILES_SRC_MATRIX , FILENAME];
  end
  
  FILES_SRC = strcat(FILES_SRC_MATRIX,' ');

  OBJS_MATRIX = [];
  
  for y = 1:size(FILES_SRC_MATRIX,'*')
    [path_f, file_f, ext_f] = fileparts(FILES_SRC_MATRIX(y));
    OBJS_MATRIX = [OBJS_MATRIX, path_f + file_f + '.obj'];
  end
  
  OBJS = strcat(OBJS_MATRIX, ' ');
  
  if (libname == '') then
    LIBRARY = 'lib' + names(1);
  else
    LIBRARY = 'lib' + libname;
  end
  
  if (libs <> []) then
    for x = libs(:)'
      if x <> '' then
        OTHERLIBS = OTHERLIBS + ' ' + x + '.lib';
      end
    end
  end
  OTHERLIBS = strsubst(OTHERLIBS,'/',filesep());

  try
    MAKEFILE_LCC = mgetl(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.LCC');
  catch
    MAKEFILE_LCC = '';
  end
  

  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__LDFLAGS__" , LDFLAGS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__SCI__" , SCIDIR);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__SCIDIR1__" , SCIDIR1);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__LCCLIBDIR__" , LCCLIBDIR);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__LIBRARY__" , LIBRARY);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__FILES_SRC__" , FILES_SRC);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__OTHERSLIBS__" , OTHERLIBS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__CFLAGS__" , CFLAGS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__MEXCFLAGS__" , MEXCFLAGS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__FFLAGS__" , FFLAGS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__MEXFFLAGS__" , MEXFFLAGS);
  MAKEFILE_LCC = strsubst(MAKEFILE_LCC , "__OBJS__" , OBJS);
  
  if ( MAKEFILE_LCC <> '') then
    fd = mopen(Makename, "wt");
    mputl(MAKEFILE_LCC, fd);
    mclose(fd);
    if ilib_verbose() > 1 then
      disp(mgetl(Makename));
    end
  else
    // TEMPLATE_MAKEFILE.LCC not found
    warning(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.LCC'+ _('not found.') );
  end
  
endfunction
//==========================================    
