function path=gethelpfile(key)
// copy of gethelpfile contained in help.sci
  global %helps %browsehelp
  sep="/";
  key=stripblanks(key)
  l=length(key)
  for k=1:size(%helps,1)
    [fd,ierr]=mopen(%helps(k,1)+sep+"whatis.htm","r");
    if ierr<>0 then
      warning(" whatis file missing in "+%helps(k,1)+". Directory ignored")
    else
       whatis=mgetl(fd);mclose(fd)
       for k2=1:size(whatis,"*")
	 lwhatis=whatis(k2)
	 i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>")
	 if j<>[] then
	   lkey=part(lwhatis,i(2)+1:j-1)
	   // key must be a word in lkey
	   if findword(lkey,key)<>[] then
	     i=strindex(lwhatis,"HREF="); j=strindex(lwhatis,">")
	     path=%helps(k,1)+sep+part(lwhatis,i+6:j(2)-2)
	     return
	   end
	 end
       end
    end
  end
  
  path=genhelpfromfunc(key)

endfunction

// find word "word" in string "str"
function res=findword(str,word)
  if str==word then res=1; return; end
  // word is at the beginning of str
  res=strindex(str,word+" ")
  if res==1 then return; end
  // word is in the middle of str
  res=strindex(str," "+word+" ")
  if res<>[] then return; end
  // word is at the end of str
  res=strindex(str," "+word)
  if res==length(str)-length(word) then return; end
  res=[]
endfunction	   

function path=genhelpfromfunc(key)
  global %helps
  path=[]
  if exists(key)==0 then return,end
  execstr('%fun='+key)
  if type(%fun)==11 then comp(%fun),end
  if type(%fun)<> 13 then return,end

  [status,message]=mkdir(TMPDIR,'localman') 
  if and(status<>[1 2]) then return, end
  if fileinfo(TMPDIR+'/localman/whatis.htm')==[] then
    W=['<html>'
       '<head>'
       '<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">'
       '<title>User Functions</title>'
       '</head>'
       '<body bgcolor=""#FFFFFF"">'
       '<dl>'
       '</dl></body></html>']
    mputl(W,TMPDIR+'/localman/whatis.htm')
  end
  nh=size(%helps,1)
  add_help_chapter('User Functions',TMPDIR+'/localman')
    

  

  l=macr2lst(%fun);l($+1)='99';
  syntax='['+strcat(l(2),',')+'] = '+key+'('+strcat(l(3),',')+')';
  pars=unique([l(2), l(3)]);pars=pars($:-1:1);
  if pars<>[] then
    P=["    <h3>";
       "      <font color=""blue"">Parameters</font>";
       "    </h3>"
       "    <ul>"];
    for p=pars
      P=[P;"      <li>";
	 "        <tt>";
	 "          <b>"+p+"</b>";
	 "        </tt>: </li>"];
    end
    P=[P;"    </ul>"];
  else
    P=[];
  end
  D=[];k=5;
  while l(k)(1)=="31"
    if l(k)(1)<>"31" then break,end
    D=[D;l(k)(2)]; k=k+2;
  end
  short=D(1)
  txt=["<html>";
       "  <head>";
       "    <meta http-equiv=""Content-Type"" content=""text/html; charset=UTF-8"">";
       "    <title>"+key+"</title>";
       "  </head>";
       "  <body bgcolor=""#FFFFFF"">";
       "    <center>Scilab Function</center>";
       "    <p>";
       "      <b>"+key+"</b> -  "+short+"</p>";
       "    <h3>";
       "      <font color=""blue"">Calling Sequence</font>";
       "    </h3>";
       "    <dl>";
       "      <dd>";
       "        <tt>"+syntax+"</tt>";
       "      </dd>";
       "    </dl>";
       P
       "    <h3>";
       "      <font color=""blue"">Description</font>";
       "    </h3>";
       "    <p>";
       "      <pre>";
       D
       "      </pre>";
       "  </body>";
       "</html>"];
 
  mputl(txt,TMPDIR+'/localman/'+key+'.htm')
  path=TMPDIR+'/localman/'+key+'.htm'
  W=mgetl(TMPDIR+'/localman/whatis.htm')
  W=[W(1:$-1);
      "<dd><A HREF="""+key+".htm"">"+key+"</A> -"+short+" </dd>"
     W($)]
  mputl(W,TMPDIR+'/localman/whatis.htm')
  if   nh==size(%helps,1)&%browsehelp=='Scilab Browser' then 
    //force a rescan
    TCL_SetVar('sciGUITable(browsehelp,nchap)',string(nh-1))
  end
endfunction
