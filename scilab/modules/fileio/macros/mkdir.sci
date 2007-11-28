function [status,msg]=mkdir(varargin)
	// mkdir Make a directory
	//------------------------------------------------------------------------
	// Allan CORNET
	// INRIA 2007
	//------------------------------------------------------------------------
	
	lhs        = argn(1);
	rhs        = argn(2);
	
	DirName    = '';
	NewDirName = '';
	status     = 0 ;
	msg        = '';
	
	select rhs

	case 0
		error('Number of parameters incorrect.');
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
		error('Number of parameters incorrect.');
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
			msg    = "Error(s) Impossible to create directory "+NewDirectory+".";
			status = 0;
		else
			msg    = '';
			status = 1;
		end
		break
		
	case 2
		msg = "This directory already exists in "+DirName+".";
		break

	case -2
		msg = "A file with the same name already exists in "+DirName+".";
		break
	
	else
		msg = "Error(s) Impossible to create directory "+NewDirectory+".";
	end
	
endfunction
