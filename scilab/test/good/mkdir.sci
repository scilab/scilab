// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [status,msg]=mkdir(varargin)
	// mkdir Make a directory
	//------------------------------------------------------------------------
	
	lhs        = argn(1);
	rhs        = argn(2);
	
	DirName    = '';
	NewDirName = '';
	status     = 0 ;
	msg        = '';
	
	select rhs

	case 0
	  error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),'mkdir'));
		break
	
	case 1
		NewDirName   = varargin(1);
		NewDirName   = stripblanks(NewDirName,%T);
		if is_absolute_path(NewDirName) then
			NewDirectory = NewDirName;
		else
			NewDirectory = fullfile(pwd(),NewDirName);
		end
		break
		
	case 2
		DirName      = varargin(1);
		NewDirName   = varargin(2);
		NewDirName   = stripblanks(NewDirName,%T);
		DirName      = stripblanks(DirName,%T);
		NewDirectory = fullfile(DirName,NewDirName);
		break
		
	else
		error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),'mkdir'));
	end
	
	if (fileinfo(NewDirectory)==[]) then
		// The directory does not exist
		status = 1;
	else
		// The directory or file exists
		if (isdir(NewDirectory)) then
			// it is a directory
			status = 2;
		else
			// it is a file
			status = -2;
		end
	end
	
	select status

	case 0
		break
		
	case 1
		status = createdir(NewDirectory);
		if (~status) then
			msg    =  msprintf(gettext("%s: An error occurred: %s\n"),'mkdir',gettext('Impossible to create directory ')) + NewDirectory;
			status = 0;
		else
			msg    = '';
			status = 1;
		end
		break
		
	case 2
	  msg    =  msprintf(gettext("%s: An error occurred: %s\n"),'mkdir',gettext('This directory already exists in ')) + NewDirectory;
		break

	case -2
	  msg    =  msprintf(gettext("%s: An error occurred: %s\n"),'mkdir',gettext('A file with the same name already exists in ')) + DirName;
		break

	else
		msg    =  msprintf(gettext("%s: An error occurred: %s\n"),'mkdir',gettext('Impossible to create directory ')) + NewDirectory;
	end
	
endfunction
