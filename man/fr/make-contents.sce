// 
dirs = dirname(%helps(:,1));
base = basename(%helps(:,1));
// help in the std man directory 
std= grep(dirs,SCI)
n=size(%helps(:,1),'*')
// help in the non std man directory 
nstd=1:n;
nstd(std)=[];

full_whatis=[];
full_whatis_name=[];

for k=std
  [fd,ierr]=mopen(%helps(k,1)+"/whatis.html","r");
  if ierr<>0 then
    warning(" whatis file missing in "+%helps(k,1)+". Directory ignored")
  else
     whatis=mgetl(fd);mclose(fd);
     ind=grep(whatis,'</A>');
     whatis=whatis(ind);
     for k1=1:size(whatis,'*')
       lwhatis = whatis(k1);
       i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>");
       lkey=part(lwhatis,i(2)+1:j-1);
       full_whatis_name=[full_whatis_name;lkey];
     end
     whatis=strsubst(whatis,"HREF=""","HREF="""+base(k)+"/");
     full_whatis=[full_whatis;whatis];
  end 
end

for k=nstd
  [fd,ierr]=mopen(%helps(k,1)+"/whatis.html","r");
  if ierr<>0 then
    warning(" whatis file missing in "+%helps(k,1)+". Directory ignored")
  else
     whatis=mgetl(fd);mclose(fd);
     ind=grep(whatis,'</A>');
     whatis=whatis(ind);
     for k1=1:size(whatis,'*')
       lwhatis = whatis(k1);
       i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>");
       lkey=part(lwhatis,i(2)+1:j-1);
       full_whatis_name=[full_whatis_name;lkey];
     end
     whatis=strsubst(whatis,"HREF=""","HREF"""+dirs(k)+"/");
     full_whatis=[full_whatis;whatis];
  end 
end

[sv,sk]=sort(full_whatis_name);
full_whatis=full_whatis(sk);

full_whatis=["<html>"
	     "<head>"
	     "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
	     "  <title>Index Général</title>"
	     "</head>"
	     "<body bgcolor=""FFFFFF"">";
	     full_whatis;
     	     "<H2>Scilab General Index</H2>";
	     "</body></html>"
	    ];

mputl(full_whatis,'contents.html')

quit
