function help(key)
if funptr("TK_EvalStr")==0 then
  write(%io(2),..
      ["Help works only with TCL/TK.";
      "Use your favorite browser to browse SCI/man/"+LANGUAGE+"/index.html"],..
      "(a)")
  return
end
[lhs,rhs]=argn(0)
if rhs==0 then
  browsehelp()
  return
else
  if rhs<>1 then error(39), end
end

path=gethelpfile(key)
if path<>[] then
  browsehelp(path,key)
else
  apropos(key)
end
endfunction

function path=gethelpfile(key)
global %helps
sep="/";
key=stripblanks(key)
l=length(key)
for k=1:size(%helps,1)
  [fd,ierr]=mopen(%helps(k,1)+sep+"whatis.html","r");
  if ierr<>0 then
    warning(" whatis file missing in "+%helps(k,1)+". Directory ignored")
  else
    whatis=mgetl(fd);mclose(fd)
    for k2=1:size(whatis,"*")
      lwhatis=whatis(k2)
      i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>")
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
path=[]
endfunction

// find word "word" in string "str"
function res=findword(str,word)
if str==word then res=1; return end
// word is at the beginning of str
res=strindex(str,word+" ")
if res==1 then return end
// word is in the middle of str
res=strindex(str," "+word+" ")
if res<>[] then return end
// word is at the end of str
res=strindex(str," "+word)
if res==length(str)-length(word) then return end
res=[]
endfunction	    

function browsehelp(path,key)
global LANGUAGE INDEX
[lhs,rhs]=argn(0)
if rhs==0 then
  path=INDEX
  key="index"
end
if or(sciargs()=="-nw") then
  write(%io(2),mgetl(path))
else
  // We must have / in paths, even for Windows
  path=strsubst(path,"\","/")
  if MSDOS then
    TK_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
    TK_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
    TK_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
    TK_EvalStr("browsehelp eval {set sciw .scihelp-"+key+"}")
    TK_EvalStr("browsehelp eval {set manpath """+path+"""}")
    TK_EvalStr("browsehelp eval {source $SciPath/tcl/browsehelp.tcl}")
  else
    TK_SetVar("lang",LANGUAGE)
    TK_SetVar("Home",INDEX)
    TK_SetVar("sciw",".scihelp-"+key)
    TK_SetVar("manpath",path)
    TK_EvalFile(SCI+"/tcl/browsehelp.tcl")
  end
end
endfunction
