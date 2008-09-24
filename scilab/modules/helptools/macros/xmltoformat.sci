// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function generated_files = xmltoformat(output_format,dirs,titles,directory_language,default_language)
	
	// =========================================================================
	// + output_format : "javaHelp", "pdf", "chm", "ps"
	//
	// + dirs : A set of directories for which help files (jar, pdf, chm, ...) are
	//        genereated
	//
	// + titles : associated title strings (optional or [])
	//
	// + directory_language : language in which the help is written
	//
	// + default_language : The directory we compiling is completed with help files
	//                      from the default_language
	//
	// if dirs is not specified or [] then standard scilab man are assumed and 
	// titles are searched in %helps
	// =========================================================================
	
	global %helps;
	global %helps_modules;
	%HELPS=[%helps_modules;%helps];
	
	SCI_long = pathconvert(getlongpathname(SCI),%F,%F);
	
	//--------------------------------------------------------------------------
	// Sauvegarde de l'environnement initial
	//--------------------------------------------------------------------------
	
	current_directory   = pwd();
	saved_helps         = %HELPS;
	%helps_save         = %helps;
	%helps_modules_save = %helps_modules;
	
	//------------------------------------------------------------------
	// Patch because scicos is not written in xml
	//------------------------------------------------------------------
	%helps(grep(%helps,filesep()+"modules"+filesep()+"scicos"+filesep()),:) = [];
	
	all_scilab_help     = %F;
	
	[lhs,rhs] = argn(0);
	
	generated_files = [];
	
	// Trop de paramêtres
	// ---------------------------------------------------------------------
	
	if rhs > 5 | rhs < 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"xmltoformat",1,5));
	end
	
	// Transformation du %helps_modules jar => help/language
	
	// Cas par défaut : construction de l'aide en ligne de Scilab
	// ---------------------------------------------------------------------
	
	if (rhs <= 1) | ((rhs == 2) & (dirs == [])) then
		
		all_scilab_help      = %T;
		
		dirs_to_build          = %HELPS;
		dirs_to_build_m        = %helps_modules;
		dirs_to_build_c        = %helps;
		
		dirs                   = get_xml_path(dirs_to_build(:,1));
		dirs_m                 = get_xml_path(dirs_to_build_m(:,1));
		dirs_c                 = get_xml_path(dirs_to_build_c(:,1));
		
		titles                 = dirs_to_build(:,2);
		titles_m               = dirs_to_build_m(:,2);
		titles_c               = dirs_to_build_c(:,2);
		
		dirs(     find(dirs   == "")) = [];
		dirs_m(   find(dirs_m == "")) = [];
		dirs_c(   find(dirs_c == "")) = [];
		titles(   find(dirs   == "")) = [];
		titles_m( find(dirs_m == "")) = [];
		titles_c( find(dirs_c == "")) = [];
		
		directory_language     = [];
		directory_language_m   = [];
		directory_language_c   = [];
		
		default_language       = [];
		default_language_m     = [];
		default_language_c     = [];
		
		language_system        = []; // language_system is equal to %T when
		language_system_m      = []; // a help directory needs to be completed
		language_system_c      = []; // with the default language
		
		if getlanguage() == getdefaultlanguage() then
			for k=1:size(dirs,'*')
				directory_language = [directory_language;getdefaultlanguage()];
				language_system    = [language_system;%F];
			end
			for k=1:size(dirs_m,'*')
				directory_language_m = [directory_language_m;getdefaultlanguage()];
				language_system_m    = [language_system_m;%F];
			end
			for k=1:size(dirs_c,'*')
				directory_language_c = [directory_language_c;getdefaultlanguage()];
				language_system_c    = [language_system_c;%F];
			end
		else
			for k=1:size(dirs,'*')
				directory_language = [directory_language;getlanguage()];
				default_language   = [default_language;getdefaultlanguage()];
				language_system    = [language_system;%T]; // Enable the language system
			end
			for k=1:size(dirs_m,'*')
				directory_language_m = [directory_language_m;getlanguage()];
				default_language_m   = [default_language_m;getdefaultlanguage()];
				language_system_m    = [language_system_m;%T]; // Enable the language system
			end
			for k=1:size(dirs_c,'*')
				directory_language_c = [directory_language_c;getlanguage()];
				default_language_c   = [default_language_c;getdefaultlanguage()];
				language_system_c    = [language_system_c;%T]; // Enable the language system
			end
		end
	
	// Cas ou seulement le ou les répertoires sont précisés
	// ---------------------------------------------------------------------
	
	elseif (rhs == 2) & (dirs <> []) then
		
		language_system    = [];
		titles             = [];
		directory_language = [];
		default_language   = [];
		
		for k=1:size(dirs,'*')
			directory_language = [directory_language ; guess_lang(dirs(k)) ]; // Language detection
			default_language   = [default_language   ; getdefaultlanguage()]; // Default Language
			titles             = [titles;gettext("Help chapter")+" ("+dirs(k)+")"];
			
			if directory_language(k) == default_language(k) then
				language_system    = [language_system;%F]; // Enable the language system
			else
				language_system    = [language_system;%T]; // Enable the language system
			end
		end
		
	// Cas ou seulement le ou les répertoires ainsi que le ou les titres
	// sont précisés
	// ---------------------------------------------------------------------
	
	elseif rhs == 3 then
		
		language_system    = [];
		directory_language = [];
		default_language   = [];
		
		for k=1:size(dirs,'*')
			directory_language = [directory_language ; guess_lang(dirs(k)) ]; // Language detection
			default_language   = [default_language   ; getdefaultlanguage()]; // Default Language
			
			if directory_language(k) == default_language(k) then
				language_system    = [language_system;%F]; // Enable the language system
			else
				language_system    = [language_system;%T]; // Enable the language system
			end
		end
	
	// Cas les répertoires,les titres ainsi que la
	// langue du répertoire sont précisés
	// ---------------------------------------------------------------------
	
	elseif rhs == 4 then
		
		language_system   = [];
		default_language  = [];
		
		for k=1:size(dirs,'*')
			default_language = [default_language   ; getdefaultlanguage()]; // Default Language
			
			if directory_language(k) == default_language(k) then
				language_system    = [language_system;%F]; // Enable the language system
			else
				language_system    = [language_system;%T]; // Enable the language system
			end
		end
	
	// Cas où tous est précisé
	// ---------------------------------------------------------------------
	
	elseif rhs == 5 then
		
		language_system   = [];
		
		for k=1:size(dirs,'*')
			if isdir(pathconvert(dirs(k)+"/../"+default_language(k),%f,%f)) then
				if directory_language(k) == default_language(k) then
					language_system    = [language_system;%F]; // Enable the language system
				else
					language_system    = [language_system;%T]; // Enable the language system
				end
			end
		end
		
	end
	
	// On transforme le ou les chemins donnés en chemin absolu
	// ---------------------------------------------------------------------
	
	for k=1:size(dirs,'*');
		if ~isdir(dirs(k)) then
			error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs(k)));
		end
		
		chdir(dirs(k));
		if MSDOS then
			dirs(k) = getlongpathname(pwd());
		else
			dirs(k) = pwd();
		end
		chdir(current_directory);
	end
	
	if all_scilab_help then
		for k=1:size(dirs_m,'*');
			if ~isdir(dirs_m(k)) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs_m(k)));
			end
			chdir(dirs_m(k));
			if MSDOS then
				dirs_m(k) = getlongpathname(pwd());
			else
				dirs_m(k) = pwd();
			end
			chdir(current_directory);
		end
		
		for k=1:size(dirs_c,'*');
			if ~isdir(dirs_c(k)) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs_c(k)));
			end
			chdir(dirs_c(k));
			if MSDOS then
				dirs_c(k) = getlongpathname(pwd());
			else
				dirs_c(k) = pwd();
			end
			chdir(current_directory);
		end
	end
	
	//----------------------------------------------------------------------
	// On établit la liste des répertoires nécéssitants d'étre reconstruit
	//----------------------------------------------------------------------
	
	need_to_be_build_tab   = [];
	need_to_be_build_tab_m = [];
	need_to_be_build_tab_c = [];
	
	for k=1:size(dirs,'*');
		if language_system(k) then
			need_to_be_build_tab = [need_to_be_build_tab;need_to_be_build(dirs(k),directory_language(k),default_language(k))];
		else
			need_to_be_build_tab = [need_to_be_build_tab;need_to_be_build(dirs(k))];
		end
	end
	
	if all_scilab_help then
		for k=1:size(dirs_m,'*');
			if language_system_m(k) then
				need_to_be_build_tab_m = [need_to_be_build_tab_m;need_to_be_build(dirs(k),directory_language_m(k),default_language_m(k))];
			else
				need_to_be_build_tab_m = [need_to_be_build_tab_m;need_to_be_build(dirs(k))];
			end
		end
		for k=1:size(dirs_c,'*');
			if language_system_c(k) then
				need_to_be_build_tab_c = [need_to_be_build_tab_c;need_to_be_build(dirs(k),directory_language_c(k),default_language_c(k))];
			else
				need_to_be_build_tab_c = [need_to_be_build_tab_c;need_to_be_build(dirs(k))];
			end
		end
	end
	
	if ~or(need_to_be_build_tab) then
		mprintf(gettext("%s: The %s files are up-to-date."),"xmltoformat",output_format);
		return;
	end
	
	// Nombre de répertoire ayant besoin d'une modification
	// ---------------------------------------------------------------------
	
	nb_dir = size( find(need_to_be_build_tab == %T) , '*' );
	
	//----------------------------------------------------------------------
	// Complete the on-line help with the default language
	//----------------------------------------------------------------------
	
	displaydone = 0;
	
	if all_scilab_help then
		
		if or(need_to_be_build_tab_m) then
			for k=1:size(dirs_m,'*')
				if  language_system_m(k) then
					default_language_path = pathconvert(dirs_m(k)+"/../"+default_language_m(k),%f,%f);
					if displaydone == 0 then
						mprintf(_("\nCopying missing help files copied from the default language\n"));
						displaydone = 1;
					end
					mprintf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					complete_with_df_lang(dirs_m(k),directory_language_m(k),default_language_m(k));
				end
			end
		end
		
		if or(need_to_be_build_tab_c) then
			for k=1:size(dirs_c,'*')
				if need_to_be_build_tab_c(k) & language_system_c(k) then
					default_language_path = pathconvert(dirs_c(k)+"/../"+default_language_c(k),%f,%f);
					if nb_dir > 1 then
						if displaydone == 0 then
							mprintf(_("\nCopying missing files copied from\n"));
							displaydone = 1;
						end
						mprintf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					else
						mprintf(_("\nCopying missing from %s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					end
					complete_with_df_lang(dirs_c(k),directory_language_c(k),default_language_c(k));
				end
			end
		end
		
	else
	
		if or(need_to_be_build_tab) then
			
			for k=1:size(dirs,'*')
				if need_to_be_build_tab(k) & language_system(k) then
					default_language_path = pathconvert(dirs(k)+"/../"+default_language(k),%f,%f);
					if nb_dir > 1 then
						if displaydone == 0 then
							mprintf(_("\nCopying missing files copied from\n"));
							displaydone = 1;
						end
						mprintf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					else
						mprintf(_("\nCopying missing from %s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					end
					complete_with_df_lang(dirs(k),directory_language(k),default_language(k));
				end
			end
		end
	
	end
	
	//----------------------------------------------------------------------
	// build all the Master document
	//----------------------------------------------------------------------
	
	if all_scilab_help then
		
		master_doc = SCI+"/modules/helptools/master_"+getlanguage()+"_help.xml";
		
		if or(need_to_be_build_tab_m) then
			mprintf(_("\nBuilding the Scilab manual master document for %s.\n"),getlanguage());
			create_MD(dirs_m,titles_m,master_doc,getlanguage());
			
		end
		
		if or(need_to_be_build_tab_c) then
			for k=1:size(dirs_c,"*")
				if need_to_be_build_tab_c(k) then
					mprintf(_("\nBuilding the master document: %s\n"),titles_c(k));
					create_MD_dir(dirs_c(k),titles_c(k),dirs_c(k)+"/master_help.xml",directory_language_c(k));
				end
			end
		end
		
	else
		
		displaydone = 0;
		for k=1:size(dirs,"*");
			if need_to_be_build_tab(k) then
				if nb_dir > 1 then
					if displaydone == 0 then
						mprintf(_("\nBuilding the master document for %s.\n"),getlanguage());
						displaydone = 1;
					end
					mprintf(_("\t%s\n"),strsubst(dirs(k),SCI_long,"SCI"));
				else
					mprintf(_("\nBuilding the master document in %s\n"),strsubst(dirs(k),SCI_long,"SCI"));
				end
				create_MD_dir(dirs(k),titles(k),dirs(k)+"/master_help.xml",directory_language(k));
			end
		end
	
	end
	
	//----------------------------------------------------------------------
	// perform the help generation
	//----------------------------------------------------------------------
	
	if output_format=="javaHelp" then
		output_format_ext = "jar";
	else
		output_format_ext = output_format;
	end
	
	is_html = (output_format == "html");
	
	if all_scilab_help then
		
		mprintf(_("\nBuilding the scilab manual file ["+output_format+"] (Please wait building ... this can take up to 10 minutes)\n"));
		
		// Define and create the path of buildDoc working directory
		buildDoc_dir  = pathconvert(dirs_c(k) + "/scilab_" + getlanguage() + "_help",%t,%f);
		
		// Define and create the final output directory if does not exist
		final_output_dir = pathconvert(SCI+"/modules/helptools/"+output_format_ext,%f,%f);
		
		if ~isdir(final_output_dir) then
			mkdir(final_output_dir);
		end
		
		if is_html then
			final_output_dir = pathconvert(final_output_dir+"/"+getlanguage(),%f,%f);
			if ~isdir(final_output_dir) then
				mkdir(final_output_dir);
			end
		end
		
		// Define the final location of the generated file
		if is_html then
			final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
		else
			final_help_file = pathconvert(final_output_dir+"/scilab_" + getlanguage() + "_help." + output_format_ext,%f,%f);
		end
		
		// Define and create the path of buildDoc working directory
		buildDoc_dir  = pathconvert(SCI+"/modules/helptools/"+ output_format + "/scilab_" + getlanguage() + "_help",%t,%f);
		if ~isdir(pathconvert(SCI+"/modules/helptools/"+output_format,%f,%f)) then
			mkdir(pathconvert(SCI+"/modules/helptools/"+output_format,%f,%f));
		end
		if ~isdir(buildDoc_dir) then
			mkdir(buildDoc_dir);
		end
		
		// Define the path of the generated file created by buildDoc
		if ~is_html then
			buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + getlanguage() + "_help." + output_format_ext,%f,%f);
		end
		
		// Save the current directory
		cur_dir = getcwd();
		
		// Change Scilab current directory so that Java Indexer works
		if ~chdir(buildDoc_dir) then
			error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
		end
		
		// process the build
		buildDoc(output_format);
		
		// Check if the help file has been generated
		if fileinfo(buildDoc_file)==[] then
			error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
		end
		
		// move the generated file(s)
		if is_html then
			my_html_files = listfiles(buildDoc_dir);
			for k=1:size(my_html_files,'*')
				if ~copyfile(my_html_files(k),pathconvert(final_output_dir+"/"+my_html_files(k),%f,%f)) then
					error(msprintf(gettext("%s: %s file hasn''t been moved in the %s directory."),"xmltoformat",my_html_files(k),final_output_dir));
				end
				mdelete(my_html_files(k));
			end
		else
			copyfile(buildDoc_file,final_help_file);
			mdelete(buildDoc_file);
		end
		
		// Move into the initial directory
		if ~chdir(cur_dir) then
			error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",oldDir));
		end
		
		// Now we can add the help file to the list of all generated files
		generated_files = [ generated_files ; final_help_file ];
		
		// Now, build toolboxes help (if any)
		
		displaydone = 0;
		
		for k=1:size(dirs_c,"*");
			if need_to_be_build_tab_c(k) then
				
				mprintf(_("\nBuilding the manual file [%s] in %s. (Please wait building ... this can take up to 10 minutes)\n"),output_format,strsubst(dirs_c(k),SCI_long,"SCI"));
				
				// Define and create the final output directory if does not exist
				final_output_dir = pathconvert(dirs_c(k)+"/../../"+output_format_ext,%f,%f);
				
				if ~isdir(final_output_dir) then
					mkdir(final_output_dir);
				end
				
				if is_html then
					final_output_dir = pathconvert(final_output_dir+"/"+directory_language_c(k),%f,%f);
					if ~isdir(final_output_dir) then
						mkdir(final_output_dir);
					end
				end
				
				// Define the final location of the generated file
				if is_html then
					final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
				else
					final_help_file = pathconvert(final_output_dir+"/scilab_" + directory_language_c(k) + "_help." + output_format_ext,%f,%f);
				end
				
				// Define and create the path of buildDoc working directory
				buildDoc_dir  = pathconvert(dirs_c(k) + "/scilab_" + directory_language_c(k) + "_help",%t,%f);
				if ~isdir(buildDoc_dir) then
					mkdir(buildDoc_dir);
				end
				
				// Define the path of the generated file created by buildDoc
				if ~is_html then
					buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + directory_language_c(k) + "_help." + output_format_ext,%f,%f);
				end
				
				// Save the current directory
				cur_dir = getcwd();
				
				// Change Scilab current directory so that Java Indexer works
				if ~chdir(buildDoc_dir) then
					error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
				end
				
				// process the build
				buildDoc(output_format,dirs_c(k)+"/master_help.xml",directory_language_c(k),dirs_c(k));
				
				// Check if the help file has been generated
				if fileinfo(buildDoc_file)==[] then
					error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
				end
				
				// move the generated file(s)
				if is_html then
					my_html_files = listfiles(buildDoc_dir);
					for k=1:size(my_html_files,'*')
						if ~copyfile(my_html_files(k),pathconvert(final_output_dir+"/"+my_html_files(k),%f,%f)) then
							error(msprintf(gettext("%s: %s file hasn''t been moved in the %s directory."),"xmltoformat",my_html_files(k),final_output_dir));
						end
						mdelete(my_html_files(k));
					end
				else
					copyfile(buildDoc_file,final_help_file);
					mdelete(buildDoc_file);
				end
				
				// Move into the initial directory
				if ~chdir(cur_dir) then
					error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",oldDir));
				end
				
				// Now we can add the help file to the list of all generated files
				generated_files = [ generated_files ; final_help_file ];
				
			end
		end
		
	else
		
		// Dirs are precised in the input arguments
		
		displaydone = 0;
		
		for k=1:size(dirs,"*");
		
			if need_to_be_build_tab(k) then
				
				if nb_dir > 1 then
					if displaydone == 0 then
						mprintf(_("\nBuilding the manual file [%s]. (Please wait building ... this can take up to 10 minutes)\n"),output_format);
						displaydone = 1;
					end
					mprintf(_("\t%s\n"),strsubst(dirs(k),SCI_long,"SCI"));
				else
					mprintf(_("\nBuilding the manual file [%s] in %s. (Please wait building ... this can take up to 10 minutes)\n"),output_format,strsubst(dirs(k),SCI_long,"SCI"));
				end
				
				// Define and create the final output directory if does not exist
				final_output_dir = pathconvert(dirs(k)+"/../../"+output_format_ext,%f,%f);
				
				if ~isdir(final_output_dir) then
					mkdir(final_output_dir);
				end
				
				if is_html then
					final_output_dir = pathconvert(final_output_dir+"/"+directory_language(k),%f,%f);
					if ~isdir(final_output_dir) then
						mkdir(final_output_dir);
					end
				end
				
				// Define the final location of the generated file
				if is_html then
					final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
				else
					final_help_file = pathconvert(final_output_dir+"/scilab_" + directory_language(k) + "_help." + output_format_ext,%f,%f);
				end
				
				// Define and create the path of buildDoc working directory
				buildDoc_dir  = pathconvert(dirs(k) + "/scilab_" + directory_language(k) + "_help",%t,%f);
				if ~isdir(buildDoc_dir) then
					mkdir(buildDoc_dir);
				end
				
				// Define the path of the generated file created by buildDoc
				if is_html then
					buildDoc_file = pathconvert(buildDoc_dir + "index.html",%f,%f);
				else
					buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + directory_language(k) + "_help." + output_format_ext,%f,%f);
				end
				
				// Save the current directory
				cur_dir = getcwd();
				
				// Change Scilab current directory so that Java Indexer works
				if ~chdir(buildDoc_dir) then
					error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
				end
				
				// process the build
				buildDoc(output_format,dirs(k)+"/master_help.xml",directory_language(k),dirs(k));
				
				// Check if the help file has been generated
				if fileinfo(buildDoc_file)==[] then
					error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
				end
				
				// move the generated file(s)
				if is_html then
					my_html_files = listfiles(buildDoc_dir);
					for k=1:size(my_html_files,'*')
						if ~copyfile(my_html_files(k),pathconvert(final_output_dir+"/"+my_html_files(k),%f,%f)) then
							error(msprintf(gettext("%s: %s file hasn''t been moved in the %s directory."),"xmltoformat",my_html_files(k),final_output_dir));
						end
						mdelete(my_html_files(k));
					end
				else
					copyfile(buildDoc_file,final_help_file);
					mdelete(buildDoc_file);
				end
				
				// Move into the initial directory
				if ~chdir(cur_dir) then
					error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",oldDir));
				end
				
				// Now we can add the help file to the list of all generated files
				generated_files = [ generated_files ; final_help_file ];
				
			end
		end
	end
	
	chdir(current_directory);
	
	//----------------------------------------------------------------------
	// Delete the xml files translated into the default language
	//----------------------------------------------------------------------
	
	displaydone = 0;
	
	if all_scilab_help then
		
		if or(need_to_be_build_tab_m) then
			for k=1:size(dirs_m,'*')
				if  language_system_m(k) then
					default_language_path = pathconvert(dirs_m(k)+"/../"+default_language_m(k),%f,%f);
					if displaydone == 0 then
						printf(_("\nDeleting files copied from\n"));
						displaydone = 1;
					end
					printf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					del_df_lang_xml_files(dirs(k),directory_language(k));
				end
			end
		end
		
		if or(need_to_be_build_tab_c) then
			for k=1:size(dirs_c,'*')
				if need_to_be_build_tab_c(k) & language_system_c(k) then
					default_language_path = pathconvert(dirs_c(k)+"/../"+default_language_c(k),%f,%f);
					if nb_dir > 1 then
						if displaydone == 0 then
							printf(_("\nDeleting files copied from\n"));
							displaydone = 1;
						end
						printf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					else
						printf(_("\nDeleting files copied from %s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					end
					del_df_lang_xml_files(dirs_c(k),directory_language_c(k));
				end
			end
		end
	else
		
		if or(need_to_be_build_tab) then
			for k=1:size(dirs,'*')
				if need_to_be_build_tab(k) & language_system(k) then
					default_language_path = pathconvert(dirs(k)+"/../"+default_language(k),%f,%f);
					if nb_dir > 1 then
						if displaydone == 0 then
							printf(_("\nDeleting files copied from\n"));
							displaydone = 1;
						end
						printf(_("\t%s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					else
						printf(_("\nDeleting files copied from %s\n"),strsubst(default_language_path,SCI_long,"SCI"));
					end
					del_df_lang_xml_files(dirs(k),directory_language(k));
				end
			end
		end
		
	end
	
	//----------------------------------------------------------------------
	// Création du fichier "directory/.last_successful_build_output_format"
	//----------------------------------------------------------------------
	
	for k=1:size(dirs,'*');
		if need_to_be_build_tab(k) then
			dateToPrint = msprintf("last_success_build_val = %d",getdate('s'));
			mputl(dateToPrint,pathconvert(dirs(k)+"/.last_successful_build_"+output_format,%f,%f));
		end
	end
	
	// On remet l'environement initial
	//--------------------------------------------------------------------------
	
	chdir(current_directory);
	%helps         = %helps_save;
	%helps_modules = %helps_modules_save;
	%HELPS         = saved_helps;
	
endfunction

function complete_with_df_lang(directory,directory_language,default_language)
	
	//--------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 26, july 2006
	//
	// Cette macro complète un répertoire dont certaines aides en ligne sont manquantes
	// Elle le complète avec les aides en ligne de la langue par défaut
	//
	// macro non-visible de l'utilisateur
	//--------------------------------------------------------------------------
	
	// Directory traitée
	directory = pathconvert(directory,%f,%f);
	
	//--------------------------------------------------------------------------
	// Nettoyage du répertoire
	// Si il existe un fichier .list_<directory_language> (fichier contenant tous les
	// fichiers traduits dans la langue du répertoire), on supprime dans l'ordre :
	//   1. Tous les fichiers n'appartenant pas à la liste contenue dans .list_<directory_language>
	//   2. Tous les fichiers de la forme .list_<language>
	//--------------------------------------------------------------------------
	
	if listfiles(pathconvert(directory+"/.list_"+directory_language,%f,%f)) <> [] then
		del_df_lang_xml_files(directory,directory_language);
		mdelete(directory+"/.list_*");
	end
	
	//--------------------------------------------------------------------------
	// Construction du fichier list_<directory_language> contenant la liste des
	// fichiers traduits dans la langue associée au répertoire
	//--------------------------------------------------------------------------
	
	dir_language_xml_files = basename(listfiles(directory+"/*.xml"));
	if dir_language_xml_files <> [] then
		mputl(dir_language_xml_files,directory+"/.list_"+directory_language);
	else
		mputl('',directory+"/.list_"+directory_language);
	end
	
	//--------------------------------------------------------------------------
	// Construction du fichier list_<default_language> contenant la liste des fichiers
	// non traduits dans la langue associée au répertoire qui seront récupérés depuis le
	// répertoire de la langue par défaut
	//--------------------------------------------------------------------------
	
	// Tous les fichiers contenus dans <directory>/../<default_language> pour commencer
	// On afinnera par la suite
	df_lang_xml_files = basename(listfiles(pathconvert(directory+"/../"+default_language+"/*.xml",%f,%f)));
	
	// On supprime de "df_lang_xml_files" tous les élément contenus dans
	// "dir_language_xml_files", c'est à dire tous les fichiers déja traduit dans la langue
	// associée au répertoire.
	
	for i=1:size(dir_language_xml_files,'*');
		df_lang_xml_files(find(df_lang_xml_files==dir_language_xml_files(i)))=[];
	end
	
	// Création du fichier
	if df_lang_xml_files <> [] then
		mputl(df_lang_xml_files,pathconvert(directory+"/.list_"+default_language,%f,%f));
	else
		mputl('',pathconvert(directory+"/.list_"+default_language,%f,%f));
	end
		
	//--------------------------------------------------------------------------
	// Copie des fichiers additionnels
	//--------------------------------------------------------------------------
	
	for i=1:size(df_lang_xml_files,'*');
		tmp_file = mgetl(pathconvert(directory+"/../"+default_language+"/"+df_lang_xml_files(i)+".xml",%f,%f));
		mputl(tmp_file,pathconvert(directory+"/"+df_lang_xml_files(i)+".xml",%f,%f));
	end
	
endfunction


function del_df_lang_xml_files(directory,directory_language)
	
	//--------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 26, july 2006
	//
	// Cette macro détruit tous les fichiers xml qui ne sont pas traduit dans
	// la langue associée au répertoire
	//
	// macro non-visible de l'utilisateur
	//--------------------------------------------------------------------------
	
	// Directory traitée
	directory = pathconvert(directory,%f,%t);
	
	if listfiles(pathconvert(directory+"/.list_"+directory_language,%f,%f)) <> [] then
		
		// Récupération de la liste des fichiers xml copiés depuis le répertoire de la langue par défaut
		dir_language_xml_files = mgetl(pathconvert(directory+"/.list_"+directory_language,%f,%f));
		
		// Liste de tous les fichiers xml contenu dans le répertoire
		all_files = basename(listfiles(directory+"/*.xml"));
		
		// On retire de "all_files" tous les éléments appartenant à "xml_directory_language_files"
		// Ce sont les aides en ligne traduite dans la langue associée répertoire
		for i=1:size(dir_language_xml_files,'*');
			all_files(find(all_files==dir_language_xml_files(i)))=[];
		end
		
		// Destruction des fichiers restant dans all_files
		for i=1:size(all_files,'*');
			mdelete(pathconvert(directory+"/"+all_files(i)+".xml",%f,%f));
		end
	end
	
endfunction


function result = need_to_be_build(directory,directory_language,default_language)
	
	//--------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 27, july 2006
	//
	// Cette fonction a pour but de déterminer si le répertoire a besoin d'être
	// reconstruit ou pas.
	//
	// On détermine la date de dernière modification la plus récente parmi les
	// dates de dernière modification suivantes :
	//     -  date de dernière modification du répertoire "directory".
	//     -  dates de dernière modification des fichiers XML du répertoire "directory".
	//     -  date de dernière modification du répertoire "directory/../<default_language>"
	//        si le système de multilinguisme est utilisé
	//      - dates de dernière modification des fichiers XML du répertoire
	//        "directory/../<default_language>" si le système de multilinguisme est utilisé
	//
	// Ensuite cette valeur est comparée à la valeur contenue dans le fichier
	// "directory/.last_successful_build". Si elle est plus grande,
	// need_to_be_build renvoie %T
	//
	// Si le fichier "directory/.last_successful_build" n'existe pas, l'aide n'a jamais été
	// construite donc need_to_be_build renvoie %T
	//
	//--------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0);
	
	// S'il n'y a pas de fichiers XML dans le répertoire ni dans son homologue,
	// Le répertoire n'a pas besoin d'être construit.
	// Cela est une sécurité pour éviter de detruire les whatis des versions binaires
	// où il n'y a pas de fichiers XML
	
	xml_file_list    = listfiles(directory+"/*.xml");
	
	if rhs == 3 then
		xml_df_file_list = listfiles(pathconvert(directory+"/../"+default_language,%f,%f)+"/*.xml");
	else
		xml_df_file_list = [];
	end
	
	if (xml_file_list == []) & (xml_df_file_list == []) then
		result = %F;
		return;
	end
	
	if fileinfo(pathconvert(directory+"/.last_successful_build_"+output_format,%f,%f)) == [] then
		result = %T;
		return;
	end
	
	exec(pathconvert(directory+"/.last_successful_build_"+output_format,%f,%f),-1);
	
	// ---------------------------------------------------------------------
	
	directory_info = fileinfo(directory);
	max_change_date = directory_info(6);
	
	for i=1:size(xml_file_list,'*');
		file_date = fileinfo(xml_file_list(i));
		if file_date(6) > max_change_date then
			max_change_date = file_date(6);
		end
	end
	
	if max_change_date > last_success_build_val then
		result = %T;
		return;
	else
		if rhs == 3 then
			
			df_lang_dir_info = fileinfo(pathconvert(directory+"/../"+default_language,%f,%f));
			
			if df_lang_dir_info(6) > max_change_date then
				max_change_date = df_lang_dir_info(6);
			end
			
			xml_df_lang_file_list = listfiles(pathconvert(directory+"/../"+default_language+"/*.xml",%f,%f));
			
			for i=1:size(xml_df_lang_file_list,'*');
				file_date = fileinfo(xml_df_lang_file_list(i));
				if file_date(6) > max_change_date then
					max_change_date = file_date(6);
				end
			end
		end
		
		if max_change_date > last_success_build_val then
			result = %T;
			return;
		else
			result = %F;
			return;
		end
	end
	
endfunction


function create_MD(dirs,titles,output_filename,language)
	
	if language == "fr_FR" then
		encoding = "ISO-8859-1";
	else
		encoding = "UTF-8";
	end
	
	// Sort dirs and titles
	my_mat = [titles,dirs];
	my_mat(grep(my_mat(:,2),"/(.)*core(.)*/","r"),1) = "AAA_Scilab";
	my_mat = gsort(my_mat,"lr","i");
	my_mat(grep(my_mat(:,2),"/(.)*core(.)*/","r"),1) = "Scilab";
	titles = my_mat(:,1);
	dirs   = my_mat(:,2);
	
	master_document = ["<?xml version=""1.0"" encoding="""+encoding+"""?>"; ..
			"<!DOCTYPE book [";
			"<!--Begin Entities-->"];
	xml_files          = listfiles(dirs+"/*.xml");
	
	if MSDOS then
	  xml_files = "file:///"+xml_files;
	end
	
	master_document    = [master_document; ..
		"<!ENTITY "+basename(xml_files)+" SYSTEM """+xml_files+""">"];
	
	master_document    = [ master_document; ..
		"<!--End Entities-->"; ..
		"]>"; ..
		"<book version=""5.0-subset Scilab"" xml:lang="""+getlanguage()+""""; ..
		"      xmlns=""http://docbook.org/ns/docbook"""; ..
		"      xmlns:xlink=""http://www.w3.org/1999/xlink"""; ..
		"      xmlns:xi=""http://www.w3.org/2001/XInclude"""; ..
		"      xmlns:svg=""http://www.w3.org/2000/svg"""; ..
		"      xmlns:mml=""http://www.w3.org/1998/Math/MathML"""; ..
		"      xmlns:html=""http://www.w3.org/1999/xhtml"""; ..
		"      xmlns:db=""http://docbook.org/ns/docbook"">"; ..
		"  <info>"; ..
		"    <title>"+gettext("Scilab manual")+"</title>"; ..
		"  </info>"; ..
		""];
		
// 	master_document    = [ master_document; ..
// 		"  <part xml:id=""UsersGuide"">"; ..
// 		"    <title>User''s guide</title>"; ..
// 		""; ..
// 		"    <chapter xml:id=""GettingStarted"">"; ..
// 		"      <title>Getting started</title>"; ..
// 		"      <para>TODO</para>"; ..
// 		"    </chapter>"; ..
// 		"  </part>"];
		
	master_document    = [ master_document; ..
		"<!--Begin Reference-->"];
	
	for k=1:size(dirs,"*");
		xml_files = gsort(basename(listfiles(dirs(k)+"/*.xml")),"lr","i");
		master_document    = [ master_document; ..
			"<reference xml:id=''"+title2category(titles(k))+"''>"; ..
			"<title>"+text2html(titles(k))+"</title>"; ..
			"&"+xml_files+";"; ..
			"</reference>"]
			
	end
	
	master_document    = [ master_document; ..
	"  <!--End Reference-->"; ..
	"</book>" ];
	
	mputl(master_document,output_filename);

endfunction

function create_MD_dir(my_dir,my_title,output_filename,language)
	
	if language == "fr_FR" then
		encoding = "ISO-8859-1";
	else
		encoding = "UTF-8";
	end
	
	xml_files   = basename(listfiles(my_dir+"/*.xml"));
	
	master_document = ["<?xml version=""1.0"" encoding="""+encoding+"""?>"; ..
			"<!DOCTYPE book [";
			"<!--Begin Entities-->"];
		
	xml_files = gsort(listfiles(my_dir+"/*.xml"),"lr","i");
	
	xml_files(grep(xml_files,"master_help.xml")) = [];
	
	if MSDOS then
	  xml_files_tmp = "file:///"+xml_files;
	else
	  xml_files_tmp = xml_files;
	end
	
	master_document    = [master_document; ..
		"<!ENTITY "+basename(xml_files)+" SYSTEM """+xml_files_tmp+""">"];
	
	master_document    = [ master_document; ..
		"<!--End Entities-->"; ..
		"]>"; ..
		"<book version=""5.0-subset Scilab"" xml:lang="""+language+""""; ..
		"      xmlns=""http://docbook.org/ns/docbook"""; ..
		"      xmlns:xlink=""http://www.w3.org/1999/xlink"""; ..
		"      xmlns:xi=""http://www.w3.org/2001/XInclude"""; ..
		"      xmlns:svg=""http://www.w3.org/2000/svg"""; ..
		"      xmlns:mml=""http://www.w3.org/1998/Math/MathML"""; ..
		"      xmlns:html=""http://www.w3.org/1999/xhtml"""; ..
		"      xmlns:db=""http://docbook.org/ns/docbook"">"; ..
		"  <info>"; ..
		"    <title>"+my_title+"</title>"; ..
		"  </info>"; ..
		"<!--Begin Reference-->"];
		
		master_document    = [ master_document; ..
			"<reference xml:id=''"+title2category(my_title)+"''>"; ..
			"<title>"+text2html(my_title)+"</title>"; ..
			"&"+basename(xml_files)+";"; ..
			"</reference>"]
			
	
	master_document    = [ master_document; ..
	"  <!--End Reference-->"; ..
	"</book>" ];
	
	mputl(master_document,output_filename);

endfunction

function category = title2category(mytitle)
		
		category = mytitle;
		category = strsubst(category , "&"  , "_" );
		
		category = strsubst(category , "À"  , "A" );
		category = strsubst(category , "Á"  , "A" );
		category = strsubst(category , "Â"  , "A" );
		category = strsubst(category , "Ã"  , "A" );
		category = strsubst(category , "Ä"  , "A" );
		category = strsubst(category , "Å"  , "A" );
		category = strsubst(category , "Æ"  , "AE");
		category = strsubst(category , "Ç"  , "C" );
		category = strsubst(category , "È"  , "E" );
		category = strsubst(category , "É"  , "E" );
		category = strsubst(category , "Ê"  , "E" );
		category = strsubst(category , "Ë"  , "E" );
		category = strsubst(category , "Ì"  , "I" );
		category = strsubst(category , "Í"  , "I" );
		category = strsubst(category , "Î"  , "I" );
		category = strsubst(category , "Ï"  , "I" );
		category = strsubst(category , "Ð"  , "D" );
		category = strsubst(category , "Ñ"  , "N" );
		category = strsubst(category , "Ò"  , "O" );
		category = strsubst(category , "Ó"  , "O" );
		category = strsubst(category , "Ô"  , "O" );
		category = strsubst(category , "Õ"  , "O" );
		category = strsubst(category , "Ö"  , "O" );
		category = strsubst(category , "Ù"  , "U" );
		category = strsubst(category , "Ú"  , "U" );
		category = strsubst(category , "Û"  , "U" );
		category = strsubst(category , "Ü"  , "U" );
		category = strsubst(category , "Ý"  , "Y" );
		category = strsubst(category , "à"  , "a" );
		category = strsubst(category , "á"  , "a" );
		category = strsubst(category , "â"  , "a" );
		category = strsubst(category , "ã"  , "a" );
		category = strsubst(category , "ä"  , "a" );
		category = strsubst(category , "å"  , "a" );
		category = strsubst(category , "æ"  , "ae");
		category = strsubst(category , "ç"  , "c" );
		category = strsubst(category , "è"  , "e" );
		category = strsubst(category , "é"  , "e" );
		category = strsubst(category , "ê"  , "e" );
		category = strsubst(category , "ë"  , "e" );
		category = strsubst(category , "ì"  , "i" );
		category = strsubst(category , "í"  , "i" );
		category = strsubst(category , "î"  , "i" );
		category = strsubst(category , "ï"  , "i" );
		category = strsubst(category , "ñ"  , "n" );
		category = strsubst(category , "ò"  , "o" );
		category = strsubst(category , "ó"  , "o" );
		category = strsubst(category , "ô"  , "o" );
		category = strsubst(category , "õ"  , "o" );
		category = strsubst(category , "ö"  , "o" );
		category = strsubst(category , "ù"  , "u" );
		category = strsubst(category , "ú"  , "u" );
		category = strsubst(category , "û"  , "u" );
		category = strsubst(category , "ü"  , "u" );
		category = strsubst(category , "ý"  , "y" );
		category = strsubst(category , "ÿ"  , "y" );
		
		category = strsubst(category , ":"  , ""  );
		category = strsubst(category , "\"  , "_" );
		category = strsubst(category , "/"  , "_" );
		category = strsubst(category , "''" , "_" );
		category = strsubst(category , "  " , " " );
		category = strsubst(category , " "  , "_" );
		category = strsubst(category , "["  , ""  );
		category = strsubst(category , "]"  , ""  );
		
		category = convstr(category,"l");
		category = "category_"+category;
		
endfunction


function out = text2html(in)
		
		out = in;
		out = strsubst(out , "&"  , "&amp;"    );
		
// 		
// 		out = strsubst(out , """" , "&quot;"   );
// 		out = strsubst(out , "&"  , "&amp;"    );
// 		out = strsubst(out , "?"  , "&euro;"   );
// 		out = strsubst(out , "À"  , "&Agrave;" );
// 		out = strsubst(out , "Á"  , "&Aacute;" );
// 		out = strsubst(out , "Â"  , "&Acirc;"  );
// 		out = strsubst(out , "Ã"  , "&Atilde;" );
// 		out = strsubst(out , "Ä"  , "&Auml;"   );
// 		out = strsubst(out , "Å"  , "&Aring;"  );
// 		out = strsubst(out , "Æ"  , "&Aelig"   );
// 		out = strsubst(out , "Ç"  , "&Ccedil;" );
// 		out = strsubst(out , "È"  , "&Egrave;" );
// 		out = strsubst(out , "É"  , "&Eacute;" );
// 		out = strsubst(out , "Ê"  , "&Ecirc;"  );
// 		out = strsubst(out , "Ë"  , "&Euml;"   );
// 		out = strsubst(out , "Ì"  , "&Igrave"  );;
// 		out = strsubst(out , "Í"  , "&Iacute;" );
// 		out = strsubst(out , "Î"  , "&Icirc;"  );
// 		out = strsubst(out , "Ï"  , "&Iuml;"   );
// 		out = strsubst(out , "Ð"  , "&eth;"    );
// 		out = strsubst(out , "Ñ"  , "&Ntilde;" );
// 		out = strsubst(out , "Ò"  , "&Ograve;" );
// 		out = strsubst(out , "Ó"  , "&Oacute;" );
// 		out = strsubst(out , "Ô"  , "&Ocirc;"  );
// 		out = strsubst(out , "Õ"  , "&Otilde;" );
// 		out = strsubst(out , "Ö"  , "&Ouml;"   );
// 		out = strsubst(out , "Ù"  , "&Ugrave;" );
// 		out = strsubst(out , "Ú"  , "&Uacute;" );
// 		out = strsubst(out , "Û"  , "&Ucirc;"  );
// 		out = strsubst(out , "Ü"  , "&Uuml;"   );
// 		out = strsubst(out , "Ý"  , "&Yacute;" );
// 		out = strsubst(out , "Þ"  , "&thorn;"  );
// 		out = strsubst(out , "ß"  , "&szlig;"  );
// 		out = strsubst(out , "à"  , "&agrave;" );
// 		out = strsubst(out , "á"  , "&aacute;" );
// 		out = strsubst(out , "â"  , "&acirc;"  );
// 		out = strsubst(out , "ã"  , "&atilde;" );
// 		out = strsubst(out , "ä"  , "&auml;"   );
// 		out = strsubst(out , "å"  , "&aring;"  );
// 		out = strsubst(out , "æ"  , "&aelig;"  );
// 		out = strsubst(out , "ç"  , "&ccedil;" );
// 		out = strsubst(out , "è"  , "&egrave"  );;
// 		out = strsubst(out , "é"  , "&eacute;" );
// 		out = strsubst(out , "ê"  , "&ecirc;"  );
// 		out = strsubst(out , "ë"  , "&euml;"   );
// 		out = strsubst(out , "ì"  , "&igrave"  );;
// 		out = strsubst(out , "í"  , "&iacute;" );
// 		out = strsubst(out , "î"  , "&icirc;"  );
// 		out = strsubst(out , "ï"  , "&iuml;"   );
// 		out = strsubst(out , "ð"  , "&eth;"    );
// 		out = strsubst(out , "ñ"  , "&ntilde;" );
// 		out = strsubst(out , "ò"  , "&ograve;" );
// 		out = strsubst(out , "ó"  , "&oacute;" );
// 		out = strsubst(out , "ô"  , "&ocirc;"  );
// 		out = strsubst(out , "õ"  , "&otilde;" );
// 		out = strsubst(out , "ö"  , "&ouml;"   );
// 		out = strsubst(out , "ù"  , "&ugrave;" );
// 		out = strsubst(out , "ú"  , "&uacute;" );
// 		out = strsubst(out , "û"  , "&ucirc;"  );
// 		out = strsubst(out , "ü"  , "&uuml;"   );
// 		out = strsubst(out , "ý"  , "&yacute;" );
// 		out = strsubst(out , "þ"  , "&thorn;"  );
// 		out = strsubst(out , "ÿ"  , "&yuml;"   );
// 		out = strsubst(out , "µ"  , "&micro;"  );
	
endfunction


function dirs_out = get_xml_path(dirs_in)
	
	dirs_out = [];
	
	for k=1:size(dirs_in,"*")
		
		if basename(dirs_in(k)) == "jar" then
			
			help_basepath = part(dirs_in(k),1:length(dirs_in(k))-4) + filesep() + "help";
			
			if isdir(help_basepath + filesep() +getlanguage()) then
				dirs_out(k) = help_basepath + filesep() + getlanguage();
			elseif isdir(help_basepath+filesep()+getdefaultlanguage()) then
				dirs_out(k) = help_basepath+filesep()+getlanguage();
			else
				dirs_out(k) = "";
			end
			
		else
			dirs_out(k) = dirs_in(k);
		end
		
	end
	
endfunction


function language_out = guess_lang(dir_in)
	
	language_out = getlanguage();
	
	[my_start,my_end,my_match] = regexp(basename(dir_in),"/([a-z][a-z]_[A-Z][A-Z])$/");
	
	if my_start <> [] then
		language_out = part(my_match,1:5);
	end
	
endfunction
