// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
// Generate a Makefile which can be used by ilib_*
// Rewrited by Sylvestre Ledru in order to use autotools
//==========================================
function ilib_gen_Make_unix(names, ..
                                 files, ..
                                 libs, ..
								 libname, ..
							 ldflags, ..
							 cflags, ..
							 fflags, ..
							 cc ..							 
							 )
						 
if libname == "" then libname = names(1);end 

if ( strncpy(libname,3) == "lib")
  l=strsplit(libname,3); libname=l(2); 
  clear l
end

warningmode = warning('query');

originPath = pwd();
linkpath = TMPDIR;
//  if ldflags <> '' | cflags <> '' | fflags <> '' | cc <> '' then
if ldflags == '' & cflags == '' & fflags == '' & cc == '' then
  commandpath = SCI+"/modules/dynamic_link/src/scripts/";
  // The normal configure in the standard path
  generateConfigure(commandpath)
end
  
	// Copy files => linkpath
	chdir(linkpath);
	if (warningmode == 'on') then
	  disp(msprintf(gettext("%s: Copy compilation files (Makefile*, libtool...) to TMPDIR\n"),"ilib_gen_Make"));
	end

 	// List of the files mandatory to generate a lib with the detection of the env
	mandatoryFiles = ["Makefile.orig", ..
	                  "configure.ac", ..
	                  "configure", ..
	                  "Makefile.am", ..
	                  "Makefile.in", ..
	                  "config.sub", ..
	                  "config.guess", ..
	                  "config.status", ..
	                  "depcomp", ..
	                  "install-sh", ..
	                  "ltmain.sh", ..
	                  "libtool", ..
	                  "missing", ..
	                  "aclocal.m4"];

	// Copy files to the working tmpdir
	for x = mandatoryFiles(:)' ;
		[status,msg]=copyfile(commandpath+"/"+x,linkpath);
		if (status <> 1)
		  error(msprintf(gettext("%s: An error occurred: %s\n"), "ilib_gen_Make",msg));
		end
	end

	filelist = "";	
	for x = files(:)' ;  
	  // Pre added file in the list ... don't really know why
	  if (x <> "csci") then
		// Old way: to compile a fun.c file, the user had to provide fun.o
		filename = strsubst(x,'.o','');
		chdir(originPath); // Switch back to the source dir in order to have only the filename
		filesMatching = ls(filename+".*");
		// The user provided the real filename
		if filesMatching == [] then
		  if (warningmode == 'on') then
			disp("Copy "+x+".* to TMPDIR")
		  end
		  copyfile(x, linkpath);
		  filelist = filelist + " " + x ;
		else
		// Or copy the file matching to what we were looking for (this stuff
		// could lead to bug if you have fun.c fun.f or fun.cxx but it was 
		// already the case before ...
		for f=filesMatching(:)' ;
		  if (warningmode == 'on') then
			disp(msprintf("%s: Copy %s to TMPDIR\n","ilib_gen_Make",f));
		  end
				copyfile(f, linkpath)
				filelist = filelist + " " + f;
			end
		end
		chdir(linkpath);


		end
	  end
	  
	  if ldflags <> '' | cflags <> '' | fflags <> '' | cc <> '' then
		// Rerun the ./configure with the flags
		generateConfigure(linkpath, ldflags, cflags, fflags, cc)
	  end
		
	// Alter the Makefile in order to compile the right files
	if (warningmode == 'on') then
	  disp(msprintf(gettext("%s: Modification of the Makefile in TMPDIR.\n"),"ilib_gen_Make"));
	end
	cmd=commandpath + "scicompile.sh " + libname + " " + filelist

	[msg,ierr] = unix_g(cmd);

	if ierr <> 0 then
	  disp(msg);
	  return;
	end
	
	chdir(originPath);
	
endfunction


function generateConfigure(workingPath, ldflags, ..
	cflags, ..
	fflags, ..
	cc)


// We launch ./configure in order to produce a "generic" Makefile 
// for this computer
cmd=''
// CFLAGS
if cflags <> '' then
  cmd = cmd +" CFLAGS="""+cflags+""""
end

// LDFLAGS
if ldflags <> '' then
  cmd = cmd +" LDFLAGS="""+ldflags+""""
end

// FFLAGS
if fflags <> '' then
  cmd = cmd +" FFLAGS="""+fflags+""""
end

// CC
if cc <> '' then
  cmd = cmd +" CC="""+cc+""""
end

if (warningmode == 'on') then
  disp(msprintf(gettext("%s: configure : Generate Makefile in %s\n"),"ilib_gen_Make",workingPath));
end
cmd=workingPath+"/compilerDetection.sh "+cmd
[msg,ierr] = unix_g(cmd);

if ierr <> 0 then
  disp(msg);
  return %F;
end
return %T

endfunction
