function path=make_help_index()
//we create a new index file each time to take into account dynamically
//loaded toolboxes
// Copyright INRIA   
  global LANGUAGE
  path=TMPDIR+"/index.htm";
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
  		"<BR><A HREF="""+%helps(:,1)+"/whatis.htm"">"+%helps(:,2)+"</A>";
 		"</body></html>"]
  	else
  		if %browsehelp == "help widget" then 
  			// Gtk browser
  			i_lines=[i_lines;
  			"<BR><A HREF="""+%helps(:,1)+"/whatis.htm"">"+%helps(:,2)+"</A>";
 			"</body></html>"]
  		else
  			// Default Internet Browser IE,Mozilla, Opera,Nautilus
  			i_lines=[i_lines;
			"<BR><A HREF=""file:///"+%helps(:,1)+"/whatis.htm"">"+%helps(:,2)+"</A>";
			"</body></html>"]
		end
  	end
  clear %browsehelp	
  mputl(i_lines,path)
endfunction
