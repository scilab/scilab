function path=make_help_index()
//we create a new index file each time to take into account dynamically
//loaded toolboxes
  global LANGUAGE
  path=TMPDIR+"/index.htm";
  i_lines=["<html>";
	 "<head>";
	 "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">";
	 "    <title>Index</title>";
	 "</head>";
	 "<body bgcolor=""FFFFFF"">";
	 "<BR><A HREF="""+%helps(:,1)+"/whatis.htm"">"+%helps(:,2)+"</A>";
	 "</body></html>"]
	];
  mputl(i_lines,path)
endfunction
