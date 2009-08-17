//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

function [scs_m, newparameters, needcompile, edited] = scicos(scs_m, menus)
//Copyright INRIA

// scicos - block diagram graphic editor
// %SYNTAX
// scs_m=scicos(scs_m,job)
// %PARAMETERS
// scs_m    : scilab list, scicos main data structure
//      scs_m.props contains system name and other infos
//      scs_m.objs(i) contains description of ith block diagram element
//      scs_m.version contains the number version of the diagram
// menus : vector of character strings,optional parameter giving usable menus
// !
//--------------------------------------------------------------------------------------------
// Copyright INRIA

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//** Magic Global Variable for Diagram Browser and more
  global %scicos_navig
  global %diagram_path_objective
  global inactive_windows
  global Scicos_commands   // programmed commands

  //** "0" standard scicos oblique link ; "1" SL orthogonanal links
  global SL_mode ; SL_mode = 0 ;

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //**-----------------------------------------------------------------------------------------
  //** Check for Scilab "command line mode" that does not support SCICOS
  //** This check verify if "scicos()" is started form a command line Scilab with no graphics
  //** support (almost obsolete function)

  noguimode = find(sciargs()=="-nogui");
  if (noguimode <>[]) then
    clear noguimode
    warning(" Scilab in no gui mode : Scicos unavailable");
    abort ; //** EXIT form Scicos ()
  end;

  clear noguimode
  //**-----------------------------------------------------------------------------------------


  //** -------------------- Check the recurring calling level of scicos ----------------------
  [%ljunk, %mac] = where() ; //** where I am ?
  slevel = prod ( size ( find ( %mac=='scicos') ) ) ; //** "slevel" is the superblock level
  super_block = slevel > 1 ; //** ... means that the actual SCICOS is a superblock diagram


  //** ----------------------- Scicos splash message and workspace stuff -----------------------
  if ~super_block then
    global next_scicos_call

    //**--------------------------- Scicos splash message --------------------------------------
    if next_scicos_call==[] then
      next_scicos_call = 1 ;
      verscicos = get_scicos_version() ;
      ttxxtt = ['Scicos version '+part(verscicos,7:length(verscicos)) + ' (adapted for Scilab 5 by The Scilab Consortium)'
                'Copyright (c) 1992-2009 Metalau project INRIA'
                'Licensed under the GNU Public License (GPL)']
      write(%io(2),ttxxtt)
    end

    //**---- prepare from and to workspace stuff
    curdir = pwd() ;
    chdir(TMPDIR)     ;
    mkdir("Workspace");
    chdir("Workspace");
    %a = who("get")   ;
    %a = %a(1:$-predef()+1);  //** exclude protected variables

    for %ij=1:size(%a,1)
      var = %a(%ij)
      if var<>'ans' & typeof(evstr(var))=='st' then
	ierr = execstr('x='+var+'.values','errcatch')
        if ierr==0 then
	   ierr = execstr('t='+var+'.time','errcatch')
        end
        if ierr==0 then
	  execstr('save('"'+var+''",x,t)')
        end
      end
    end

    chdir(curdir)
    //**----- end of /prepare from and to workspace stuff

    // set up navigation
    super_path    = [] // path to the currently opened superblock
    %scicos_navig = []
    inactive_windows = list(list(),[])
    Scicos_commands = [];
  end
  //**----------------------------------------------------------------------------------------------


  //**----------------------------------------------------------------------------------------------
  %diagram_open = %t     //** default choice
  if super_path<>[] then //** main diagram
    if isequal(%diagram_path_objective,super_path) then
      if %scicos_navig<>[] then
	%diagram_open = %t
	%scicos_navig = []
	gh_curwin = scf(curwin);
      end
    elseif %scicos_navig<>[] then
      %diagram_open = %f
    end
  end

  //**-----------------------------------------------------------------------------------------------
  if ~super_block then

    // Check and define SCICOS palette , menu , shortcut , display mode , palette libraries
    if exists('scicos_pal')==0 | exists('%scicos_menu')==0 | exists('%scicos_short')==0 | ...
	  exists('%scicos_display_mode')==0 | exists('scicos_pal_libs')==0 | ...
	  exists('%scicos_lhb_list')==0 | exists('%CmenuTypeOneVector')==0 | ...
          exists('%scicos_gif')==0 | exists('%scicos_contrib')==0  then

      [scicos_pal_d, %scicos_menu_d, %scicos_short_d, %scicos_help_d,...
       %scicos_display_mode_d, modelica_libs_d, scicos_pal_libs_d,...
       %scicos_lhb_list_d, %CmenuTypeOneVector_d, %scicos_gif_d,...
       %scicos_contrib_d ] = initial_scicos_tables() ;

      if exists('scicos_pal')==0 then
	message(["scicos_pal not defined"; "using default values"])
	scicos_pal = scicos_pal_d ;
      end

      if exists('%scicos_menu')==0 then
	message(["%scicos_menu not defined"; "using default values"])
	%scicos_menu = %scicos_menu_d ;
      end

      if exists('%scicos_short')==0 then
	message(["%scicos_short not defined"; "using default values"])
	%scicos_short = %scicos_short_d ;
      end

      if exists('%scicos_help')==0 then
	message(["%scicos_help not defined"; "using default values"])
	%scicos_help = %scicos_help_d ;
      end

      if exists('%scicos_display_mode')==0 then
	message(["%scicos_display_mode not defined"; "using default values"])
	%scicos_display_mode = %scicos_display_mode_d ;
      end

      if exists('modelica_libs')==0 then
	message(["modelica_libs not defined"; "using default values"])
	modelica_libs = modelica_libs_d ;
      end

      if exists('scicos_pal_libs')==0 then
	message(["scicos_pal_libs not defined"; "using default values"])
	scicos_pal_libs = scicos_pal_libs_d ;
      end

      if exists('%scicos_lhb_list')==0 then
	message(["%scicos_lhb_list not defined"; "using default values"])
	%scicos_lhb_list = %scicos_lhb_list_d ;
      end

      if exists('%CmenuTypeOneVector')==0 then
	message(["%CmenuTypeOneVector not defined"; "using default values"])
	%CmenuTypeOneVector = %CmenuTypeOneVector_d ;
      end

      if exists('%scicos_gif')==0 then
        message(["%scicos_gif not defined"; "using default values"])
        %scicos_gif = %scicos_gif_d ;
      end

      if exists('%scicos_contrib')==0 then
        message(["%scicos_contrib not defined"; "using default values"])
        %scicos_contrib = %scicos_contrib_d ;
      end
    end //** ... of the initialization variable


    //**--------------------------------------------------------------
    //** initialize the "scicos_contex" data structure (Scilab script inside SCICOS simulation)

        if ~exists('%scicos_context') then
	  %scicos_context = struct() ;
	end

	//**-----------------------------

	//** initialize a "scicos_debug_gr" variable for debugging editor
	if ~exists('%scicos_debug_gr') then
	  %scicos_debug_gr = %f; //** debug mode : default is "%f"
	end

	//** initialize a "scicos_with_grid" variable for drawing a grid
	if ~exists('%scicos_with_grid') then
	  %scicos_with_grid = %f;
	end
	if ~exists('%scs_wgrid') then
	  //** %scs_wgrid(1:2) : space of grid
	  //** %scs_wgrid(3) : color
	  %scs_wgrid = [10;10;12];
	end

	//** disable scilab function protection
	prot = funcprot();
	funcprot(0);


	//** Activate Graphics editor in Scicos
        exec(SCI+'/modules/graphics/macros/ged.sci',-1);

	//** restore scilab function protection
	funcprot(prot)

	//** check and - eventually - load the Scicos function library
	if exists('scicos_scicoslib')==0 then
	  load("SCI/modules/scicos/macros/scicos_scicos/lib") ;
	end

	exec(loadpallibs, 1) //** load library that contains the INTERFACING functions


  end //** end of the main if() not superblock initialization

  //** ----------------------------- End the NOT-Superbloc initialization and check ----------------------

  //** Magic Numbers
  Main_Scicos_window = 1000 ; //** set default value of the main scicos window

  //** Initialisation
  newparameters = list() ;
  enable_undo = %f
  edited = %f
  needreplay = %f
  %path='./'
  %exp_dir = PWD


  global %tableau

  //**----------------------------------------------------------------------------------

  //** Some "magic numbers", try to load ".scicos_pal" and "scicos_short" trom the current
  //** folder
  //**
  if ~super_block then
    %zoom    = 1.4      ; //** original value by Ramine
    pal_mode = %f       ;  // Palette edition mode

    scicos_paltmp = scicos_pal ;

    //** try to load the local ".scicos_pal" files that contains the user defined
    //** palette relatives to the local directory
    if execstr('load(''.scicos_pal'')','errcatch')==0 then
      //** if the load has been positive
      scicos_pal = gunique(scicos_pal,scicos_paltmp); //** remove the duplicate item(s) in the palette
      //** check is given palettes paths are still valid
      scicos_pal = check_palettes_paths(scicos_pal)

    end

    //** load - if present - the used defined local shortcut
    execstr('load(''.scicos_short'')','errcatch')  // keyboard shortcuts

  end
  //**----------------------------------------------------------------------------------

  //** Scilab 5 patch for font handling. This patch fif the "Symbol" font issue
  //**
  if ~super_block then
    scilab5fonts = xlfont() ; //** recover the full font list
    xlfont(scilab5fonts(1), 1) ; //** substitute the font in position one
  end
  //**----------------------------------------------------------------------------------

  [lhs, rhs] = argn(0) ; //** recover the arguments of "scicos(<rhs>)"

  if rhs>=1 then //** scicos_new(...) is called with some arguments

    if type(scs_m)==10 then // diagram is given by its filename
      %fil = scs_m ;
      alreadyran = %f
      [ok,scs_m,%cpr,edited] = do_load(%fil,'diagram');
      if ~ok then return, end

      if size(%cpr)==0 then
	needcompile = 4 ;
	%state0     = list();
      else
	%state0 = %cpr.state;
	needcompile=0
      end

    else // diagram is given by its data structure

      if ~super_block then
	%cpr=list(); needcompile=4 ; alreadyran=%f , %state0=list()
      end

    end

  else //** scicos() is called without arguments (AND - implicitly - is NOT a superblock)

    gh_Main_Scicos_window = scf(Main_Scicos_window);
    //** In case a back up file exists
    ierr = execstr('load(TMPDIR+''/BackupSave.cos'')','errcatch')
    if ierr<>0 then
      scs_m = scicos_diagram(version = get_scicos_version()) ;
      %cpr = list()    ;
      needcompile = 4  ;
      alreadyran = %f  ;
      %state0 = list() ;
    else
      load(TMPDIR+'/BackupInfo');
    end
  end

  //

  if typeof(scs_m)<>'diagram' then
    error("First argument must be a Scicos diagram");
  end

  if ~super_block then

    //**----------------------- Dynamic menu and shortcut preparation -----------------------------------------
    //**
    %cor_item_exec = []; //** init

    //** scan all the "%scicos_menu" an load "%cor_item_exec" ; dispose the first string (2:$) because
    //** is the name of the dynamic menu
    for %Y=1 : length(%scicos_menu)
      %cor_item_exec = [%cor_item_exec, %scicos_menu(%Y)(2:$)] ;
    end

    %cor_item_exec = %cor_item_exec'; //** transpose the vector
    %R = %cor_item_exec;
    %R = stripblanks(%R)+'_' ; //** delete all the blanks and add an underscore at the end "Save As PAlette" -> "SaveAsPalette_"
    %R = strsubst(%R,'/','') ; //** delete "/"
    %R = strsubst(%R,' ','') ; //** delete " "
    %R = strsubst(%R,'.','') ; //** delete "."
    %R = strsubst(%R,'-','') ; //** delete "-"
    %cor_item_exec = [%cor_item_exec, %R]; //** create the two column matrix [<MenuName> <FunctionToCall>]

    // add fixed menu items not visible
    %cor_item_exec = [%cor_item_exec;
		      _('Link')        ,    'Link_'           ;
		      _('Open/Set')    ,    'OpenSet_'        ;
		      _('MoveLink')    ,    'MoveLink_'       ;
		      _('SelectLink')  ,    'SelectLink_'     ;
		      _('CtrlSelect')  ,    'CtrlSelect_'     ;
		      _('SelectRegion'),    'SelectRegion_'   ;
		      _('Popup')       ,    'Popup_'          ;
		      _('PlaceinDiagram'),  'PlaceinDiagram_' ;
		      _('TkPopup')     ,    'TkPopup_'        ;
		      _('BrowseTo')    ,    'BrowseTo_'       ;
		      _('Place in Browser'), 'PlaceinBrowser_';
		      _('Select All'),       'SelectAll_'
		     ];

    menus = tlist('xxx');

    for %Y=1:length(%scicos_menu)
      menus(1)  = [menus(1), %scicos_menu(%Y)(1)];
      menus($+1)= %scicos_menu(%Y)(2:$);
    end

    for %Y=1:length(%scicos_menu)
      %R = %scicos_menu(%Y);
      %w = 'menus('''+%R(1)+''')('+ string(1:(size(%R,'*')-1)) + ')';
      execstr(%R(1)+ '=%w;');
    end

    //----------------------------------------------------------------
    // single key shortcut: keyboard definition
    %tableau = emptystr([1:100]);
    for %Y = 1 : size(%scicos_short,1)
      %tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
    end
    //----------------------------------------------------------------

  end

  // viewport
  options = scs_m.props.options

  // solver
  %scicos_solver = scs_m.props.tol(6)

  //** ------- GRAPHICS INITIALIZATION: Palettes, TK functions, ---------
  //**-------------------------- I'm NOT inside a superblock  -----------
  if ~super_block then

    gh_current_window = scf(Main_Scicos_window);
    curwin = get ( gh_current_window, "figure_id") ;

    palettes = list();
    noldwin = 0      ;
    windows = [1 curwin] ;
    pixmap = %scicos_display_mode ; //** obsolete: the pixmap is "on" as default


    // ** Bruno
    // Force sci_gui_mode to 1
    // Scicos does not absolutely need Tk
    // Except for some Screen Size
    // will be improved soon
    //

    //if ~exists('%scicos_gui_mode') then
      //if with_tk() then
        %scicos_gui_mode = 1
      //else
       // %scicos_gui_mode = 0
      //end
    //end

    %diagram_i_h = generic_i_h //** allows to write inside the "user_data" field
                               //** of the graphics windows

    if %scicos_gui_mode==1 then
      //** scicos is active in graphical mode
      prot = funcprot();
      funcprot(0);

      getfile  = uigetfile ; //** brand new aliases
      savefile = uigetfile ;

      mpopup = createpopup ;
      mdialog = x_mdialog  ;

      //** In Scilab 4.1.2 "getvalue  = tk_getvalue ;"
      //** In scilab 5 we use "modules/scicos/macros/scicos_util/getvalue.sci"
      //** by Alan Layec, modified in order to accept string, etc.

      if MSDOS then
	//** Windows
	choose    = tk_scicos_choose ; //** please check with Allan Cornet
       else
        //** Linux
 	deff('x = choose(varargin)', 'x = x_choose(varargin(1:$))');
      end

      funcprot(prot);

    else
      //** Scicos works in "text mode"
      disp("Scicos in text mode is not supported");
      //** RAMINE : TEXT MODE is not supported !
      // deff('x=getfile(varargin)','x=xgetfile(varargin(1:$))');
      // savefile = getfile;
      // deff('Cmenu=mpopup(x)','Cmenu=[]')
      // deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
    end //** of %scicos_gui_mode

    //

  else //** super block case

    //** NO Pupup function definition in the super block ------------
    //** because they are already defined
    noldwin = size(windows,1)           ;
    windows = [windows ; slevel curwin] ;
    palettes = palettes                 ;

  end //** end of not superblock



  //**------------------------- CONTEXT -----------------------
  //set context (variable definition...)

  if type(scs_m.props.context) == 10 then //** if the variable is defined

    gh_percent_now_win = gcf(); //** save current figure handle

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
      clear('%fonct')
      //** perform eval only if context contains functions which may give
      //** different results from one execution to next
      if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then
	DisableAllMenus()
	[scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr);
	EnableAllMenus()
      end
    end

    scf(gh_percent_now_win); //** restore current figure handle

  else

    scs_m.props.context = ' ' ; //** put the variable to empty state

  end
  //** ---------- End of "Contex" handling and evaluation -----------
  //**

  //** Begin of the Main command interpreter loop

  //** Initial conditions
  Cmenu = []     ; //** valid command = empty
  %pt   = []     ; //** valid last mouse position = empty
  %win  = curwin ; //** curwin is dynamically modified if a superblock window is open
  %curwpar = []  ; // window dimensions

  //** 'Select' and 'Select_back' are matrix;
  //**  Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
  //**                                  and "win_id"    is the Scilab window id.
  //**  Multiple selection is permitted: each object is a line of the matrix.
  Select = []      ; //** empty
  Select_back = [] ; //** empty
  %ppt = []; //** used to store last valid click position for "Paste" operation
  //Clipboard = []; //** used in Copy Cut and Paste function
   //** ------------------- GRAPHICS INITIALIZATION ---------

  //** This section is executed in any case
  //   initialize graphics

  if %diagram_open then
    gh_current_window = gcf() ; //** get the current graphics window

    if (gh_current_window.user_data==[]) | (~isequalbitwise(gh_current_window.user_data(1),scs_m)) then
       %zoom = restore(gh_current_window) ;
       execstr('drawobjs(scs_m)', 'errcatch') ;
     else
       Select = gh_current_window.user_data(2) ;
       enable_undo = gh_current_window.user_data(3) ;
       scs_m_save  = gh_current_window.user_data(4) ;
       nc_save     = gh_current_window.user_data(5) ;
       xselect();
     end

  else //** diagram is NOT open

    if or(curwin==winsid()) then
      gh_current_window = scf(curwin) ;
      if (gh_current_window.user_data~=[]) & (isequalbitwise(gh_current_window.user_data(1),scs_m)) then
	Select = gh_current_window.user_data(2) ;
      end
    end

  end
  exec(restore_menu,-1);

//** --- End of initialization -----------------------------------------------------------

  global Clipboard  //** to make it possible to copy and paste from one
                    //** super block to another


  //** -------------    M A I N    L O O P  -----------------------------
  //** --------- Command Interpreter / State Machine ------------
  while ( Cmenu <> "Quit" & Cmenu <> "Leave"  ) //** Cmenu -> exit from Scicos

    //** Dynamic stacksize for very big diagram           //
    [%stack] = stacksize()                                //
    if %stack(2)/%stack(1)> 0.3 then                      //
      stacksize(2*%stack(1))                              //
      disp("Stacksize increased to "+string(2*%stack(1))) //
    end                                                   //
    //**----------------------------------------------------

    //** Dynamic window resizing and centering -------------------------
    if or(winsid()==curwin) then //** if the current window is in the
      winsize = gh_current_window.figure_size;
      axsize  = gh_current_window.axes_size;

      //** +21 is to compensate for scrollbar under windows
      if or(winsize > axsize+21) then
        viewport = gh_current_window.viewport;
        viewport = max([0,0], min(viewport,-winsize+axsize));
        window_set_size(gh_current_window, viewport)
        drawnow(); //** update the diagram on screen
      end

      if edited then
	// store win dims, it should only be in do_exit but not possible
	// now
        scf(gh_current_window);
        gh_axes = gca();
        data_bounds = gh_axes.data_bounds;
	winpos   = gh_current_window.figure_position ;
        viewport = gh_current_window.viewport ;
	axsize   = gh_current_window.axes_size;
        %curwpar = [data_bounds(:)', axsize, viewport, winsize, winpos, %zoom];

       if ~isequal(scs_m.props.wpar, %curwpar) then
	  scs_m.props.wpar = %curwpar  // keep window dimensions
	end

      end //** edited section

    end
    //**... end of: Dynamic window resizing and centering ----------

    //** Dynamic mark size
    //** mark_size = int(%zoom*3.0); //** in pixel : size of the selection square markers

    if %scicos_navig==[] then
      if Scicos_commands<>[] then
	execstr(Scicos_commands(1))
	Scicos_commands(1)=[]
      end
    end

    if Cmenu=="Quit" then
        break ; //** EXIT point ...
    end

    //**--------------------------------------------------------------------
    if %scicos_navig<>[] then //** navigation mode active
      while %scicos_navig<>[] do
	if ~isequal(%diagram_path_objective,super_path) then
	  %diagram_open  = %f
          Select_back=Select
	  [Cmenu,Select] = Find_Next_Step(%diagram_path_objective, super_path)

	  if or(curwin==winsid()) & ~isequal(Select,Select_back) then
             drawlater() ;
	       selecthilite(Select_back, "off") ; // unHilite previous objects
	       selecthilite(Select, "on") ;       // Hilite the actual selected object
             drawnow() ;
          end

          if Cmenu=="OpenSet" then
	    ierr=0
	    execstr('ierr=exec(OpenSet_,''errcatch'',-1)')
            //execstr('exec(OpenSet_,-1)')
	    //**---------------------------------------------------
	    if ierr<>0 then message(lasterror()),end
	    if isequal(%diagram_path_objective,super_path) then // must add after testing &%scicos_navig<>[]
	      if ~or(curwin==winsid()) then
		gh_current_window = scf(curwin);
                %zoom = restore(gh_current_window);
                execstr('drawobjs(scs_m)', 'errcatch') ;
		%scicos_navig =[];
		Select_back   =[];
                Select        =[];
	      else
		gh_current_window = scf(curwin);
	      end
	    else
	      if ~or(curwin==winsid())&%scicos_navig==[] then
		%scicos_navig = 1 ;
		%diagram_path_objective = [] ;
	      end
	    end
	  elseif Cmenu=="Quit" then
	    do_exit() ;
	    return ;
	  end
	  //**---------------------------------------------------
	else
	  %scicos_navig = [] ;
	end
      end

    else

      //** not in navigation mode
      %diagram_open = %t ;

      if ~or(curwin==winsid()) then
	gh_current_window = scf(curwin);
        %zoom = restore(gh_current_window)
        execstr('drawobjs(scs_m)', 'errcatch') ;
	Select_back = [] ;
        Select      = [] ;
      else
        gh_current_window = scf(curwin);
      end

      if Select<>[] then
	if ~or(Select(1,2) == winsid()) then
	  Select = [] ; //** imply a full Reset
	end
      end

      //**--------------------------------------------------------------------

      //** Command classification and message retrivial
      [CmenuType, mess] = CmType(Cmenu); //** local function: see below in this file
      xinfo(mess); //** show the message associated to the command

      //** ----------------- State variable filtering ----------------
      //** clear the %pt information for backward compatibility
      //** if 'Cmenu' is empty (no command) but '%pt' is not , it is better to clear '%pt'

      if ( Cmenu == [] & %pt <> []  ) then %pt=[]; end

      //** if 'Cmenu' is NOT empty and 'CmenuType' is "0" I don't' need '%pt' then clear '%pt'
      if ( Cmenu<> [] & CmenuType==0) then %pt=[]; end
      	gh_current_window = scf(curwin);
      //** if 'Cmenu' is NOT empty and 'CmenuType' is "1" and there is at least one object selected
      if (Cmenu<>[] & CmenuType==1 & %pt==[] & Select<>[]) then
	[%pt,%win] = get_selection(Select) //** recover the %pt and %win from 'Select'
      end
      //** ------------------------------------------------------------------------------------

      //** ---------------------- Main decisional section --------------------------------------
      //** if no command is issued "Cmenu==[]" or
      //**    CmenuType==1 and no %pt information and no object selected
      if ( Cmenu==[] | (CmenuType==1 & %pt==[] & Select==[]) ) then

	//** I'm not ready to exec a command: I need more information using cosclik()
        EnableAllMenus()
	 [btn_n, %pt_n, win_n, Cmenu_n] = cosclick() ;
        DisableAllMenus()

	if (Cmenu_n=="SelectLink" | Cmenu_n=="MoveLink") & Cmenu<>[] & CmenuType==1 & %pt==[] then
	  if %pt_n<>[] then %pt = %pt_n; end
	else
	  if Cmenu_n<>[] then Cmenu = Cmenu_n; end
	  if %pt_n <> [] then %pt = %pt_n;     end
	end

	%win = win_n

      else

	//** I'm ready to exec a command
	%koko = find( Cmenu==%cor_item_exec(:,1) );
	if size(%koko,'*') == 1 then

	  Select_back = Select; //** save the selected object list
          ierr = 0

          //** exec(path [,mode]) executes sequentialy the scilab instructions contained in the file given by path with
          //** an optional execution mode mode .
          //**            0 : the default value
          //**           -1 : nothing is printed
          //**            1 : echo of each command line
          //**            2 : prompt --> is printed
          //**            3 : echoes + prompts
          //**            4 : stops before each prompt. Execution resumes after a carriage return.
          //**            7 : stops + prompts + echoes : useful mode for demos.

          //** Used for AGGRESSIVE DEBUG ONLY -->
	  //** execstr('exec('+%cor_item_exec(%koko,2)+',1)')

          //** Used for standard DEBUG ONLY -->
          //** disp(%cor_item_exec(%koko,2)); //** disp the current exec
          //** execstr('exec('+%cor_item_exec(%koko,2)+',-1)'); //** nothing is printed

          //** RELEASE --> Please reactivate the error catcher before final release
	       execstr('ierr=exec('+%cor_item_exec(%koko,2)+',''errcatch'',-1)')

	  if ierr > 0 then
	    Cmenu = "Replot"
	    Select_back = [];
            Select = [] ;
	    terr = ["I recovered from the following error:";
		     lasterror();
		    'in '+%cor_item_exec(%koko,2)'+' action.'];
	    mprintf('%s\n',terr);

	  elseif or(curwin==winsid()) then
	    gh_current_window = scf(curwin);

            if ~isequal(Select,Select_back) then
              drawlater();
	        selecthilite(Select_back, "off") ; // unHilite previous objects
	        selecthilite(Select, "on") ;       // Hilite the actual selected object
              drawnow();
	    end

          else
	    if %scicos_navig==[] then // in case window is not open
	      %scicos_navig = 1
	      %diagram_path_objective = []
	    end
	  end

	else
	  //** if the command is not valid clear the state variable
	  Cmenu = []; %pt = []
	end //** a valid/invalid command to exec

      end //** not_ready / ready ... to exec a command

    end //** not in navigation mode ???

  end //**--->  end of the while loop: exit with the 'Quit' OR 'Leave' commands

  //** if you are exited from the main loop with 'Quit'
  if Cmenu=="Quit" then
    //**  -------------- 'Quit' ------------------------------------
    //**  Quit exit from suberblock or go back to Scilab command line
    do_exit() ;
    if ~super_block then // even after quiting, workspace variables
                         // must be saved and be usable in Scilab
      if find(%mac=='scilab2scicos') ==[] then
	[txt,files]=returntoscilab()
	n=size(files,1)
	for i=1:n
	  load(TMPDIR+'/Workspace/'+files(i))
	  execstr(files(i)+'=struct('"values'",x,'"time'",t)')
	end
	execstr(txt)
      end

      mdelete(TMPDIR+'/BackupSave.cos') // no backup needed
      mdelete(TMPDIR+'/BackupInfo')
      //close TCL windows
      //TCL_EvalStr('catch {destroy .palettes}')
      //TCL_EvalStr('catch {destroy .ss}')
      //TCL_EvalStr('catch {destroy .scsTree}')
      // clear all globals defore leaving
      clearglobal Clipboard
      clearglobal Scicos_commands
      clearglobal %tableau
      clearglobal %scicos_navig
      clearglobal %diagram_path_objective
      close_inactive_windows(inactive_windows,[])
      clearglobal inactive_windows

      //** restore the original Scilab 5 font list before exit
      //** xlfont(scilab5fonts(2),1);
      xlfont('reset');
      //** TO DO
      //** For future implementation .....
      //** This is the place of the windows cleaning section

    end

  elseif Cmenu=="Leave" then
   //**  -------------- 'Leave' ------------------------------------
   //**  "Leave" go to Scilab leaving Scicos active in the background
   //**  and use "seteventhandler" do to the job
    ok = do_save(scs_m,TMPDIR+'/BackupSave.cos')
    if ok then  //need to save %cpr because the one in .cos cannot be
                //used to continue simulation
      if ~exists('%tcur') then %tcur=[];end
      if ~exists('%scicos_solver') then %scicos_solver=0;end
      save(TMPDIR+'/BackupInfo', edited,needcompile,alreadyran, %cpr,%state0,%tcur,..
                                            %scicos_solver,inactive_windows)

      OpenPals=windows(find(windows(:,1)<0),2 )  //close palettes
      for winu=OpenPals'
        if or(winu==winsid()) then
          gh_del = scf(winu) ; //** select the 'winu'window and get the
                               //handle
          delete (gh_del)   ; //** delete the window
        end
      end
    end

    if ~ok then
      message(["Problem saving a backup; I cannot activate Scilab.";
	       "Save your diagram scs_m manually."]);
      pause ;
    end

    AllWindows = unique([windows(:,2);inactive_windows(2)])
    AllWindows = intersect(AllWindows',winsid())

    //** activate the same event handler for all the active Scicos windows
    for win_i= AllWindows
      scf(win_i); //** select
      seteventhandler("scilab2scicos"); //** activate the event handler
    end

    save(TMPDIR+'/AllWindows',AllWindows)
    //   scf(0)  // to protect scicos windows when in Scilab
    mprintf('%s\n','To reactivate Scicos, click on a diagram or type '"scicos();'"')


    if edited then
      mprintf("%s\n","Your diagram is not saved. Do not quit Scilab or "+...
	      "open a new Scicos diagram before returning to Scicos.")
    end
    // prepare from and to workspace stuff

    if find(%mac=='scilab2scicos') ==[] then
      [txt,files] = returntoscilab();
      n = size(files,1)
      for i=1:n
	load(TMPDIR+'/Workspace/'+files(i))
	execstr(files(i)+'=struct('"values'",x,'"time'",t)')
      end
      execstr(txt);
    end

  end

endfunction //** scicos(); end here :) : you had a good day
//**------------------------------------------------------------------------------------


function [itype, mess] = CmType(Cmenu)
//** look inside "%CmenuTypeOneVector" if the command is type 1 (need both Cmenu and %pt)
  k = find (Cmenu == %CmenuTypeOneVector(:,1));
  if k==[] then //** if is not type 1 (empty k)
    itype = 0 ; //** type zero
    mess=''   ; //** set message to empty
    return    ; //** --> EXIT point : return back
  end

  if size(k,'*')>1 then //** if found more than one command
    message('Warning '+string( size(k,'*'))+' menus have identical name '+Cmenu);
    k = k(1); //** extract the index
  end

  itype = 1 ; //** type one

  mess = %CmenuTypeOneVector(k,2) ; //** use the index to recover the message

endfunction

//** ----------------------------------------------------------------------------------------------------------------

function uni = gunique(m1,m2)
// Used to eliminate duplicate palette item(s)
  uni = [m1;m2] ; //** merge the two matrix
  [j,ind] = unique(uni(:,1) + uni(:,2)) ; //** remove the duplicate palette name
  uni = uni(-gsort(-ind),:);  //** reorder the merged palette without duplicate
endfunction

//** ----------------------------------------------------------------------------------------------------------------

function restore_menu()
  //** disp("//** [Call] restore_menu...")
  for %Y=1:length(%scicos_menu)
    execstr( %scicos_menu(%Y)(1)+'_'+string(curwin)+'='+%scicos_menu(%Y)(1) )
  end
endfunction

//**---------------------------------------------------------------------------------------------------------------------

function scicos_pal = check_palettes_paths(scicos_pal)
  toremove=[]
  for k=1:size(scicos_pal,1)
    if fileinfo(scicos_pal(k,2))==[] then toremove=[toremove k],end
  end
  //remove invalid ones out of scicos_pal
  if toremove<>[] then
    rmpal=scicos_pal(toremove,:)
    rmpal(:,1)=part(rmpal(:,1),1:max(length(rmpal(:,1))))
    message(['Following palette(s) ignored (associated file(s) no more exist):';
	     ' '
	     rmpal(:,1)+':  '+rmpal(:,2)
	     ' '
	     ' To avoid this message, please update the ""'+pwd()+filesep()+'.scicos_pal"" file'])
    scicos_pal(toremove,:)=[];
  end
endfunction

