function path=gethelpfile(key)
// copy of gethelpfile contained in help.sci
  global %helps
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
  path=[]
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
