function help(key)
// when %gtk is set we use the prefered 
// broswer given by %gtkhelp
// when %gtkhelp is [] a choose menu is given 
  global %gtkhelp;
  
  // set of possible modes for gtk
  gtk_modes=['help widget';'nautilus';'mozilla';'netscape';'opera'];
  if %tk then gtk_mode=[gtk_modes;'tcltk'];end 
    
  [lhs,rhs]=argn(0);
  
  if %gtkhelp<>[] then 
    help_mode = %gtkhelp;
  else
     %gtkhelp= gtk_help_ask(gtk_modes);
  end
  
  //for compatibility with toolboxes making use of old cat files
  change_old_man() 
  INDEX=make_index()

  if rhs==0 then
    browsehelp(INDEX,"index");
    return
  end 
  
  if rhs<>1 then error(39),return; end
  path=gethelpfile(key)
  if path<>[] then
    browsehelp(path,key)
  else
     apropos(key)
  end
endfunction

function path=gethelpfile(key)
  // copy of gethelpfile contained in help.sci
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

function browsehelp(path,key)
  global LANGUAGE INDEX 
  global %gtkhelp
  [lhs,rhs]=argn(0)
  if rhs==0 then
    path=INDEX
    key="index"
  end
  if or(sciargs()=="-nw") then
    // the no window case 
    if %gtk then 
      gtk_help(path,key);
    else 
       write(%io(2),mgetl(path))
    end
  else
     if %gtk then 
       gtk_help(path,key);
     elseif %tk then 
	tcltk_help(path,key);
     else
	error(999,'I cannot browse help files');
     end
  end
endfunction


function tcltk_help(path,key) 
  // the tck tk help browser 
  global LANGUAGE INDEX 
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
endfunction

function gtk_help(path,key) 
// the gtk help browser 
// gtk_modes=['help widget';'nautilus';'tcltk'];
  [lhs,rhs]=argn(0);
  global LANGUAGE INDEX 
  global %gtkhelp
  select %gtkhelp 
   case 'nautilus' then  unix_s("nautilus --no-desktop "+path+ '&'); 
   case 'help widget' 
    help_gtk(SCI+"/man/",LANGUAGE,path);
   case 'tcl/tl' then  tcltk_help(path,key); 
   case 'mozilla' then unix_s(%gtkhelp + " file://" +path+ '&'); 
   case 'netscape' then unix_s(%gtkhelp + " file://" +path+ '&'); 
   case 'opera' then unix_s(%gtkhelp + " file://" +path+ '&'); 
  else
     write(%io(2),mgetl(path))
  end
endfunction 

function path=make_index()
//we create a new index file each time to take into account dynamically
//loaded toolboxes
  global LANGUAGE
  path=TMPDIR+"/index.html";
  i_lines=["<html>";
	 "<head>";
	 "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">";
	 "    <title>Index</title>";
	 "</head>";
	 "<body bgcolor=""FFFFFF"">";
	 "<BR><A HREF="""+%helps(:,1)+"/whatis.html"">"+%helps(:,2)+"</A>";
	 "</body></html>"]
	];
  mputl(i_lines,path)
endfunction

function change_old_man()
  // Given an old fashion help chapter, this function translate it in an 
  // HTML version located in TMPDIR/man<number>/ where <number> is the
  // index of the chapter in %help
  global %helps
  for k=1:size(%helps,1)
    if fileinfo(%helps(k,1)+"/whatis")<>[] then
      txt=mgetl(%helps(k,1)+"/whatis")
      whatispath=TMPDIR+"/man"+string(k)

      if MSDOS then
	p=strsubst(whatispath,'/','\')
      else
	p=whatispath
      end
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
	mputl(["<html><pre>";mgetl(%helps(k,1)+"/"+fil(i)+'.cat');"</pre></html>"],whatispath+'/'+fil(i)+'.html')
	end
      end
      mputl("<BR><A HREF="""+fil+".html"">"+name+"</A> - "+def,whatispath+'/whatis.html')
      %helps(k,1)=whatispath
    end
  end
endfunction

function gtk_mode=gtk_help_ask(modes)
  n=x_choose(modes,['Choose the help browser';'you want to use']);
  if n<>0 then 
    gtk_mode=modes(n)
  else    
     gtk_mode=gtk_help_ask(modes)
  end
endfunction

     

