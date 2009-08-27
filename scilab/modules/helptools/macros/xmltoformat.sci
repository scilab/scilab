// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2008-2009 DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2009 DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
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
	
	generated_files = [];
	
	all_scilab_help     = %F;
	my_wanted_language  = getlanguage(); // This variable is only need when
	                                     // build all scilab help
	
	[lhs,rhs] = argn(0);
	
	// Too much parameters
	// ---------------------------------------------------------------------
	
	if rhs > 5 | rhs < 1 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"xmltoformat",1,5));
	end
	
	// Transformation du %helps_modules jar => help/language
	
	// Default case : construction de l'aide en ligne de Scilab
	// ---------------------------------------------------------------------
	
	if (  (rhs <= 1) ..
		| ((rhs == 2) & (dirs == [])) ..
		| ((rhs == 3) & (dirs == []) & (titles == [])) ..
		| ((rhs == 4) & (dirs == []) & (titles == [])) ) then
		
		all_scilab_help        = %T;
		
		// "directory_language" input argument is defined !
		if rhs == 4 then
			my_wanted_language = directory_language;
			x2f_reset_help_mod_var(my_wanted_language);
			%HELPS=[%helps_modules;%helps];
		end
		
		dirs_to_build          = %HELPS;
		dirs_to_build_m        = %helps_modules;
		dirs_to_build_c        = %helps;
		
		dirs                   = x2f_get_xml_path(dirs_to_build(:,1),my_wanted_language);
		dirs_m                 = x2f_get_xml_path(dirs_to_build_m(:,1),my_wanted_language);
		dirs_c                 = x2f_get_xml_path(dirs_to_build_c(:,1),my_wanted_language);
		
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
		
		if my_wanted_language == getdefaultlanguage() then
			for k=1:size(dirs,'*')
				directory_language = [directory_language;my_wanted_language];
				language_system    = [language_system;%F];
			end
			for k=1:size(dirs_m,'*')
				directory_language_m = [directory_language_m;my_wanted_language];
				language_system_m    = [language_system_m;%F];
			end
			for k=1:size(dirs_c,'*')
				directory_language_c = [directory_language_c;my_wanted_language];
				language_system_c    = [language_system_c;%F];
			end
		else
			for k=1:size(dirs,'*')
				directory_language = [directory_language;my_wanted_language];
				default_language   = [default_language;getdefaultlanguage()];
				language_system    = [language_system;%T]; // Enable the language system
			end
			for k=1:size(dirs_m,'*')
				directory_language_m = [directory_language_m;my_wanted_language];
				default_language_m   = [default_language_m;getdefaultlanguage()];
				language_system_m    = [language_system_m;%T]; // Enable the language system
			end
			for k=1:size(dirs_c,'*')
				directory_language_c = [directory_language_c;my_wanted_language];
				default_language_c   = [default_language_c;getdefaultlanguage()];
				language_system_c    = [language_system_c;%T]; // Enable the language system
			end
		end
	
	// Only directories are precised
	// ---------------------------------------------------------------------
	
	elseif (rhs == 2) & (dirs <> []) then
		
		language_system    = [];
		titles             = [];
		directory_language = [];
		default_language   = [];
		
		for k=1:size(dirs,'*')
			directory_language = [directory_language ; x2f_guess_lang(dirs(k)) ]; // Language detection
			default_language   = [default_language   ; getdefaultlanguage()]; // Default Language
			titles             = [titles;gettext("Help chapter")+" "+string(k)];
			
			if directory_language(k) == default_language(k) then
				language_system    = [language_system;%F]; // Enable the language system
			else
				language_system    = [language_system;%T]; // Enable the language system
			end
		end
		
	// Only directories and title are precised
	// ---------------------------------------------------------------------
	
	elseif rhs == 3 then
		
		language_system    = [];
		directory_language = [];
		default_language   = [];
		
		for k=1:size(dirs,'*')
			directory_language = [directory_language ; x2f_guess_lang(dirs(k)) ]; // Language detection
			default_language   = [default_language   ; getdefaultlanguage()]; // Default Language
			
			if directory_language(k) == default_language(k) then
				language_system    = [language_system;%F]; // Enable the language system
			else
				language_system    = [language_system;%T]; // Enable the language system
			end
		end
	
	// Directories, title and languages are specified
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
	
	// All is specified
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
	
	// Convert paths into absolute paths
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
	
	// =========================================================================
	// Build the trees
	// =========================================================================
	
	if all_scilab_help then
		
		// ast_tree : "Modules" Tree
		
		if directory_language_m(1) == "fr_FR" then
			scilab_manual = "Manuel Scilab"
		elseif directory_language_m(1) == "pt_BR" then
			scilab_manual = "Manual Scilab"
		else
			scilab_manual = "Scilab manual"
		end
		
		modules_tree = struct();
		modules_tree("level")      = 0; // It's a book
		modules_tree("title")      = scilab_manual;
		modules_tree("book_title") = scilab_manual;
		modules_tree("language")   = directory_language_m(1);
		modules_tree("path")       = SCI;
		modules_tree("master.xml") = %F;
		modules_tree("xml_number") = 0;
		modules_tree("xml_list")   = [];
		
		//
		// Scilab Modules
		//
		
		for k=1:size(dirs_m,"*")
			
			this_tree                     = x2f_dir_to_tree(dirs_m(k),1);
			this_tree("title_addchapter") = titles_m(k);
			this_tree("language")         = directory_language_m(k);
			
			if  language_system_m(k) then
				default_language_path = pathconvert(part(dirs_m(k),1:length(dirs_m(k))-5)+default_language_m(k));
				if isdir(default_language_path) then
					this_tree_dl = x2f_dir_to_tree(default_language_path,1);
					this_tree    = x2f_merge_trees(this_tree_dl,this_tree);
				end
			end
			
			modules_tree("dir_"+getmd5(dirs_m(k),"string")) = this_tree;
			
		end
		
		//
		// Scilab Toolboxes
		//
		
		contrib_tree = struct();
		
		for k=1:size(dirs_c,"*")
			
			this_tree                     = x2f_dir_to_tree(dirs_c(k),0);
			this_tree("title_addchapter") = titles_c(k);
			this_tree("language")         = directory_language_c(k);
			
			if  language_system_c(k) then
				default_language_path = pathconvert(part(dirs_c(k),1:length(dirs_c(k))-5)+default_language_c(k));
				if isdir(default_language_path) then
					this_tree_dl = x2f_dir_to_tree(default_language_path,0);
					this_tree    = x2f_merge_trees(this_tree_dl,this_tree);
				end
			end
			
			contrib_tree(dirs_c(k)) = this_tree;
		end
		
	else
		
		//
		// Scilab Toolboxes
		//
		
		contrib_tree = struct();
		
		for k=1:size(dirs,"*")
			
			this_tree                     = x2f_dir_to_tree(dirs(k),0);
			this_tree("title_addchapter") = titles(k);
			this_tree("language")         = directory_language(k);
			
			if  language_system(k) then
				default_language_path = pathconvert(part(dirs(k),1:length(dirs(k))-5)+default_language(k));
				if isdir(default_language_path) then
					this_tree_dl = x2f_dir_to_tree(default_language_path,0);
					this_tree    = x2f_merge_trees(this_tree_dl,this_tree);
				end
			end
			
			contrib_tree(dirs(k)) = this_tree;
		end
		
	end
	
	// =========================================================================
	// Build the master documents
	// =========================================================================
	
	if all_scilab_help then
		
		//
		// Scilab Modules
		//
		
		mprintf(_("\nBuilding the Scilab manual master document for %s.\n"),my_wanted_language);
		
		master_doc = pathconvert(SCI+"/modules/helptools/master_"+my_wanted_language+"_help.xml",%F);
		modules_tree("master_document") = master_doc;
		master_str = x2f_tree_to_master(modules_tree);
		mputl(master_str,master_doc);
		
		//
		// Scilab Toolboxes
		//
		
		nb_dir = size(dirs_c,"*");
		displaydone = 0;
		
		for k=1:nb_dir
			
			if nb_dir > 1 then
				if displaydone == 0 then
					mprintf(_("\nBuilding master documents:\n"));
					displaydone = 1;
				end
			else
				mprintf(_("\nBuilding the master document:\n"));
			end
			
			mprintf(_("\t%s\n"),strsubst(dirs_c(k),SCI_long,"SCI"));
			
			this_tree  = contrib_tree(dirs_c(k));
			master_doc = pathconvert(dirs_c(k)+"/master_help.xml",%F);
			this_tree("master_document") = master_doc;
			master_str = x2f_tree_to_master(this_tree);
			mputl(master_str,master_doc);
			contrib_tree(dirs_c(k)) = this_tree;
			
		end
		
	else
		
		nb_dir = size(dirs,"*");
		displaydone = 0;
		
		for k=1:nb_dir
			
			if nb_dir > 1 then
				if displaydone == 0 then
					mprintf(_("\nBuilding master documents:\n"));
					displaydone = 1;
				end
			else
				mprintf(_("\nBuilding the master document:\n"));
			end
			
			mprintf(_("\t%s\n"),strsubst(dirs(k),SCI_long,"SCI"));
			
			this_tree  = contrib_tree(dirs(k));
			
			if isfield(this_tree,"master.xml") & this_tree("master.xml") then
				this_tree("master_document") = pathconvert(dirs(k)+"/master.xml",%F);
				contrib_tree(dirs(k)) = this_tree;
				continue;
			end
			
			master_doc = pathconvert(dirs(k)+"/master_help.xml",%F);
			this_tree("master_document") = master_doc;
			master_str = x2f_tree_to_master(this_tree);
			mputl(master_str,master_doc);
			contrib_tree(dirs(k)) = this_tree;
			
		end
	
	end
	
	// =========================================================================
	// Perform the help generation
	// scilab help files generation
	// 2 steps : 
	// -- scilab help
	// -- scilab internal toolbox (example: scicos)
	// =========================================================================
	
	if output_format=="javaHelp" then
		output_format_ext = "jar";
	else
		output_format_ext = output_format;
	end
	
	is_html = (output_format == "html");
	is_chm = (output_format == "chm");
	
	if all_scilab_help then
		
		mprintf(_("Building the scilab manual file ["+output_format+"] (Please wait building ... this can take a while)\n"));
		
		// Define and create the final output directory if does not exist
		final_output_dir = pathconvert(SCI+"/modules/helptools/"+output_format_ext,%f,%f);
		
		if ~isdir(final_output_dir) then
			mkdir(final_output_dir);
		end
		
		if is_chm then
		
		elseif is_html then
			final_output_dir = pathconvert(final_output_dir+"/"+my_wanted_language,%f,%f);
			if ~isdir(final_output_dir) then
				mkdir(final_output_dir);
			end
		end
		
		// Define and create the path of buildDoc working directory
		buildDoc_dir  = pathconvert(SCI+"/modules/helptools/"+ output_format + "/scilab_" + my_wanted_language + "_help",%t,%f);
		
		if ~isdir(pathconvert(SCI+"/modules/helptools/"+output_format,%f,%f)) then
			mkdir(pathconvert(SCI+"/modules/helptools/"+output_format,%f,%f));
		end
		
		if ~isdir(buildDoc_dir) then
			mkdir(buildDoc_dir);
		end
		
		// Define the final location of the generated file
		if is_chm then
			final_help_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
		elseif is_html then
			final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
		else
			final_help_file = pathconvert(final_output_dir+"/scilab_" + my_wanted_language + "_help." + output_format_ext,%f,%f);
		end
		
		// Define the path of the generated file created by buildDoc
		if is_chm then
			buildDoc_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
		elseif is_html then
			buildDoc_file = pathconvert(buildDoc_dir + "index.html",%f,%f);
		else
			buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + my_wanted_language + "_help." + output_format_ext,%f,%f);
		end
		
		// Save the current directory
		cur_dir = pwd();
		
		// Change Scilab current directory so that Java Indexer works
		if ~chdir(buildDoc_dir) then
			error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
		end
		
		// process the build
		buildDoc(output_format,modules_tree("master_document"),my_wanted_language);
		
		// Check if the help file has been generated
		if fileinfo(buildDoc_file)==[] then
			error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
		end
		
		// move the generated file(s)
		if is_chm then
		
		elseif is_html then
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
			error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",cur_dir));
		end
		
		// Now we can add the help file to the list of all generated files
		generated_files = [ generated_files ; final_help_file ];
		
		// internal toolbox
		// example: scicos
		// Now, build toolboxes help (if any)
		
		for k=1:size(dirs_c,"*");
			
			this_tree  = contrib_tree(dirs_c(k));
			
			mprintf(_("\nBuilding the manual file [%s] in %s. (Please wait building ... this can take a while)\n"),output_format,strsubst(dirs_c(k),SCI_long,"SCI"));
			
			// Define and create the final output directory if does not exist
			final_output_dir = pathconvert(dirs_c(k)+"/../../"+output_format_ext,%f,%f);
			
			if ~isdir(final_output_dir) then
				mkdir(final_output_dir);
			end
			
			// To improve the final_output_dir path
			
			if ~chdir(final_output_dir) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",final_output_dir));
			end
			
			final_output_dir = pathconvert(pwd(),%f,%f);
			
			if ~chdir(cur_dir) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",cur_dir));
			end
			
			if is_chm then
				final_output_dir = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
			elseif is_html then
				final_output_dir = pathconvert(final_output_dir+"/"+directory_language_c(k),%f,%f);
				if ~isdir(final_output_dir) then
					mkdir(final_output_dir);
				end
			end
			
			// Define and create the path of buildDoc working directory
			buildDoc_dir  = pathconvert(dirs_c(k) + "/scilab_" + directory_language_c(k) + "_help",%t,%f);
			if ~isdir(buildDoc_dir) then
				mkdir(buildDoc_dir);
			end
			
			// Define the final location of the generated file
			if is_chm then
				final_help_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
			elseif is_html then
				final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
			else
				final_help_file = pathconvert(final_output_dir+"/scilab_" + directory_language_c(k) + "_help." + output_format_ext,%f,%f);
			end
			
			
			// Define the path of the generated file created by buildDoc
			if is_chm then
				buildDoc_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
			elseif is_html then
				buildDoc_file = pathconvert(buildDoc_dir + "index.html",%f,%f);
			else
				buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + directory_language_c(k) + "_help." + output_format_ext,%f,%f);
			end
			
			// Save the current directory
			cur_dir = pwd();
			
			// Change Scilab current directory so that Java Indexer works
			if ~chdir(buildDoc_dir) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
			end
			
			// process the build
			buildDoc(output_format,this_tree("master_document"),directory_language_c(k),dirs_c(k));
			
			// Check if the help file has been generated
			if fileinfo(buildDoc_file)==[] then
				error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
			end
			
			// move the generated file(s)
			if is_chm then
			
			elseif is_html then
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
		
	else
	// ------------------------------
	// external (Toolboxes)
	// ------------------------------

		
		// Dirs are precised in the input arguments
		
		nb_dir      = size(dirs,"*");
		displaydone = 0;
		
		for k=1:nb_dir
			
			// Save the current directory
			cur_dir = pwd();
			
			this_tree  = contrib_tree(dirs(k));
			
			if nb_dir > 1 then
				if displaydone == 0 then
					mprintf(_("\nBuilding the manual file [%s]. (Please wait building ... this can take a while)\n"),output_format);
					displaydone = 1;
				end
				mprintf(_("\t%s\n"),strsubst(dirs(k),SCI_long,"SCI"));
			else
				mprintf(_("\nBuilding the manual file [%s] in %s. (Please wait building ... this can take a while)\n"),output_format,strsubst(dirs(k),SCI_long,"SCI"));
			end
			
			// Define and create the final output directory if does not exist
			final_output_dir = pathconvert(dirs(k)+"/../../"+output_format_ext,%f,%f);
			
			if ~isdir(final_output_dir) then
				mkdir(final_output_dir);
			end
			
			// To improve the final_output_dir path
			
			if ~chdir(final_output_dir) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",final_output_dir));
			end
			
			final_output_dir = pathconvert(pwd(),%f,%f);
			
			if is_chm then
				// nothing to do
			else
				if is_html then
					final_output_dir = pathconvert(final_output_dir+"/"+directory_language(k),%f,%f);
					if ~isdir(final_output_dir) then
						mkdir(final_output_dir);
					end
				end
			end

			// Define and create the path of buildDoc working directory
			buildDoc_dir  = pathconvert(dirs(k) + "/scilab_" + directory_language(k) + "_help",%t,%f);
			if ~isdir(buildDoc_dir) then
				mkdir(buildDoc_dir);
			end
			
			// Define the final location of the generated file
			if is_chm then
				final_help_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
			else
				if is_html then
					final_help_file = pathconvert(final_output_dir+"/index.html",%f,%f);
				else
					final_help_file = pathconvert(final_output_dir+"/scilab_" + directory_language(k) + "_help." + output_format_ext,%f,%f);
				end
			end
			
			// Define the path of the generated file created by buildDoc
			if is_chm then
				buildDoc_file = pathconvert(buildDoc_dir + "htmlhelp.hhp",%f,%f);
			elseif is_html then
				buildDoc_file = pathconvert(buildDoc_dir + "index.html",%f,%f);
			else
				buildDoc_file = pathconvert(buildDoc_dir + "scilab_" + directory_language(k) + "_help." + output_format_ext,%f,%f);
			end
			
			// Change Scilab current directory so that Java Indexer works
			if ~chdir(buildDoc_dir) then
				error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",buildDoc_dir));
			end
			
			// process the build
			buildDoc(output_format,this_tree("master_document"),directory_language(k),dirs(k));
			
			// Check if the help file has been generated
			if fileinfo(buildDoc_file)==[] then
				error(msprintf(gettext("%s: %s has not been generated."),"xmltoformat",buildDoc_file));
			end
			
			// move the generated file(s)
			if is_chm then
			// nothing to do
			elseif is_html then
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
	
	chdir(current_directory);
	
	// On remet l'environement initial
	//--------------------------------------------------------------------------
	
	chdir(current_directory);
	%helps         = %helps_save;
	%helps_modules = %helps_modules_save;
	%HELPS         = saved_helps;
	
endfunction


// =============================================================================
// dirs_out = x2f_get_xml_path(dirs_in,my_wanted_language)
// =============================================================================

function dirs_out = x2f_get_xml_path(dirs_in,my_wanted_language)
	
	dirs_out = [];
	
	for k=1:size(dirs_in,"*")
		
		if basename(dirs_in(k)) == "jar" then
			
			help_basepath = part(dirs_in(k),1:length(dirs_in(k))-4) + filesep() + "help";
			
			if isdir(help_basepath + filesep() +my_wanted_language) then
				dirs_out(k) = help_basepath + filesep() + my_wanted_language;
			elseif isdir(help_basepath+filesep()+getdefaultlanguage()) then
				dirs_out(k) = help_basepath+filesep()+getdefaultlanguage();
			else
				dirs_out(k) = "";
			end
			
		else
			dirs_out(k) = dirs_in(k);
		end
		
	end
	
endfunction

// =============================================================================
// language_out = x2f_guess_lang(dir_in)
// =============================================================================

function language_out = x2f_guess_lang(dir_in)
	
	language_out = getlanguage();
	
	[my_start,my_end,my_match] = regexp(basename(dir_in),"/([a-z][a-z]_[A-Z][A-Z])$/");
	
	if my_start <> [] then
		language_out = part(my_match,1:5);
	end
	
endfunction

// =============================================================================
// x2f_reset_help_mod_var(language)
// =============================================================================

function x2f_reset_help_mod_var(language)
	
	global %helps_modules;
	
	// Reset the variable in hand
	%helps_modules = [];
	
	// Get module list
	module_list = getmodules();
	
	// Loop on modules
	for k=1:size(module_list,'*');
		addchapter_path = getlongpathname(SCI+"/modules/"+module_list(k)+"/help/"+language+"/addchapter.sce");
		if fileinfo(addchapter_path)<>[] then
			exec(addchapter_path,-1);
		end
	end
	
endfunction

// =============================================================================
// tree = x2f_dir_to_tree(directory,level)
//
// level :
//  0 : book 
//  1 : part
//  2 : chapter
//  3 : sect1
//  4 : sect2
//  5 : sect3
//  6 : sect4
// ...
//
// Date : 24/04/2009
// =============================================================================

function tree = x2f_dir_to_tree(directory,level)
	
	tree = struct();
	rhs  = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs<>2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_dir_to_tree",2));
	end
	
	// Check input argument type
	// =========================================================================
	
	if type(directory) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"x2f_dir_to_tree",1));
	end
	
	if type(level) <> 1 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A integer expected.\n"),"x2f_dir_to_tree",2));
	end
	
	// Check input argument dimension
	// =========================================================================
	
	if size(directory,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"x2f_dir_to_tree",1));
	end
	
	if size(level,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A integer expected.\n"),"x2f_dir_to_tree",1));
	end
	
	// Check the directory existence
	// =========================================================================
	
	if ~isdir(directory) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid existing directory is expected.\n"),"x2f_dir_to_tree",1));
	end
	
	// ... and now, Action
	// =========================================================================
	
	// Normalize the directory path
	directory    = pathconvert(directory);
	
	
	// init the structure
	tree          = struct();
	tree("path")  = directory;
	tree("level") = level;
	
	// If a master.xml file exists, don't go past
	if fileinfo(directory+"master.xml")<>[] then
		tree("master.xml") = %T;
		return;
	else
		tree("master.xml") = %F;
	end
	
	//
	// Title management
	// 
	
	// Parse the CHAPTER file to get the directory title if this file is present
	if fileinfo(directory+"CHAPTER")<>[] then
		tree = x2f_cat(tree,x2f_read_CHAPTER(directory+"CHAPTER"));
	end
	
	// Check if the addchapter.sce is present
	if fileinfo(directory+"addchapter.sce")<>[] then
		tree("title_addchapter") = basename(directory);
	end
	
	// Check if the last_successful_build is present
	if fileinfo(directory+".last_successful_build")<>[] then
		tree = x2f_cat(tree , x2f_read_lsb(directory+".last_successful_build"));
	end
	
	// Get the default title
	
	if MSDOS then
		tmpdirectory = strsubst(directory,"/\\$/","","r");
	else
		tmpdirectory = strsubst(directory,"/\/$/","","r");
	end
	
	tree("title_default") = basename(tmpdirectory);
	
	//
	// XML list management
	//
	
	// get the xml list
	xmlfiles = x2f_get_xml_files(directory);
	
	// Allocate the xml list
	tree("xml_list")    = xmlfiles;
	tree("xml_number")  = size(xmlfiles(:,1),"*");
	
	//
	// Sub Trees management
	// 
	
	// get the directory list
	directories = x2f_get_directories(directory);
	
	for i=1:size(directories,"*")
		this_dir_tree = x2f_dir_to_tree(directory+directories(i),tree("level") + 1);
		if this_dir_tree("xml_number") > 0 then
			tree("dir_"+getmd5(directories(i),"string")) = this_dir_tree;
			tree("xml_number") = tree("xml_number") + this_dir_tree("xml_number");
		end
	end
	
endfunction

// =============================================================================
// xmlfiles = x2f_get_xml_files(directory)
// return an n X 3 matrix
//
// xmlfiles(:,1) : basename of each XML file
// xmlfiles(:,2) : full path of each XML file
// xmlfiles(:,3) : Last modification time of each XML file
//
// Date : 24/04/2009
// =============================================================================

function xmlfiles = x2f_get_xml_files(directory)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"x2f_get_xml_files",1));
	end
	
	// Check input argument type
	// =========================================================================
	
	if type(directory) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"x2f_get_xml_files",1));
	end
	
	// Check input argument dimension
	// =========================================================================
	
	if size(directory,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"x2f_get_xml_files",1));
	end
	
	// Check the directory existence
	// =========================================================================
	
	if ~isdir(directory) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid existing directory is expected.\n"),"x2f_get_xml_files",1));
	end
	
	// ... and now, Action
	// =========================================================================
	
	directory = pathconvert(directory);
	
	xmlfiles = gsort(basename(listfiles(directory+"*.xml")),"lr","i");
	xmlfiles(grep(xmlfiles,"master_help")) = [];
	xmlfiles(grep(xmlfiles,"master"))      = [];
	
	// Loop on all xml files to get their full path
	// =========================================================================
	
	if xmlfiles <> [] then
		xmlpaths = directory + xmlfiles + ".xml";
	else
		xmlpaths = [];
	end
	
	// Get XML modification time
	// =========================================================================
	
	if xmlpaths<>[] then
		infos = fileinfo(xmlpaths);
		format(20);
		lmt   = string(infos(:,7));
	else
		lmt   = [];
	end
	
	// Build the final matrix
	// =========================================================================
	xmlfiles = [ xmlfiles xmlpaths lmt ];
	
endfunction

// =============================================================================
// directories = x2f_get_directories(directory)
//
// Returns a colum vector that contain the list of subdirectories of <directory>
// 
// Date : 24/04/2009
// =============================================================================

function directories = x2f_get_directories(directory)
	
	rhs  = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"x2f_get_directories",1));
	end
	
	// Check input argument type
	// =========================================================================
	
	if type(directory) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"x2f_get_directories",1));
	end
	
	// Check input argument dimension
	// =========================================================================
	
	if size(directory,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"x2f_get_directories",1));
	end
	
	// Check the directory existence
	// =========================================================================
	
	if ~isdir(directory) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid existing directory is expected.\n"),"x2f_get_directories",1));
	end
	
	// ... and now, Action
	// =========================================================================
	
	// Normalize the directory path
	directory = pathconvert(directory);
	
	items       = listfiles(directory);
	directories = [];
	
	for i=1:size(items,"*") 
		if isdir(directory+items(i)) then
			directories = [ directories ; items(i) ];
		end
	end
	
	// Sort directories
	directories = gsort(basename(directories),"lr","i");
	
	// Remove directories that start with a dot from the list
	directories(grep(directories,"/^\./","r")) = [];
	
	// Remove blank strings from the list
	directories(find(directories == '')) = [];
	
	// Remove all scilab_xx_XX_help directories from the list
	directories( grep(directories,"/^scilab_[a-z][a-z]_[A-Z][A-Z]_help$/","r") ) = []
	
endfunction

// =============================================================================
// desc_out = x2f_read_CHAPTER(file_in)
// Parse a CHAPTER file
// 
// Date : 24/04/2009
// =============================================================================

function desc_out = x2f_read_CHAPTER(file_in)
	
	desc_out = struct();
	
	rhs  = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"x2f_read_CHAPTER",1));
	end
	
	// Check input arguments value
	// =========================================================================
	
	if regexp( file_in,"/CHAPTER$/") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that end with CHAPTER expected.\n"),"x2f_read_CHAPTER",1));
	end
	
	// Check the input file existence
	// =========================================================================
	
	if fileinfo( file_in ) == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid existing file is expected.\n"),"x2f_read_CHAPTER",1));
	end
	
	// ... and now, Action
	// =========================================================================
	
	FILETOPARSE = mgetl(file_in);
	
	current_field = "";
	
	for i=1:size(FILETOPARSE,"*")
		
		// Second case : Current field continuation
		if (regexp(FILETOPARSE(i),"/^\s/","o") == 1) & (current_field <> "") then
			current_value = part(CHAPTER(i),2:length(CHAPTER(i)));
			desc_out(current_field) = [ desc_out(current_field) ; current_value ];
			continue;
		end
		
		// First case : new field
		if regexp(FILETOPARSE(i),"/^[a-zA-Z][a-zA-Z0-9_]*\s=\s/","o") == 1 then
			current_field_length    = regexp(FILETOPARSE(i),"/\s=\s/","o")
			current_field           = part(FILETOPARSE(i),1:current_field_length-1);
			current_value           = part(FILETOPARSE(i),current_field_length+3:length(FILETOPARSE(i)));
			desc_out(current_field) = current_value;
			continue;
		end
		
		// Third case : Blank line
		if length(FILETOPARSE(i)) == 0 then
			continue;
		end
		
		// Else Error
		error(msprintf(gettext("%s: The CHAPTER file is not well formated at line %d\n"),"x2f_read_CHAPTER",i));
		
	end
	
endfunction

// =============================================================================
// desc_out = x2f_read_lsb(file_in)
// Parse a last_successful_build file
// 
// Date : 24/04/2009
// =============================================================================

function desc_out = x2f_read_lsb(file_in)
	
	desc_out = struct();
	rhs      = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"x2f_read_lsb",1));
	end
	
	// Check input arguments value
	// =========================================================================
	
	if regexp( file_in,"/\.last_successful_build$/") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that end with .last_successful_build expected.\n"),"x2f_read_lsb",1));
	end
	
	// Check the input file existence
	// =========================================================================
	
	if fileinfo( file_in ) == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid existing file is expected.\n"),"x2f_read_lsb",1));
	end
	
	// ... and now, Action
	// =========================================================================
	
	FILETOPARSE = stripblanks(mgetl(file_in));
	
	for i=1:size(FILETOPARSE,"*")
		
		// First case : new field
		if regexp(FILETOPARSE(i),"/^[a-zA-Z][a-zA-Z0-9_]*\s=\s/","o") == 1 then
			current_field_length    = regexp(FILETOPARSE(i),"/\s=\s/","o")
			current_field           = part(FILETOPARSE(i),1:current_field_length-1);
			current_value           = part(FILETOPARSE(i),current_field_length+3:length(FILETOPARSE(i)));
			
			if regexp(current_value,"/^[0-9]+$/") == [] then
				error(msprintf(gettext("%s: The last_successful_build file is not well formated at line %d\n"),"x2f_read_lsb",i));
			end
			
			desc_out(current_field) = strtod(current_value);
			continue;
		end
		
		// Third case : Blank line
		if length(FILETOPARSE(i)) == 0 then
			continue;
		end
		
		// Else Error
		error(msprintf(gettext("%s: The last_successful_build file is not well formated at line %d\n"),"x2f_read_lsb",i));
		
	end
	
endfunction

// =============================================================================
// desc_out = x2f_cat( desc_in_1 , desc_in_2)
// 
// Concatenate two mlist
// 
// Date : 27/04/2009
// =============================================================================

function desc_out = x2f_cat(desc_in_1,desc_in_2)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_cat",2));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(desc_in_1) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_cat",1));
	end
	
	if type(desc_in_2) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_cat",2));
	end
	
	// ... and now, Action
	// =========================================================================
	
	fields_in_2      = getfield(1,desc_in_2);
	fields_in_2(1:2) = [];
	
	if or(isfield(desc_in_1,fields_in_2)) then
		error(msprintf(gettext("%s: The 2 mlist must not have any field in common .\n"),"x2f_cat"));
	end
	
	desc_out = desc_in_1;
	
	for i=1:size(fields_in_2,"*")
		desc_out(fields_in_2(i)) = desc_in_2(fields_in_2(i));
	end
	
endfunction

// =============================================================================
// master_document = x2f_tree_to_master( tree )
// 
// Date : 27/04/2009
// =============================================================================

function master_document = x2f_tree_to_master( tree )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_tree_to_master",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_tree_to_master",1));
	end
	
	// if a master.xml exists in the directory, return this file
	// =========================================================================
	
	if isfield(tree,"master.xml") & tree("master.xml")  then
		str = mgetl( tree("path") + "master.xml" );
		return;
	end
	
	// Start output
	// =========================================================================
	
	if isfield(tree,"language") then
		language = tree("language");
	else
		language = "en_US";
	end
	
	master_document = ["<?xml version=""1.0"" encoding=""UTF-8""?>";..
		"<!DOCTYPE book [";
		"<!--Begin Entities-->"];
	
	// Get the list of all xml files present in this tree
	tree_xmllist = x2f_cat_xmllist( tree , [] )
	
	// Process the path if under windows
	if MSDOS then
		tree_xmllist(:,2) = "file:///"+ getshortpathname(tree_xmllist(:,2));
	end
	
	// Add entities
	master_document = [ master_document ; "<!ENTITY "+tree_xmllist(:,1)+" SYSTEM """+tree_xmllist(:,2)+""">" ]
	
	// title management
	
	
	if isfield(tree,"book_title") & (tree("book_title") <> "") then
		// title defined in CHAPTER
		book_title = tree("book_title");
	
	
	elseif isfield(tree,"title_addchapter") & (tree("title_addchapter") <> "") then
		// title defined in addchapter.sce
		book_title = tree("title_addchapter");
	
	else
		// title built with the directory name
		book_title = tree("title_default");
	end
	
	book_title = strsubst(book_title,"&"  ,"&amp;");
	book_title = strsubst(book_title,"""" ,"&quot;");
	book_title = strsubst(book_title,">"  ,"&gt;");
	book_title = strsubst(book_title,"<"  ,"&lt;");
	
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
		"  <info xml:id=''"+x2f_title2id(book_title)+"_manual''>"; ..
		"    <title>"+book_title+"</title>"; ..
		"  </info>"; ..
		""];
	
	// title management
	// =========================================================================
	
	// title management
	if isfield(tree,"title") & (tree("title") <> "") then
		// title defined in CHAPTER
		section_title = tree("title");
	elseif isfield(tree,"title_addchapter") & (tree("title_addchapter") <> "") then
		// title defined in addchapter.sce
		section_title = tree("title_addchapter");
	else
		// title built with the directory name
		section_title = tree("title_default");
	end
	
	// xml_list management
	// =========================================================================
	
	offset = 0;
	
	if isfield(tree,"xml_list") then
		xmllist        = tree("xml_list");
		if xmllist <> [] then
			master_document = [ master_document ; "<part xml:id=''section_"+getmd5(tree("path"),"string")+"''>" ];
			master_document = [ master_document ; "<title>"+section_title+"</title>" ];
			master_document = [ master_document ; "&"+xmllist(:,1)+";" ];
			offset          = 1;
		end
	end
	
	// Loop on dir_
	// =========================================================================
	
	my_subtrees = getfield(1,tree);
	my_subtrees(find(part(my_subtrees,1:4)<>"dir_")) = [];
	
	for i=1:size(my_subtrees,"*")
		master_document = [ master_document ; x2f_tree_to_section( tree(my_subtrees(i)) , offset ) ];
	end
	
	// End of master document
	// =========================================================================
	
	if offset == 1 then
		master_document = [ master_document ; "</part>" ];
	end
	
	master_document = [ master_document; "</book>" ];
	
endfunction

// =============================================================================
// master_section = x2f_tree_to_section( tree , offset )
//
// Date : 27/04/2009
// =============================================================================

function master_section = x2f_tree_to_section( tree , offset )
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_tree_to_section",2));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_tree_to_section",1));
	end
	
	if type(offset) <> 1 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A integer expected.\n"),"x2f_tree_to_section",2));
	end
	
	// And now, action ...
	// =========================================================================
	
	//
	// Section Type management
	//
	
	// level :
	//  0 : book 
	//  1 : part
	//  2 : chapter
	//  3 : sect1
	//  4 : sect2
	//  5 : sect3
	//  6 : sect4	
	
	section_types = [ ..
				"book"    ; ..
				"part"    ; ..
				"chapter" ; ..
				"section" ; ..
				"section"   ; ..
				"section"   ; ..
				"section"   ; ..
				"section"   ];
	
	if isfield(tree,"level") & ((tree("level") + offset)>0) & ((tree("level") + offset)<8) then
		section_type = section_types( tree("level") + offset + 1 );
	else
		section_type = "reference";
	end
	
	//
	// title management
	//
	
	if isfield(tree,"title_addchapter") & (tree("title_addchapter") <> "") then
		// title defined in addchapter.sce
		section_title = tree("title_addchapter");
	elseif isfield(tree,"title") & (tree("title") <> "") then
		// title defined in CHAPTER
		section_title = tree("title");
	else
		// title built with the directory name
		section_title = tree("title_default");
	end
	
	section_title  = strsubst(section_title,"&"  ,"&amp;");
	section_title  = strsubst(section_title,"""" ,"&quot;");
	section_title  = strsubst(section_title,">"  ,"&gt;");
	section_title  = strsubst(section_title,"<"  ,"&lt;");
	
	master_section = [];
	master_section = [ master_section ; "<"+section_type+" xml:id=''section_"+getmd5(tree("path"),"string")+"''>" ];
	master_section = [ master_section ; "<title>"+section_title+"</title>" ];
	
	// Loop on dir_
	// =========================================================================
	
	my_subtrees = getfield(1,tree);
	my_subtrees(find(part(my_subtrees,1:4)<>"dir_")) = [];
	
	for i=1:size(my_subtrees,"*")
		master_section = [ master_section ; x2f_tree_to_section( tree(my_subtrees(i)) , offset ) ];
	end
	
	// xml list
	// =========================================================================
	
	if isfield(tree,"xml_list") then
		xmllist        = tree("xml_list");
		if xmllist <> [] then
			master_section = [ master_section ; "&"+xmllist(:,1)+";" ];
		end
	end
	
	master_section = [ master_section ; "</"+section_type+">" ];
	
endfunction

// =============================================================================
// tree_out = x2f_merge_trees( tree_in_1 , tree_in_2 )
//
// Date : 27/04/2009
// =============================================================================

function tree_out = x2f_merge_trees( tree_in_1 , tree_in_2 )
	
	// tree_in_1 : reference tree
	// tree_in_2 : tree to fill
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_merge_trees",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree_in_1) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_merge_trees",1));
	end
	
	if type(tree_in_2) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_merge_trees",2));
	end
	
	tree_out = tree_in_2;
	
	// Loop on dir_
	// =========================================================================
	
	my_subtrees = getfield(1,tree_in_1);
	my_subtrees(find(part(my_subtrees,1:4)<>"dir_")) = [];
	
	for i=1:size(my_subtrees,"*")
		// Check if the subtree exists in tree_in_2
		if ~isfield(tree_in_2,my_subtrees(i)) then
			// if not, copy the whole subtree in tree_in_2
			this_subtree             = tree_in_1(my_subtrees(i));
			tree_out(my_subtrees(i)) = tree_in_1(my_subtrees(i));
			tree_out("xml_number")   = tree_out("xml_number") + this_subtree("xml_number");
		
		else
			// if yes, iterate with x2f_merge_trees
			this_subtree_before      = tree_in_2(my_subtrees(i))
			xml_number_before        = this_subtree_before("xml_number");
			this_subtree_after       = x2f_merge_trees( tree_in_1(my_subtrees(i)) , this_subtree_before );
			xml_number_after         = this_subtree_after("xml_number");
			
			tree_out(my_subtrees(i)) = this_subtree_after;
			tree_out("xml_number")   = tree_out("xml_number") + xml_number_after - xml_number_before;
		end
	end
	
	// XML list management
	// =========================================================================
	
	if isfield(tree_in_1,"xml_list") then
		xmllist_1 = tree_in_1("xml_list");
	else
		xmllist_1 = [];
	end
	
	if isfield(tree_in_2,"xml_list") then
		xmllist_2 = tree_in_2("xml_list");
	else
		xmllist_2 = [];
	end
	
	xmllist_out = xmllist_2;
	
	for i=1:size(xmllist_1(:,1),"*")
		if find(xmllist_2(:,1) == xmllist_1(i,1)) == [] then
			xmllist_out             = [ xmllist_out ; xmllist_1(i,:) ];
			tree_out("xml_number")  = tree_out("xml_number") + 1;
		end
	end
	
	tree_out("xml_list") = xmllist_out;
	
endfunction

// =============================================================================
// xmllist_out = x2f_cat_xmllist( tree , xmllist_in )
//
// Returns a n x 3 matrix that list all xmlfile in this tree and subtrees
// 
// Date : 27/04/2009
// =============================================================================

function xmllist_out = x2f_cat_xmllist( tree , xmllist_in )
	
	rhs         = argn(2);
	xmllist_out = xmllist_in;
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_cat_xmllist",2));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_cat_xmllist",1));
	end
	
	if (xmllist_in<> []) & (type(xmllist_in) <> 10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"x2f_cat_xmllist",2));
	end
	
	// This tree xml list to start
	// =========================================================================
	
	if isfield(tree,"xml_list") then
		xmllist_out = [ xmllist_out ; tree("xml_list") ];
	end
	
	// Loop on dir_
	// =========================================================================
	
	my_subtrees = getfield(1,tree);
	my_subtrees(find(part(my_subtrees,1:4)<>"dir_")) = [];
	
	for i=1:size(my_subtrees,"*")
		xmllist_out = x2f_cat_xmllist( tree(my_subtrees(i)) , xmllist_out );
	end
	
endfunction

// =============================================================================
// section_id = x2f_title2id(title)
// 
// Returns a normalized string from <title> string
//
// Date : 27/04/2009
// =============================================================================

function section_id = x2f_title2id(title_in)
	
	section_id = title_in;
	
	section_id = strsubst(section_id , "&"  , "_" );
	
	section_id = strsubst(section_id , "À"  , "A" );
	section_id = strsubst(section_id , "Á"  , "A" );
	section_id = strsubst(section_id , "Â"  , "A" );
	section_id = strsubst(section_id , "Ã"  , "A" );
	section_id = strsubst(section_id , "Ä"  , "A" );
	section_id = strsubst(section_id , "Å"  , "A" );
	section_id = strsubst(section_id , "Æ"  , "AE");
	section_id = strsubst(section_id , "Ç"  , "C" );
	section_id = strsubst(section_id , "È"  , "E" );
	section_id = strsubst(section_id , "É"  , "E" );
	section_id = strsubst(section_id , "Ê"  , "E" );
	section_id = strsubst(section_id , "Ë"  , "E" );
	section_id = strsubst(section_id , "Ì"  , "I" );
	section_id = strsubst(section_id , "Í"  , "I" );
	section_id = strsubst(section_id , "Î"  , "I" );
	section_id = strsubst(section_id , "Ï"  , "I" );
	section_id = strsubst(section_id , "Ð"  , "D" );
	section_id = strsubst(section_id , "Ñ"  , "N" );
	section_id = strsubst(section_id , "Ò"  , "O" );
	section_id = strsubst(section_id , "Ó"  , "O" );
	section_id = strsubst(section_id , "Ô"  , "O" );
	section_id = strsubst(section_id , "Õ"  , "O" );
	section_id = strsubst(section_id , "Ö"  , "O" );
	section_id = strsubst(section_id , "Ù"  , "U" );
	section_id = strsubst(section_id , "Ú"  , "U" );
	section_id = strsubst(section_id , "Û"  , "U" );
	section_id = strsubst(section_id , "Ü"  , "U" );
	section_id = strsubst(section_id , "Ý"  , "Y" );
	section_id = strsubst(section_id , "à"  , "a" );
	section_id = strsubst(section_id , "á"  , "a" );
	section_id = strsubst(section_id , "â"  , "a" );
	section_id = strsubst(section_id , "ã"  , "a" );
	section_id = strsubst(section_id , "ä"  , "a" );
	section_id = strsubst(section_id , "å"  , "a" );
	section_id = strsubst(section_id , "æ"  , "ae");
	section_id = strsubst(section_id , "ç"  , "c" );
	section_id = strsubst(section_id , "è"  , "e" );
	section_id = strsubst(section_id , "é"  , "e" );
	section_id = strsubst(section_id , "ê"  , "e" );
	section_id = strsubst(section_id , "ë"  , "e" );
	section_id = strsubst(section_id , "ì"  , "i" );
	section_id = strsubst(section_id , "í"  , "i" );
	section_id = strsubst(section_id , "î"  , "i" );
	section_id = strsubst(section_id , "ï"  , "i" );
	section_id = strsubst(section_id , "ñ"  , "n" );
	section_id = strsubst(section_id , "ò"  , "o" );
	section_id = strsubst(section_id , "ó"  , "o" );
	section_id = strsubst(section_id , "ô"  , "o" );
	section_id = strsubst(section_id , "õ"  , "o" );
	section_id = strsubst(section_id , "ö"  , "o" );
	section_id = strsubst(section_id , "ù"  , "u" );
	section_id = strsubst(section_id , "ú"  , "u" );
	section_id = strsubst(section_id , "û"  , "u" );
	section_id = strsubst(section_id , "ü"  , "u" );
	section_id = strsubst(section_id , "ý"  , "y" );
	section_id = strsubst(section_id , "ÿ"  , "y" );
	
	section_id = strsubst(section_id , ":"  , ""  );
	section_id = strsubst(section_id , "\"  , "_" );
	section_id = strsubst(section_id , "/"  , "_" );
	section_id = strsubst(section_id , "''" , "_" );
	section_id = strsubst(section_id , "  " , " " );
	section_id = strsubst(section_id , " "  , "_" );
	section_id = strsubst(section_id , "["  , ""  );
	section_id = strsubst(section_id , "]"  , ""  );
	
	section_id = convstr(section_id,"l");
	
endfunction

// =============================================================================
// timestamp = x2f_get_most_recent( tree )
//
// Return the timestamp associated with the most recent XML file, ... and its
// path if a second output argument is precised
//
// Date : 27/04/2009
// =============================================================================

function [timestamp,path] = x2f_get_most_recent( tree )
	
	rhs         = argn(2);
	timestamp   = 0;
	path        = "";
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"x2f_get_most_recent",1));
	end
	
	// Check input arguments type
	// =========================================================================
	
	if type(tree) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"x2f_cat_xmllist",1));
	end
	
	// Get the full XML list
	// =========================================================================
	
	xmllist =  x2f_cat_xmllist(tree,[]);
	
	[values,index] = gsort(strtod(xmllist(:,3)));
	
	timestamp = xmllist( index(1) , 3 );
	path      = xmllist( index(1) , 2 );
	
endfunction
