function help(key,flag)
  //for compatibility with toolboxes making use of old cat files
  change_old_man()
  INDEX=make_index()
  [lhs,rhs]=argn(0);
   
  if rhs==0 then
    browsehelp(INDEX,"index");
    return
  elseif type(key)==1 then //called by the menu
    browsehelp(INDEX,"index");
    return
  end
 
  if rhs> 2 then error(39),return; end
  key=stripblanks(key)
  if or(part(key,1)==['(',')','[',']','{','}','%','''','""',':','*','/', ...
		      '\','.','<','>','&','^','|','~']) then
    key="symbols";
  end

  if rhs == 2 then
    help_apropos(key)
    return
  end
 
  path=gethelpfile(key)
  if path<>[] then
    browsehelp(path,key)
  else
     help_apropos(key)
  end
endfunction
function help_menu(i)
  browsehelp(INDEX,"index");
endfunction

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

function browsehelp(path,key)
// when %browsehelp is [] a  menu proposes to select a browser
  global LANGUAGE INDEX
  global %browsehelp
  
  [lhs,rhs]=argn(0);
  
  // set of possible browsers
  
  if ~MSDOS then
    browse_modes=['nautilus';
		  'mozilla/netscape (gnome-moz-remote)';
		  'opera'
		  'quanta (kde)'];
    if with_tk() then browse_modes=[browse_modes;'tcltk'];end
    if with_gtk() then browse_modes=['help widget';browse_modes];end
    
    
    if %browsehelp<>[] then //help mode already selected
      if and(browse_modes<>%browsehelp) then
	warning('Unhandled  help browser '+%browsehelp)
	%browsehelp= help_ask(browse_modes);
      end
      help_mode = %browsehelp;
    else // ask for an help mode
	%browsehelp= help_ask(browse_modes);
    end
  else //for windows 
    //tcltk forced because it is still not possible to start another
    //thread using unix
    browse_modes='tcltk'
    help_mode = 'tcltk'
  end
 
  if argn(2)==0 then
    path=INDEX
    key="index"
  end
  if or(sciargs()=="-nw") then // the no window case
    if  with_gtk () then
      run_help(path,key);
    else
       write(%io(2),mgetl(path))
    end
  else
    run_help(path,key)
  end
endfunction


function tcltk_help(path,key)
  // the tck tk help browser
  global LANGUAGE INDEX
  // We must have / in paths, even for Windows
  path=strsubst(path,"\","/")
  INDEX=strsubst(INDEX,"\","/")
  key1=key //for temp file and widget name
  K=[' ','(',')','[',']','{','}','""','/','\','.','<','>']
  for k=K,key1=strsubst(key1,k,'_'),end
  if MSDOS then
    TK_EvalStr('set isbrowsehelpinterp [interp exists browsehelp]');
    if TK_GetVar("isbrowsehelpinterp")=='0' then    
      TK_EvalStr("interp create browsehelp")
      TK_EvalStr("load {"+SCI+"/bin/tk83.dll} Tk browsehelp")
      TK_EvalStr("browsehelp eval {wm withdraw .}")
    end
    TK_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
    TK_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
    TK_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
    TK_EvalStr("browsehelp eval {set sciw .scihelp-"+key1+"}")
    TK_EvalStr("browsehelp eval {set manpath """+path+"""}")
    TK_EvalStr("browsehelp eval {source ""'+SCI+'/tcl/browsehelp.tcl""}")
  else
     TK_SetVar("lang",LANGUAGE)
     TK_SetVar("Home",INDEX)
     TK_SetVar("sciw",".scihelp-"+key1+"}")
     TK_SetVar("manpath",path)
     TK_EvalFile(SCI+"/tcl/browsehelp.tcl")
  end
endfunction

function run_help(path,key)
// the  help browser
// browse_modes=['help widget';'nautilus';'tcltk'];
  [lhs,rhs]=argn(0);
  global LANGUAGE INDEX
  global %browsehelp
  key1=strsubst(key,' ','_') //for temp file and widget name

  select %browsehelp
   case 'help widget' then
    help_gtk(SCI+"/man/",LANGUAGE,path);
   case 'nautilus' then 
    unix_s("nautilus --no-desktop "+path+ '&');
   case 'mozilla/netscape (gnome-moz-remote)' then
    unix_s("gnome-moz-remote --raise  file://"+path+ '&');
   case 'opera' then
    unix_s(%browsehelp + " file://" +path+ '&');
   case 'quanta' then
    unix_s(%browsehelp + " --unique file://" +path+ '&');
   case 'tcltk' then 
   if MSDOS then
     tcltk_help(path,key);
   else
     unix(SCI+'/tcl/browsehelpexe ' +path+ '&');
   end
  else
     write(%io(2),mgetl(path))
  end
endfunction

function path=make_index()
//we create a new index file each time to take into account dynamically
//loaded toolboxes
  global LANGUAGE
  path=TMPDIR+"/index.htm";
  i_lines=["<html>";
	 "<head>";
	 "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">";
	 "    <title>Index</title>";
	 "</head>";
	 "<body bgcolor=""FFFFFF"">";
	 "<BR><A HREF="""+%helps(:,1)+"/whatis.htm"">"+%helps(:,2)+"</A>";
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
	mputl([head;"<pre>";mgetl(%helps(k,1)+"/"+fil(i)+'.cat');"</pre></html></body>"],...
	      whatispath+'/'+fil(i)+'.htm')
	end
      end
      head=["<html>"
	    "<head>"
	    "  <meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
	    "    <title>"+%helps(k,2)+"</title>"
	    "</head>"
	    "<body bgcolor=""FFFFFF"">"];
      mputl([head;"<BR><A HREF="""+fil+".htm"">"+name+"</A> - "+def;
	     "</body></html>"],whatispath+'/whatis.htm')
      %helps(k,1)=whatispath;
    end
  end
endfunction

function md=help_ask(modes)
  n=0
  while n==0 then
    n=x_choose(modes,['Choose the help browser';'you want to use']);
  end
  md=modes(n)
endfunction


function help_apropos(key)
  global %helps LANGUAGE INDEX
  global %browsehelp;
  [lhs,rhs]=argn(0);
  // list relevant man for key
  provpath =apropos_gener(key);
  browsehelp(provpath,key);
endfunction

function tcltk_apropos(path)
// calling the tck tk help browser
// for apropos
  global LANGUAGE INDEX
  // We must have / in paths, even for Windows
  path=strsubst(path,"\","/")
  INDEX=strsubst(INDEX,"\","/")
  if MSDOS then
    TK_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
    TK_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
    TK_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
    TK_EvalStr("browsehelp eval {set sciw .sciapropos}")
    TK_EvalStr("browsehelp eval {set manpath """+path+"""}")
    TK_EvalStr("browsehelp eval {source $SciPath/tcl/browsehelp.tcl}")
  else
     TK_SetVar("lang",LANGUAGE)
     TK_SetVar("Home",INDEX)
     TK_SetVar("sciw",".sciapropos")
     TK_SetVar("manpath",path)
     TK_EvalFile(SCI+"/tcl/browsehelp.tcl")
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
      f=grep(whatis,key);
      if whatis<>[] then
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
