// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function check_help(dirs)
	
	// =========================================================================
	// Date : 08/08/2006
	//
	// Update by Sylvestre LEDRU <sylvestre.ledru@inria.fr> on the 04/10/2007
	// dirs is a set of directories for which html manuals are to be generated
	// =========================================================================
	
	
	// Save the current path and the variable %helps
	//--------------------------------------------------------------------------
	
	current_directory = pwd();
	
	[lhs,rhs]=argn(0);
	
	// Too many parameters
	// -------------------------------------------------------------------------
	
	if rhs > 1 then
		error(39);
		return;
	end
	
	// Default case : Check of the Scilab Online help
	// -------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		global %helps;
		global %helps_modules
		%HELPS=[%helps_modules;%helps];
		dirs_to_build = %HELPS;
		clear %HELPS;
		
		//----------------------------------------------------------------------
		// Special case because scicos is not written in xml
		//----------------------------------------------------------------------
		scs = grep(dirs_to_build,'scicos');
		if size(scs,'*') == 1 then dirs_to_build(scs,:)=[]; end
		// End of patch --------------------------------------------------------
		
		dirs = gsort(dirs_to_build(:,1),"lr","i");
	end
	
	// Transform the relative path to the absolute one
	// -------------------------------------------------------------------------
	
	for k=1:size(dirs,'*');
		chdir(dirs(k));
		if MSDOS then
			dirs(k) = getlongpathname(pwd());
		else
			dirs(k) = pwd();
		end
		chdir(current_directory);
	end
	
	// Management of the log file
	//--------------------------------------------------------------------------
	logfile    = pathconvert(SCIHOME+"/check_help_"+getlanguage()+".log",%f,%f);
	logfile_id = mopen(logfile,"w");
	mclose(logfile_id);
	
	// Number of files with a bad XML syntax in all the processed directories
	//--------------------------------------------------------------------------
	
	nb_badfiles = 0;
	badfiles    = [];
	
	// Loop on the directory
	//--------------------------------------------------------------------------
	
	for k1=1:size(dirs,'*')
		
		// Number of files with a bad XML syntax in the current directory
		//----------------------------------------------------------------------
		nb_badfiles_dir = 0;
		
		// Creation of the XML list file
		//----------------------------------------------------------------------
		chdir(dirs(k1));
		xml = listfiles('*.xml');
		
		if xml <> [] then
		
			// Check if we must verify this directory
			//------------------------------------------------------------------
			
			if fileinfo(".last_successful_check") == [] then
				need_to_be_checked = %T;
			else
				exec(".last_successful_check",-1);
				
				// Check which file is the more recent
				//--------------------------------------------------------------
				
				max_change_date = 0;
				
				for i=1:size(xml,'*')
					file_date = fileinfo(xml(i));
					if file_date(6) > max_change_date then
						max_change_date = file_date(6);
					end
				end
				
				if max_change_date > last_success_check_val then
					need_to_be_checked = %T;
				else
					need_to_be_checked = %F;
				end
			end
			
			// Loop on the XML files
			//------------------------------------------------------------------
			
			if need_to_be_checked then
				
				if xml <> [] then
				
					for k2=1:size(xml,'*')
						
						if k2 == 1 then
							mprintf("%s\n",strsubst(dirs(k1),SCI,"SCI"));
						end
						
						unix(SCI+"/modules/helptools/bin/scivalid """+xml(k2)+""" > "+TMPDIR+"/check_help.txt 2>&1");
						
						my_log = mgetl(TMPDIR+"/check_help.txt");
						
						if ( size( grep(my_log,"/IDREF(.)*without matching ID/","r"), "*" ) <> size(my_log,"*") ) then
							
							nb_badfiles     = nb_badfiles + 1;
							nb_badfiles_dir = nb_badfiles_dir + 1;
							
							badfile         = pathconvert(dirs(k1)+"/"+xml(k2),%f,%f);
							badfiles        = [badfiles;badfile];
							logfile_id      = mopen(logfile,"a+");
							
							mfprintf(logfile_id,"\n----------------------------------------------------------------------\n");
							mfprintf(logfile_id,"%s\n",badfile);
							mfprintf(logfile_id,"----------------------------------------------------------------------\n");
							
							for line=1:size(my_log,'*')
								if( grep( my_log(line) ,"/IDREF(.)*without matching ID/","r" ) == [] ) then
									mfprintf(logfile_id,"%s\n",my_log(line));
								end
							end
							
							mclose(logfile_id);
						end
					end
				end
				
				// Creation of the file "directory/.last_successful_check"
				//--------------------------------------------------------------
				
				if nb_badfiles_dir == 0 then
					dateToPrint = msprintf("last_success_check_val = %d",getdate('s'));
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
			if (isdef('editor') | (funptr('editor')<>0)) then
			  editor(logfile);
			else
			  mprintf("\n\tSee %s\n",logfile);
			end
		else
			mprintf("\n\tSee %s\n",logfile);
		end
	end
	
	// Restore the initial environement
	//--------------------------------------------------------------------------
	
	chdir(current_directory);
	
endfunction
