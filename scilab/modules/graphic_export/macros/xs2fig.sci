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
		error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "xs2fig"));
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
	
	//checking file extension	 				
	[path,fname,extension] = fileparts(fileName)
	
  
	//create the fig file
	fileExport = TMPDIR + "\" + fileName + ".eps";	
	xs2eps(figureNumber, fileExport); 			  
			  
	if (getos() == "Windows") then 
				
		if (extension <> ".fig") then							
			unix_w(SCI + "\tools\pstoedit\pstoedit -f plot-fig " + fileExport + " " + fileName + ".fig"); 				  
				  
		elseif (extension == ".fig") then			  
			unix_w(SCI + "\tools\pstoedit\pstoedit -f plot-fig " + fileExport + " " + fileName);  
				  
		else
			error('Problem generating file , perhaps this option is not available with this OS.')
		end			
	end
	
	//delete the eps file 
	mdelete(fileExport);
  
endfunction


