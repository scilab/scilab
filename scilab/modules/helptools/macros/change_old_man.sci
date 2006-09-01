function change_old_man()
  // Given an old fashion help chapter, this function translate it in an
  // HTML version located in TMPDIR/man<number>/ where <number> is the
  // index of the chapter in %help
  global %helps
  for k=1:size(%helps,1)
    flag1 = fileinfo(%helps(k,1)+"/whatis.htm");
    flag2 = fileinfo(%helps(k,1)+"/whatis");
    if flag1 == [] & flag2 <> []  then
      txt=mgetl(%helps(k,1)+"/whatis")
      whatispath=TMPDIR+"/man"+string(k);
      p=pathconvert(whatispath);
      if fileinfo(p)==[] then unix_s("mkdir "+p),end
     
      name=[],fil=[],def=[]
      for i=1:size(txt,1)
	p=strindex(txt(i)," - ")
	if p<>[] then
	name(i)=stripblanks(part(txt(i),1:p(1)))
	q=strindex(txt(i),'@')
	if q==[] then
	  fil(i)=name(i)
	  q=length(txt(i))+1
	else
	  q=q(1)
	  fil(i)=stripblanks(part(txt(i),q+1:length(txt(i))))
	end
	def(i)=part(txt(i),p(1)+3:q-1)
	
	head=["<html>";
	      "  <head><meta http-equiv=""Content-Type"" content="""+...
	      " text/html; charset=ISO-8859-1"">";
	      "  <title>"+name(i)+"</title>";
	      "<body>"];
	pin=pathconvert(%helps(k,1)+"/"+fil(i)+'.cat',%f,%f)
	pout=pathconvert(whatispath+'/'+fil(i)+'.htm',%f,%f)
	mputl([head;"<pre>";mgetl(pin);"</pre></html></body>"],pout)
	end
      end
      head=["<html>"
	    "<head>"
	    "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
	    "    <title>"+%helps(k,2)+"</title>"
	    "</head>"
	    "<body bgcolor=""FFFFFF"">"];
      pout=pathconvert(whatispath+'/whatis.htm',%f,%f)
      mputl([head;"<BR><A HREF="""+fil+".htm"">"+name+"</A> - "+def;
	     "</body></html>"],pout)
      %helps(k,1)=whatispath;
    end
  end
endfunction
