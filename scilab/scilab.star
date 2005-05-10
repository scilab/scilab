// Main Scilab initifalisation file 
// Copyright INRIA
mode(-1);  // silent execution mode

// clean database when restarted ======================================
predef('clear'); //unprotect all variables 
clear  // erase all variables 
clear  scicos_pal // explicitly clear %helps scicos_pal variables
clearglobal();

// Set stack size   ===================================================
newstacksize=5000000;
old=stacksize()
if old(1)<>newstacksize then stacksize(newstacksize),end
clear old newstacksize

// Special variables definition =======================================
ieee(2);%inf=1/0;ieee(0);%nan=%inf-%inf;

%s=poly(0,'s');%z=poly(0,'z');
$=poly(0,'$')

%T=%t;%F=%f;       // boolean variables

// Startup message  ===================================================
verbose=sciargs()<>"-nb"
if verbose then
  if ~fromjava() then
    banner(); 
    write(%io(2),[' ';' ';'Startup execution:'])
  end
end

// Load scilab functions libraries  ===================================
if verbose then 
  if ~fromjava() then
    write(%io(2),'  loading initial environment')
  end  
end 
clear verbose

load('SCI/macros/mtlb/lib')
load('SCI/macros/sci2for/lib')
load('SCI/macros/tdcs/lib')
load('SCI/macros/tksci/lib')
load('SCI/macros/arma/lib')
load('SCI/macros/metanet/lib')
load('SCI/macros/sound/lib')
load('SCI/macros/robust/lib')
load('SCI/macros/auto/lib')
load('SCI/macros/optim/lib')
load('SCI/macros/signal/lib')
load('SCI/macros/algebre/lib')
load('SCI/macros/statistics/lib')
load('SCI/macros/util/lib')
load('SCI/macros/elem/lib')
load('SCI/macros/int/lib')
load('SCI/macros/calpol/lib')
load('SCI/macros/percent/lib')
if with_texmacs() then load('SCI/macros/texmacs/lib'),end
clear with_texmacs
load('SCI/macros/xdess/lib')
load('SCI/macros/sparse/lib')
//load('SCI/macros/HighLevelPlotting/lib')

// Create some configuration variables ================================
// path of scilab main directory
SCI=getenv('SCI');
// use MSDOS syntax?
MSDOS = (getos() == "Windows")
TMPDIR=getenv('TMPDIR')
PWD = getcwd()
[home,SCIHOME]=sethomedirectory();
clear sethomedirectory ExistScilabHomeDirectory CreateScilabHomeDirectory
if MSDOS then
  SCI=strsubst(getshortpathname(SCI),'\','/');
  // path of scilab main directory for Windows
  WSCI=getlongpathname(pathconvert(SCI,%f,%f,'w'))
  if with_pvm() then
    setenv("PVM_ROOT",pathconvert(SCI,%f,%f,'w')+"\pvm3")
    setenv("PVM_ARCH","WIN32")
    setenv("PVM_TMP",getenv("TEMP"))
  end
  clear pathconvert
else
  if getenv('PRINTERS','ndef')=="ndef" then
    setenv("PRINTERS","lp")
  end
end
setenv("VERSION",getversion())
// Set LANGUAGE  ======================================================
// used mainly for on-line help
global LANGUAGE
LANGUAGE="eng"
args=sciargs(); larg=find(args=="-l")
if larg<>[] & larg<=size(args,"*") then
  L=args(larg+1)
  if L=="eng" | L=="fr" then
    LANGUAGE=L
  else
    write(%io(2),[" "
		  "Unsupported language """+L+"""."+..
		  "Choosing default language """+LANGUAGE+"""."])
  end
end
clear  larg L

//Scilab Help Chapters, ===============================================
global %helps
%helps=initial_help_chapters(LANGUAGE)
clear initial_help_chapters //%helps

// Define Initial demo tables, ========================================
//demolist is a two column matrix of strings
global demolist
demolist=initial_demos_tables()
clear initial_demos_tables demolist

// Scipad font size micro=10,small=12,medium=14,large=18
//Overridden by the preference file ES 15/11/04
//global %scipad_fontsize;%scipad_fontsize=12
//clear %scipad_fontsize //remove the local variable

// Set the preferred browser  ==========================================
global %browsehelp
%browsehelp=loaddefaultbrowser();
clear %browsehelp with_tk with_gtk loaddefaultbrowser //remove the local variable
      
// Menu for Help and editor ===========================================
if grep(args,'scilex')<>[] then
  if (args<>"-nw")&(args<>"-nwni")&(args<>"--texmacs") then
    delmenu("Help")
    if ~MSDOS then 
      addmenu("Help",["Help browser","Apropos","Configure"],list(2,"help_menu")),
    end
    if with_tk() then
	delmenu("Editor")
      if ~MSDOS then addmenu("Editor",list(2,"scipad();")),end
    end
  end
end
clear ans  %b_h_s args with_tk LANGUAGE

// LCC initialization =================================================
global LCC
if MSDOS then
  LCC=%f;
else
  LCC=%f
end
clear LCC 

// Protect variable previously defined  ================================
predef('all') 

// Set LANGUAGE for Scipad ============================================
global LANGUAGE
global %scipad_language
%scipad_language=LANGUAGE
clear %scipad_language
clear  LANGUAGE

// Graphic mode and Startup info ======================================
set old_style off
show_startupinfo();clear show_startupinfo


// Define Scicos data tables ===========================================
[scicos_pal,%scicos_menu,%scicos_short,%scicos_help,..
	%scicos_display_mode,modelica_libs,scicos_pal_libs]=initial_scicos_tables()
clear initial_scicos_tables


// load contrib menu if present ========================================
[fd,ierr]=mopen(SCI+'/contrib/loader.sce');
if ierr== 0 then;
  mclose(fd); 
  global %toolboxes
  global %toolboxes_dir
  exec(SCI+'/contrib/loader.sce');
  clear %toolboxes %toolboxes_dir
end
clear fd ierr

// load history file ==================================================
if MSDOS then
  loadhistory(SCIHOME+'\history.scilab')
else
  loadhistory(SCIHOME+'/.history.scilab')
end


// Configure Environment Variables for Ms Visual C ====================
if MSDOS then
  configure_msvc()
  clear configure_msvc setmsvc71 setmsvc70 setmsvc60 setmsvc50
end

// calling user initialization =========================================
// Home dir startup (if any)
if sciargs()<>'-nouserstartup' then
  if MSDOS then
    startup_path=SCIHOME+'\'
  else
    startup_path=SCIHOME+'/'
  end

  [startup,ierr]=mopen(startup_path+'.scilab','r')
  if ierr==0 then
    exec(startup,-1);mclose(startup)
  else
    [startup,ierr]=mopen(startup_path+'scilab.ini','r')
    if ierr==0 then
      exec(startup,-1);mclose(startup)
    end
  end
  clear startup ierr

  // working dir startup (if any)
  if  home<>PWD then
    [startup,ierr]=mopen('.scilab','r')
    if ierr==0 then
      exec(startup,-1);mclose(startup)
    else
      [startup,ierr]=mopen('scilab.ini','r')
      if ierr==0 then
	exec(startup,-1);mclose(startup)
      end
    end
  end
end
clear startup ierr

