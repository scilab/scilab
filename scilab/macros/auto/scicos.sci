function [scs_m,newparameters,needcompile,edited]=scicos(scs_m,menus)
// scicos - block diagram graphic editor
//%SYNTAX
// scs_m=scicos(scs_m,job)
//%PARAMETERS
// scs_m    : scilab list, scicos main data structure
//      scs_m.props contains system name and other infos
//      scs_m.objs(i) contains description of ith block diagram element
// menus : vector of character strings,optional parameter giving usable menus 
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
//check if superblock editing mode
[%ljunk,%mac]=where()
slevel=prod(size(find(%mac=='scicos')))
super_block=slevel>1
if ~super_block then
  // define scicos libraries
  if exists('scicos_pal')==0 | exists('%scicos_menu')==0 | exists('%scicos_short')==0 |..
	exists('%scicos_display_mode')==0 then 
    [scicos_pal_0,%scicos_menu_0,%scicos_short_0,%scicos_help_0,%scicos_display_mode_0]= ...
	initial_scicos_tables()
    if exists('scicos_pal')==0 then
      x_message(['scicos_pal not defined';
		 'using default values'])
      scicos_pal=scicos_pal_0
    end
    if exists('%scicos_menu')==0 then
      x_message(['%scicos_menu not defined';
		 'using default values'])
      %scicos_menu=%scicos_menu_0
    end
    if exists('%scicos_short')==0 then
      x_message(['%scicos_short not defined';
		 'using default values'])
      %scicos_short=%scicos_short_0
    end
    if exists('%scicos_help')==0 then
      x_message(['%scicos_help not defined';
		 'using default values'])
      %scicos_help=%scicos_help_0
    end
    if exists('%scicos_display_mode')==0 then
      x_message(['%scicos_display_mode not defined';
		 'using default values'])
      %scicos_display_mode=%scicos_display_mode_0
    end
  end
  if exists('scicoslib')==0 then load('SCI/macros/scicos/lib'),end
  if exists('blockslib')==0 then load('SCI/macros/scicos_blocks/lib'),end
end



scicos_ver='scicos2.7' // set current version of scicos
Main_Scicos_window=1000

//Initialisation
newparameters=list()
enable_undo=%f
edited=%f
needreplay=%f
%path='./'
%exp_dir=PWD
global %tableau  

if ~super_block then // global variables
  %zoom=1.8
  pal_mode=%f // Palette edition mode
  newblocks=[] // table of added functions in pal_mode
  super_path=[] // path to the currently opened superblock

  scicos_paltmp=scicos_pal;
  if execstr('load(''.scicos_pal'')','errcatch')==0 then
    scicos_pal=[scicos_paltmp;scicos_pal]
    [%junk,%palce]=gunique(scicos_pal(:,2))
    %palce=-sort(-%palce);
    scicos_pal=scicos_pal(%palce,:);
  end

  execstr('load(''.scicos_short'')','errcatch')  //keyboard shortcuts
end
//
if rhs>=1 then
  if type(scs_m)==10 then //diagram is given by its filename
    %fil=scs_m
    alreadyran=%f
    [ok,scs_m,%cpr,edited]=do_load(%fil,'diagram')
    if ~ok then return,end
    if size(%cpr)==0 then
      needcompile=4
      %state0=list()
    else
      %state0=%cpr.state;
      needcompile=0
    end
  else //diagram is given by its data structure
    if ~super_block then 
      %cpr=list();needcompile=4;alreadyran=%f,%state0=list()
    end
  end
else
  xset('window',Main_Scicos_window);
  scs_m=scicos_diagram()
  %cpr=list();needcompile=4;alreadyran=%f;%state0=list()
end
//
if typeof(scs_m)<>'diagram' then error('first argument must be a scicos diagram'),end



%cor_item_exec=[];
for %Y=1:length(%scicos_menu)
  for %R=2:size(%scicos_menu(%Y),2)
    %kaka=%scicos_menu(%Y)(%R)
    %koko=stripblanks(%kaka)+'_'
    %koko=strsubst(%koko,'/','')
    %koko=strsubst(%koko,' ','')
    %koko=strsubst(%koko,'.','')
    %koko=strsubst(%koko,'-','')
    %cor_item_exec=[%cor_item_exec;[%kaka,%koko]];
  end
end

      


menus=tlist('xxx')
for %Y=1:length(%scicos_menu)
  menus(1)=[menus(1),%scicos_menu(%Y)(1)];
  execstr(%scicos_menu(%Y)(1)+'_t=[]');
  for %R=2:size(%scicos_menu(%Y),2)
    execstr(%scicos_menu(%Y)(1)+'_t=['+%scicos_menu(%Y)(1)+'_t '''+%scicos_menu(%Y)(%R)+''']')
end
  menus($+1)=evstr(%scicos_menu(%Y)(1)+'_t');
end

%rpar=')'
for %Y=1:length(%scicos_menu)
%w='menus('''+%scicos_menu(%Y)(1)+''')(';
menu_e=%scicos_menu(%Y)(1)+'_t'
execstr(%scicos_menu(%Y)(1)+'=%w(ones('+%scicos_menu(%Y)(1)+'_t))+string(1:size('+%scicos_menu(%Y)(1)+'_t,''*''))+%rpar(ones('+%scicos_menu(%Y)(1)+'_t))')
end


//keyboard definiton
%tableau=emptystr([1:100]);
for %Y=1:size(%scicos_short,1)
%tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
end


//viewport
options=scs_m.props.options

if ~super_block then
  xset('window',Main_Scicos_window);
  curwin=xget('window');
  palettes=list();
  noldwin=0
  windows=[1 curwin]
  pixmap=%scicos_display_mode
//
else
  noldwin=size(windows,1)
  windows=[windows;slevel curwin]
  palettes=palettes;
end

//initialize graphics
//xdel(curwin)
xset('window',curwin);
set("figure_style","old")
xset('default')
xbasc();

if pixmap then xset('pixmap',1); end
xset('pattern',1)
xset('dashes',1)
if ~set_cmap(options('Cmap')) then // add colors if required
  options('3D')(1)=%f //disable 3D block shape
end
if pixmap then xset('wwpc');end
xbasc();xselect()
%dr=driver();driver('Rec');
set_background()

rect=dig_bound(scs_m);

if rect<>[] then 
  %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
else
  %wsiz=[600/%zoom,400/%zoom]
end
// 1.3 to correct for X version
xset('wpdim',min(1000,%zoom*%wsiz(1)),min(800,%zoom*%wsiz(2)))

window_set_size()

xset('alufunction',6)

for %Y=1:length(%scicos_menu)
execstr(%scicos_menu(%Y)(1)+'_'+string(curwin)+'='+%scicos_menu(%Y)(1))
end


menu_stuff()

if ~super_block then
  delmenu(curwin,'stop')
  addmenu(curwin,'stop',list(1,'haltscicos'))
  unsetmenu(curwin,'stop')
else
  unsetmenu(curwin,'Simulate')
end

//set context (variable definition...)
if type(scs_m.props.context)==10 then
  %now_win=xget('window')
  if execstr(scs_m.props.context,'errcatch') <>0 then
    message(['Error occur when evaluating context:'
	     lasterror() ])
  end
  xset('window',%now_win)
else
  scs_m.props.context=' ' 
end
drawobjs(scs_m)

if pixmap then xset('wshow'),end
%pt=[];%win=0;
Cmenu='Open/Set'
while %t
  while %t do
    [%stack]=stacksize()
    if %stack(2)/%stack(1)>.3 then
      stacksize(2*%stack(1))
      disp('stacksize increased to '+string(2*%stack(1)))
    end
    if Cmenu==[]&%pt==[] then
      [btn,%xc,%yc,%win_1,Cmenu]=cosclick()
      if Cmenu<> [] then 
	%pt=[];
	break
      elseif btn>31 then
	%pt=[%xc;%yc];%win=%win_1;Cmenu=%tableau(min(100,btn-31));
	if Cmenu==emptystr() then Cmenu=[];%pt=[];end
	break
      end
    else
      break
    end
  end
  
  if Cmenu=='Quit' then do_exit();break;end
  
  %koko=find(Cmenu==%cor_item_exec(:,1));
  if size(%koko,'*')==1 then
    execstr('exec('+%cor_item_exec(%koko,2)+',-1)')
  else
    Cmenu=[];%pt=[]
  end
if pixmap then xset('wshow'),end
end

  
endfunction
function [x,k]=gunique(x)
    [x,k]=gsort(x);
    keq=find(x(2:$)==x(1:$-1))
    x(keq)=[]
    k(keq)=[]
endfunction
