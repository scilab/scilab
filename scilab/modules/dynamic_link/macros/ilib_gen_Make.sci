// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Makename=ilib_gen_Make(name,tables,files,libs,makename,with_gateway,ldflags,cflags,fflags,cc)
//------------------------------------
// generate a Makefile for gateway

  [lhs,rhs] = argn(0);
  if rhs < 4 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_gen_Make"));
    return
  end

  if argn(2)<6 then with_gateway=%t,ldflags='',cflags='',fflags='', cc='';end
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(ext_f == ['.o','.obj']) then
      files(i) = path_f + file_f;
    else
      files(i) = path_f + file_f + ext_f;
    end
  end
  
  // change table if necessary 
  if typeof(tables)<>'list' then 
    tables= list(tables)
  end
  L=length(tables); 
  for it = 1:L 
    table = tables(it);
    [mt,nt]=size(table);
	
	if nt==2 then 
	  col= "csci";  // @TODO what is csci
	  table = [table, col(ones(mt,1))];
	  nt=3; 
	end 
	
	if nt<>3 then 
	  error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"),"ilib_gen_Make",2));
	end 
	
	tables(it)=table;
  end
      
  comp_target = COMPILER;
  if with_lcc() == %T then
  	Makename = makename+'.lcc'
  	ilib_gen_Make_lcc(name,tables,files,libs,Makename,with_gateway,ldflags,cflags,fflags)
  else if getenv('WIN32','NO')=='OK' then
    select comp_target
     case 'VC++'   then Makename = makename+'.mak'
      ilib_gen_Make_win32(name,tables,files,libs,Makename,with_gateway,ldflags,cflags,fflags)
     case 'gcc' then 
      Makename = makename; 
      ilib_gen_Make_unix(name,files,libs,Makename,with_gateway,ldflags,cflags,fflags,cc,tables)
    else
       Makename = makename;
       ilib_gen_Make_win32(name,tables,files,libs,Makename,with_gateway,ldflags,cflags,fflags)
    end
  else
     Makename = makename;
     ilib_gen_Make_unix(name,files,libs,name,ldflags,cflags,fflags,cc,tables)
  end
  end
endfunction


function ilib_gen_Make_win32(name,table,files,libs,Makename,with_gateway,ldflags,cflags,fflags)
 
  managed_ext = ['.cxx', '.cpp', '.c', '.f90', '.f'];
  obj_ext = ['.o', '.obj', ''];
  
  SCIDIR = SCI;
  SCIDIR1 = pathconvert(SCI,%f,%f,'w');
  LIBRARY = name;
  FILES_SRC = '';
  OBJS = '';
  OBJS_WITH_PATH = '';
  CPP_RUNTIME = '';
  FORTRAN_RUNTIME = '';
  OTHERLIBS = '';
  CC = '';
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
  
  if typeof(tables) <> 'list' then 
    tables = list(tables);
  end
  L = length(tables); 

  if with_gateway then 
    if L == 1 then 
      FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + '.c'];
    else
       for i=1:L
        FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + string(i) + '.c'];
       end 
    end
  end

  for it=1:L 
    table = tables(it);
    [mt,nt] = size(table);
 
    for i=1:mt  
      if table(i,3)=='cmex' | table(i,3)=='fmex' | table(i,3)=='Fmex' then
        MEXCFLAGS = "-Dmexfunction_=mex" + table(i,2) + "_ -DmexFunction=mex_" + table(i,2);
        MEXFFLAGS = "-Dmexfunction=mex" + table(i,2);
        if table(i,3)=='cmex' then
          filenameMex = table(i,2) + '.c';
        else
          filenameMex = table(i,2) + '.f';
        end
        if grep(FILES_SRC_MATRIX,filenameMex) == [] then
          FILES_SRC_MATRIX = [FILES_SRC_MATRIX , filenameMex];
        end
      end
    end
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
  
  OBJS = strcat(OBJS_MATRIX, ' ');
  OBJS_WITH_PATH =  strcat(OBJS_WITH_PATH_MATRIX, ' ');

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
  
  for x=libs(:)'
     if OTHERLIBS <> '' then
       OTHERLIBS = OTHERLIBS + ' ' + x + '.lib';
     else
       OTHERLIBS = x + '.lib';
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
//---------------------------------------------------------------------------------------
         
function ilib_gen_Make_lcc(name,table,files,libs,Makename,with_gateway,ldflags,cflags,fflags)
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
  LIBRARY = name;
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

  if typeof(tables) <> 'list' then 
    tables = list(tables);
  end
  L = length(tables); 

  if with_gateway then 
    if L == 1 then 
      FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + '.c'];
    else
       for i=1:L
        FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + string(i) + '.c'];
       end 
    end
  end
  
  for it=1:L 
    table = tables(it);
    [mt,nt] = size(table);
 
    for i=1:mt ; 
      // mex files to be added 
      if table(i,3)=='cmex' | table(i,3)=='fmex' | table(i,3)=='Fmex' 
        FILES_SRC_MATRIX = [FILES_SRC_MATRIX , table(i,2)];
      end
    end
  end
  
  FILES_SRC = strcat(FILES_SRC_MATRIX,' ');
  
  if table(i,3)=='cmex' | table(i,3)=='fmex' | table(i,3)=='Fmex' then
    MEXCFLAGS = "-Dmexfunction_=mex$*_ -DmexFunction=mex_$*";
    MEXFFLAGS = "-Dmexfunction=mex$*";
  end 

  OBJS_MATRIX = [];
  
  for y = 1:size(FILES_SRC_MATRIX,'*')
    [path_f, file_f, ext_f] = fileparts(FILES_SRC_MATRIX(y));
    OBJS_MATRIX = [OBJS_MATRIX, path_f + file_f + '.obj'];
  end
  
  OBJS = strcat(OBJS_MATRIX, ' ');

  for x=libs(:)'
     if OTHERLIBS <> '' then
       OTHERLIBS = OTHERLIBS + ' ' + x + '.lib';
     else
       OTHERLIBS = x + '.lib';
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
//---------------------------------------------------------------------------------------
