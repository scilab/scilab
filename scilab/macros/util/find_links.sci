function flag=find_links(filein,fileout)
  // return %t if a LINK were found
  [lhs,rhs]=argn(0)
  flag=%f;
  if rhs<>2 then error(39), end
  if MSDOS then sep='\',else sep='/',end
  txt=mgetl(filein);
  d=grep(txt,"<LINK>");
  if d==[] then mputl(txt,fileout); return; end
  for k=d
    tt=txt(k);
    l1=strindex(tt,"<LINK>");
    l2=strindex(tt,"</LINK>");
    nlink=size(l1,"*")
    for i=1:nlink
      name=part(tt,[l1(1)+6:l2(1)-1])
      path=get_absolute_file_path(filein)+filein
      if length(name)<>0 then 
	l=getlink(name,path,filein)
      else
	l="unknown";
      end
      // jpc 17 sept 2003
      l=strsubst(l,"//","/")
      tt=part(tt,[1:l1(1)-1])+..
	 "<A href="""+l+"""><VERB>"+name+"</VERB></A>"+..
	 part(tt,[l2(1)+7:length(tt)])
      l1=strindex(tt,"<LINK>")
      l2=strindex(tt,"</LINK>")
    end
    txt(k)=tt
  end
  mputl(txt,fileout)
  flag = %t 
endfunction

function t=getlink(name,absolute_path,path)
  global %helps
  name=stripblanks(name)
  if MSDOS then sep='\',else sep='/',end
  man=[]
  for k=1:size(%helps,1)
    whatis=mgetl(%helps(k,1)+sep+'whatis.htm')
    f=grep(whatis,name)
    if f<>[] then
      for k1=f
	w=whatis(k1)
	i=strindex(w,">"); j=strindex(w,"</A>")
	if j<>[] then 
	  lname=part(w,i(2)+1:j-1)
	  lnames=getwords(lname) 
	  // transforms "toto titi tata" into ["toto" "titi" "tata"]
	  for ii=lnames
	    ok=%F
	    if ii==name then
	      i=strindex(w,"HREF="""); j=strindex(w,""">")
	      if part(%helps(k,1),length(%helps(k,1)))==sep then
		man=%helps(k,1)+part(w,[i+6:j-1])
	      else
		man=%helps(k,1)+sep+part(w,[i+6:j-1])
	      end
	    end
	    if man<>[] then break; end
	  end
	  if man<>[] then break; end
	end
      end
    end
    if man<>[] then break; end
  end
  if man==[] then
    write(%io(2),"Bad LINK """+name+""" in file "+path);
    t=[]
    return;
  end
  t=relative_path(man,absolute_path)
endfunction

function p=relative_path(path,relative)
  // path here are html path thus the 
  // correct sep is always '/' 
  path=getshortpathname(path);
  path=strsubst(path,'\','/');
  relative=getshortpathname(relative);
  relative=strsubst(relative,'\','/');
  cpath=str2code(path)
  crelative=str2code(relative)
  n=min(size(cpath,"*"),size(crelative,"*"))
  ncommon=find((cpath(1:n)==crelative(1:n))==%F)
  ncommon=ncommon(1)-1
  strcommon=part(path,[1:ncommon])
  k=strindex(strcommon,'/')
  ncommon=k($)
  ndir=size(strindex(part(relative,[ncommon+1:length(relative)]),'/'),"*")
  p=""
  for i=1:ndir
    p=p+"../"
  end
  p=p+part(path,[ncommon+1:length(path)])
endfunction

function vnames=getwords(names)
  v=strindex(names," ")
  if v==[] then
    vnames=[names]
  else
     vnames=[]; i=1
     for j=v
       vnames=[vnames,part(names,i:j-1)]
       i=j+1
     end
     vnames=[vnames,part(names,i:length(names))]
  end
endfunction
