function apropos(key)
global %helps LANGUAGE INDEX
if funptr("TK_EvalStr")==0 then
  write(%io(2),..
      ["Help works only with TCL/TK.";
      "Use your favorite browser to browse SCI/man/"+LANGUAGE+"/index.html"],..
      "(a)")
  return
end
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end

sep="/";
key=stripblanks(key)
l=length(key)
found=[];foundkey=[]
for k=1:size(%helps,1)
  [fd,ierr]=mopen(%helps(k,1)+sep+"whatis.html","r");
  if ierr==0 then
    whatis=mgetl(fd);mclose(fd)
    f=grep(whatis,key)
    if f<>[] then
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
    mprintf("No man found for: "+key+"\n")
  case "fr"
    mprintf("Pas de manuel trouvé pour : "+key+"\n")
  end
else
  provpath=TMPDIR+sep+"apropos"
  [s,k]=sort(foundkey)
  mputl(found(k),provpath)
  if or(sciargs()=="-nw") then
    write(%io(2),found)
  else
    // We must have / in paths, even for Windows
    provpath=strsubst(provpath,"\","/")
    if MSDOS then
      TK_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
      TK_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
      TK_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
      TK_EvalStr("browsehelp eval {set sciw .sciapropos}")
      TK_EvalStr("browsehelp eval {set manpath """+provpath+"""}")
      TK_EvalStr("browsehelp eval {source $SciPath/tcl/browsehelp.tcl}")
    else
      TK_SetVar("lang",LANGUAGE)
      TK_SetVar("Home",INDEX)
      TK_SetVar("sciw",".sciapropos")
      TK_SetVar("manpath",provpath)
      TK_EvalFile(SCI+"/tcl/browsehelp.tcl")
    end
  end
end
endfunction
