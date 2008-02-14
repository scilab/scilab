//==========================================
// Copyright ENPC
// Copyright INRIA 
//==========================================
function libn = ilib_compile(lib_name,makename,files)

  if ~haveacompiler() then
  	error(_("A Fortran or C compiler is required."))
  	return;
  end

  [lhs,rhs]=argn(0);
  
  if rhs < 3 then files = []; end 
  
  if typeof(lib_name)<>'string' then
    error(msprintf(gettext("%s: Wrong type for first input argument: String expected.\n"),"ilib_compile"));
    return ;
  end
  
  oldpath = getcwd();
  files = files(:)';
  files1 = strsubst(strsubst(files,'.obj','') ,'.o','');
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names(lib_name,makename,files);

  if path<> '';  chdir(path);  end 
  
  // first try to build each file step by step 
  if MSDOS then
  
    nf = size(files,'*');
    
    for i=1:nf 
      write(%io(2),_("   compilation of ")+files1(i));
      unix_s(make_command+makename + ' '+ files(i)); 
    end
    
    // then the shared library 
    write(%io(2),_("   building shared library (be patient)"));
    unix_s(make_command + makename + ' '+ lib_name); 
   
  else
	  oldPath = pwd();
	  // Switch back to the TMPDIR where the mandatory files are
	  chdir(TMPDIR);

	  	// TODO : voir quoi faire du CFLAGS
	[msg,ierr] = unix_g("make CFLAGS=-I"+SCI+"/modules/core/includes/");
	if ierr <> 0 then
	  disp(msg);
	  return;
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

  files = strsubst(strsubst(files,'.obj','') ,'.o',''); //compatibility
  
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
  
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      makename = makename + '.mak' ; 
      make_command = 'nmake /Y /nologo /f '
      
      if files<>[] then 
	      files = files + '.obj' ;
      end
    else
      // LCC-WIN32
      if findlcccompiler() then
        makename = makename + '.lcc' ; 
        make_command = 'make -f ';
      
        if files<>[] then 
          files = files + '.obj' ;
        end
      else
      // TO DO : Add another compiler here
      end
    end
  else // LINUX
  
    make_command = 'make '; 
    
    if files <> [] then 
      files = files + '.o';
    end
    
  end
  
endfunction 

//==========================================
