//-----------------------------------------------------------------------------
function libn=ilib_compile(lib_name,makename,files)
// Copyright ENPC/INRIA
// Updated by Allan CORNET INRIA 2006
// call make for target files or objects depending
// on OS and compilers
// very similar to G_make
// if files is given the make is performed on each 
// target contained in files then a whole make is performed 
//-----------------------------------------------------------------------------
  [lhs,rhs]=argn(0);
  if rhs < 3 then files=[]; end 
  if typeof(lib_name)<>'string' then
    error('ilib_compile: first argument must be a string');
    return ;
  end
  oldpath=getcwd();
  files=files(:)';
  files1=strsubst(strsubst(files,'.obj','') ,'.o','');
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names(lib_name,makename,files)  
  if path<> '';  chdir(path);  end 
  // first try to build each file step by step 
  nf = size(files,'*');
  for i=1:nf 
    write(%io(2),'   compilation of '+files1(i));
    unix_s(make_command+makename + ' '+ files(i)); 
  end
  // then the shared library 
  write(%io(2),'   building shared library (be patient)');
  unix_s(make_command+makename + ' '+ lib_name); 
  // a revoir 
  libn=path+lib_name_make ; 
  chdir(oldpath)
endfunction
//-----------------------------------------------------------------------------
function [make_command,lib_name_make,lib_name,path,makename,files]=ilib_compile_get_names(lib_name,makename,files) 
// return is res the correct name for 
// makefile, libname, files 
  files=strsubst(strsubst(files,'.obj','') ,'.o',''); //compat
  k=strindex(makename,['/','\'])
  if k~=[] then
    path=part(makename,1:k($))
    makename=part(makename,k($)+1:length(makename))
  else
     path=''
  end
  comp_target = COMPILER;
  if with_lcc()==%T then
  	lib_name=lib_name+'.dll'
    	lib_name_make=lib_name;
    	makename = makename + '.lcc' ; 
      	make_command = 'make -f '
      	if files<>[] then files = files + '.obj' ;
      	end
  else if getenv('WIN32','NO')=='OK' then
    lib_name=lib_name+'.dll'
    lib_name_make=lib_name;
    select comp_target
     case 'VC++' then 
      makename = makename + '.mak' ; 
      vcvompilerversion=findmsvccompiler();
      if (vcvompilerversion=='msvc80express') | (vcvompilerversion=='msvc80pro') | (vcvompilerversion=='msvc80std') then
        make_command = 'nmake /Y /nologo /f '
      else
        make_command = 'nmake /nologo /f '
      end  
      if files<>[] then 
	files = files + '.obj' ;
      end
     case 'gcc' then 
       makename = makename;
       make_command = 'make -f '
       if files<>[] then 
	 files = files + '.o' ;
       end
    else // like gnuwin32 
       makename = makename;
       make_command = 'make -f '
       if files<>[] then 
	 files = files + '.o' ;
       end
    end
  else
     if files <> [] then 
       files = files + '.o';
     end
     lib_name_make=lib_name+'.'+ilib_unix_soname();
     lib_name = lib_name+'.la'; 
     make_command = 'make -f ';
  end
  end
  
endfunction 
//-----------------------------------------------------------------------------
