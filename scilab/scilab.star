// Main Scilab initifalisation file 
// Copyright INRIA
mode(-1);  // silent execution mode

// clean database when restarted ======================================
predef('clear'); //unprotect all variables 
clear  // erase all variables 
clear  scicos_pal // explicitly clear %helps scicos_pal variables
clearglobal();

// Set stack size   ===================================================
newstacksize=1000000;
old=stacksize()
if old(1)<>newstacksize then stacksize(newstacksize),end
clear old newstacksize

// Startup message  ===================================================
verbose=sciargs()<>"-nb"
if verbose then 
  write(%io(2),[' ';' ';'Startup execution:'])
end

// Special variables definition =======================================
ieee(2);%inf=1/0;ieee(0);%nan=%inf-%inf;

%s=poly(0,'s');%z=poly(0,'z');
$=poly(0,'$')

%T=%t;%F=%f;       // boolean variables

// Load scilab functions libraries  ===================================
if verbose then 
  write(%io(2),'  loading initial environment')
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
load('SCI/macros/xdess/lib')

// Create some configuration variables ================================
TMPDIR=getenv('TMPDIR')
PWD = getcwd()
home= getenv('HOME','ndef');
if home=='ndef',home=unix_g('cd; pwd');end 
// use MSDOS syntax?
MSDOS = getenv('WIN32','NO')=='OK' & ..
	or(COMPILER==['VC++' 'ABSOFT' 'gcc'])

SCI=getenv('SCI')  // path of scilab main directory
if getenv('WIN32','NO')=='OK' then
  WSCI=getenv('WSCI')  // path of scilab main directory for Windows
end

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
//%helps is a two column matrix of strings
global %helps
%helps=initial_help_chapters(LANGUAGE)
clear initial_help_chapters

// Define Initial demo tables, ========================================
//demolist is a two column matrix of strings
global demolist
demolist=initial_demos_tables()
clear initial_demos_tables

// Menu for Help and editor ===========================================
if grep(args,'scilex')<>[] then
  if (args<>"-nw")&(args<>"-nwni")&(args<>"--texmacs") then
    delmenu("Help")
    if ~MSDOS then 
      addmenu("Help",["Help browser","Apropos","Configure"],list(2,"help_menu")),
    end
    if with_tk() then
      delmenu("Editor")
      if ~MSDOS then addmenu("Editor",list(2,"scipad")),end
    end
  end
end

// Protect variable previously defined  ================================
clear ans  %b_h_s args
predef('all') 

// Set the preferred browser  ==========================================
global %browsehelp
if with_tk()& ~with_gtk()
  %browsehelp="Scilab Browser";
elseif with_gtk()
  %browsehelp="help widget";
end
clear %browsehelp //remove the local variable

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
end
clear fd ierr

// load history file ==================================================
loadhistory()

// LCC initialization =================================================
global LCC
if MSDOS then
	LCC=%f;
else
	LCC=%f
end
// Graphic mode and Startup info ======================================
set old_style off
show_startupinfo()

// Scipad font size
// micro=10
// small=12
// medium=14
// large=18
global %scipad_fontsize
 %scipad_fontsize=12

// calling user initialization =========================================
// Home dir startup (if any)
[startup,ierr]=mopen('home/.scilab','r')
if ierr==0 then
  exec(startup,-1);mclose(startup)
else
  [startup,ierr]=mopen('home/scilab.ini','r')
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
clear startup ierr

