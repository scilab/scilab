lines(0);
// look for .xml files
if MSDOS then
  xml=unix_g('dir /B *.xml');
  xml=xml($:-1:1);
else
  xml=unix_g('ls  -t1 *.xml');
end
l=0;
// find the title 
ind=grep(%helps(:,1),getcwd());
if ind<>[] then 
  whatis_title= %helps(ind,2)
else
   whatis_title='Chapitre du Help '
end
line=["<html>"
  "<head>"
  "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
  "    <title>"+whatis_title+"</title>"
  "</head>"
  "<body bgcolor=""FFFFFF"">"];
l=size(line,'*')  
for k1=1:size(xml,'*')  // loop on .xml files
  path=xml(k1);
  txt=mgetl(path);
  d=grep(txt,"<SHORT_DESCRIPTION");
  f=grep(txt,"</SHORT_DESCRIPTION");
  if d<>[] then 
    for k2=1:size(d,"*")
      // loop on the lines of short description if necessary
      tt="";
      for k3=d(k2):f(k2)
	tt=tt+txt(k3);
      end
      i=strindex(tt,"""");
      name=part(tt,i(1)+1:i(2)-1);
      i1=strindex(tt,">"); i2=strindex(tt,"<");
      desc=stripblanks(part(tt,i1(1)+1:i2(2)-1));
      l=l+1; fname=part(path,[1:length(path)-4])+".html";
      line(l)="<BR><A HREF="""+fname+""">"+name+"</A> - "+desc;
    end
  end
end
line = [line;"</body></html>"]
mputl(sort(line),"whatis.html");
quit

