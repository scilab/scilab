
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function clean_help(dirs)
	
	// dirs is a set of directories for which html manuals are to be deleted
	
	lines(0);
	
	global %helps;
	global %helps_modules;
	%HELPS=[%helps_modules;%helps];
	
	ok = %F;
	
	// Check of arguments
	// ------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs > 1 then error(39); end
	
	if exists('dirs') then
		if type(dirs) <> 10 then error(55,1); end
	end
	
	// Save of the current dir
	// ------------------------------------------------------------------------
	current_directory = pwd();
	
	// Default case, clean up of the %helps directories
	// ------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		dirs_to_clean = %HELPS;
		
		//---------------------------------------------------------------------
		// Workaround because scicos is not written in xml
		//---------------------------------------------------------------------
		scs = grep(dirs_to_clean,'scicos');
		if size(scs,'*') == 1 then dirs_to_clean(scs,:)=[]; end
		// End of patch -------------------------------------------------------
		dirs = dirs_to_clean(:,1);
	end
	
	// Translate path(s) into absolute path
	// ------------------------------------------------------------------------
	
	for k=1:size(dirs,'*')
		try
			if typeof(dirs(k)) <> 'string' then
				// Probleme ? ? ?
			else
				chdir(dirs(k));
				if MSDOS then
					dirs(k) = getlongpathname(pwd());
				else
					dirs(k) = pwd();
				end
				chdir(current_directory);
			end
		catch
		  // Probleme ? ? ?
		end
	end

	// Cleanup directories one by one
	// ------------------------------------------------------------------------

	mprintf(gettext("-- Deleting "+getlanguage()+" help files --"));
	
	for k=1:size(dirs,'*')
		
		files_to_delete = listfiles([ ..
			dirs(k)+"/*.htm"; ..
			dirs(k)+"/.list*"; ..
			dirs(k)+"/.last_successful_build*"; ..
			dirs(k)+"/*.log"; ..
			dirs(k)+"/master_help.xml"; ..
			dirs(k)+"/*_help.jar"; ..
			dirs(k)+"/*_help.pdf"; ..
			dirs(k)+"/*_help.ps"; ..
			dirs(k)+"/*_help.chm" ..
		]);
		
		for i=1:size(files_to_delete,'*')
			mprintf(".");
			mdelete(files_to_delete(i));
		end
	end
	
	// Clean up helptools dir
	// ------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
	
		if fileinfo(SCI+pathconvert("/modules/helptools/index_"+getlanguage()+".htm",%f,%f)) <> [] then
			mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/index_"+getlanguage()+".htm",%f,%f));
		end
		
		if fileinfo(SCI+pathconvert("/modules/helptools/contents_"+getlanguage()+".htm",%f,%f)) <> [] then
			mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/contents_"+getlanguage()+".htm",%f,%f));
		end
		
		if fileinfo(SCI+pathconvert("/modules/helptools/jar/scilab_"+getlanguage()+"_help.jar",%f,%f)) <> [] then
			mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/jar/scilab_"+getlanguage()+"_help.jar",%f,%f));
		end
		
		if fileinfo(SCI+pathconvert("/modules/helptools/scilab_"+getlanguage()+"_help.jar.log",%f,%f)) <> [] then
			mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/scilab_"+getlanguage()+"_help.jar.log",%f,%f));
		end
		
		if fileinfo(SCI+pathconvert("/modules/helptools/master_"+getlanguage()+"_help.xml",%f,%f)) <> [] then
			mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/master_"+getlanguage()+"_help.xml",%f,%f));
		end
		
	end
	
	mprintf("\n");
	
endfunction
