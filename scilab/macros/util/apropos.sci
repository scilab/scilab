function apropos(key)
  if argn(2)<>1 then error(39), end
  change_old_man()
  INDEX=make_help_index()

  global %helps LANGUAGE INDEX
  global %browsehelp;
  [lhs,rhs]=argn(0);
  // list relevant man for key
  if %browsehelp=="Scilab Browser" then
    browsehelp(" ",key);
  else
    key=convstr(key)
    provpath =apropos_gener(key);
    browsehelp(provpath,key);
  end

endfunction 

function [provpath]=apropos_gener(key)
// generate html file for apropos key
// provpath is the path of generated html file
  global %helps LANGUAGE INDEX
  sep="/";
  key1=key
  K=[' ','(',')','[',']','{','}','""','/','\','.','<','>']
  for k=K,key1=strsubst(key1,k,'_'),end
  l=length(key)
  found=[];foundkey=[]
  for k=1:size(%helps,1)
    [fd,ierr]=mopen(%helps(k,1)+sep+"whatis.htm","r");
    if ierr==0 then
      whatis=mgetl(fd);mclose(fd)
      ind=grep(whatis,'</A>');
      whatis=whatis(ind);
      if whatis<>[] then
	f=grep(convstr(whatis),key);
	lwhatis=strsubst(whatis(f),"HREF=""", "HREF="""+%helps(k,1)+sep)
	found=[found;lwhatis];
	for k1=f
	  i=strindex(whatis(k1),">"); j=strindex(whatis(k1),"</A>")
	  lkey=part(whatis(k1),i(2)+1:j-1)
	  foundkey=[foundkey;lkey]
	end
      end
    end
  end
  if found==[] then
    select LANGUAGE
     case "eng"
      found="<H3>No man found for: "+key+"</H3>";
     case "fr"
      found="<H3>Pas de manuel trouvé pour : "+key+"</H3>";
    end
  else
     [s,k]=sort(foundkey);
     found= found(k);
  end
  provpath=TMPDIR+sep+"apropos_"+key1;
  if MSDOS then
  	provpath=provpath+'.htm';
  end

  apropos_txt =["<html>";
		 "<head>";
		 "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">";
		 "    <title>Apropos "+key+"</title>";
		 "</head>";
		 "<body bgcolor=""FFFFFF"">";
		 found;
  		 "</body></html>"];
  mputl(apropos_txt,provpath)

endfunction
