function check_help(dirs)
	
	// =========================================================================================
	// Authors : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	//
	// Date : 08/08/2006
	// 
	// dirs is a set of directories for which html manuals are to be generated
	// =========================================================================================
	
	
	// Sauvegarde du chemin courant et de la variable %helps
	//------------------------------------------------------------------------------------------
	
	current_directory = pwd();
	
	[lhs,rhs]=argn(0);
	
	// Trop de paramètres
	// -----------------------------------------------------------------------------------------
	
	if rhs > 1 then
		error(39);
		return;
	end
	
	// Cas par défaut : Vérification de l'aide en ligne de Scilab
	// -----------------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		global %helps;
		dirs_to_build = %helps;
		clear %helps;
		
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
	
	// Gestion du fichier de log
	//------------------------------------------------------------------------------------------
	
	global LANGUAGE;
	logfile = pathconvert(SCIHOME+"/check_help_"+LANGUAGE+".log",%f,%f);
	clear LANGUAGE;
	
	logfile_id = mopen(logfile,"w");
	mclose(logfile_id);
	
	// Nombre de fichiers ayant une mauvaise syntaxe dans tous les répertoires traités
	//------------------------------------------------------------------------------------------
	
	nb_badfiles = 0;
	badfiles = [];
	
	// Boucle sur les répertoires
	//------------------------------------------------------------------------------------------
	
	for k1=1:size(dirs,'*')
		
		// Nombre de fichiers ayant une mauvaise syntaxe dans le répertoire traité
		//----------------------------------------------------------------------------------
		nb_badfiles_dir = 0;
		
		// Etablissement de la liste des fichiers XML
		//----------------------------------------------------------------------------------
		chdir(dirs(k1));
		xml = listfiles('*.xml');
		
		if xml <> [] then
		
			// On vérifie si le répertoire à besoin d'être vérifié
			//--------------------------------------------------------------------------
			
			if fileinfo(".last_successful_check") == [] then
				need_to_be_checked = %T;
			else
				exec(".last_successful_check",-1);
				
				// On détermine le fichier XML le plus récent
				//------------------------------------------------------------------
				
				max_change_date = 0;
				
				for i=1:size(xml,'*')
					file_date = fileinfo(xml(i));
					if file_date(6) > max_change_date then
						max_change_date = file_date(6);
					end
				end
				
				if max_change_date > last_successful_check_value then
					need_to_be_checked = %T;
				else
					need_to_be_checked = %F;
				end
			end
			
			// Boucle sur les fichiers XML
			//--------------------------------------------------------------------------
			
			if need_to_be_checked then
				
				if xml <> [] then 
					for k2=1:size(xml,'*')
						
						if k2 == 1 then
							mprintf("%s\n",dirs(k1));
						end
						
						if MSDOS then
							stat = unix(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+xml(k2)+" > NUL 2>&1");
						else
							stat = unix("xmllint --noout --valid "+xml(k2)+" > /dev/null 2>&1");
						end
						
						if stat <> 0 then
							
							nb_badfiles = nb_badfiles + 1;
							nb_badfiles_dir = nb_badfiles_dir + 1;
							
							badfile = pathconvert(dirs(k1)+"/"+xml(k2),%f,%f);
							badfiles = [badfiles;badfile];
							logfile_id = mopen(logfile,"a+");
							mfprintf(logfile_id,"\n----------------------------------------------------------------------\n");
							mfprintf(logfile_id,"%s\n",badfile);
							mfprintf(logfile_id,"----------------------------------------------------------------------\n");
							mclose(logfile_id);
							
							if MSDOS then
								unix(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid """+xml(k2)+""" >> """+logfile+""" 2>&1");
							else
								unix("xmllint --noout --valid """+xml(k2)+""" >> """+logfile+""" 2>&1");
							end
						end
					end
				end
				
				// Création du fichier "directory/.last_successful_check"
				//------------------------------------------------------------------
				
				if nb_badfiles_dir == 0 then
					dateToPrint = msprintf("last_successful_check_value = %d",getdate('s'));
					mputl(dateToPrint,pathconvert(".last_successful_check",%f,%f));
				end
			end
		end
	end
	
	
	if nb_badfiles == 1 then
		mprintf("\nBad syntax in 1 file :\n");
	elseif nb_badfiles > 0 then
		mprintf("\nBad syntax in %d files :\n",nb_badfiles);
	else
		mprintf("\nAll xml files are correct\n");
	end
	
	
	if nb_badfiles > 0 then
		for k=1:size(badfiles,'*')
			mprintf("\t- %s\n",badfiles(k));
		end
		
		if grep(sciargs(),"-nw") == [] then
			scipad(logfile);
		else
			mprintf("\n\tSee %s\n",logfile);
		end
	end
	
	// On remet l'environement initial
	//------------------------------------------------------------------------------------------
	
	chdir(current_directory);
	
endfunction
