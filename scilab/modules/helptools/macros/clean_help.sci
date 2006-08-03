function clean_help(dirs)
	
	// =========================================================================================
	// Author : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : August 2st 2006
	// 
	// dirs is a set of directories for which html manuals are to be deleted
	// =========================================================================================
	
	global LANGUAGE %helps
	
	ok = %F;
	
	// Vérification des paramètres
	// -----------------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs > 1 then error(39); end
	
	if exists('dirs') then
		if type(dirs) <> 10 then error(55,1); end
	end
	
	// Sauvegarde du chemin dans lequel l'on se trouve
	// -----------------------------------------------------------------------------------------
	current_directory = pwd();
	
	// Cas par défaut : Nettoyage des répertoires cités dans %helps
	// -----------------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		dirs_to_clean = %helps;
		
		//----------------------------------------------------------------------------------
		// Patch because scicos is not written in xml
		//----------------------------------------------------------------------------------
		scs = grep(dirs_to_clean,'scicos');
		if size(scs,'*') == 1 then dirs_to_clean(scs,:)=[]; end
		// End of patch --------------------------------------------------------------------
		
		dirs = dirs_to_clean(:,1);
	end
	
	// On transforme le ou les chemins donnés en chemin absolu
	// -----------------------------------------------------------------------------------------
	
	for k=1:size(dirs,'*')
		chdir(dirs(k));
		if MSDOS then
			dirs(k) = getlongpathname(pwd());
		else
			dirs(k) = pwd();
		end
		chdir(current_directory);
	end
	
	// Nettoyage des répertoires un par un
	// -----------------------------------------------------------------------------------------
	
	mprintf("deleting files ");
	
	for k=1:size(dirs,'*')
		files_to_delete = listfiles([dirs(k)+"/*.htm";dirs(k)+"/.list*";dirs(k)+"/.last_successful_build*"]);
		for i=1:size(files_to_delete,'*')
			mprintf(".");
			mdelete(files_to_delete(i));
		end
	end
	
	// Nettoyage des index et contents
	// -----------------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		select LANGUAGE
		
		case 'eng' then
			if fileinfo(SCI+pathconvert("/modules/helptools/index_eng.htm",%f,%f)) <> [] then
				mprintf(".");
				mdelete(SCI+pathconvert("/modules/helptools/index_eng.htm",%f,%f));
			end
			
			if fileinfo(SCI+pathconvert("/modules/helptools/contents_eng.htm",%f,%f)) <> [] then
				mprintf(".");
				mdelete(SCI+pathconvert("/modules/helptools/contents_eng.htm",%f,%f));
			end
			
		case 'fr' then
			if fileinfo(SCI+pathconvert("/modules/helptools/index_fr.htm",%f,%f)) <> [] then
				mprintf(".");
				mdelete(SCI+pathconvert("/modules/helptools/index_fr.htm",%f,%f));
			end
			
			if fileinfo(SCI+pathconvert("/modules/helptools/contents_fr.htm",%f,%f)) <> [] then
				mprintf(".");
				mdelete(SCI+pathconvert("/modules/helptools/contents_fr.htm",%f,%f));
			end
			
		end
	end
	
	mprintf("\n");
	
endfunction
