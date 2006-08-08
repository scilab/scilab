function check_help(dirs)
	
	// =========================================================================================
	// Authors : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// 
	// dirs is a set of directories for which html manuals are to be generated
	// =========================================================================================
	
	global LANGUAGE %helps
	
	//------------------------------------------------------------------------------------------
	// Sauvegarde du chemin courant et de la variable %helps
	//------------------------------------------------------------------------------------------
	
	current_directory = pwd();
	saved_helps = %helps;
	
	//------------------------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0);
	
	// Trop de paramètres
	// -----------------------------------------------------------------------------------------
	
	if rhs > 1 then
		error(39);
		%helps = saved_helps;
		chdir(current_directory);
		return;
	end
	
	// Cas par défaut : Vérification de l'aide en ligne de Scilab
	// -----------------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		dirs_to_build = %helps;
		
		//----------------------------------------------------------------------------------
		// Patch because scicos is not written in xml
		//----------------------------------------------------------------------------------
		scs = grep(dirs_to_build,'scicos');
		if size(scs,'*') == 1 then dirs_to_build(scs,:)=[]; end
		// End of patch --------------------------------------------------------------------
		
		dirs = dirs_to_build(:,1);
	end
	
	// On transforme le ou les chemins donnés en chemin absolu
	// -----------------------------------------------------------------------------------------
	
	for k=1:size(dirs,'*');
		chdir(dirs(k));
		if MSDOS then
			dirs(k) = getlongpathname(pwd());
		else
			dirs(k) = pwd();
		end
		chdir(current_directory);
	end
	
	// 
	//------------------------------------------------------------------------------------------
	
	logfile = pathconvert(TMPDIR+"/check_help.log",%f,%f);
	
	logfile_id = mopen(logfile,"w");
	mclose(logfile_id);
	
	nb_badfiles = 0;
	badfiles = [];
	
	
	for k1=1:size(dirs,'*')
		
		// Etablissement de la liste des fichiers XML
		//----------------------------------------------------------------------------------
		
		chdir(dirs(k1));
		xml = listfiles('*.xml');
		
		// Boucle sur les fichiers XML
		//----------------------------------------------------------------------------------
		
		if xml <> [] then 
			for k2=1:size(xml,'*')
				
				if k2 == 1 then
					mprintf("%s\n",dirs(k1));
				end
				
				// mprintf(".");
				
				stat = unix("xmllint --noout --valid "+xml(k2)+" > /dev/null 2>&1");
				
				if stat <> 0 then
					nb_badfiles = nb_badfiles+1;
					badfile = pathconvert(dirs(k1)+"/"+xml(k2),%f,%f);
					badfiles = [badfiles;badfile];
					logfile_id = mopen(logfile,"a+");
					mfprintf(logfile_id,"\n----------------------------------------------------------------------\n");
					mfprintf(logfile_id,"%s\n",badfile);
					mfprintf(logfile_id,"----------------------------------------------------------------------\n");
					mclose(logfile_id);
					unix("xmllint --noout --valid "+xml(k2)+" >> "+logfile+" 2>&1");
				end
				
				// if k2 == size(xml,'*') then
				//	mprintf("\n");
				// end
			end
		end
	end
	
	if nb_badfiles == 1 then
		mprintf("\nBad syntax in 1 file :\n");
		for k=1:size(badfiles,'*')
			mprintf("\t- %s\n",badfiles(k));
		end
		scipad(logfile);
	elseif nb_badfiles > 0 then
		mprintf("\nBad syntax in %d files :\n",nb_badfiles);
		for k=1:size(badfiles,'*')
			mprintf("\t- %s\n",badfiles(k));
		end
		scipad(logfile);
	else
		mprintf("\nAll xml files are correct\n");
	end
	
endfunction
