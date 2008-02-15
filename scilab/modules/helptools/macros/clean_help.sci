
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function clean_help(dirs)
	
	// =========================================================================================
	// Author : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : August 2st 2006
	//
	// dirs is a set of directories for which html manuals are to be deleted
	// =========================================================================================
	
	lines(0);
	
	global %helps;
	global %helps_modules;
	%HELPS=[%helps_modules;%helps];
	
	ok = %F;
	
	// V�rification des param�tres
	// --------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs > 1 then error(39); end
	
	if exists('dirs') then
		if type(dirs) <> 10 then error(55,1); end
	end
	
	// Sauvegarde du chemin dans lequel l'on se trouve
	// --------------------------------------------------------------------------------
	current_directory = pwd();
	
	// Cas par d�faut : Nettoyage des r�pertoires cit�s dans %helps
	// --------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		dirs_to_clean = %HELPS;
		
		//----------------------------------------------------------------------------------
		// Patch because scicos is not written in xml
		//----------------------------------------------------------------------------------
		scs = grep(dirs_to_clean,'scicos');
		if size(scs,'*') == 1 then dirs_to_clean(scs,:)=[]; end
		// End of patch --------------------------------------------------------------------
		
		dirs = dirs_to_clean(:,1);
	end
	
	// On transforme le ou les chemins donn�s en chemin absolu
	// --------------------------------------------------------------------------------
	
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

	// Nettoyage des r�pertoires un par un
	// --------------------------------------------------------------------------------

	mprintf(gettext("-- Deleting "+getlanguage()+" help files --"));
	
	for k=1:size(dirs,'*')
		files_to_delete = listfiles([dirs(k)+"/*.htm";dirs(k)+"/.list*";dirs(k)+"/.last_successful_build*"]);
		for i=1:size(files_to_delete,'*')
			mprintf(".");
			mdelete(files_to_delete(i));
		end
	end
	
	// Nettoyage des index et contents
	// --------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
	
	if fileinfo(SCI+pathconvert("/modules/helptools/index_"+getlanguage()+".htm",%f,%f)) <> [] then
		  mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/index_"+getlanguage()+".htm",%f,%f));
		end
			
		if fileinfo(SCI+pathconvert("/modules/helptools/contents_"+getlanguage()+".htm",%f,%f)) <> [] then
		  mprintf(".");
			mdelete(SCI+pathconvert("/modules/helptools/contents_"+getlanguage()+".htm",%f,%f));
		end
	end
	
	mprintf("\n");
	
endfunction
