
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path=make_help_index()
//we create a new index file each time to take into account dynamically
//loaded toolboxes

  OSseparator=filesep(); 
  
  global %helps
  global %helps_modules
  %HELPS=[%helps_modules;%helps];
  path=TMPDIR+OSseparator+"index.htm";
  
  i_lines=["<html>";
	 "<head>";
	 "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">";
	 "    <title>Index</title>";
	 "</head>";
	 "<body bgcolor=""FFFFFF"">";]
  global %browsehelp
  if %browsehelp == "Old Scilab Browser" then
  	// Scilab Browser
  	i_lines=[i_lines;
  		"<BR><A HREF="""+%HELPS(:,1)+OSseparator+"whatis.htm"">"+%HELPS(:,2)+"</A>";
 		"</body></html>"]
  	else
  			// Default Internet Browser IE,Mozilla, Opera,Nautilus
  			i_lines=[i_lines;
			"<BR><A HREF=""file:///"+%HELPS(:,1)+OSseparator+"whatis.htm"">"+%HELPS(:,2)+"</A>";
			"</body></html>"]
  	end
  clear %browsehelp	OSseparator
  mputl(i_lines,path)
endfunction
