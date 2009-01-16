// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Koumar
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xs2emf(figureNumber, fileName, orientation)

	//Input arguments checking
  [lhs,rhs]=argn(0);
	if rhs <> 2 & rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xs2emf",2,3));
		return;
	end

	//first argument checking
	if type(figureNumber) <> 1 | size(figureNumber,'*') <> 1 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Integer expected.\n"), "xs2emf",1));
		return;
	end
		
	//second argument checking
	if type(fileName) <> 10 | fileName == "" then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xs2emf",2));
		return;
	end
	
	//third argument checking
	if rhs == 3 then  
		if orientation == 'l' | orientation == 'landscape' then 
			orientation = "landscape";
		elseif orientation == 'p' | orientation == 'portrait' then 
			orientation = "portrait"; 
		else
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "xs2emf", 3, "landscape", "portrait"));
			return;
		end
	else // rhs == 2
		orientation = "portrait";
	end
	
	if ~MSDOS then
    // os is a unix one
	  error(msprintf(gettext("%s: EMF format only available under Microsoft Windows OS.\n"), "xs2emf"));
	  return;
	end
	
  if fileinfo(SCI+'/tools/pstoedit/gsdll32.dll') == [] then
    msgErr1 = "Please install Ghostscript 32 bits to export an EMF file.";
    msgErr2 = "http://www.ghostscript.com/awki";
    msg = [gettext(msgErr1);gettext(msgErr2)];
    messagebox(msg, "Scilab error", "error");
    return;
  end
	
	//checking file extension
	[path, fname, extension] = fileparts(fileName);
	if (extension <> ".emf") then
	  // appened emf at the end of the file name.
	  generatedFileName = fileName + ".emf";
	else
	  generatedFileName = fileName;
	end
	
  // pstoedit is embedded in Scilab
  pstoeditPath = SCI + "\tools\pstoedit\pstoedit.exe";
  if fileinfo(pstoeditPath) == [] then 
    error(msprintf(gettext("%s: Unable to find pstoedit.\n"), "xs2emf"));
  end
	
	//When the graphic-export is too long, we inform the user that the figure is exporting
	f = gcf();
	oldInfoMessage = f.info_message;
	f.info_message = "Exporting figure, please wait...";	
	
	//create the eps file
	fileExport = TMPDIR + filesep() + fname + ".eps";	
	xs2eps(figureNumber, fileExport, orientation);
		
	// convert it to emf
	//get short  path name for windows because path is > then 6 caracters
	[shortpath, bOK] = getshortpathname(path);	
	[path2, fname2, extension2] = fileparts(generatedFileName);
	generatedFileName = shortpath + fname2 + extension2;	
	
	//Check if we have the permission to export this file
	[fd, errPermission] = mopen(generatedFileName, "wt");
	if errPermission <> 0 then
		f.info_message = oldInfoMessage;
		error(msprintf(gettext("%s: Unable to create export file, permission denied.\n"), "xs2emf"));
		return;
	else
		mclose(fd);
	end

	pstoeditOptions = "-f ""emf""";
	[stdout, status, stderr] = unix_g(pstoeditPath + " " + pstoeditOptions + " " + fileExport + " " + generatedFileName);		

	if status <> 0 then
		f.info_message = oldInfoMessage;
		error(msprintf(gettext("%s: Unable to execute pstoedit.\n"), "xs2emf"));
		disp(stderr);
	end
	
	//delete the temporary eps file 
	mdelete(fileExport);
	
	//Put back the old infoMessage
	f.info_message = oldInfoMessage;
  
endfunction
