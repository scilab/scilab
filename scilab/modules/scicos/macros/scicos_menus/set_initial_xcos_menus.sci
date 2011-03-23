//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <Serge.Steer@inria.fr>
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
// See the file ../license.txt
//

function [scicos_menu,scicos_lhb_list, scicos_short,CmenuTypeOneVector]= set_initial_xcos_menus()
//** Sept 2009, Serge Steer
//  a better data organization should be made to avoid redundancy and for
//  a better efficiency (see scicos_menubar, XcosPopup)
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
	      'XcosMenuQuit',                 _("Close");
	      'XcosMenuExitScicos',           _("Exit")];

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
	      'XcosMenuBlockDoc'              _("Block Documentation");
	      'XcosMenuLabel',                _("Label")];

  View     = ['XcosMenuView',                 _("View");
              'XcosMenuZoomIn',               _("Zoom in");
	      'XcosMenuZoomOut',              _("Zoom out");
              'XcosMenuFitToFigure',          _("Fit diagram to figure");
  	      'XcosMenuDefaultWindow',        _("Default window parameters");
              'XcosMenuDefinedVars',          _("Available Parameters");
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
              'XcosMenuSetGrid',              _("Set grid");
   	      'XcosMenuAddColor',             _("Add color");
	      'XcosMenuDefaultLinkCols',      _("Default link colors");
	      'XcosMenuColor',                _("Color");
	      'XcosMenuBackgroundColor',      _("Background color");
	      'XcosMenuShowBlockShadow',      _("Show Block Shadow");
	      'XcosMenuSetLinkMode',         _("Set link edition mode");
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
              'XcosMenuScicosDoc',            _("Scicos Documentation");
	      'XcosMenuDemos',                _("Demos");
	      'XcosMenuAboutScicos',          _("About Scicos")];

  //The Unvisible "Menus" are actions used internally but not displayed
  //neither in the menubar nor in contextual menus
  //le nom MoveLink n'est pas correct l'action permet de preciser les
  //actions a realiser (a revoir)
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
	      'XcosMenuSelectAll',            _("SelectAll") 
	      'XcosMenuProperties'            _("Properties") //added for XcosGetMenuLabel
	      'XcosMenuMask'                  _("Mask") //added for XcosGetMenuLabel
	      'XcosMenuAtomic'                _("Atomic")]//added for XcosGetMenuLabel

  scicos_menu = list(File,Diagram,Palette,Edit,View,Simulate,Format,Tools,Help,Unvisible);

  //**-----------------------------------------------------------------------
  //**----------------------------- Contextual menus ------------------------
  //**-----------------------------------------------------------------------
  
  //  Define the menus subset to appear in contextual menus
  scicos_lhb_list = list();
  
  //** Contextual menu for a Scicos object in a Scicos Edition window
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
			           'XcosMenuBlockDoc'),..
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

  //** Contextual menu  in the void of the Scicos edition Window
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

  //** //** Contextual menu for a Scicos object inside a PALETTE 
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
      'XcosMenuRegionToSuper',         _("Press left mouse button, drag region and release (right button to cancel)");
      'XcosMenuRegionToPalette',       _("Press left mouse button, drag region and release (right button to cancel)");
      'XcosMenuSmartMove',             _("Click object to move, drag and click (left to fix, right to cancel)");
      'XcosMenuMove',                  _("Click object to move, drag and click (left to fix, right to cancel)");
      'XcosMenuDuplicate',             _("Click on the object to duplicate, drag, click (left to copy, right to cancel)");
      'XcosMenuAlign',                 _("Click on a port of object to be moved");
      'XcosMenuLink',                  _("Drag, click left for final or intermediate points or right to cancel");
      'XcosMenuSmartLink',             _("Drag, click left for final or intermediate points or right to cancel");
      'XcosMenuDelete',                _("Delete: Click on the object to delete");
      'XcosMenuCut',                   _("Cut: Click on the object to cut");
      'XcosMenuFlip',                  _("Click on block to be flipped");
      'XcosMenuRotateLeft',            _("Click on block to be turned left");
      'XcosMenuRotateRight',           _("Click on block to be turned right");
      'XcosMenuOpenSet',               _("Click to open block or make a link");
      'XcosMenuMoveLink',              ''                                  ; //** hidden commands
      'XcosMenuSelectLink',            ''                                  ;
      'XcosMenuCtrlSelect',            ''                                  ;
      'XcosMenuSelectRegion',          ''                                  ;
      'XcosMenuPopup',                 ''                                  ;
      'XcosMenuLabel',                 _("Click block to label");
      'XcosMenuGetInfo',               _("Click on object  to get information on it");
      'XcosMenuCodeGeneration',        _("Click on a Super Block (without activation output) to obtain a coded block!") ;
      'XcosMenuIcon',                  _("Click on block to edit its icon");
      'XcosMenuIconEditor',            _("Click on block to  open graphic icon editor");
      'XcosMenuColor',                 _("Click on object to choose its color");
      'XcosMenuIdentification',        _("Click on an object to set or get identification");
      'XcosMenuResize',                _("Click block to resize");
      'XcosMenuBlockDoc',              _("Click on a block to set or get its documentation");
      'XcosMenuCreateMask',            _("Click on a super block");
      'XcosMenuRemoveMask',            _("Click on a super block")
      'XcosMenuCustomizeMask',         _("Click on a super block")
      'XcosMenuCreateAtomic',          _("Click on a super block");
      'XcosMenuRemoveAtomic',          _("Click on a super block")
		      ];

  CmenuTypeOneVector=stripblanks(CmenuTypeOneVector)
endfunction
