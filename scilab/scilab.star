// Main Scilab initialisation file
// Copyright INRIA
mode(-1);  // silent execution mode

// clean database when restarted
predef('clear'); //unprotect all variables 
clear  // erase all variables 
clear  scicos_pal // explicitly clear %helps scicos_pal variables
clearglobal();

// Set stack size
newstacksize=1000000;
old=stacksize()
if old(1)<>newstacksize then stacksize(newstacksize),end
clear old newstacksize

// Startup message
if sciargs()<>"-nb" then 
  verbose=%t;
else  
   verbose=%f;
end

if verbose then 
  t=[' '
     ' '
     'Startup execution:'];
  write(%io(2),t)
  clear t;
end

// Special variables definition
ieee(2);%inf=1/0;ieee(0);%nan=%inf-%inf;

%s=poly(0,'s');%z=poly(0,'z');
$=poly(0,'$')

%T=%t;%F=%f;       // boolean variables
SCI=getenv('SCI')  // path of scilab main directory
if getenv('WIN32','NO')=='OK' then
  WSCI=getenv('WSCI')  // path of scilab main directory for Windows
end

// Load scilab functions libraries
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
if or(sciargs()=="--texmacs") then load('SCI/macros/texmacs/lib'),end
load('SCI/macros/xdess/lib')

// Create a temporary directory
TMPDIR=getenv('TMPDIR')

PWD = getcwd()
home= getenv('HOME','ndef');
if home=='ndef',home=unix_g('cd; pwd');end 

// in inisci.f COMPILER=getenv('COMPILER','NO');
// 
// use MSDOS syntax?
MSDOS = getenv('WIN32','NO')=='OK' & ..
	or(COMPILER==['VC++' 'ABSOFT' 'gcc'])


// LANGUAGE TO USE FOR ONLINE MAN
global LANGUAGE
LANGUAGE="eng" // default language
args=sciargs(); larg=find(args=="-l")
if larg<>[] & larg<=size(args,"*") then
  L=args(larg+1)
  if L=="eng" | L=="fr" then
    LANGUAGE=L
  else
    write(%io(2)," ")
    write(%io(2),"Unsupported language """+L+"""."+..
   	         " Choosing default language """+LANGUAGE+""".")
  end
end
clear args larg L

//Scilab Help Chapters, %helps is a two column matrix of strings
global %helps
%helps=initial_help_chapters(LANGUAGE)
clear initial_help_chapters

// Define Initial demo tables, demolist is a two column matrix of strings
global demolist
demolist=initial_demos_tables()
clear initial_demos_tables

// Menu for Help and editor
if (sciargs()<>"-nw")&(sciargs()<>"-nwni")&(sciargs()<>"--texmacs") then
  delmenu("Help")
  addmenu("Help",list(2,"help"))
  if with_tk() then
    delmenu("Editor")
//retricted to Windows version because of problems on some unix platform	
    if MSDOS then  
      addmenu("Editor",list(2,"scipad")),
    end
  end
end


// Protect variable previously defined 
clear ans  %b_h_s
predef('all') 

// this variable is used to store the preferred browser 
global %browsehelp
if with_tk()
  %browsehelp="tcltk";//set scilab  browser by default. may be changed in
		      //the user startup file
end
if with_gtk()
  %browsehelp="help widget";//set scilab  browser by default. may be changed in
		      //the user startup file
end

clear %browsehelp //remove the local variable



// Define scicos palettes of blocks, scicos_menus and shortcuts
[scicos_pal,%scicos_menu,%scicos_short,%scicos_help,%scicos_display_mode]=initial_scicos_tables()

clear initial_scicos_tables


// load contrib menu if present 
//=================================

[fd,ierr]=mopen(SCI+'/contrib/loader.sce');
if ierr== 0 then;
	mclose(fd); 
	global %toolboxes
	global %toolboxes_dir
	exec(SCI+'/contrib/loader.sce');
end
clear fd ierr

// calling user initialization
//=============================
// Home dir
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

// working dir
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

