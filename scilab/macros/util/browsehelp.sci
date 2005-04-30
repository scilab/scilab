function browsehelp(path,key)
// when %browsehelp is [] a  menu proposes to select a browser
  global LANGUAGE INDEX %helps
  global %browsehelp
  
  browsehelp_configure()
  // set of possible browsers
  
  if argn(2)==0 then
    path=INDEX
    key="index"
  end
  if or(sciargs()=="-nw") then // the no window case
    if  with_gtk ()| ..
	  (with_tk()& or(%browsehelp==['Scilab Browser','Old Scilab Browser'])) | ..
	  or(%browsehelp==['nautilus','mozilla/netscape (gnome-moz-remote)','opera','quanta (kde)']) then
      run_help(path,key);
    else
      write(%io(2),mgetl(path))
    end
  else
    run_help(path,key)
  end
endfunction

function  browsehelp_configure(job)
  global %browsehelp
  if argn(2)<1 then job='check',end
  if ~MSDOS then
    if job=='set' then oldbrowsehelp=%browsehelp;%browsehelp=[],end
    browse_modes=['nautilus';
		  'mozilla/netscape (gnome-moz-remote)';
		  'opera'
		  'quanta (kde)'];
    if with_tk() then 
      browse_modes=[browse_modes;
		    'Scilab Browser'
		    'Old Scilab Browser'];
    end
    
    if with_gtk() then 
      browse_modes=['help widget';browse_modes];
    end
    
    if %browsehelp<>[] then //help mode already selected
      if and(browse_modes<>%browsehelp) then
	      warning('Unhandled  help browser '+%browsehelp)
	      %browsehelp=oldbrowsehelp; // If user select cancel
	      %browsehelp= help_ask(browse_modes);
      end
    else // ask for an help mode
	    %browsehelp=oldbrowsehelp; // If user select cancel
	    %browsehelp= help_ask(browse_modes);
    end
  else //for windows 
    if job=='set' then oldbrowsehelp=%browsehelp;%browsehelp=[],end
    browse_modes=['Default Windows Browser';];
    if with_tk() then 
      browse_modes=[browse_modes;
		    'Scilab Browser'
		    'Old Scilab Browser'];
    end
    
    if %browsehelp<>[] then //help mode already selected
      if and(browse_modes<>%browsehelp) then
	      warning('Unhandled  help browser '+%browsehelp)
	      %browsehelp=oldbrowsehelp; // If user select cancel
	      %browsehelp= help_ask(browse_modes);
      end
    else // ask for an help mode
	    %browsehelp=oldbrowsehelp; // If user select cancel
	    %browsehelp= help_ask(browse_modes);
    end
    
  
    //%browsehelp = 'tcltk'
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
  case 'Default Windows Browser' then
    winopen(path);
  case 'Scilab Browser' then 
    sciGUIhelp(key);
  case 'Old Scilab Browser' then 
    if MSDOS then
      tcltk_help(path,key);
    else
      unix(SCI+'/tcl/browsehelpexe '+path+' '+INDEX+' '+LANGUAGE+ '&');
    end
  else
     write(%io(2),mgetl(path))
  end
endfunction

function md=help_ask(modes)
  global %browsehelp
  n=0
  n=x_choose(modes,['Choose the help browser';'you want to use']);
  if n==0 then // if user cancels then old browser is kept
    md=%browsehelp;
  else
    md=modes(n)
  end
  savedefaultbrowser(md);    
endfunction

function tcltk_help(path,key,key1)
  // the tck tk help browser
  global LANGUAGE INDEX
  // We must have / in paths, even for Windows
  path=strsubst(path,"\","/")
  INDEX=strsubst(INDEX,"\","/")
  if argn(2)<3 then key1=key,end //for temp file and widget name
  K=[' ','(',')','[',']','{','}','""','/','\','.','<','>']
  for k=K,key1=strsubst(key1,k,'_'),end
  if MSDOS then
    TCL_EvalStr('set isbrowsehelpinterp [interp exists browsehelp]');
    if TCL_GetVar("isbrowsehelpinterp")=='0' then    
      TCL_EvalStr("interp create browsehelp")
      TCL_EvalStr("load {"+SCI+"/bin/tk84.dll} Tk browsehelp")
      TCL_EvalStr("browsehelp eval {wm withdraw .}")
    end
    TCL_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
    TCL_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
    TCL_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
    TCL_EvalStr("browsehelp eval {set sciw .scihelp-"+key1+"}")
    TCL_EvalStr("browsehelp eval {set manpath """+path+"""}")
    TCL_EvalStr("browsehelp eval {source ""'+SCI+'/tcl/browsehelp.tcl""}")
  else
     TCL_SetVar("lang",LANGUAGE)
     TCL_SetVar("Home",INDEX)
     TCL_SetVar("sciw",".scihelp-"+key1+"}")
     TCL_SetVar("manpath",path)
     TCL_EvalFile(SCI+"/tcl/browsehelp.tcl")
  end
endfunction

function tcltk_apropos(path)
// calling the tck tk help browser
// for apropos
  global LANGUAGE INDEX
  // We must have / in paths, even for Windows
  path=strsubst(path,"\","/")
  INDEX=strsubst(INDEX,"\","/")
  if MSDOS then
    TCL_EvalStr("browsehelp eval {set lang "+LANGUAGE+"}")
    TCL_EvalStr("browsehelp eval {set SciPath """+SCI+"""}")
    TCL_EvalStr("browsehelp eval {set Home """+INDEX+"""}")
    TCL_EvalStr("browsehelp eval {set sciw .sciapropos}")
    TCL_EvalStr("browsehelp eval {set manpath """+path+"""}")
    TCL_EvalStr("browsehelp eval {source $SciPath/tcl/browsehelp.tcl}")
  else
     TCL_SetVar("lang",LANGUAGE)
     TCL_SetVar("Home",INDEX)
     TCL_SetVar("sciw",".sciapropos")
     TCL_SetVar("manpath",path)
     TCL_EvalFile(SCI+"/tcl/browsehelp.tcl")
  end
endfunction

