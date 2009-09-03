function [scicos_menu,scicos_lhb_list, scicos_short,CmenuTypeOneVector]= set_initial_xcos_menus()
  
  //**-----------------------------------------------------------------------
  //**----------------------------- Menubar menus ------------------------
  //**-----------------------------------------------------------------------

// each menu and submenu is defined by a unique Id and a localized label. The
// Id is used for the help and the callback, the label is the string that
// appears in the GUI.
// The Ids must be valid Scilab variable names
  
  File     = ['XcosMenuFile',                 _("File"); 
	      'XcosMenuNew',                  _("New");
	      'XcosMenuOpen',                 _("Open");
	      'XcosMenuSave',                 _("Save");
	      'XcosMenuSaveAs',               _("Save As");
	      'XcosMenuSaveAsInterfFunc',     _("Save as Interf Func");
  	      'XcosMenuExport',               _("Export");
  	      'XcosMenuExportAll',            _("Export All");
	      'XcosMenuExitScicos',           _("Exit");
	      'XcosMenuQuit',                 _("Quit")];

  Diagram  = ['XcosMenuDiagram',              _("Diagram");
	      'XcosMenuReplot',               _("Replot");
	      'XcosMenuRename',               _("Rename");
	      'XcosMenuPurge',                _("Purge");
	      'XcosMenuSetDiagramInfo',       _("Set Diagram Info");
	      'XcosMenuRegionToSuper',        _("Region to Super Block");
              'XcosMenuUpToMain',             _("Up To Main Diagram");
	      'XcosMenuContext',              _("Context")];

  Palette  = ['XcosMenuPalette',              _("Palette");
              'XcosMenuPalettes',             _("Palettes");
	      'XcosMenuPalEditor',            _("Pal editor");
	      'XcosMenuRegionToPalette',      _("Region to Palette");
	      'XcosMenuLoadAsPalette',        _("Load as Palette");
	      'XcosMenuSaveAsPalette',        _("Save as Palette");
              'XcosMenuPalettesTree',         _("Palettes Tree")];

  Edit     = ['XcosMenuEdit',                 _("Edit");
              'XcosMenuUndo',                 _("Undo");
	      'XcosMenuCut',                  _("Cut");
	      'XcosMenuCopy',                 _("Copy");
	      'XcosMenuPaste',                _("Paste");
	      'XcosMenuDuplicate',            _("Duplicate");
	      'XcosMenuDelete',               _("Delete");
	      'XcosMenuMove',                 _("Move");
	      'XcosMenuSmartMove',            _("Smart Move");
	      'XcosMenuOpenSet',              _("Open/Set");
	      'XcosMenuAlign',                _("Align");
	      'XcosMenuFlip',                 _("Flip");
	      'XcosMenuRotateLeft',           _("Rotate Left");
	      'XcosMenuRotateRight',          _("Rotate Right");
	      'XcosMenuAddNewBlock',          _("Add new block");
	      'XcosMenuBlockDocumentation'    _("Block Documentation");
	      'XcosMenuLabel',                _("Label")];

  View     = ['XcosMenuView',                 _("View");
              'XcosMenuZoomIn',               _("Zoom in");
	      'XcosMenuZoomOut',              _("Zoom out");
              'XcosMenuFitToFigure',          _("Fit diagram to figure");
  	      'XcosMenuWindowParameters',     _("Default window parameters");
              'XcosMenuDefinedVariables',     _("Available Parameters");
              'XcosMenuGrid',                 _("Toggle grid")];

  Simulate = ['XcosMenuSimulate',             _("Simulate");
              'XcosMenuSetup',                _("Setup");
	      'XcosMenuCompile',              _("Compile");
	      'XcosMenuModelicaInit',         _("Modelica initialize");
	      'XcosMenuEval',                 _("Eval");
	      'XcosMenuAnalyzeDiagram',       _("Analyze Diagram");
	      'XcosMenuDebugLevel',           _("Debug Level");
	      'XcosMenuRun',                  _("Run")];

  Format   = ['XcosMenuFormat',               _("Format");
              'XcosMenuSetDefaultAction',     _("Set default action");
              'XcosMenuSetGrid',              _("Set grid");
   	      'XcosMenuAddColor',             _("Add color");
	      'XcosMenuDefaultLinkColor',     _("Default link colors");
	      'XcosMenuColor',                _("Color");
	      'XcosMenuBackgroundColor',      _("Background color");
	      'XcosMenuShowBlockShadow',      _("Show Block Shadow");
	      'XcosMenuOrtLink',              _("Ort Link");
	      'XcosMenuResize',               _("Resize");
	      'XcosMenuIdentification',       _("Identification");
	      'XcosMenuSetIDFonts',           _("ID fonts");
	      'XcosMenuIcon',                 _("Icon");
	      'XcosMenuIconEditor',           _("Icon Editor")];

  Tools    = ['XcosMenuTools',                _("Tools");
              'XcosMenuCreateMask',           _("Create Mask");
              'XcosMenuRemoveMask',           _("Remove Mask");
              'XcosMenuCustomizeMask',        _("Customize Mask");
              'XcosMenuSaveBlockGUI',         _("Save Block GUI");
	      'XcosMenuCreateAtomic',         _("Create Atomic");
	      'XcosMenuRemoveAtomic',         _("Remove Atomic");
              'XcosMenuGetInfo',              _("Get Info");
              'XcosMenuDetails',              _("Details");
              'XcosMenuBrowser',              _("Browser");
	      'XcosMenuCodeGeneration',       _("C Code Generation");
	      'XcosMenuShortcuts',            _("Shortcuts");
	      'XcosMenuActivateScilab',       _("Activate Scilab Window");
	      'XcosMenuCalc',                 _("Calc")];

  Help     = ['?',                            _("?");
              'XcosMenuHelp',                 _("Help");
              'XcosMenuScicosDocumentation',  _("Scicos Documentation");
	      'XcosMenuDemos',                _("Demos");
	      'XcosMenuAboutScicos',          _("About Scicos")];

  //The Unvisible "Menus" are actions used internally but not displayed
  //neither in the menubar nor in contextual menus
  Unvisible= ['Unvisible'                      ""
	      'XcosMenuLink',                 _("Link")            
	      'XcosMenuMoveLink',             _("MoveLink ")       
	      'XcosMenuSelectLink',           _("SelectLink  ")    
	      'XcosMenuCtrlSelect',           _("CtrlSelect")      
	      'XcosMenuSelectRegion',         _("SelectRegion")    
	      'XcosMenuPopup',                _("Popup")           
	      'XcosMenuPlaceinDiagram',       _("PlaceinDiagram")  
	      'XcosMenuTkPopup',              _("TkPopup ")        
	      'XcosMenuBrowseTo',             _("BrowseTo")        
	      'XcosMenuPlaceinBrowser',       _("PlaceinBrowser")  
	      'XcosMenuSelectAll',            _("SelectAll") ]       

  scicos_menu = list(File,Diagram,Palette,Edit,View,Simulate,Format,Tools,Help,Unvisible);

  //**-----------------------------------------------------------------------
  //**----------------------------- Contextual menus ------------------------
  //**-----------------------------------------------------------------------
  
  //  Define the menus subset to appear in contextual menus
  scicos_lhb_list = list();
  
  //** Contextual menu for a valid object inside the CURRENT Scicos
  //Window here only Ids are used
  scicos_lhb_list(1) = list('XcosMenuOpenSet',..
			     'XcosMenuCut',..
			     'XcosMenuCopy',..
                             'XcosMenuDelete',..
			     'XcosMenuSmartMove',..
			     'XcosMenuMove',..
			     'XcosMenuDuplicate',..
			     'XcosMenuLink',..
			     'XcosMenuAlign',..
			     'XcosMenuFlip',..
			     'XcosMenuRotateLeft',..
			     'XcosMenuRotateRight',..
			     list( 'XcosMenuProperties',..
			           'XcosMenuResize',..
			           'XcosMenuIcon',..
			           'XcosMenuIconEditor',..
			           'XcosMenuColor',..
			           'XcosMenuLabel',..
			           'XcosMenuGetInfo',..
                                   'XcosMenuDetails',..
			           'XcosMenuIdentification',..
			           'XcosMenuBlockDocumentation'),..
			     'XcosMenuCodeGeneration',..
                             list( 'XcosMenuMask',..
                                   'XcosMenuCreateMask',..
                                   'XcosMenuRemoveMask',..
                                   'XcosMenuCustomizeMask',..
                                   'XcosMenuSaveBlockGUI'),..
			     list('XcosMenuAtomic',..
			          'XcosMenuCreateAtomic',..
			          'XcosMenuRemoveAtomic'),..
			     'XcosMenuRegionToSuper',..
			     'XcosMenuRegionToPalette',..
			     'XcosMenuHelp');

  //** Contextual menu  in the void of the CURRENT Scicos Window
  scicos_lhb_list(2) = list('XcosMenuUndo',..
                             'XcosMenuPaste',..
			     'XcosMenuPalettes',..
			     'XcosMenuContext',..
			     'XcosMenuAddNewBlock',..
			     'XcosMenuReplot',..
			     'XcosMenuSave',..
			     'XcosMenuSaveAs',..
			     'XcosMenuExport',..
			     'XcosMenuQuit',..
			     'XcosMenuBackgroundColor',..
			     'XcosMenuShowBlockShadow'  ,..
			     'XcosMenuZoomIn' ,..
			     'XcosMenuZoomOut',..
                             'XcosMenuPalettesTree',..
			     'XcosMenuRegionToSuper',..
			     'XcosMenuRegionToPalette',..
                             'XcosMenuBrowser',..
                             'XcosMenuDetails',..
			     'XcosMenuHelp');

  //** //** Contextual menu for a valid object inside a PALETTE or 
  //**                 not a current Scicos window
  //**
  scicos_lhb_list(3) = list('XcosMenuCopy',..
			     'XcosMenuHelp');

  //**-----------------------------------------------------------------------
  //**----------------------------- ShortCuts       ------------------------
  //**-----------------------------------------------------------------------
  scicos_short=['a','XcosMenuAlign'
		 'd','XcosMenuDelete';
		 'c','XcosMenuDuplicate';
		 'm','XcosMenuMove';
		 'z','XcosMenuSmartMove';
                 'w','XcosMenuIdentification';
		 'u','XcosMenuUndo';
		 'f','XcosMenuFlip';
		 't','XcosMenuRotateLeft';
		 'T','XcosMenuRotateRight';
		 'o','XcosMenuOpenSet';
		 's','XcosMenuSave';
		 'i','XcosMenuGetInfo';
		 'r','XcosMenuReplot';
		 'l','XcosMenuLink';
		 'q','XcosMenuQuit';
		 '-','XcosMenuZoomOut';
		 '+','XcosMenuZoomIn';
		 '=','XcosMenuFitToFigure';
		 'g','XcosMenuGrid';
		 'h','XcosMenuHelp']
  
  
   //** Scicos "xinfo" messages ===========================================
  //**
  //** "CmenuTypeOneVector" store the list of the commands/function to be called that require both 'Cmenu' AND 'pt'
  //** menus of type 1 (require %pt)
  CmenuTypeOneVector =[
      'RegionToSuper',         _("Press lef mouse button, drag region and release (right button to cancel)");
      'RegionToPalette',       _("Press lef mouse button, drag region and release (right button to cancel)");
      'SmartMove',             _("Click object to move, drag and click (left to fix, right to cancel)");
      'Move',                  _("Click object to move, drag and click (left to fix, right to cancel)");
      'Duplicate',             _("Click on the object to duplicate, drag, click (left to copy, right to cancel)");
      'Align',                 _("Click on an a port, click on a port of object to be moved");
      'Link',                  _("Drag, click left for final or intermediate points or right to cancel");
      'SmartLink',             _("Drag, click left for final or intermediate points or right to cancel");
      'Delete',                _("Delete: Click on the object to delete");
      'Cut',                   _("Cut: Click on the object to cut");
      'Flip',                  _("Click on block to be flipped");
      'RotateLeft',            _("Click on block to be turned left");
      'RotateRight',           _("Click on block to be turned right");
      'OpenSet',               _("Click to open block or make a link");
      'MoveLink',              ''                                  ; //** hidden commands
      'SelectLink',            ''                                  ;
      'CtrlSelect',            ''                                  ;
      'SelectRegion',          ''                                  ;
      'Popup',                 ''                                  ;
      'Label',                 _("Click block to label");
      'GetInfo',               _("Click on object  to get information on it");
      'CodeGeneration',        _("Click on a Super Block (without activation output) to obtain a coded block!") ;
      'Icon',                  _("Click on block to edit its icon");
      'Color',                 _("Click on object to choose its color");
      'Identification',        _("Click on an object to set or get identification");
      'Resize',                _("Click block to resize");
      'BlockDocumentation',    _("Click on a block to set or get it''s documentation")];

  
endfunction
