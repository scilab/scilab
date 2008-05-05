// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xs2fig(figureNumber, fileName)

	[lhs,rhs]=argn(0);
	
	//Input arguments checking
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected\n"), "xs2fig",2));
		return;
	end

	//first argument checking
	if type(figureNumber) <> 1 | size(figureNumber) <> [1,1] then
		error(msprintf(gettext("%s: Wrong type for input argument: Integer expected.\n"), "xs2fig"));
		return;
	end
		
	//second argument checking
	if type(fileName) <> 10 | fileName == "" then
		error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"), "xs2fig"));
		return;
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
	
	//create the eps file
	fileExport = TMPDIR + filesep() + fileName + ".eps";	
	xs2eps(figureNumber, fileExport);
		
	// convert it to fig
	pstoeditOptions = "-f ""fig""";
	unix_s(pstoeditPath + " " + pstoeditOptions + " " + fileExport + " " + generatedFileName);
	// @TODO catch errors and display them if occurs
	
	//delete the temporary eps file 
	mdelete(fileExport);
  
endfunction


