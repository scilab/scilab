// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xs2fig(figureNumber, fileName, orientation)

	[lhs,rhs]=argn(0);
	
	//Input arguments checking
	if rhs <> 2 & rhs <> 3 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xs2emf",2 ,3));
		return;
	end

	//first argument checking
	if type(figureNumber) <> 1 | size(figureNumber) <> [1,1] then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Integer expected.\n"), "xs2fig",1));
		return;
	end
		
	//second argument checking
	if type(fileName) <> 10 | fileName == "" then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xs2fig",2));
		return;
	end
	
	//third argument checking
	if rhs == 3 then  
		if orientation == 'l' | orientation == 'landscape' then 
			orientation = "landscape";
		elseif orientation == 'p' | orientation == 'portrait' then 
			orientation = "portrait"; 
		else
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "xs2fig", 3, "landscape", "portrait"));
			return;
		end
	else
		orientation = "portrait";
	end
	
	if ~MSDOS then
	  // os is a unix one
	  // check that pstoedit is available on the computer
	  checkPstoedit = unix_g("which pstoedit");
	  if (checkPstoedit == [] | checkPstoedit == "") then
	    error(msprintf(gettext("%s: Unable to locate pstoedit. Please install pstoedit or add it to the PATH to be able to use this feature.\n"), "xs2fig"));
	    return;
	  end
	end

	
	//checking file extension	 				
	[path,fname,extension] = fileparts(fileName);
	if (extension <> ".fig") then
	  // appened fig at the end of the file name.
	  generatedFileName = fileName + ".fig";
	else
	  generatedFileName = fileName;
	end
	
	// compute pstoedit path
	if MSDOS then
	  // pstoedit is embedded in Scilab
	  pstoeditPath = SCI + "\tools\pstoedit\pstoedit";
	else
	  // pstoedit is installed on the computer.
	  pstoeditPath = "pstoedit";
	end
	
	//When the graphic-export is too long, we inform the user that the figure is exporting
	f = gcf();
	oldInfoMessage = f.info_message;
	f.info_message = "Exporting figure, please wait...";
	
	//create the eps file
	fileExport = TMPDIR + filesep() + fname + ".eps";	
	xs2eps(figureNumber, fileExport, orientation);
		
	// convert it to fig
	//get short  path name for windows because path is > then 6 caracters
	[shortpath,bOK]=getshortpathname(path);
	[path2,fname2,extension2] = fileparts(generatedFileName);
	generatedFileName = shortpath + fname2 + extension2;	
	
	pstoeditOptions = "-f ""fig""";
	
	//Check if we have the permission to export this file
	[fd,errPermission]=mopen(generatedFileName, "w");
	if errPermission <> 0 then
		f.info_message = oldInfoMessage;
		error(msprintf(gettext("%s: Unable to create export file, permission denied.\n"), "xs2fig"));
		return;
	else
		mclose(fd);
	end
	
	// unix_s(pstoeditPath + " " + pstoeditOptions + " " + fileExport + " " + generatedFileName);
	// @TODO catch errors and display them if occurs
	[stdout, status, stderr] = unix_g(pstoeditPath + " " + pstoeditOptions + " " + fileExport + " " + generatedFileName);
	
	if status <> 0 then
		f.info_message = oldInfoMessage;
		error(msprintf(gettext("%s: Unable to execute pstoedit.\n"), "xs2fig"));
		disp(stderr);
	end
	
	//delete the temporary eps file 
	mdelete(fileExport);
	
	//Put back the old infoMessage
	f.info_message = oldInfoMessage;
  
endfunction
