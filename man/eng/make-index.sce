lines(0);
find_links;
sep="/";if MSDOS then sep="\",end
path=get_absolute_file_path("html.xsl")+"html.xsl"
dirs=%helps(:,1)
txt=%helps(:,2)
l=0
for k=1:size(dirs,'*')
  d=dirs(k)
  w=relative_path(d+sep+"whatis.html",path)
  l=l+1;
  line(l)="<BR><A HREF="""+w+""">"+txt(k)+"</A>"
end
mputl(line,"index.html")
quit
