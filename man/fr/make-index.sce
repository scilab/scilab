lines(0);
find_links;
sep="/";if MSDOS then sep="\",end
path=get_absolute_file_path("html.xsl")+"html.xsl"
dirs=%helps(:,1)
txt=%helps(:,2)
line=["<html>"
  "<head>"
  "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
  "    <title>Index</title>"
  "</head>"
  "<body bgcolor=""FFFFFF"">"];
l=size(line,'*')  
for k=1:size(dirs,'*')
  d=dirs(k)
  w=relative_path(d+sep+"whatis.htm",path)
  l=l+1;
  line(l)="<BR><A HREF="""+w+""">"+txt(k)+"</A>"
end
line = [line;"</body></html>"]
mputl(line,"index.htm")
quit
