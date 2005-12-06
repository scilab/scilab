function [scs_m,newparameters,needcompile,edited]=scicos_new(scs_m,menus)
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


//xset('window',curwin);
olds=get('old_style')
set("old_style","on")
//xset('default')

//check if superblock editing mode
[%ljunk,%mac]=where()
slevel=prod(size(find(%mac=='scicos_new')))
super_block=slevel>1
if ~super_block then
  // TO BE REMOVED LATER
  [scicos_pal,%scicos_menu,%scicos_short,%scicos_help,..
   %scicos_display_mode,modelica_libs,scicos_pal_libs]=initial_scicosnew_tables()
   //
  write(%io(2),['Warning:'
		'This is the development version'
		'of the new Scicos editor (scicos_new).'
		'The stable editor is scicos.'
		'The new editor has not been fully tested'
		'and validated; save your diagram regularly.' 
		'Diagrams edited with the two editors are'
		'compatible so you can switch at any time.'
		'Menus cannot currently be customized with'
		'the new editor.'
		'Send all bug reports and suggestions to ramine.nikoukhah@inria.fr'])
 
  
  
  // define scicos libraries
  if exists('scicos_pal')==0 | exists('%scicos_menu')==0 | exists('%scicos_short')==0 |..
	exists('%scicos_display_mode')==0| exists('scicos_pal_libs') ==0 then 
    [scicos_pal_0,%scicos_menu_0,%scicos_short_0,%scicos_help_0,..
     %scicos_display_mode_0,modelica_libs_0,scicos_pal_libs_0]=initial_scicos_tables()
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
    
    if exists('modelica_libs')==0 then
      x_message(['modelica_libs not defined';
		 'using default values']) 
      modelica_libs=modelica_libs_0
    end
    if exists('scicos_pal_libs')==0 then
      x_message(['scicos_pal_libs not defined';
		 'using default values']) 
      scicos_pal_libs=scicos_pal_libs_0
    end
  end
  
  if ~exists('%scicos_context') then
    %scicos_context=struct()
  end
  
  //intialize lhb menu
 
 %scicos_lhb_list=list()
 %scicos_lhb_list(1)=list('Open/Set',..
			  'Cut',..
			  'Copy',..
			  'Smart Move'  ,..
			  'Move'  ,..
			  'Duplicate',..
			  'Delete',..
			  'Link',..
			  'Align',..
			  'Replace',..
			  'Flip',..
			  list('Properties',..
			       'Resize',..
			       'Icon',..
			       'Icon Editor',..
			       'Color',..
			       'Label',..
			       'Get Info',..
			       'Identification',..
			       'Documentation'),..
			  'Code Generation',..
			  'Help')
 %scicos_lhb_list(2)=list('Undo','Paste','Palettes','Context','Add new block',..
			  'Duplicate Region','Delete Region','Region to Super Block',..
			  'Replot','Save','Save As',..
			  'Load','Export','Quit','Background color','Aspect',..
			  'Zoom in',  'Zoom out','Help')
 %scicos_lhb_list(3)=list('Copy','Duplicate','Duplicate Region','Help')
 
 
 // menus of type 1 (require %pt)
 CmenuTypeOneVector =.. 
     ['Region to Super Block',' Click, drag region and click (left to fix, right to cancel)';
      'Smart Move','Click object to move, drag and click (left to fix, right to cancel)';
     'Move','Click object to move, drag and click (left to fix, right to cancel)';
      'Duplicate','Click on the object to duplicate, drag, click (left to copy, right to cancel)';
      'Duplicate Region','Duplicate Region: Click, drag region, click (left to fix, right to cancel)'; 
      'Replace','Click on new object , click on object to be replaced'; 
      'Align','Click on an a port , click on a port of object to be moved'; 
      'Link','Click link origin, drag, click left for final or intermediate points or right to cancel'; 
      'Delete','Delete: Click on the object to delete'; 
      'Delete Region','Delete Region: Click, drag region and click (left to delete, right to cancel)'; 
      'Flip','Click on block to be flipped'; 
      'Open/Set','Click to open block or make a link'; 
      'MoveLink','';
      'SelectLink','';
      'CtrlSelect','';
      'SelectRegion','';
      'Popup','';
      'Label', 'Click block to label';
      'Get Info','Click on object  to get information on it';
      'Code Generation','Click on a Superblock (without activation output) t"+...
      "o obtain a coded block!';
      'Icon', 'Click on block to edit its icon';
      'Color', 'Click on object to paint';
      'Help', 'Click on object or menu to get help'
      'Identification','Click on an object to set or get identification';
      'Resize','Click block to resize';
      'Documentation','Click on a block to set or get it''s documentation'
     ]
 
 
 
 //

 if exists('scicos_newlib')==0 then 
   load('SCI/macros/scicos_new/lib'),
 end
 exec(loadpallibs,-1) //to load the palettes libraries
 //To be removed
 scicos_newlib.SUPER_f
 //To be removed later: it is important because SUPER_f is duplicated in scicos_new/lib

end



scicos_ver='scicos2.7.3' // set current version of scicos
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
  %zoom=1.4
  pal_mode=%f // Palette edition mode
//  newblocks=[] // table of added functions in pal_mode
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
  %cor_item_exec=[%cor_item_exec,%scicos_menu(%Y)(2:$)];
end
%cor_item_exec=%cor_item_exec';
%R = %cor_item_exec; 
%R= stripblanks(%R)+'_'
%R=strsubst(%R,'/','')
%R=strsubst(%R,' ','')
%R=strsubst(%R,'.','')
%R=strsubst(%R,'-','');
%cor_item_exec=[%cor_item_exec, %R];

//add fixed menu items not visible 
%cor_item_exec=[%cor_item_exec;
		'MoveLink','MoveLink_';
		'SelectLink','SelectLink_';
		'CtrlSelect','CtrlSelect_';
		'SelectRegion','SelectRegion_';
	       'Popup','Popup_'];


menus=tlist('xxx')
for %Y=1:length(%scicos_menu)
  menus(1)=[menus(1),%scicos_menu(%Y)(1)];
  menus($+1)=%scicos_menu(%Y)(2:$); 
end

for %Y=1:length(%scicos_menu)
  %R= %scicos_menu(%Y);
  %w='menus('''+%R(1)+''')('+ string(1:(size(%R,'*')-1)) + ')';
  execstr(%R(1)+ '=%w;');
end


//keyboard definiton
%tableau=emptystr([1:100]);
for %Y=1:size(%scicos_short,1)
%tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
end


//viewport
options=scs_m.props.options
//solver
%scicos_solver=scs_m.props.tol(6)

if ~super_block then
  xset('window',Main_Scicos_window);
  curwin=xget('window');
  palettes=list();
  noldwin=0
  windows=[1 curwin]
  pixmap=%scicos_display_mode
  //
  if ~exists('%scicos_gui_mode') then 
    if with_tk() then %scicos_gui_mode=1,else %scicos_gui_mode=0,end
  end
  //%scicos_gui_mode=0
  if %scicos_gui_mode==1 then
    getfile=tk_getfile;
    savefile=tk_savefile;
    if MSDOS then getvalue=tk_getvalue,end
    if MSDOS then mpopup=tk_mpopup, else mpopup=tk_mpopupX,end
    if MSDOS then choose=tk_choose; else
      deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
    end
    funcprot(0);getcolor=tk_getcolor;funcprot(1);
  else
    deff('x=getfile(varargin)','x=xgetfile(varargin(1:$))');
    savefile=getfile;
    deff('Cmenu=mpopup(x)','Cmenu=[]')
    deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
  end
//
else
  noldwin=size(windows,1)
  windows=[windows;slevel curwin]
  palettes=palettes;
end

//initialize graphics
//xdel(curwin)

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

pwindow_set_size()
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

  [%scicos_context,ierr]=script2var(scs_m.props.context,%scicos_context)
  //for backward compatibility for scifunc
  if ierr==0 then
    %mm=getfield(1,%scicos_context)
    for %mi=%mm(3:$)
      ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
      if ierr<>0 then
	break
      end
    end
  end
  //end of for backward compatibility for scifunc
  if ierr  <>0 then
    message(['Error occur when evaluating context:'
	     lasterror() ])
  else
    deff('%fonct()',scs_m.props.context)
    %outfun=macrovar(%fonct);
    //perform eval only if context contains functions which may give
    //different results from one execution to next
    if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') ...
    then
      disablemenus()
      [scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
      enablemenus()
    end
  end
  xset('window',%now_win)
else
  scs_m.props.context=' ' 
end
drawobjs(scs_m)

if pixmap then xset('wshow'),end


//
%pt = [];Cmenu = [];%win = curwin;// state machine variables 
Select=[];%ppt=[];Clipboard=[];hilite_image=list(), //state machine variables windowish behavior

while ( Cmenu <> 'Quit' ) 
  [%stack]=stacksize()
  if %stack(2)/%stack(1)>.3 then
    stacksize(2*%stack(1))
    disp('stacksize increased to '+string(2*%stack(1)))
  end
  if Select<>[] then
    if ~or(Select(1,2)==winsid()) then Select=[],end
  end
  
  [CmenuType,mess] = CmType(Cmenu);
  //** clear the %pt information for backward compatibility 
  if ( %pt <> [] & Cmenu==[] ) then %pt=[]; end 
  if (Cmenu<>[] & CmenuType==0) then %pt=[];end  
  // no argument needed
  // if (Cmenu<>[] & CmenuType==1 & %pt==[] & Select<>[]) then
  //   [%pt,%win]=get_selection(Select) //in case object selected
  // end
//disp(2),disp(Cmenu)
  xinfo(mess);
  if ( Cmenu==[] | (CmenuType == 1 & %pt==[]& Select==[]) ) then // need MORE information
    [btn_n,%pt_n,win_n,Cmenu_n] = cosclick()  
    if (Cmenu_n=='SelectLink'| Cmenu_n=='MoveLink') & Cmenu<>[] & CmenuType == 1 & %pt==[] then
      if %pt_n <> [] then %pt = %pt_n; end 
    else
      if Cmenu_n<>[] then Cmenu = Cmenu_n; end    
      if %pt_n <> [] then %pt = %pt_n; end
    end
    %win=win_n
  else   
//    disp(3),disp(Cmenu)
    %koko=find(Cmenu==%cor_item_exec(:,1));
    if size(%koko,'*')==1 then
      select_unhilite(hilite_image)
      execstr('exec('+%cor_item_exec(%koko,2)+',-1)')
      hilite_image=select_hilite2(Select)
    else
      Cmenu=[];%pt=[]
    end
  end 
end
do_exit()
if pixmap then xset('wshow'),end
set('old_style',stripblanks(olds))
endfunction

  
  

function [itype,mess] = CmType(Cmenu)

  k=find (Cmenu == CmenuTypeOneVector(:,1));
  if k==[] then itype=0;mess=''; return ; end
  if size(k,'*')>1 then 
    message('Warning '+string( size(k,'*'))+' menus have identical name '+Cmenu);
    k=k(1);
  end
  itype = 1
  mess=CmenuTypeOneVector(k,2)
endfunction 

function [x,k]=gunique(x)
    [x,k]=gsort(x);
    keq=find(x(2:$)==x(1:$-1))
    x(keq)=[]
    k(keq)=[]
endfunction

