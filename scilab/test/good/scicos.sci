function [scs_m,newparameters,needcompile,edited] = scicos(scs_m,menus)
//** 06 Feb 2006 : restart of the mods job :
//**
//** 22 May 2006 : New restart
//**
//** 19 Jun 2006 : Last restart
//**
//** 10 Jul 2006 : looking for a residual oldgraphics instruction
//**
//** Comments & mods by Simone Mannori
//**
//**----------------------------------------------------------------------------------------
// scicos - block diagram graphic editor
// %SYNTAX
// scs_m=scicos(scs_m,job)
// %PARAMETERS
// scs_m    : scilab list, scicos main data structure
//      scs_m.props contains system name and other infos
//      scs_m.objs(i) contains description of ith block diagram element
// menus : vector of character strings,optional parameter giving usable menus
// !
//--------------------------------------------------------------------------------------------
// Copyright INRIA

//** 20 dec 2005: Code modified by Simone Mannori

	noguimode=find(sciargs()=="-nogui");
	if (noguimode <>[]) then
	 clear noguimode
	 warning(" Scilab in no gui mode : Scicos unavailable");
	 abort;
	end;
	clear noguimode

    [lhs,rhs] = argn(0) ;

//** ----------------------------- Check the recurring calling level of scicos_new -------------------------------

  //check if superblock editing mode
  [%ljunk,%mac] = where()
  slevel = prod ( size ( find ( %mac=='scicos') ) )
  super_block = slevel > 1


if ~super_block then
  // TO BE REMOVED LATER
  [scicos_pal,%scicos_menu,%scicos_short,%scicos_help, %scicos_display_mode,modelica_libs,scicos_pal_libs] = initial_scicosnew_tables() ;
   //

  // define scicos libraries
  if exists('scicos_pal')==0 | exists('%scicos_menu')==0 | exists('%scicos_short')==0 | exists('%scicos_display_mode')==0| exists('scicos_pal_libs')==0 then
    [scicos_pal_0,%scicos_menu_0,%scicos_short_0,%scicos_help_0, %scicos_display_mode_0,modelica_libs_0,scicos_pal_libs_0]=initial_scicos_tables()

    if exists('scicos_pal')==0 then
      x_message(['scicos_pal not defined'; 'using default values'])
      scicos_pal=scicos_pal_0
    end

    if exists('%scicos_menu')==0 then
      x_message(['%scicos_menu not defined'; 'using default values'])
      %scicos_menu=%scicos_menu_0
    end

    if exists('%scicos_short')==0 then
      x_message(['%scicos_short not defined'; 'using default values'])
      %scicos_short=%scicos_short_0
    end

    if exists('%scicos_help')==0 then
      x_message(['%scicos_help not defined'; 'using default values'])
      %scicos_help=%scicos_help_0
    end

    if exists('%scicos_display_mode')==0 then
      x_message(['%scicos_display_mode not defined'; 'using default values'])
      %scicos_display_mode=%scicos_display_mode_0
    end

    if exists('modelica_libs')==0 then
      x_message(['modelica_libs not defined'; 'using default values'])
      modelica_libs=modelica_libs_0
    end

    if exists('scicos_pal_libs')==0 then
      x_message(['scicos_pal_libs not defined'; 'using default values'])
      scicos_pal_libs=scicos_pal_libs_0
    end
  end
//**-------------------------------------------------------------------------------------------------------------------------------------------------------

  if ~exists('%scicos_context') then
    %scicos_context=struct()
  end

//**--------------------------------------------------------------------------------------------------------------------------------------------------------

  //intialize lhb menu

 %scicos_lhb_list = list()
 %scicos_lhb_list(1) = list('Open/Set',..
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
			  'Help');

 %scicos_lhb_list(2)=list('Undo','Paste','Palettes','Context','Add new block',..
			  'Duplicate Region','Delete Region','Region to Super Block',..
			  'Replot','Save','Save As',..
			  'Load','Export','Quit','Background color','Aspect',..
			  'Zoom in',  'Zoom out','Help');

 %scicos_lhb_list(3)=list('Copy','Duplicate','Duplicate Region','Help');


 // menus of type 1 (require %pt)
 CmenuTypeOneVector =..
     ['Region to Super Block',' Click, drag region and click (left to fix, right to cancel)';
      'Smart Move','Click object to move, drag and click (left to fix, right to cancel)';
      'Move','Click object to move, drag and click (left to fix, right to cancel)';
      'Duplicate','Click on the object to duplicate, drag, click (left to copy, right to cancel)';
      'Duplicate Region','Duplicate Region: Click, drag region, click (left to fix, right to cancel)';
      'Replace','Click on new object , click on object to be replaced';
      'Align','Click on an a port , click on a port of object to be moved';
      'Link','Click link origi-n, drag, click left for final or intermediate points or right to cancel';
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
      "Code Generation','Click on a Superblock (without activation output) t"+...
      "o obtain a coded block!";
      'Icon', 'Click on block to edit its icon';
      'Color', 'Click on object to paint';
      'Help', 'Click on object or menu to get help'
      'Identification','Click on an object to set or get identification';
      'Resize','Click block to resize';
      'Documentation','Click on a block to set or get it''s documentation'
     ]
  //

 if exists('scicoslib')==0 then
   load('SCI/modules/scicos/macros/scicos/lib') ; //** load all the libraries relative to the palettes
 end

 exec(loadpallibs,-1) // to load the palettes libraries

 end
//** ------------------------------------ End the NOT-Superbloc initialization and check ----------------------------------------


scicos_ver = 'scicos4' // set current version of scicos

Main_Scicos_window = 1000 ; //** set default value of the main scicos window

//Initialisation
newparameters = list() ;
enable_undo = %f
edited = %f
needreplay = %f
%path='./'
%exp_dir = PWD

global %tableau

if ~super_block then // global variables
  %zoom = 1.4
  pal_mode=%f // Palette edition mode
//  newblocks=[] // table of added functions in pal_mode
  super_path=[] // path to the currently opened superblock

  scicos_paltmp=scicos_pal ;

  if execstr('load(''.scicos_pal'')','errcatch')==0 then
    scicos_pal=[scicos_paltmp;scicos_pal]
    [%junk,%palce]=gunique(scicos_pal(:,2))
    %palce=-sort(-%palce);
    scicos_pal=scicos_pal(%palce,:);
  end

  execstr('load(''.scicos_short'')','errcatch')  // keyboard shortcuts

end
//


//** --- Temp assignements to move to the new graphics
global gh_Main_Scicos_window; //**

if rhs>=1 then //** scicos_new(...) is called with some arguments

  if type(scs_m)==10 then //diagram is given by its filename
    %fil=scs_m
    alreadyran=%f
    [ok,scs_m,%cpr,edited] = do_load(%fil,'diagram')
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
      %cpr=list(); needcompile=4 ; alreadyran=%f , %state0=list()
    end

  end

else //** scicos_new() is called without arguments (AND - implicitly - is NOT a superblock)
//**----------- Normal : not a superblock -----------------
  gh_Main_Scicos_window = scf(Main_Scicos_window); //** new way to open a brand new graphics windows

  scs_m = scicos_diagram() ;

  %cpr = list(); needcompile = 4; alreadyran = %f; %state0 = list() ;

end

//

if typeof(scs_m)<>'diagram' then error('first argument must be a scicos diagram'),end

%cor_item_exec=[];

for %Y=1 : length(%scicos_menu)
  %cor_item_exec=[%cor_item_exec,%scicos_menu(%Y)(2:$)];
end

%cor_item_exec = %cor_item_exec';
%R = %cor_item_exec;
%R = stripblanks(%R)+'_'
%R = strsubst(%R,'/','')
%R = strsubst(%R,' ','')
%R = strsubst(%R,'.','')
%R = strsubst(%R,'-','');
%cor_item_exec = [%cor_item_exec, %R];

//add fixed menu items not visible
%cor_item_exec=[%cor_item_exec;
		'MoveLink','MoveLink_';
		'SelectLink','SelectLink_';
		'CtrlSelect','CtrlSelect_';
		'SelectRegion','SelectRegion_';
	       'Popup','Popup_'];

menus = tlist('xxx')

for %Y=1:length(%scicos_menu)
  menus(1)=[menus(1),%scicos_menu(%Y)(1)];
  menus($+1)=%scicos_menu(%Y)(2:$);
end

for %Y=1:length(%scicos_menu)
  %R= %scicos_menu(%Y);
  %w='menus('''+%R(1)+''')('+ string(1:(size(%R,'*')-1)) + ')';
  execstr(%R(1)+ '=%w;');
end

//----------------------------------------------------------------
// single key shortcut: keyboard definition
%tableau = emptystr([1:100]);
for %Y = 1 : size(%scicos_short,1)
  %tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
end
//----------------------------------------------------------------

// viewport
options = scs_m.props.options

// solver
%scicos_solver = scs_m.props.tol(6)

//**-------------------------- I'm NOT inside a superblock  -----------------------------------------------
if ~super_block then

  gh_current_window = scf(Main_Scicos_window);
  curwin = get ( gh_current_window, "figure_id") ;

  palettes = list();
  noldwin = 0
  windows = [1 curwin]
  pixmap = %scicos_display_mode
  //

  if ~exists('%scicos_gui_mode') then
    if with_tk() then %scicos_gui_mode=1, else %scicos_gui_mode=0, end
  end
  //%scicos_gui_mode=0

  if %scicos_gui_mode==1 then

    getfile=tk_getfile;

    savefile=tk_savefile;

     if MSDOS then
         getvalue = tk_getvalue
     end

     if MSDOS then
         mpopup = tk_mpopup
      else
         mpopup = tk_mpopupX
     end

     if MSDOS then choose = tk_choose; else
       deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
     end

     funcprot(0);
     getcolor = tk_getcolor;
     funcprot(1);
  else
    deff('x=getfile(varargin)','x=xgetfile(varargin(1:$))');
    savefile = getfile;
    deff('Cmenu=mpopup(x)','Cmenu=[]')
    deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
  end
//
else //** super blok case
  noldwin = size(windows,1)
  windows = [windows;slevel curwin]
  palettes = palettes;
end


//** --------------------------------------- GRAPHICS INITIALIZATION -----------------------------------
//   initialize graphics

clf(); //** clear current graphic window and delete all his children

//** this handle acquisition works for both MainScicosWindow and SuperBlock window
gh_current_window = gcf() ; //** get the current graphics window

if ~set_cmap(options('Cmap')) then // add colors if required
  options('3D')(1) = %f //disable 3D block shape
end

set_background() ; //** -->

pwindow_set_size() //** phisical window set size --> ok

window_set_size()  //** virtual window set sixe --> ok

for %Y=1:length(%scicos_menu)
    execstr( %scicos_menu(%Y)(1)+'_'+string(curwin)+'='+%scicos_menu(%Y)(1))
end

menu_stuff() //** delete the default menus in the graphics window

//** The expandend superblock windows does not have the [stop] simulation button
if ~super_block then
  delmenu (curwin,'stop')
  addmenu (curwin,'stop',list(1,'haltscicos'))
  unsetmenu(curwin,'stop')
else
  unsetmenu(curwin,'Simulate')
end


//** ----------------------------------------------------------------------------------------------

//** ---------------- This piece of code is not very clear ????????????????????????????????????????

//set context (variable definition...)
if type(scs_m.props.context) == 10 then

  // %now_win = xget('window')
  gh_percent_now_win = gcf();

  [%scicos_context,ierr] = script2var(scs_m.props.context,%scicos_context)
  //for backward compatibility for scifunc
  if ierr==0 then
    %mm = getfield(1,%scicos_context)

    for %mi=%mm(3:$)
       ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
       if ierr<>0 then
	        break
       end
    end

  end
  //end of for backward compatibility for scifunc

  if ierr  <>0 then
    message(['Error occur when evaluating context:' lasterror() ]) ;
  else
    deff('%fonct()',scs_m.props.context)
    %outfun = macrovar(%fonct);

    //perform eval only if context contains functions which may give
    //different results from one execution to next
    if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then
      disablemenus() ;
      [scs_m,%cpr,needcompile,ok] = do_eval(scs_m,%cpr);
      enablemenus() ;
    end

  end

  //** xset('window',%now_win)
  scf(gh_percent_now_win);
else

  scs_m.props.context=' '

end

//** -------------------- end of the very obscure code ---------------------------------------------

drawobjs(scs_m) ; //** draw the full diagram

//** --------------------------------------------------------------------------------------------------------
//** Begin of the command interpreter loop

// state machine variables

%pt = [];

Cmenu = [];

%win = curwin;

//state machine variables windowish behavior

Select = [];
Select_back=[];

%ppt = [];

Clipboard = [];

//** hilite_image = list() ; // create an empty list for the hilite images

//** ----------------------------- real command interpreter / state machine loop ----------------------------

while ( Cmenu <> 'Quit' ) //** Cmenu -> exit from Scicos

//** ---------------------------------------------------------------// Used for "dynamic" stack allocation
  [%stack] = stacksize()                                            //
  if %stack(2)/%stack(1)> 0.3 then                                  //
    stacksize(2*%stack(1))                                          //
    disp('stacksize increased to '+string(2*%stack(1)))             //
  end                                                               //
//** ---------------------------------------------------------------

  //** unselect object in the current window

  if Select<>[] then
    if ~or(Select(1,2) == winsid()) then Select=[],end
  end

  [CmenuType,mess] = CmType(Cmenu);
  //** clear the %pt information for backward compatibility
  if (  %pt <> [] & Cmenu == [] ) then %pt=[]; end
  if ( Cmenu<> [] & CmenuType==0) then %pt=[]; end

  // no argument needed
  if (Cmenu<>[] & CmenuType==1 & %pt==[] & Select<>[]) then
       [%pt,%win]=get_selection(Select) //in case object selected
  end

  xinfo(mess);

    if ( Cmenu==[] | (CmenuType==1 & %pt==[] & Select==[]) ) then // need MORE information

      [btn_n, %pt_n, win_n, Cmenu_n] = cosclick() ; //** <-- The input function <-------------------
           // disp("|*"); pause
      if (Cmenu_n=='SelectLink'| Cmenu_n=='MoveLink') & Cmenu<>[] & CmenuType == 1 & %pt==[] then
           // disp ("|:") ; pause

	   if %pt_n <> [] then %pt = %pt_n; end
       else
         if Cmenu_n<>[] then Cmenu = Cmenu_n; end
         if %pt_n <> [] then %pt = %pt_n; end
      end

      %win = win_n

    else

      %koko = find( Cmenu==%cor_item_exec(:,1) );

      //**------------------------------------------------------
      if size(%koko,'*') == 1 then

        //** need carefull modification

        Select_back=Select;

        //** ---------- For debugging purpose only ------------------
        exestring = "Executing ...... " + %cor_item_exec(%koko,2); //
        disp(exestring)       ;                                    //
        //** --------------------------------------------------------
        //** Don't ever think to touch this line of code ;)
        execstr('exec('+%cor_item_exec(%koko,2)+',-1)') ; //** call the function that
                                                          //exec the desired action
	//** after the execution I will show the graphics datastructure
	gh_test = gcf(1000);
	gh_spy = gh_test.children.children ;
	disp (size(gh_spy)); disp (size(scs_m.objs) );

        //** need carefull modification

	//supprimer les doublons dans Select et select_back

	if or(Select<>Select_back) then
	  // Select_back: objects to
	  // unselect, Select :object to select
	  selecthilite(Select_back, 'off') ; // update the image
	  selecthilite(Select, 'on') ;       // update the image
	                                     //** hilite the selected object and update the data struct
	end

      else

         Cmenu=[]; %pt=[]

      end
      //**-------------------------------------------------------

    end

end //** ---------------- end of the while loop ---------------------------------------------------------------

do_exit()

endfunction

//** ----------------------------------------------------------------------------------------------------------------
//*******************************************************************************************************************

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

//** ----------------------------------------------------------------------------------------------------------------

function [x,k]=gunique(x)
    [x,k]=gsort(x);
    keq=find(x(2:$)==x(1:$-1))
    x(keq)=[]
    k(keq)=[]
endfunction

//** ----------------------------------------------------------------------------------------------------------------

function selecthilite(Select, flag)  // update the image

  gh_winback = gcf() //** get the window

  for i=1:size(Select,1)

    [junk, win, o] = get_selection(Select(i,:))

    gh_curwin = scf(win); //** select current window

    o_size = size ( gh_curwin.children.children ) ;
    //** initial size
    //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
    //** I need an equivalent index for the graphics

    //** "k" is the object index in the data structure "scs_m"
    //** compute the equivalent "gh_k" for the graphics datastructure
    k = Select (i,1)
    gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)

    if typeof(o)=='Block' then
      //** ------------- Blocks ---------------------------------

      gh_blk = gh_curwin.children.children(gh_k);
      gh_blk.children(1).visible = flag ;

    elseif typeof(o)=='Link' then
      //**-------------- Links ---------------------------------
      gh_blk = gh_curwin.children.children(gh_k);
      gh_blk.children(1).mark_mode = flag  ;

    end //** if()

  end //** end the for(..) loop

  drawnow(); show_pixmap();

  scf(gh_winback)

endfunction

//**---------------------------------------------------------------------------------------------------------------------