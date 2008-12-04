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
//
//**
//** INRIA
//** Comments by Simone Mannori
//** 04 Sep 2006 : updated file with the last "scicos_new" mods
//**
//** 30 August 2007: some data structure are moved from scicos.sci here.
//**
//** 23 May 2008 : final porting on Scilab 5  
//**
//** 04 Nov 2008 : the "Grid" function is temporally disabled 
//**
//** N.B. : This function is called from "SCI/modules/scicos/etc/scicos.start" 
//**        at Scilab's startup IF the "scicos" module is active in
//**        "SCI/etc/modules.xml" file :)

function [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
          %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
          %scicos_lhb_list, %CmenuTypeOneVector,%scicos_gif, ...
          %scicos_contrib] = initial_scicos_tables()

  %scicos_display_mode = 0 ; //** obsolete: with the new graphics the display mode
                             //** is fixed

  %scicos_with_grid = %f; //** define grid

  //** %scs_wgrid(1:2) : space of grid
  //** %scs_wgrid(3) : color
  %scs_wgrid = [10;10;12]; //**define aspect of grid

  //Scicos palettes names ============================================================
  //**
  pal_names = ['Sources'    ;'Sinks'           ;'Linear'          ;'Non_linear';
               'Matrix'     ;'Integer'         ;'Events'          ;'Threshold' ;
               'Others'     ;'Branching'       ;'Electrical'      ;'ThermoHydraulics';
               'OldBlocks'  ;'DemoBlocks'];

  scicos_pal = [pal_names  'SCI/modules/scicos/macros/scicos_scicos/'+pal_names+'.cosf']

  %scicos_gif = SCI+"/modules/scicos/help/scicos_doc/man/gif_icons/";

  %scicos_contrib = [];

  //Scicos palettes libs list ========================================================
  //**
  scicos_pal_libs = ['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	             'MatrixOp','NonLinear','Sources','Electrical','Hydraulics','PDE',...
	             'IntegerOp'];

  //Scicos Menu definitions==========================================================
  //**

  File     = ['File',..
              'New',..
	      'Open',..
	      'Save',..
	      'Save As',..
	      'Save as Interf Func',..
  	      'Export',..
  	      'Export All',..
              'Exit Scicos',..
	      'Quit' ];

  Diagram  = ['Diagram',..
              'Replot',..
	      'Rename',..
	      'Purge',..
	      'Set Diagram Info',..
	      'Region to Super Block',..
              'Up To Main Diagram',..
	      'Context' ];

  Palette  = ['Palette',..
              'Palettes',..
	      'Pal editor',..
	      'Load as Palette',..
	      'Save as Palette',..
              'Pal Tree']; 

  Edit     = ['Edit',..
              'Undo',..
	      'Cut',..
	      'Copy',..
	      'Paste',..
	      'Duplicate',..
	      'Delete',..
	      'Move',..
	      'Smart Move',..
	      'Align',..
	      'Flip',..
	      'Rotate Left',..
	      'Rotate Right',..
	      'Add new block',..
	      'Block Documentation'..
	      'Label'];

  View     = ['View',..
              'Zoom in',..
	      'Zoom out',..
              'Fit diagram to figure',..
  	      'Default window parameters',..
              'Available Parameters'];
//**              'Grid'];

  Simulate = ['Simulate',..
              'Setup',..
	      'Compile',..
	      'Eval',..
	      'Debug Level',..
	      'Run'];

  Format   = ['Format',..
   	      'Add color',..
	      'Default link colors',..
	      'Color',..
	      'Background color',..
	      'Show Block Shadow',..
	      'Resize',..
	      'Identification',..
	      'ID fonts',..
	      'Icon',..
	      'Icon Editor'];
//**              'Set grid',..

  Tools    = ['Tools',..
              'Create Mask',..
              'Remove Mask',..
              'Customize Mask',..
              'Save Block GUI',..
              'Get Info',..
              'Details',..
              'Browser',..
	      'Code Generation',..
	      'Shortcuts',..
	      'Activate Scilab Window',..
	      'Calc'];

  Help     = ['?',..
              'Help',..
              'Scicos Documentation',..
	      'Demos',..
	      'About Scicos'];

  %scicos_menu = list(File,Diagram,Palette,Edit,View,Simulate,Format,Tools,Help);

  //Scicos Right Mouse Button Menu ===========================================
  //**

    //**----------------------------- RIGHT MOUSE BUTTON MENUS (Popup) ------------------------
  //** Right Mouse Button Menus:
  //**        "%scicos_lhb_list" data structure initialization
  //**
  //** Create an empty list() data structure
  %scicos_lhb_list = list();

  //** Fill the data structure with menu/command/functions definitions

  //** state_var = 1 : right click over a valid object inside the CURRENT Scicos Window
  %scicos_lhb_list(1) = list('Open/Set',..
			     'Cut',..
			     'Copy',..
                             'Paste',..
                             'Delete',..
			     'Smart Move',..
			     'Move',..
			     'Duplicate',..
			     'Link',..
			     'Align',..
			     'Flip',..
			     'Rotate Left',..
			     'Rotate Right',..
			     list( 'Properties',..
			           'Resize',..
			           'Icon',..
			           'Icon Editor',..
			           'Color',..
			           'Label',..
			           'Get Info',..
                                   'Details',..
			           'Identification',..
			           'Block Documentation'),..
			           'Code Generation',..
                             list( 'Mask',..
                                   'Create Mask',..
                                   'Remove Mask',..
                                   'Customize Mask',..
                                   'Save Block GUI'),..
			           'Region to Super Block',..
			           'Help');

  //** state_var = 2 : right click in the void of the CURRENT Scicos Window
  %scicos_lhb_list(2) = list('Undo',..
                             'Paste',..
			     'Palettes',..
			     'Context',..
			     'Add new block',..
			     'Replot',..
			     'Save',..
			     'Save As',..
			     'Export',..
			     'Quit',..
			     'Background color',..
			     'Show Block Shadow'  ,..
			     'Zoom in' ,..
			     'Zoom out',..
                             'Pal Tree',..
			     'Region to Super Block',..
                             'Browser',..
                             'Details',..
			     'Help');

  //** state_var = 3 : right click over a valid object inside a PALETTE or
  //**                 not a current Scicos window
  //**
  %scicos_lhb_list(3) = list('Copy',..
			     'Help');
 //**------------------------------

  //Scicos Shortcuts definitions===========================================
  //** single key shortcut "quick menu"
  %scicos_short=['a','Align'
		 'd','Delete';
		 'c','Duplicate';
		 'm','Move';
		 'z','Smart Move';
                 'w','Identification';
		 'u','Undo';
		 'f','Flip';
		 't','Rotate Left';
		 'T','Rotate Right';
		 'o','Open/Set';
		 's','Save';
		 'i','Get Info';
		 'r','Replot';
		 'l','Link';
		 'q','Quit';
		 '-','Zoom out';
		 '+','Zoom in';
		 '=','Fit diagram to figure';
		 'h','Help']

//** 		 'g','Grid'; //** it will be reactivated later 
            
  //Scicos Modelica librabry path definitions==============================
  modelica_libs='SCI/modules/scicos_blocks/macros/'+['Electrical','Hydraulics'];

  //add TMPDIR/Modelica for generic modelica blocks
  status=mkdir(TMPDIR,'Modelica');
  if isdir(TMPDIR+'/Modelica') then
    modelica_libs=[modelica_libs,TMPDIR+'/Modelica'];
  end

  //** Scicos "xinfo" messages ===========================================
  //**
  //** "%CmenuTypeOneVector" store the list of the commands/function to be called that require both 'Cmenu' AND '%pt'
  //** menus of type 1 (require %pt)
  %CmenuTypeOneVector =..
     ['Region to Super Block', "Press lef mouse button, drag region and release (right button to cancel)";
      'Smart Move',            "Click object to move, drag and click (left to fix, right to cancel)";
      'Move',                  "Click object to move, drag and click (left to fix, right to cancel)";
      'Duplicate',             "Click on the object to duplicate, drag, click (left to copy, right to cancel)";
      'Align',                 "Click on an a port, click on a port of object to be moved";
      'Link',                  "Drag, click left for final or intermediate points or right to cancel";
      'Delete',                "Delete: Click on the object to delete";
      'Flip',                  "Click on block to be flipped"      ;
      'Rotate Left',           "Click on block to be turned left"  ;
      'Rotate Right',          "Click on block to be turned right" ;
      'Open/Set',              "Click to open block or make a link";
      'MoveLink',              ''                                  ; //** hidden commands
      'SelectLink',            ''                                  ;
      'CtrlSelect',            ''                                  ;
      'SelectRegion',          ''                                  ;
      'Popup',                 ''                                  ;
      'Label',                 "Click block to label";
      'Get Info',              "Click on object  to get information on it";
      'Code Generation',       "Click on a Super Block (without activation output) to obtain a coded block!" ;
      'Icon',                  "Click on block to edit its icon";
      'Color',                 "Click on object to choose its color";
      'Identification',        "Click on an object to set or get identification";
      'Resize',                "Click block to resize";
      'Block Documentation',   "Click on a block to set or get it''s documentation"
     ]

  //**-----------------------------

  //Scicos Help definitions===========================================
  //** 08/09/07 : Alan's patch

  deff('%scicos_help=scicos_help(%scicos_help,entry,doc)',...
       ['ind_entry=find(getfield(1,%scicos_help)==entry)';
        'if ind_entry==[] then '+...
        'setfield(1,[%scicos_help(1),entry],%scicos_help), '+...
        'setfield(lstsize(%scicos_help)+1,doc,%scicos_help), '+...
        'else '+...
        'setfield(ind_entry,doc,%scicos_help), '+...
        'end']);

  //Enter here Menus Help definitions=========================
  %scs_help_menu=tlist(['menu']);

  //****** File Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'New',...
               [" Clicking on the New menu item loads an empty diagram in the";
                " active editor Scicos window. If the previous content of the";
                " window is not saved, it will be lost."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Open',...
               [" Select the Open menu item to load an ASCII or binary file";
                " containing a saved block diagram or palette.";
                " A dialog box allows user choosing the file."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rename',...
               [" This menu allows to change the diagram name. An editable";
                "  dialog box opens."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save',...
               [" Select the save menu item to save the block diagram";
                " in a binary file already selected by a previous";
                " select the Save As menu item. If you select this";
                " menu item and you have never clicked on the Save As";
                " menu item, the diagram is saved in the current directory";
                " as ""window_name"".cos where ""window_name"" is the name";
                " of the window appearing on top of the window (usually";
                " Untitled or Super Block).";
                " ";
                " The .cos binary files are machine independent."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save As',...
               [" Select the Save As menu item to save the block diagram or";
                " palette in a file. A dialog box allows choosing the file";
                " which must have a .cos or .cosf extension. The diagram";
                " takes the name of the file (without the extension).";
                " ";
                " If extension is "".cosf"" an ASCII  formatted save is performed";
                " instead of binary save. Formatted save is slower than regular ";
                " save."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save as Interf Func',...
               [" Select ""the Save as Interf. Func."" menu item to save the ";
                " diagram as a new Scicos block. A Scilab function is generated";
                " and saved in a file with "".sci"" extension. File name and path";
                " are to be set in a ""File menu"" dialog."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Export',...
               [" This menu is used to export a figure of the "
                " current Scicos diagram. The export can be done"
                " directly in postscript format or done first in a"
                " graphic window to export in a second step in all"
                " the format that scilab can provide."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Export All',...
               [" This menu is used to export Scicos diagrams in postscript format."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Exit Scicos',...
               [" Click on the Exit Scicos menu item to close Scicos.";
                " That menu allows to return to Scilab session ";
                " even if the current diagram is a Super block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Quit',...
               [" Click on the Quit menu item to close current diagram. ";
                " If current diagram is not a Super block, Exit menu closes ";
                " Scicos and returns to Scilab session. Save your diagram ";
                " or/and your palette before leaving.";
                " ";
                " File/Close menu has the same effect."])

  //**** Diagram Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Replot',...
               [" Select the Replot menu item to replot the content of";
                " the graphics window. Graphics window stores complete";
                " history of the editing session in memory.";
                " ";
                " Replot is useful for ''cleaning'' this memory."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Purge',...
               [" Select the Purge menu item to get a clean data structure.";
                " If diagram has been hugely modified many deleted blocks";
                " may remain in the data structure. It may be  useful to";
                " suppress then before saving."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Set Diagram Info',...
               [" This menu allows to set user''s diagram information.";
                " This info is stored in the diagram data structure";
                " and may be used for diagram documentation.";
                " ";
                " Information format may be redefined by user."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Region to Super Block',...
               [" This menu allows to transform a rectangular region of the";
                " current diagram by a super block.";
                " Click  on a corner of the region, drag an click left to";
                " fix the region (left click cancels selection).";
                " ";
                " Region is replaced by a super block and links are redrawn."])


   %scs_help_menu=scicos_help(%scs_help_menu,...
               'Up To Main Diagram',...
                ["Opens, if closed, and puts the focus on the main diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Context',...
               [" When you select this menu item you obtain a dialog to enter ";
                " Scilab instructions for defining symbolic Scicos parameters ";
                " used in block definitions or to do whatever you want.";
                " ";
                " These instructions will be evaluated each time the diagram ";
                " is loaded.";
                " ";
                " If you change the value of a symbolic Scicos parameters in ";
                " the context, all the blocks are updated (Eval is performed)."])

  //**** Palette Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Palettes',...
               [" Select the Palettes menu item to open a predefined palette."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Pal editor',...
               [" Pal editor allows you to define and reorder the list of palettes."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Load as Palette',...
               [" Select this Load menu item to load an ASCII or binary file";
                " containing a saved block diagram as a palette.";
                " A dialog box allows user choosing the file."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save as Palette',...
               [" Select the Save as Palette menu item to save the block diagram"
                " as a palette in a file. A dialog box allows choosing ";
                " the file which must have a .cos or .cosf extension. The palette";
                " takes the name of the file (without the extension).";
                " ";
                " If extension is "".cosf"" an ASCII  formatted save is performed";
                " instead of binary save. It may take a while.";
                " ";
                " .scilab user file is updated if necessary."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Pal Tree',...
               ["An alternative to Palettes menu for accessing palettes."])

  //****** Edit Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Undo',...
               [" Select the Undo menu item to undo the last edit operation.";
                " It is not possible to undo more!."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Cut',...
               [" Cut is used to remove the selected object from the diagram";
                " and keep a copy in the clipboard if the object is a block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Copy',...
               [" Copy is used to place a copy of the selected object";
                " in the clipboard if the object is a block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Paste',...
               [" Paste places the object in the Clipboard in the diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Duplicate',...
               [" To duplicate a block in the active editor Scicos window";
                " select first the Duplicate menu item, then";
                " click (with left button) on the to-be-copied block";
                " in Scicos windows or in a palette), and";
                " finally click left where you want the copy";
                " be placed in the active editor Scicos window.";
                " ";
                " The lower left corner of the block is placed";
                " at the selected point.";
                " ";
                " Right click cancel the copy action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Delete',...
               [" To delete  blocks or a links, select first the Delete";
                " menu item, then click successively on the selected objects";
                " (with left button).";
                " ";
                " When you delete a block all links connected to it";
                " are deleted as well."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Move',...
               [" To move a block in the active editor Scicos window";
                " or in edited palette,";
                " select first the Move menu item, ";
                " then click on the selected block, link segment or link";
                " corner, drag the mouse to the desired new block position ";
                " and click left again to fix the position.";
                " ";
                " Right click cancel the move action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Smart Move',...
               [" To move a block in  the active editor Scicos window";
                " or in edited palette keeping horizontal and vertical";
                " links, select first the ""Smart Move"" menu item, ";
                " then click on the selected block, link segment or link";
                " corner, drag the mouse to the desired new  position ";
                " and click left again to fix the position.";
                " ";
                " Right click cancel the move action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Align',...
               [" To obtain nice diagrams, you can align ports of";
                " different blocks, vertically and horizontally.";
                " Select first the Align menu item, then click on the first";
                " port and finally on the second port.";
                " The block corresponding to the second port is moved.";
                " ";
                " A connected block cannot be aligned."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Flip',...
               [" To reverse the positions of the (regular) inputs";
                " and outputs of a block placed on its sides,";
                " select the Flip menu item first and then click on the";
                " selected block. This does not affect the order,";
                " nor the position of the input and output event";
                " ports which are numbered from left to right."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rotate Left',...
               [" Rotate Left allows to turn a block on the Left.";
                " Each time the block is turned left, his angle is";
                " decresead of 45 degrees.";
                " ";
                " If no blocks or many blocks are selected, this is";
                " the block under the mouse pointer which turns."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rotate Right',...
               [" Rotate Right allows to turn a block on the right.";
                " Each time the block is turned right, his angle is";
                " incresead of 45 degrees.";
                " ";
                " If no blocks or many blocks are selected, this is";
                " the block under the mouse pointer which turns."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Add new block',...
               [" To add a newly defined block to the current palette or diagram";
                " select first this menu item. A dialog box will pop up ";
                " asking for the name of the GUI function associated ";
                " with the block. If this function is not already loaded";
                " it was search in the current directory. The user may then";
                " click at the desired position of the block icon."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Block Documentation',...
               [" This menu allows to set or get documentation for a block.";
                " ";
                " Selecting this menu and clicking on a block opens an";
                " editable dialog box."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Label',...
               [" To add a label to block, select first this menu item, click next";
                " on the desired block. A dialog appear that allows you to enter ";
                " the desired label.";
                " Labels are used to import data from a block in an other one."])

  //****** View Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Zoom in',...
               [" When you select this menu item the diagram is zoomed in ";
                " by a factor of 10%."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Zoom out',...
               [" When you select this menu item the diagram is zoomed out ";
                " by a factor of 10%."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Fit diagram to figure',...
               [" When you select this menu item";
                " the diagram is fit to the size";
                " of the current window."])


   %scs_help_menu=scicos_help(%scs_help_menu,...
                'Available Parameters',...
                ["This menu shows the list of symbolic parameters defined";
                 "in various contexts and available for use in the definition";
                 "of block parameters in this diagram. The values of the parameters"
                 "are also displayed (unless the size is too big."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Grid',...
               [" Show/hide a grid in the current diagram."])

  //**** Simulate Menu ****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Setup',...
               [" In the main Scicos window, clicking on the Setup menu item";
                " invokes a dialog box that allows you to change ";
                " integration parameters: ";
                " ";
                "   - Final integration time (integration ends at";
                "           this time, it always starts from 0), ";
                " ";
                "   - Real time scaling (forces real time simulation";
                "           by setting Scicos unit of time to 1 second), ";
                " ";
                "   - Absolute and relative error tolerances (solver properties), ";
                " ";
                "   - Time tolerance (the smallest time interval for which ";
                "         the ode solver is used to update continuous states), ";
                " ";
                "   - Max integration time interval (the maximum time interval";
                "     for each call to solver, it must be reduced if the error";
                "     message ""too many calls"" is encountered), ";
                " ";
                "   - Solver (choose the numerical solver to be used), ";
                " ";
                "   Max step size (max time step taken by solver)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Compile',...
               [" Select the Compile menu item to compile the block diagram.";
                " This menu item need never be used since compilation is";
                " performed automatically, if necessary, before";
                " the beginning of every simulation (Run menu item).";
                " ";
                " Normally, a new compilation is not needed if only";
                " system parameters and internal states are modified.";
                " In some cases however these modifications are not";
                " correctly updated and a manual compilation may be";
                " needed before a Restart or a Continue.";
                " Please report if you encounter such a case."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Eval',...
               [" All dialogs user answers may be Scilab instructions.";
                " They are evaluated immediately and stored as character strings.";
                " Select this menu item to have them re-evaluated according to";
                " new values of underlying Scilab variables. ";
                " ";
                " These underlying Scilab variables may be user global variables";
                " defined before Scicos was launch, They may also be defined in";
                " by the Scicos context (see Context menu item)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Debug Level',...
               [" Set the debug level: 0 (no debugging), 1 (light";
                " debugging information printout), 2 (more information";
                " printout and execution of Debug Block if any in diagram),"
                " 3 (silent debugging mode)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Run',...
               [" Select the Run menu item to start the simulation.";
                " If the system has already been simulated, a";
                " dialog box appears where you can choose to Continue,";
                " Restart or End the simulation.";
                " ";
                " You may interrupt the simulation by clicking on the ";
                " ""stop"" button, change any of the block parameters";
                " and continue the simulation with the new values."])

  //***** Format Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Set grid',...
               [" Set the size and the color of ";
                " the grid for the current diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Add color',...
               [" This menu allows to add new color to the diagram private";
                " color map. Added colors are stored in the diagram data structure."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Default link colors',...
               [" This menu allows to change the default color for regular ";
                " and activation links."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Color',...
               [" To change the background color of an object, select first ";
                " this menu item, click next on the desired object. A dialog appear";
                " that allows you to choose the desired color."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Background color',...
               [" This menu allows to change the background and default";
                " foreground colors."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Show Block Shadow',...
               [" This menu allows to select 3D shape for blocks and ";
                " associated parameters."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Resize',...
               [" To change the size of a block, select first this menu item,";
                " click next on the desired block. A dialog appear that allows ";
                " you to change the width and/or height of the block shape."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'ID fonts',...
               [" This menu allows to change the font used to write in the block";
                " identifications (see ""Set block ID"" menu)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Identification',...
               [" This menu allows to set an identificator to a link or a block ";
                " block identificators are drawn under the block icon. Super blocks";
                " input/output ports identificators are replicated over the block";
                " shape ports. Links identificators are not displayed.";
                " ";
                " Selecting this menu and clicking on a block or links opens an";
                " editable dialog box."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Icon',...
               [" To change the icon of a block, select first this menu item,";
                " click next on the desired block. A dialog appear that allows ";
                " you to enter Scilab instructions used to draw the icon.";
                " ";
                " You may use the icon_editor menu to generate the Scilab";
                " instructions."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Icon Editor',...
               [" Icon Editor allows you to define graphically the icon of the block."])

  //***** Tools Menu ******/
  //***********************/
  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Create Mask',..
               [" This menu applies only to a Super Block, transforming it into a";
                " block that resembles a basic block with a standard dialog box. The block";
                " parameters are identifed automatically by searching all the symbolic";
                " parameters used in the definition of the block parameters inside the"
                " Super block that are not defined in the appropriate contexts within";
                " the Super Block;"])

  %scs_help_menu=scicos_help(%scs_help_menu,...
              'Remove Mask',..
              [" This menu applies only to an already masked block. It removes the mask";
               " and the Super Block within it can be recovered. Note that the icon of the";
               " Super Block remains the same as that of the masked block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
              'Customize Mask',...
              [" Creating a mask leads to a block with parameters that can be set by the";
               " block''s dialog box. The title and the description of the parameters in the";
               " dialog box can be set by this menu."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
              'Save Block GUI',..
              [" This operation can only be applied to a masked block (see Create Mask operation).";
               " The result, the interfacing function of this block, is saved in an .sci file, which ";
               " can be placed in any diagram/palette using the Add New Block operation. ";
               " The interfacing function is loaded autmatically for immediate use after the operation.";
               " ";
               " Note that the resulting block is not a masked block; it is a compiled super block."])


  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Get Info',...
               [" This menu allows user to obtain information on an object and on ";
                " its connection with other diagram objects.";
                " ";
                " Select this menu and click on an object. See also ''Details'' menu."])

   %scs_help_menu=scicos_help(%scs_help_menu,...
                'Details',...
                ["This item can be applied to a block, link or text. It can also be";
                 "applied to the diagram. In either case, it displays the content of";
                 "the Scilab structure defining the object."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Browser',...
               [" This menu opens a graphic window with a tree ";
                " representation of the super blocks hierarchy.";
                " Each node represents a Super Block.";
                " ";
                " Browser window is useful to open directly a Super Block";
                " everywhere in the hierarchy. To open the main diagram if it has";
                " been closed, use the ''Up to Main Diagram'' menu."]);

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Code Generation',...
               [" This menu allows to generate the simulation code associated with";
                " a discrete time Super Block.";
                "";
                " The code generation is obtained simply by selecting this menu and  ";
                " then the desired Super Block.";
                " ";
                " If the Super Block satisfies the required conditions, a dialog box";
                " pops up to ask for a block name, a directory where to put the";
                " generated files and for optional libraries requested by the linker.";
                " ";
                " Given this information the code is generated, compiled and linked with ";
                " Scilab. The Super Block is automatically replaced by a new block which ";
                " implements the generated code. It is then possible to run the modified ";
                " diagram. The code for standalone use is also generated."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Shortcuts',...
               [" Used to define personalized shortcuts."])


   %scs_help_menu=scicos_help(%scs_help_menu,...
                'Activate Scilab Window',...
                ["This item activates the Scilab window without closing the Scicos diagram.";
                 "You can use Scilab as usual and later return to editing your Scicos diagram."
                 "Note that if you activate the Scilab window with this menu without saving";
                 "your diagram, if you leave Scilab without returning to Scicos, your diagram";
                 "will be lost."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Calc',...
               [" When you select this menu item you switch Scilab to ";
                " the pause mode (see the help on pause).";
                " In the Scilab main window and you may enter Scilab instructions";
                " as usual. In most cases, user should use the ''Activate Scilab Window''";
                " menu instead. The ''Calc'' menu should only be used for advanced debugging."])

  //****** Help Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Help',...
               [" To get help on an object or menu buttons,";
                " select first Help menu item and then on ";
                " the selected object or menu item."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Scicos Documentation',...
               [" That item opens the html ";
                " browser."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Demos',...
               [" The Demos menu allows to open some";
                " examples of scicos diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'About Scicos',...
               ["About Scicos item display the";
                "current version of Scicos and";
                "gives some useful informations."])

//****** Old items ******/
//***********************/
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Display mode',...
//                ["Display mode can be 0 or 1. In mode 1, scicos editor uses the";
//                 "pixmap option of Scilab graphics to create   a backing";
//                 "store for the graphics used for displaying scicos diagrams. ";
//                 "By default, Display mode is 0. Under Windows operating system";
//                 "the backing store is handled in a different way. "])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Window',...
//                [" In the active editor Scicos window, clicking on the ";
//                 " Window menu item invokes a dialog box that allows you to change ";
//                 " window dimensions."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Focus',...
//                [" Focus menu allows to select a zone (click left, drag zone, click";
//                 " to select) which is focused on.";
//                 " To change focus back use ""Zoom in"" menu."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Shift',...
//                [" To shift the diagram to left, right, up or down,";
//                 " select this menu item, then click on the point you want ";
//                 " to appear in the middle of the graphics window. "])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Duplicate Region',...
//                [" To copy a region in the active editor Scicos window";
//                 " select first the Copy menu item, then";
//                 " click (with right button) on a corner of the desired";
//                 " region (in Scicos windows or in a palette), drag to ";
//                 " define the region, click to fix the region  and";
//                 " finally click left where you want the copy.";
//                 " to be placed in the active editor Scicos window.";
//                 " NOTE: If source diagram is big be patient, region selection ";
//                 " may take a while.";
//                 " ";
//                 " The lower left corner of the block is placed";
//                 " at the selected point.";
//                 " ";
//                 " Right click cancel the copy action."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Replace',...
//                [" To replace a block in the active editor Scicos window";
//                 " select first the Replace menu item, then";
//                 " select the replacement block (in Scicos window or in a";
//                 " palette), and  finally click on the to-be-replaced block."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Link',...
//                [" To connect an output port to an input port,";
//                 " select first the Link menu item, then click on the output";
//                 " port, drag, click left on each intermediate points";
//                 " and finally click left on the input port.";
//                 " ";
//                 " To split a link, select first  the Link menu item,";
//                 " then click left on the link where the split should be placed,";
//                 " drag, click left on each intermediate points";
//                 " and finally click left on the input port.";
//                 " ";
//                 " Right click cancel the link action";
//                 " ";
//                 " Only one link can go from and to a port.";
//                 " Link color can be changed directly by clicking";
//                 " on the link.";
//                 " ";
//                 " This menu remains active until user choose another one."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Delete Region',...
//                ["To delete a blocks in  a region, select first  the Delete Region";
//                 " menu item, then click  on a corner of the ";
//                 " desired region, drag to define the region, and click left to ";
//                 " fix the region. All connected links will be destroyed as";
//                 " well";
//                 " ";
//                 " Right click instead of left cancels the delete action."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Open/Set',...
//                [" To change the parameters of a regular block or link, or ";
//                 " to open a super block, select first ";
//                 " this menu item, click next on the desired object.";
//                 " A dialog or an editing window appears";
//                 " which allows you to modify object properties.";
//                 " ";
//                 " It is also possible to select a super block to open clicking";
//                 " on a node of the ""Navigator"" window."])
  //==========================================================

  //Enter here Scilab list Help definitions=========================
  %scs_help_scilst=tlist(['scilst',...
                          'diagram','params','model',...
                          'graphics','link','state',...
                          'sim','cpr','block']);

  //** define scs_help_scilst lists
  %scs_help_diagram=tlist(['lst']);    //** diagr
  %scs_help_params=tlist(['lst']);   //** params
  %scs_help_model=tlist(['lst']);    //** model
  %scs_help_graphics=tlist(['lst']); //** graphics
  %scs_help_link=tlist(['lst']);     //** link
  %scs_help_state=tlist(['lst']);    //** state
  %scs_help_sim=tlist(['lst']);      //** sim
  %scs_help_cpr=tlist(['lst']);      //** cpr
  %scs_help_block=tlist(['lst']);      //** block

  //******* cpr *******/
  //*******************/
  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'cpr',...
                 [" The Scilab object {scicos_cpr} contains "
                  " the result of the compilation."
                  " That structure includes fields {scicos_state}, {scicos_sim},"
                  " {cor} and {corinv}."
                  ""
                  " Size : 5.";
                  " Type : scilab list."])


  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'state',...
                 [" Scilab typed list of type {xcs}. It contains all"
                  " the states of the model, that is, everything than"
                  " can evolve during the simulation.";
                  " state contains {x}, {z}, {oz}, {iz}, "
                  " {tevts}, {evtspt}, {pointi} and {outtb}."
                  " ";
                  " Size : 9.";
                  " Type : scilab tlist."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'sim',...
                 [" Scilab typed list of type {scs}. It contains "
                  " static arrays coming from the result of the"
                  " compilation. That arrays does not evolve"
                  " during the simulation."
                  " ";
                  " Size : 33.";
                  " Type : scilab tlist."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'cor',...
                 [" It is a list with same recursive structure"
                  " as scs_m. Each leaf contains the index of "
                  " associated block in sim data structure. "
                  " ";
                  " Size : number of objects in scs_m.";
                  " Type : scilab list."])


  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'corinv',...
                 [" corinv(i) is the path of i th block"
                  " defined in sim data structure in the"
                  " scs_m data structure."
                  " ";
                  " Size : number of blocks in the compiled structre.";
                  " Type : scilab list."])

  //******* scicos_block *******/
  //****************************/
  %scs_help_block=scicos_help(%scs_help_block,...
                 'block',...
                 [" Basic  structure that define a Scicos block.";
                  " That structure includes fields {scicos_graphics}, {scicos_model},"
                  " {gui} and {doc}."
                  ""
                  " Size : 5.";
                  " Type : scilab list."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'graphics',...
                 [" Scilab object including"
                  " graphical information concerning"
                  " the features of the block.";
                  " "
                  " Size : 14.";
                  " Type : scilab list."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'model',...
                 [" Scilab list that contains the features";
                  " of the block used for the compilation.";
                  " ";
                  " Size : 23.";
                  " Type : Scilab list."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'gui',...
                 [" The name of the Scilab GUI function associated"
                  " with the block."
                  " ";
                  " Size : 1.";
                  " Type : string."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'doc',...
                 [" Field used for documentation"
                  " of the block"
                  " ";
                  " Size : 1.";
                  " Type : string."])

  //****** scicos_diagram ******/
  //****************************/
  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'diagram',...
                 [" ";
                  " Size : 4.";
                  " Type : scilab list."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'props',...
                 [" Diagram properties.";
                  " This entry contains various informations such";
                  " some main diagram initials values.";
                  " ";
                  " This variable is a tlist of type {scicos_params} and";
                  " contains {wpar}, {title}, {tol}, {tf}, {context},";
                  " {options} and {doc}.";
                  " ";
                  " Size : 11.";
                  " Type : Scilab tlist of type {scicos_params}."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'objs',...
                 [" List of objects included in the Scicos diagram.";
                  " The objects used in scicos are {scicos_block}, {scicos_link} and Text.";
                  " The objects can also be deleted object data structure.";
                  " Deleted object data structure is marked list(''Deleted'').";
                  " ";
                  " ";
                  " Size : total number of objects in the diagram.";
                  " Type : Scilab tlist of type {scicos_block}, {scicos_link} or Text."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'version',...
                 [" A string that gives the version of the Scicos diagram.";
                  " This is used to provide compatibility with old diagram."
                  " Note that you can get the current version of Scicos by";
                  " using the entry ''About scicos'' in the help menu"
                  " or by using the function get_scicos_version()."
                  ""
                  " Size : 1.";
                  " Type : String."])

  //****** scicos_params ******/
  //***************************/
  %scs_help_params=scicos_help(%scs_help_params,...
                 'params',...
                 [" ";
                  " Size : 11.";
                  " Type : scilab list."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'wpar',...
                 [" This vector is not currently used."
                  " It may be used in the future to code "
                  " window sizes of the editor.";
                  " ";
                  " Size : 6.";
                  " Type : column vector or real."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'title',...
                 [" Vector of character strings, where the first one is the"
                  " diagram title and default name of save file name, "
                  " and the second one is the path of the directory of "
                  " the file name.";
                  " ";
                  " Size : 2.";
                  " Type : row vector of strings."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tol',...
                 ["";
                  " A vector containing simulation parameters including"
                  " various tolerances used by the solver: ";
                  " ";
                  "  -- {atol}   : Integrator absolute tolerance for the numerical solver.";
                  " ";
                  "  -- {rtol}   : Integrator relative tolerance for the numerical solver.";
                  " ";
                  "  -- {ttol}   : Tolerance on time."
                  "                If an integration period is less";
                  "                than {ttol}, the numerical solver is not called."
                  " ";
                  "  -- {deltat} : Maximum integration time interval."
                  "                If an integration period is larger than"
                  "                {deltat}, the numerical solver is called more than"
                  "                once in such a way that for each call the integration"
                  "                period remains below {deltat}";
                  " ";
                  "  -- {scale}  : Real-time scaling; the value 0 corresponds to no"
                  "                real-time scaling."
                  "                It associates a Scicos simulation time to the real"
                  "                time in seconds."
                  "                A value of 1 means that each Scicos unit of time"
                  "                corresponds to one second.";
                  " ";
                  "  -- {solver} : Choice of numerical solver."
                  "                The value 0 implies an ODE solver and 100"
                  "                implies a DAE solver."
                  " ";
                  "  -- {hmax}   : Maximum step size for the numerical solver.";
                  "                0 means no limit.";
                  " ";
                  " Size : 7.";
                  " Type : column vector of real."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tf',...
                 [" Final time simulation."
                  " The simulation stops at this time."
                  " The default value is 100000.";
                  " ";
                  " Size : 1.";
                  " Type : real."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'context',...
                 [" A vector of strings containing Scilab instructions"
                  " defining Scilab variables to be used inside block''s dialog box"
                  " as symbolic parameters."
                  " All valid Scilab instructions can be used and "
                  " also comments.";
                  " ";
                  " Size : number of lines of the context.";
                  " Type : column vector of strings."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void1',...
                 [" unused field.";
                  " ";
                  " Size : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'options',...
                 [" Scilab object of type {scsopt} defining"
                  " graphical properties of the editor such"
                  " as background color and link color."
                  " ";
                  " The fields are the following:";
                  " ";
                  "  -- {3D}         : A list with two entries. The first one is a boolean"
                  "                    indicating whether or not blocks should have 3D aspect."
                  "                    The second entry indicates the color in the current colormap"
                  "                    to be used to create the 3D effect."
                  "                    The default is 33 which corresponds to gray added by"
                  "                    Scicos to the standard colormap, which contains 32 colors."
                  "                    The default value is list(%t,33)."
                  " ";
                  "  -- {Background} : Vector with two entries: background and"
                  "                    foreground colors."
                  "                    The default value is [8,1]."
                  " ";
                  "  -- {link}       : Default link colors for regular and activation links."
                  "                    These colors are used only at link construction."
                  "                    Changing them does not affect already constructed links."
                  "                    The default value is [1,5], which corresponds to black and"
                  "                    red if the standard Scilab colormap is used."
                  " ";
                  "  -- {ID}         : A list of two vectors including font number and sizes."
                  "                    The default value is [5,1],[4,1]."
                  " ";
                  "  -- {Cmap}       : An n,3 matrix containing RGB values of colors"
                  "                    to be added to the colormap."
                  "                    The default value is, [0.8,0.8,0.8] i.e., the color gray."
                  " ";
                  " Size : 6.";
                  " Type : scilab tlist of type {scsopt}."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void2',...
                 [" unused field.";
                  " ";
                  " Size : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void3',...
                 [" unused field.";
                  " ";
                  " Size : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'doc',...
                 [" User defined diagram documentation structure.";
                  " ";
                  " Size : 1.";
                  " Type : Strings."])

  //****** scicos_model ******/
  //**************************/
  %scs_help_model=scicos_help(%scs_help_model,...
                 'model',...
                 [" Scilab list that contains the features";
                  " of the block used for the compilation.";
                  " ";
                  " Size : 23.";
                  " Type : Scilab list."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'sim',...
                 ["A list containing two elements. The first element is"
                  "a string containing the name of the computational function"
                  "(C, Fortran,or Scilab). The second element is an integer"
                  "specifying the type of the computational function. Currently"
                  "type 4 and 5 are used, but older types continue to work to "
                  "ensure backward compatibility.";
                  "For some older case, {sim} can be a single string and that"
                  "means that the type is supposed to be 0."
                  " ";
                  " Size : 2.";
                  " Type : Scilab list."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in',...
                 [" A vector specifying the number and size of the first"
                  " dimension of regular input ports indexed from top to";
                  " bottom of the block. If no input port exist in==[].";
                  " ";
                  " The size can be negative, equal to zero or positive :";
                  " ";
                  "  -- If a size is less than zero, the compiler will";
                  "     try to find the appropriate size.";
                  " ";
                  "  -- If a size is equal to zero, the compiler will";
                  "     affect this dimension by added all positive size";
                  "     found in that vector";
                  " ";
                  "  -- If a size is greater than zero, then the size is"
                  "     explicitly given.";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in2',...
                 [" A vector specifying the second dimension of regular"
                  " input ports indexed from top to bottom of the block."
                  " {in} with {in2} formed then the regular input sizes matrix.";
                  " For compatibility, this dimension can stay empty ([]).";
                  "  That means that the dimensions of input ports will be in,1";
                  " ";
                  " The size can be negative, equal to zero or positive :";
                  " ";
                  "  -- If a size is less than zero, the compiler will";
                  "     try to find the appropriate size.";
                  " ";
                  "  -- If a size is equal to zero, the compiler will";
                  "     affect this dimension by added all positive size";
                  "     found in that vector.";
                  " ";
                  "  -- If a size is greater than zero, then the size is"
                  "     explicitly given.";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'intyp',...
                 [" A vector specifying the types of regular input ports."
                  " Its sizes is equal to the sizes of {in}.";
                  " The types of regular input ports can be :";
                  ""
                  "  -- 1  real matrix,";
                  "  -- 2  complex matrix,";
                  "  -- 3  int32 matrix,";
                  "  -- 4  int16 matrix,";
                  "  -- 5  int8 matrix,";
                  "  -- 6  uint32 matrix,";
                  "  -- 7  uint16 matrix,";
                  "  -- 8  uint8 matrix.";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out',...
                 ["";
                  " A vector specifying the number and size of the first"
                  " dimension of regular output ports indexed from top to";
                  " bottom of the block. If no output port exist out==[].";
                  " ";
                  " The size can be negative, equal to zero or positive :";
                  " ";
                  "  -- If a size is less than zero, the compiler will";
                  "     try to find the appropriate size.";
                  " ";
                  "  -- If a size is equal to zero, the compiler will";
                  "     affect this dimension by added all positive size";
                  "     found in that vector";
                  " ";
                  "  -- If a size is greater than zero, then the size is"
                  "     explicitly given.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out2',...
                 [" A vector specifying the second dimension of regular"
                  " output ports indexed from top to bottom of the block."
                  " {out} with {out2} formed then the regular output sizes matrix.";
                  " For compatibility, this dimension can stay empty ([]). That";
                  " means that the dimensions of output ports will be out,1";
                  " ";
                  " That dimension can be negative, equal to zero or positive :";
                  " ";
                  "  -- If a size is less than zero, the compiler will";
                  "     try to find the appropriate size.";
                  " ";
                  "  -- If a size is equal to zero, the compiler will";
                  "     affect this dimension by added all positive size";
                  "     found in that vector.";
                  " ";
                  "  -- If a size is greater than zero, then the size is"
                  "     explicitly given.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'outtyp',...
                 [" A vector specifying the types of regular output ports."
                  " Its sizes is equal to the sizes of {out}.";
                  " The types of regular output ports can be :";
                  ""
                  "  -- 1  real matrix,";
                  "  -- 2  complex matrix,";
                  "  -- 3  int32 matrix,";
                  "  -- 4  int16 matrix,";
                  "  -- 5  int8 matrix,";
                  "  -- 6  uint32 matrix,";
                  "  -- 7  uint16 matrix,";
                  "  -- 8  uint8 matrix.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtin',...
                 [" A vector specifying the number and sizes of"
                  " activation inputs. Currently activation ports"
                  " can be only of size one.";
                  " If no event input port exists evtin must be "
                  " equal to [].";
                  " ";
                  " Size : number of input event ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtout',...
                 [" A vector specifying the number and sizes of"
                  " activation outputs."
                  " Currently activation ports can be only of "
                  " size one.";
                  " If no event output port exists evtout must be "
                  " equal to [].";
                  " ";
                  " Size : number of output event ports.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'state',...
                 [" Vector containing initial values of continuous-time state.";
                  " Must be [] if no continuous state.";
                  " ";
                  " Size : number of continuous-time state.";
                  " Type : column vector of real numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dstate',...
                 [" Vector containing initial values of discrete-time state.";
                  " Must be [] if no discrete state.";
                  " ";
                  " Size : number of discrete-time state.";
                  " Type : column vector of real numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'odstate',...
                 [" List containing initial values of objects state.";
                  " Must be list() if no objects state.";
                  " ";
                  " Objects state can be any types of scilab variable.";
                  " In the computational function case of type 4 (C blocks)";
                  " only elements containing matrix of real, complex,";
                  " int32, int16 ,int8 ,uint32, uit16 and uint8 will be correctly";
                  " provided for readind/writing.";
                  " ";
                  " Size : number of objects state.";
                  " Type : scilab list of scilab objects."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'rpar',...
                 [" The vector of floating point block parameters."
                  " Must be [] if no floating point parameters.";
                  " ";
                  " Size : number of real parameters.";
                  " Type : column vector of real numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'ipar',...
                 [" The vector of integer block parameters."
                  " Must be [] if no integer parameters.";
                  " ";
                  " Size : number of integer parameters.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'opar',...
                 [" List of objects block parameters. "
                  " Must be list() if no objects parameters.";
                  " ";
                  " Objects parameters can be any types of scilab variable.";
                  " In the computational function case of type 4 (C blocks)";
                  " only elements containing matrix of real, complex,";
                  " int32, int16 ,int8 ,uint32, uit16 and uint8 will be correctly";
                  " provided for reading.";
                  " ";
                  " Size : number of objetcs parameters.";
                  " Type : list of scilab object."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'blocktype',...
                 [" Character that can be set to ''c'' or ''d''"
                  " indifferently for standard blocks. ''x'' is used"
                  " if we want to force the computational function to"
                  " be called during the simulation phase even if"
                  " the block does not contribute to computation of"
                  " the state derivative.";
                  " ''l'', ''m'' and ''s'' are reserved. Not to be used."
                  " ";
                  " Size : 1.";
                  " Type : Character."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'firing',...
                 [" Vector of initial event firing times of size "
                  " equal to the number of activation output ports";
                  " (see {evout}). It contains output initial event dates";
                  " (Events generated before any input event arises). "
                  " Negative values stands for no initial event on the "
                  " corresponding port. ";
                  ""
                  " Size : number of output event ports.";
                  " Type : column vector of real numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dep_ut',...
                 ["Boolean vector [dep_u, dep_t]."
                  " ";
                  "  -- dep_u : true if block is always active."
                  "          (output depends continuously of the time)";
                  " ";
                  "  -- dep_t : true if block has direct feed-through,"
                  "          i.e., at least one of the outputs depends"
                  "          directly (not through the states) on one"
                  "          of the inputs.   "
                  "          In other words, when the computational"
                  "          function is called with flag 1, the value "
                  "          of an input is used to compute the output."
                  " ";
                  " Size : 2.";
                  " Type : Boolean vector."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'label',...
                 [" String that defines a label. "
                  " It can be used to identify a block "
                  " in order to access or modify its "
                  " parameters during simulation.";
                  " ";
                  " Size : 1.";
                  " Type : string."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nzcross',...
                 [" Number of zero-crossing surfaces.";
                  " ";
                  " Size : Number of zero-crossing surfaces.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nmode',...
                 [" Length of the mode register. Note that this gives the size of"
                  " the vector mode and not the total number of modes"
                  " in which a block can operate in. Suppose a block "
                  " has 3 modes and each mode can take two values, then"
                  " the block can have up to 2^3=8 modes.";
                  " ";
                  " Size : Number of modes.";
                  " Type : column vector of integer numbers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'equations',...
                 [" Used in case of implicit blocks.";
                  " Data structure of type {modelica} which contains modelica"
                  " code description if any. That list contains four entries :";
                  " ";
                  "  -- model : a string given the name of the file that contains"
                  "             the modelica function.";
                  " ";
                  "  -- inputs : a colunm vector of strings that contains the names of";
                  "              the modelica variables used as inputs.";
                  ""
                  "  -- outputs : a colunm vector of strings that contains the names of";
                  "               the modelica variables used as outputs.";
                  " ";
                  "  -- parameters : a list with two entries. The first is a vector of strings"
                  "                  for the name of modelica variable names used as parameters";
                  "                  and the second entries is a list that contains the value of"
                  "                  parameters.";
                  "                  Names of modelica states can also be informed with"
                  "                  {parameters}. In that case a third entry is used to do the"
                  "                  difference between parameters and states."
                  "                  For i,e :";
                  "                    mo.parameters=list([''C'',''v''],list(C,v),[0,1])"
                  "                  means that ''C'' is a parameter(0) of value C, and ''v''";
                  "                  is a state(1) with initial value v.";
                  " ";
                  " Size : 5.";
                  " Type : scilab list."])

  //****** scicos_graphics ******/
  //*****************************/
  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'graphics',...
                 [" Scilab object including"
                  " graphical information concerning"
                  " the features of the block.";
                  " "
                  " Size : 14.";
                  " Type : scilab list."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'orig',...
                 [" Vector [xo,yo], where xo is the x coordinate"
                  " of the  block origin and yo is the y coordinate"
                  " of the  block origin.";
                  " ";
                  " [xo,yo] is the coordinate of down-left point of"
                  " the block shape.";
                  " ";
                  " Size : 2.";
                  " Type : row vector of real."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'sz',...
                 [" Vector [w,h], where w is the block width and"
                  " h the block height.";
                  " ";
                  " Size : 2.";
                  " Type : row vector of real."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'flip',...
                 [" Set the block orientation. If true the input ports"
                  " are on the left of the box and output ports are on the right."
                  " If false the input ports are on the right of the box and "
                  " output ports are on the left.";
                  " ";
                  " Size : 1.";
                  " Type : boolean."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'theta',...
                 [" Set the angle of the Scicos object."
                  " This value is in degree and is "
                  " included in [-360,360].";
                  " ";
                  " Size : 1.";
                  " Type : real."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'exprs',...
                 [" Strings including formal expressions"
                  " used in the dialog box of the block.";
                  " ";
                  " Size : number of formal expressions.";
                  " Type : column vector of strings."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pin',...
                 [" Vector. pin(i) is the number  of the link"
                  " connected to the ith regular input port "
                  " (counting from one), or 0 if this port is"
                  " not connected.";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pout',...
                 [" Vector. pout(i) is the number of the link"
                  " connected to the ith regular output port "
                  " (counting from one), or 0 if this port is"
                  " not connected.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pein',...
                 [" Vector. pein(i) is the number of the link"
                  " connected to the ith event input port "
                  " (counting from one), or 0 if this port is"
                  " not connected.";
                  " ";
                  " Size : number of events input ports.";
                  " Type : column vector of integers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'peout',...
                 [" Vector. peout(i) is the number of the link"
                  " connected to the ith event output port "
                  " (counting from one), or 0 if this port is"
                  " not connected.";
                  " ";
                  " Size : number of events output ports.";
                  " Type : column vector of integers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'gr_i',...
                 [" Strings including Scilab graphics"
                  " expressions for customizing the block graphical"
                  " aspect. This field may be set with {Icon} sub_menu."
                  " ";
                  " Size : -.";
                  " Type : column vector of strings."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'id',...
                 [" A string including an identification for the"
                  " block. The string is displayed under the"
                  " block in the diagram.";
                  " ";
                  " Size : 1.";
                  " Type : string."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'in_implicit',...
                 [" A vector of strings including ''E'' or ''I''."
                  " ''E'' and ''I'' stand respectively for explicit and"
                  " implicit port, and this vector indicates the nature"
                  " of each input port. For regular blocks (not implicit),"
                  " this vector is empty or contains only ""E"".";
                  " ";
                  " Size : nul or number of regular input ports.";
                  " Type : column vector of strings."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'out_implicit',...
                 [" A vector of strings including ''E'' or ''I''."
                  " ''E'' and ''I'' stand respectively for explicit and"
                  " implicit port, and this vector indicates the nature"
                  " of each output port. For regular blocks (not implicit),"
                  " this vector is empty or contains only ""E"".";
                  " ";
                  " Size : nul or number of regular output ports.";
                  " Type : column vector of strings."])

  //****** scicos_link ******/
  //*************************/
  %scs_help_link=scicos_help(%scs_help_link,...
                 'link',...
                 [" ";
                  " Size : 8.";
                  " Type : scilab list."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'xx',...
                 [" Vector of x coordinates of the link path.";
                  " A link is defined as a polyline line.";
                  " ";
                  " Size : number of points of the link.";
                  " Type : column vector of real numbers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'yy',...
                 [" Vector of y coordinates of the link path.";
                  " A link is defined as a polyline line.";
                  " ";
                  " Size : number of points of the link.";
                  " Type : column vector of real numbers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'id',...
                 [" Character string, the link identification.";
                  " ";
                  " Size : 1.";
                  " Type : string."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'thick',...
                 [" Vector of size two defining line thickness.";
                  " ";
                  " Size : 2.";
                  " Type : row vector of integers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'ct',...
                 [" The first entry of this vector designates the color,"
                  " and the second, the nature of the link. The second"
                  " entry is 1 for a regular link, -1 for an activation"
                  " link, and 2 for an implicit link.";
                  " ";
                  " Size : 2.";
                  " Type : row vector of integers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'from',...
                 [" Vector of size three including the block number,"
                  " port number, and port type (0 for output, 1 for"
                  " input) at the origin of the link. Note that the "
                  " third entry may be 1 if the link is implicit;  "
                  " otherwise it is zero.";
                  " ";
                  " Size : 3.";
                  " Type : row vector of integers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'to',...
                 [" Vector of size three including the block number,"
                  " port number, and port type (0  for output, 1 for"
                  " input) at the destination of the link. Note that the "
                  " third entry may be 1 if the link is implicit;  "
                  " otherwise it is zero.";
                  " ";
                  " Size : 3.";
                  " Type : row vector of integers."])

  //****** scicos_state ******/
  //**************************/
  %scs_help_state=scicos_help(%scs_help_state,...
                 'state',...
                 [" Scilab typed list of type {xcs}. It contains all"
                  " the states of the model, that is, everything than"
                  " can evolve during the simulation.";
                  " state contains {x}, {z}, {oz}, {iz}, "
                  " {tevts}, {evtspt}, {pointi} and {outtb}."
                  " ";
                  " Size : 9.";
                  " Type : scilab tlist."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'x',...
                 [" The continuous-time state register, which is obtained "
                  " by concatenating the continuous-time states of all "
                  " the blocks.";
                  " ";
                  " Size : total of all the size of continuous-time state registers.";
                  " Type : column vector of real numbers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'z',...
                 [" The discrete-time state register, which is obtained"
                  " by concatenating the discrete-time states of all"
                  " the blocks.";
                  " ";
                  " Size : total of all the size of discrete-time state registers.";
                  " Type : column vector of real number."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'oz',...
                 [" The list of the object discrete-time state, which"
                  " is obtained by concatenating the object "
                  " discrete-time states of all the blocks.";
                  " ";
                  " Size : total of all the size of object state.";
                  " Type : list of scilab object."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'iz',...
                 [" Vector of size equal to the number of blocks."
                  " That vector is used to store pointers of the "
                  " working state register (work). If a block needs "
                  " to allocate memory at initialization (flag 4), "
                  " the associated pointer is saved here.";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of real numbers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'tevts',...
                 [" Vector of size equal to the number of activation"
                  " sources. It contains the scheduled times for"
                  " programmed activations in {evtspt}.";
                  " ";
                  " Size : number of activation sources.";
                  " Type : column vector of integers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'evtspt',...
                 [" Vector of size equal to the number of activation"
                  " sources. It is an event scheduler.";
                  " ";
                  " Size : number of activation sources.";
                  " Type : column vector of integers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'pointi',...
                 [" The number of the next programmed event.";
                  " ";
                  " Size : 1.";
                  " Type : integer."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'outtb',...
                 [" Scilab list containing all output registers "
                  " of blocks. Each element of that list contains "
                  " typed matrix-based data.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : list of scilab matrix."])

  //****** scicos_sim ******/
  //************************/
  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'sim',...
                 [" Scilab typed list of type {scs}. It contains "
                  " static arrays coming from the result of the"
                  " compilation. That arrays does not evolve"
                  " during the simulation."
                  " ";
                  " Size : 33.";
                  " Type : scilab tlist."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funs',...
                 [" A list containing names of the computational"
                  " functions or scilab functions.";
                  " "
                  " Size : number of blocks.";
                  " Type : list of strings and/or scilab function."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'xptr',...
                 [" A vector pointer to the continuous time state "
                  " register {x}. The continuous-time state of "
                  " block {i} is "
                  " {state.x(sim.xptr(i):sim.xptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zptr',...
                 [" A vector pointer to the discrete time state "
                  " register {z}. The discrete-time state of "
                  " block {i} is "
                  " {state.z(sim.zptr(i):sim.zptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ozptr',...
                 [" A vector pointer to the object discrete state "
                  " register {oz}. The object discrete state of "
                  " block {i} is "
                  " {state.oz(sim.ozptr(i):sim.ozptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zcptr',...
                 [" A vector pointer to the zero-crossing surfaces.";
                  " register. That vector gives by block the used";
                  " number of the zero-crossing.";
                  "";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inpptr',...
                 [" (sim.inpptr(i+1)-sim.inpptr(i)) gives the number of";
                  " regular input ports of the i block.";
                  " inpptr(i) points to the beginning of ith block";
                  " inputs within the indirection table {inplnk}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outptr',...
                 [" (sim.outptr(i+1)-sim.outptr(i)) gives the number of";
                  " regular ouyput ports of the i block.";
                  " outptr(i) points to the beginning of ith block";
                  " outputs within the indirection table {outlnk}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inplnk',...
                 [" (cpr.sim.inplnk(cpr.sim.inpptr(i)-1+j))"
                  " is the index of the link connected to "
                  " the jth input port of the ith block where "
                  " j goes from 1 to "
                  " (cpr.sim.inpptr(i+1)-cpr.sim.inpptr(i)).";
                  " ";
                  " Size : total number of regular input port.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outlnk',...
                 [" (cpr.sim.outlnk(cpr.sim.outptr(i)-1+j))"
                  " is the index of the link connected to "
                  " the jth output port of the ith block where "
                  " j goes from 1 to "
                  " (cpr.sim.outptr(i+1)-cpr.sim.outptr(i)).";
                  " ";
                  " Size : total number of regular output port.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpar',...
                 [" Vector of real parameters that is "
                  " obtained by concatenating the real"
                  " parameters registers of all the blocks.";
                  " ";
                  " Size : total number of real parameters.";
                  " Type : column vector of real numbers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpptr',...
                 [" A vector pointer to the real parameters"
                  " register {rpar}. The real parameters of"
                  " block i are "
                  " {sim.rpar(sim.rpptr(i):sim.rpptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipar',...
                 [" Vector of integer parameters that is "
                  " obtained by concatenating the integer"
                  " parameters registers of all the blocks.";
                  " ";
                  " Size : total number of integer parameters.";
                  " Type : column vector of integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipptr',...
                 [" A vector pointer to the integer parameters"
                  " register {ipar}. The integer parameters of"
                  " block i are "
                  " {sim.ipar(sim.ipptr(i):sim.ipptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of real numbers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opar',...
                 [" List of object parameters that is "
                  " obtained by concatenating the list"
                  " of object parameters of all the blocks.";
                  " ";
                  " Size : total number of object parameters.";
                  " Type : list of scilab objects."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opptr',...
                 [" A vector pointer to the object parameters"
                  " list {opar}. The object parameters of"
                  " block i are "
                  " {sim.opar(sim.opptr(i):sim.opptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'clkptr',...
                 [" A vector pointer to output activation ports.";
                  " (cpr.sim.clkptr(i):cpr.sim.clkptr(i+1)-1) "
                  " gives the number of output event ports";
                  " of the block i.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordptr',...
                 [" A vector pointer to {ordclk} designating the"
                  " part of {ordclk} corresponding to a given"
                  " activation.";
                  "(cpr.sim.ordptr(i):cpr.sim.ordptr(i+1)-1)"
                  " points to the region within ordclk indicates "
                  " the number of blocks activated by the output"
                  " event ports numbered i."
                  " ";
                  " Size : number of sources of activation + 1.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'execlk',...
                 ["Unused.";
                  " ";
                  " Size : -";
                  " Type : matrix of real."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordclk',...
                 [" A matrix associated to blocks activated by"
                  " output activation ports. The first column"
                  " contains the block number, and  the second,"
                  " the event code by which the block should be"
                  " called.";
                  " ";
                  " Size : total number of blocks summed by source of activations.";
                  " Type : matrix of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'cord',...
                 [" A matrix associated to always active blocks."
                  " The first column contains the block number, "
                  " and the second, the event code by which the "
                  " block should be called.";
                  " ";
                  " Size : ncord.";
                  " Type : matrix of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'oord',...
                 [" Subset of {cord}. Blocks of that matrix "
                  " have outputs which affect computation of"
                  " continuous state derivatives."
                  " ";
                  " Size : noord.";
                  " Type : matrix of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zord',...
                 [" Subset of {zord}. Blocks of that matrix "
                  " have outputs which affect computation of"
                  " zero-crossing surfaces."
                  " ";
                  " Size : nzord.";
                  " Type : matrix of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'critev',...
                 [" A vector of size equal to the number of "
                  " activations and containing zeros and "
                  " ones. The value one indicates that the "
                  " activation is critical in the sense that"
                  " the continuous-time solver must be cold "
                  " restarted.";
                  " ";
                  " Size : number of source of activation.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nb',...
                 [" Number of blocks. Note that the number of"
                  " blocks may differ from the original number"
                  " of blocks in the diagram because {c_pass2}"
                  " may duplicate some conditional blocks."
                  " ";
                  " Size : 1.";
                  " Type : integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ztyp',...
                 [" A vector of size equal to the number of blocks."
                  " A 1 entry indicates that the block may have "
                  " zero-crossings, even if it doesn''t in the "
                  " context of the diagram.  Usually not used by "
                  " the simulator.";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nblk',...
                 ["Not used. Set to {nb}.";
                  " ";
                  " Size : 1";
                  " Type : integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ndcblk',...
                 ["Not used.";
                  " ";
                  " Size : -";
                  " Type : integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'subscr',...
                 [" Not used.";
                  " ";
                  " Size : 0";
                  " Type : empty real."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funtyp',...
                 [" A vector of size equal to the number of blocks"
                  " indicating the type of the computational function"
                  " of the block. Block type can be 0 through 5."
                  " Currently only type 4 (C language) and type 5"
                  " (Scilab language) computational functions should"
                  " be used. But older blocks can also be used.";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of integer."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'iord',...
                 [" A matrix associated to blocks that"
                  " must be activated at the start of "
                  " the simulation. This includes blocks "
                  " inheriting from constant blocks and "
                  " always active blocks.";
                  " ";
                  " Size : niord.";
                  " Type : matrix of integers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'labels',...
                 [" A string vector of size equal"
                  " to the number of blocks "
                  " containing block labels.";
                  " ";
                  " Size : numbers of blocks.";
                  " Type : column vector of strings."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'modptr',...
                 [" A vector pointer to the block modes.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integer."])

  //==========================================================

  //Enter here traduction for initial scicos tables ==========
  if ~exists('LANGUAGE') then global LANGUAGE, end

  if LANGUAGE=='fr' then

  //******* cpr *******/
  //*******************/
  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'cpr',...
                 [" La liste scilab {scicos_cpr} contient "
                  " le r�sultat de la compilation."
                  " Cette structure est compos�e des champs"
                  " {scicos_state}, {scicos_sim}, {cor} et {corinv}."
                  ""
                  " Taille : 5.";
                  " Type : tlist scilab."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'state',...
                  [" Une liste typ�e scilab de type {xcs}. Cette";
                   " liste contient tous les �tats du diagramme qui";
                   " vont �voluer pendant la simulation.";
                   " state contient {x}, {z}, {oz}, {iz}, "
                   " {tevts}, {evtspt}, {pointi} et {outtb}."
                   " ";
                   " Taille : 9.";
                   " Type : tlist scilab."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'sim',...
                 [" Une liste typ�e scilab de type {scs}. Cette";
                  " liste contient des tableaux statiques issus"
                  " du r�sultat de la compilation. Ces tableaux"
                  " n''�voluent pas pendant la simulation."
                  " ";
                  " Taille : 33.";
                  " Type : tlist scilab."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'cor',...
                 [" C''est une liste avec la m�me structure r�cursive"
                  " que la liste scs_m. Chaque feuille contient l''indice "
                  " d''un bloc associ� � la structure compil�e sim."
                  " ";
                  " Taille : nombre d''objets dans scs_m.";
                  " Type : scilab list."])

  %scs_help_cpr=scicos_help(%scs_help_cpr,...
                 'corinv',...
                 [" corinv(i) est le chemin du bloc i"
                  " d�finit dans la structure compil�e sim"
                  " dans la structure scs_m."
                  " ";
                  " Taille : nombre de blocs dans la structure compil�e.";
                  " Type : scilab list."])

  //******* scicos_block *******/
  //****************************/
  %scs_help_block=scicos_help(%scs_help_block,...
                 'block',...
                 [" Une structure de base qui d�finit un bloc Scicos.";
                  " Cette structure inclut les champs {scicos_graphics}, {scicos_model},"
                  " {gui} et {doc}."
                  ""
                  " Size : 5.";
                  " Type : scilab list."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'graphics',...
                 [" Liste Scilab contenant les informations"
                  " sur les donn�es graphiques du bloc."
                  " "
                  " Taille : 14.";
                  " Type : scilab list."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'model',...
                 [" Liste Scilab qui contient les informations";
                  " du bloc utilis�es pour la compilation.";
                  " ";
                  " Taille : 23.";
                  " Type : liste scilab."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'gui',...
                 [" Le nom de la fonction d''interface Scilab";
                  " associ�e au bloc.";
                  " ";
                  " Taille : 1.";
                  " Type : cha�ne de caract�res."])

  %scs_help_block=scicos_help(%scs_help_block,...
                 'doc',...
                 [" Un champ utilis� pour la documentation";
                  " du bloc.";
                  " ";
                  " Taille : 1.";
                  " Type : cha�ne de caract�res."])

  //****** scicos_diagram ******/
  //****************************/
  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'diagram',...
                 [" ";
                  " Taille : 4.";
                  " Type : liste scilab."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'props',...
                 [" Propri�t�s du diagramme.";
                  " Cette entr�e contient des informations diverses";
                  " telles que des valeurs initiales du diagramme";
                  " principal.";
                  " ";
                  " Cette variable est une tlist de type {scicos_params} et";
                  " contient {wpar}, {title}, {tol}, {tf}, {context},";
                  " {options} et {doc}.";
                  " ";
                  " Taille : 11.";
                  " Type : tlist scilab de type {scicos_params}."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'objs',...
                 [" Une liste d''objets inclus dans le diagramme Scicos.";
                  " Les objets utilis�s dans Scicos sont {scicos_block}, {scicos_link} et";
                  " Text.";
                  " Les objets peuvent aussi �tre des structures de donn�e effac�es."
                  " Les objets effac�s sont marqu�s par list(''Deleted'').";
                  " ";
                  " Taille : nombre total d''objets dans le diagramme.";
                  " Type : tlist scilab de type {scicos_block}, {scicos_link} ou Text."])

  %scs_help_diagram=scicos_help(%scs_help_diagram,...
                 'version',...
                 [" Une cha�ne de caract�res qui donne le num�ro de version";
                  " du diagramme Scicos. Cela est utilis� pour fournir une";
                  " compatibilit� avec les anciens diagrammes."
                  " Notez que vous pouvez retrouver la version courante de"
                  " Scicos en utilisant l''entr�e ''About scicos'' dans le"
                  " menu help ou en utilisant la fonction get_scicos_version()."
                  " ";
                  " Taille : 1.";
                  " Type : cha�ne de caract�res."])

  //****** scicos_params ******/
  //***************************/
  %scs_help_params=scicos_help(%scs_help_params,...
                 'params',...
                 [" ";
                  " Size : 11.";
                  " Type : liste scilab."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'wpar',...
                  [" Ce vecteur n''est actuellement pas utilis�.";
                   " Il devrait �tre utilis� dans le futur";
                   " pour enregistrer les param�tres de la"
                   " fen�tre tels que la position et la taille.";
                   " ";
                   " Taille : 6.";
                   " Type : vecteur colonne de nombre r�els."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'title',...
                 [" Vecteur de cha�ne de caract�res, o� la premi�re contient";
                  " le titre du diagramme (le nom par d�faut du ficher de";
                  " sauvegarde) et la deuxi�me est le chemim du r�pertoire.";
                  " ";
                  " Taille : 2.";
                  " Type : vecteur line de cha�ne de caract�res."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tol',...
                 [" Un vecteur contenant les param�tres de la simulation qui";
                  " inclue les tol�rances utilis�es par le solveur: ";
                  " ";
                  "  -- {atol}   : tol�rance absolue pour l''int�gration";
                  " ";
                  "  -- {rtol}   : tol�rance relative pour l''int�gration.";
                  " ";
                  "  -- {ttol}   : tol�rance sur le temps. Si une p�riode d''int�gration est";
                  "               inf�rieure � {ttol}, le solveur n''est pas appell�."
                  " ";
                  "  -- {deltat} : Interval temporel maximal entre deux points d''int�gration."
                  "               Si une p�riode d''int�gration est plus grande que {deltat}, ";
                  "               le solveur est appel� plus d''une fois de mani�re � maintenir"
                  "               le pas au niveau de {deltat}";
                  " ";
                  "  -- {scale}  : Facteur d''�chelle temps-r�el (Real-time scaling);"
                  "               la valeur 0 correspond � un facteur d''�chelle nul."
                  "               Ce facteur associe le temps de la simulation au temps"
                  "               r�el en seconde. Une valeur �gale � 1, signifie que "
                  "               chaque untit� de temps Scicos correspond � une seconde."
                  " ";
                  "  -- {solver} : Choix du solveur num�rique. Une valeur 0 signifie qu''un"
                  "               solveur ODE sera utilis� et 100 signifie qu''un solveur"
                  "               DAE sera utilis�.";
                  " ";
                  "  -- {hmax}   : Pas maximal d''int�gration pour le solveur.";
                  "               0 signifie pas de limites.";
                  " ";
                  " Taille : 7.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tf',...
                [" Le temps final de simulation."
                 " La simulation s''arr�te a ce temps."
                 " La valeur par d�faut est 100000.";
                 " ";
                 " Taille : 1.";
                 " Type : r�el."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'context',...
                 [" Un vecteur de cha�nes de caract�res contenant des";
                  " instructions Scilab d�finissant des variables";
                  " Scilab � utiliser dans les bo�tes de dialogues ";
                  " des blocs (comme param�tres symboliques).";
                  " Toutes instructions valides Scilab peuvent �tre";
                  " utilis�es ainsi que des commentaires.";
                  " ";
                  " Taille : nombre de lignes du contexte.";
                  " Type : Vecteur colonne de cha�nes de caract�res."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void1',...
                 [" Champ inutilis�.";
                  " ";
                  " Taille : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'options',...
                 [" Liste Scilab de type {scsopt} d�finissant les";
                  " propri�t�s graphiques de l''�diteur telles que";
                  " la couleur de fond et la couleur des liens.";
                  " ";
                  " Les champs sont les suivant:";
                  " ";
                  "  -- {3D}         : Une liste � deux entr�es. La premi�re est un bool�en"
                  "                   indiquant si les blocs doivent avoir un aspect 3D.";
                  "                   La seconde entr�e indique la couleur dans la palette de"
                  "                   couleur courante (colormap) qui doit �tre utilis�e pour"
                  "                   cr�er l''effet 3D. Par d�faut celle ci est 33 ce qui"
                  "                   correspond � gris, couleur ajout�e par Scicos � la palette"
                  "                   standard qui contients 32 couleurs."
                  " ";
                  "                   La valeur par d�fault de cette list est donc list(%t,33)."
                  "  -- {Background} : Un vecteur avec deux entr�es: la couleur d''arri�re plan et"
                  "                   la couleur d''avant plan. La valeur par d�faut est [8,1].";
                  " ";
                  "  -- {link}       : Les couleurs par d�fauts pour les liens r�guleurs et les"
                  "                   liens d''activations. Ces couleurs sont utilis�es uniquement";
                  "                   pour de nouvelles constructions. Ces choix n''affectent pas";
                  "                   les liens d�j� pr�sent sur le diagramme.";
                  " ";
                  "                   Les valeurs par d�faut sont [1,5] ce qui correspond � noir";
                  "                   et rouge pour la palette de couleurs standard de Scilab.";
                  "  -- {ID}         : Une liste de deux vecteurs qui d�finit le type de la police"
                  "                   et la taille de la police. Les valeurs par d�faut sont";
                  "                   [5,1],[4,1].";
                  " ";
                  "  -- {Cmap}       : Une matrice de taille 3,n contenant les valeurs RGB des"
                  "                   couleurs � ajouter � la palette de couleur. La valeur par";
                  "                   d�faut est [0.8,0.8,0.8] (couleur grise).";
                  " ";
                  " Taille : 6.";
                  " Type : tlist scilab de type {scsopt}."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void2',...
                 [" Champ inutilis�.";
                  " ";
                  " Taille : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void3',...
                 [" Champ inutilis�.";
                  " ";
                  " Taille : -.";
                  " Type : -."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'doc',...
                 [" Structure utilisateur pour la documentation des diagrammes.";
                  " ";
                  " Taille : 1.";
                  " Type : Cha�nes de caract�res."])

  //****** scicos_model ******/
  //**************************/
  %scs_help_model=scicos_help(%scs_help_model,...
                 'model',...
                 [" Liste Scilab qui contient les informations";
                  " du bloc utilis�es pour la compilation.";
                  " ";
                  " Taille : 23.";
                  " Type : liste scilab."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'sim',...
                 [" Une liste Scilab contenant deux �l�ments. Le premier �l�ment";
                  " est une cha�ne de caract�res contenant le nom de la fonction";
                  " de calcul (C, Fortran ou Scilab). Le deuxi�me �l�ment est un";
                  " entier sp�cifiant le type de la fonction de calcul. Les types";
                  " courant sont 4 et 5, mais les anciens types sont toujours";
                  " compatibles.";
                  " Pour des anciens blocs, {sim} peut �tre une simple liste, ce";
                  " qui signifie que le type est suppos� �tre 0.";
                  " ";
                  " Taille : 2.";
                  " Type : liste Scilab."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in',...
                 [" Un vecteur sp�cifiant le nombre et la premi�re";
                  " dimension des ports d''entr�e r�guliers du bloc";
                  " ind�x�s du haut vers le bas.";
                  " Si il n''existe pas de port d''entr�es, alors";
                  " in=[].";
                  ""
                  " Les dimensions peuvent �tre n�gatives, �gales � z�ro"
                  " ou positives : "
                  ""
                  "  -- Lorsqu''une dimension n�gative est utilis�e, "
                  "     alors le compilateur essaiera de d�terminer "
                  "     quelle est la dimension appropri�e."
                  ""
                  "  -- Lorsqu''une dimension �gale � z�ro est utilis�e, "
                  "     alors le compilateur d�terminera la valeur de "
                  "     cette dimension en sommant toutes les tailles "
                  "     positives trouv�es dans ce vecteur de dimensions."
                  ""
                  "  -- Si les dimensions sont positives, elles sont"
                  "     alors explicitement renseign�es."
                  " "
                  " Taille : nombre de ports r�guliers d''entr�e.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in2',...
                 [" Un vecteur sp�cifiant la deuxi�me dimension";
                  " des ports r�guliers d''entr�es du bloc (ind�x�";
                  " du haut vers le bas)."
                  " {in} avec {in2} forment alors les tailles des matrices"
                  " d''entr�e."
                  " Pour des raisons de compatibilit�, cette dimension"
                  " peut rester non renseign�e ([]), ce qui signifiera"
                  " que les dimensions des ports seront in,1."
                  ""
                  " Les dimensions peuvent �tre n�gatives, �gales � z�ro"
                  " ou positives : "
                  ""
                  "  -- Lorsqu''une dimension n�gative est utilis�e, "
                  "     alors le compilateur essaiera de d�terminer "
                  "     quelle est la dimension appropri�e."
                  ""
                  "  -- Lorsqu''une dimension �gale � z�ro est utilis�e, "
                  "     alors le compilateur d�terminera la valeur de "
                  "     cette dimension en sommant toutes les tailles "
                  "     positives trouv�es dans ce vecteur de dimensions."
                  ""
                  "  -- Si les dimensions sont positives, elles sont"
                  "     alors explicitement renseign�es."
                  " "
                  " Taille : nombre de ports r�guliers d''entr�e.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'intyp',...
                 [" Un vecteur sp�cifiant les types des ports d''entr�e r�guliers.";
                  " Sa taille est �gale � {in}. ";
                  " Les types des ports d''entr�e peuvent �tre :";
                  "  -- 1 matrice de nombres r�els,";
                  "  -- 2 matrice de nombres complexes,";
                  "  -- 3 matrice de int32,";
                  "  -- 4 matrice de int16,";
                  "  -- 5 matrice de int8,";
                  "  -- 6 matrice de uint32,";
                  "  -- 7 matrice de uint16,";
                  "  -- 8 matrice de uint8.";
                  " ";
                  " Taille : nombre de ports r�guliers d''entr�e.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out',...
                 [" Un vecteur sp�cifiant le nombre et la premi�re";
                  " dimension des ports de sortie r�guliers du bloc";
                  " ind�x�s du haut vers le bas.";
                  " Si il n''existe pas de port de sorties, alors";
                  " out=[].";
                  ""
                  " Les dimensions peuvent �tre n�gatives, �gales � z�ro"
                  " ou positives : "
                  ""
                  "  -- Lorsqu''une dimension n�gative est utilis�e, "
                  "     alors le compilateur essaiera de d�terminer "
                  "     quelle est la dimension appropri�e."
                  ""
                  "  -- Lorsqu''une dimension �gale � z�ro est utilis�e, "
                  "     alors le compilateur d�terminera la valeur de "
                  "     cette dimension en sommant toutes les tailles "
                  "     positives trouv�es dans ce vecteur de dimensions."
                  ""
                  "  -- Si les dimensions sont positives, elles sont"
                  "     alors explicitement renseign�es."
                  " "
                  " Taille : nombre de ports r�guliers de sortie.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out2',...
                 [" Un vecteur sp�cifiant la deuxi�me dimension";
                  " des ports r�guliers de sortie du bloc (ind�x�";
                  " du haut vers le bas)."
                  " {out} avec {out2} forment alors les tailles des matrices"
                  " de sorties."
                  " Pour des raisons de compatibilit�, cette dimension"
                  " peut rester non renseign�e ([]), ce qui signifiera"
                  " que les dimensions des ports seront out,1."
                  ""
                  " Les dimensions peuvent �tre n�gatives, �gales � z�ro"
                  " ou positives : "
                  ""
                  "  -- Lorsqu''une dimension n�gative est utilis�e, "
                  "     alors le compilateur essaiera de d�terminer "
                  "     quelle est la dimension appropri�e."
                  ""
                  "  -- Lorsqu''une dimension �gale � z�ro est utilis�e, "
                  "     alors le compilateur d�terminera la valeur de "
                  "     cette dimension en sommant toutes les tailles "
                  "     positives trouv�es dans ce vecteur de dimensions."
                  ""
                  "  -- Si les dimensions sont positives, elles sont"
                  "     alors explicitement renseign�es."
                  " "
                  " Taille : nombre de ports r�guliers de sortie.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'outtyp',...
                 [" Un vecteur sp�cifiant les types des ports de sortie r�guliers.";
                  " Sa taille est �gale � {out}. ";
                  " Les types des ports de sortie peuvent �tre :";
                  "  -- 1  matrice de nombres r�els,";
                  "  -- 2  matrice de nombres complexes,";
                  "  -- 3  matrice de int32,";
                  "  -- 4  matrice de int16,";
                  "  -- 5  matrice de int8,";
                  "  -- 6  matrice de uint32,";
                  "  -- 7  matrice de uint16,";
                  "  -- 8  matrice de uint8.";
                  " ";
                  " Taille : nombre de ports r�guliers de sortie.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtin',...
                 [" Un vecteur indiquant la taille et le nombre";
                  " de ports �v�nementiels d''entr�e. Actuellement";
                  " les ports peuvent seulement avoir une taille";
                  " �gale � 1.";
                  " Si aucun port �v�nementiel existe alors";
                  " evtin est �gal � 1.";
                  " ";
                  " Taille : nombre de ports �v�nementiel d''entr�e.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtout',...
                 [" Un vecteur indiquant la taille et le nombre";
                  " de ports �v�nementiels de sorties. Actuellement";
                  " les ports peuvent seulement avoir une taille";
                  " �gale � 1.";
                  " Si aucun port �v�nementiel existe alors";
                  " evtout est �gal � 1.";
                  " ";
                  " Taille : nombre de ports �v�nementiel de sortie.";
                  " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'state',...
                 [" Un vecteur contenant les valeurs initiales des �tats";
                  " continus.";
                  " Ce vecteur doit �tre �gal � [] si le bloc ne poss�de";
                  " pas d''�tat continus.";
                  " ";
                  " Taille : nombre d''�tats continus.";
                  " Type : vecteur colonne de nombres r�els."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dstate',...
                 [" Un vecteur contenant les valeurs initiales des �tats";
                  " discrets.";
                  " Ce vecteur doit �tre �gal � [] si le bloc ne poss�de";
                  " pas d''�tat discrets.";
                  " ";
                  " Taille : nombre d''�tats discrets.";
                  " Type : vecteur colonne de nombres discrets."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'odstate',...
                 [" Une liste contenant les valeurs initiales des �tats";
                  " objets.";
                  " Ce vecteur doit �tre �gal � [] si le bloc ne poss�de";
                  " pas d''�tat discrets";
                  " ";
                  " Les �tats objets acceptent tous les types de variables";
                  " Scilab.";
                  " Dans le cas des fonctions de calcul de type 4 (bloc C),";
                  " seul les �l�ments contenant des matrices de nombres r�els,";
                  " complexes, int32, int16 ,int8 ,uint32, uit16 et uint8 seront";
                  " correctement fournis pour la lecture/�criture.";
                  " ";
                  " Taille : nombre d''�tats objets.";
                  " Type : liste scilab."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'rpar',...
                 [" Le vecteur des param�tres � virgule flotante.";
                  " Doit �tre [] si le bloc ne poss�de pas de";
                  " param�tres r�els.";
                  " ";
                  " Taille : nombre de param�tres r�els.";
                  " Type : vecteur colonne de nombre r�els."])

 %scs_help_model=scicos_help(%scs_help_model,...
                 'ipar',...
                 [" Le vecteur des parametres entiers.";
                  " Doit �tre [] si le bloc ne poss�de pas de";
                  " param�tres entiers.";
                  " ";
                  " Taille : nombre de param�tres entiers.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'opar',...
                 [" La liste des parametres objets.";
                  " Doit �tre list() si le bloc ne poss�de pas de";
                  " param�tres objets.";
                  " Les param�tres objets acceptent tous les types"
                  " de variables Scilab.";
                  " ";
                  " Dans le cas des fonctions de calcul de type 4 (bloc C),";
                  " seul les �l�ments contenant des matrices de nombres r�els,";
                  " complexes, int32, int16 ,int8 ,uint32, uit16 et uint8 seront";
                  " correctement fournis pour la lecture.";
                  " ";
                  " Taille : nombre de param�tres objets.";
                  " Type : liste d''objets scilab."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'blocktype',...
                 [" Un caract�re qui peut �tre mis indiff�remment �";
                  " ''c'' ou ''d'' pour les blocs standards. ''x'' est";
                  " utilis� pour forcer l''appel � la fonction de calcul";
                  " d''un bloc lors des calculs des �tats continu, m�me ";
                  " si le bloc ne poss�de pas de tels �tats.";
                  " ''l'', ''m'' et ''s'' sont r�serv�s et ne doivent pas";
                  " �tre utilis�s.";
                  " ";
                  " Taille : 1.";
                  " Type : caract�re."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'firing',...
                 [" Un vecteur des temps initiaux de taille";
                  " �gale au nombre de ports �v�nementiels de";
                  " sortie. Celui ci contient les dates initiales";
                  " (�v�nements g�n�r�s avant tout autre �v�nements)";
                  " Des valeurs n�gatives indiquent qu''aucun";
                  " �v�nement inital ne sera g�n�r�.";
                  " ";
                  " Taille : nombre de ports �v�nementiel de sortie.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dep_ut',...
                 [" Un vecteur de bool�en. [dep_u, dep_t].";
                  " ";
                  "  -- dep_u : vrai si le bloc est toujours actif."
                  "          (la sortie d�pend continuellement du temps)";
                  " ";
                  "  -- dep_t : vrai si le bloc � une relation directe entre"
                  "          une entr�e r�guli�re et une sortie r�guli�re."
                  "          En d''autres termes, lorsque la fonction de calcul"
                  "          est appel�e avec flag 1, la valeur d''une entr�e"
                  "          est utilis�e pour calculer la sortie."
                  " ";
                  " Taille : 2.";
                  " Type : vecteur de bol�ens."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'label',...
                 [" Cha�ne de caract�res qui d�finit un label.";
                  " Le label peut �tre utilis� pour identifier";
                  " un bloc pour avoir acc�s ou modifier ses "
                  " param�tres pendant la simulation."
                  " ";
                  " Taille : 1.";
                  " Type : chaine de caract�res."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nzcross',...
                 [" Nombre de d�tection de surfaces.";
                  " ";
                  " Taille : nombre de d�tection de surfaces.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nmode',...
                 [" La longeur du registre des modes. Notez que cela donne la taille";
                  " du vecteur des modes mais pas le nombre total de";
                  " modes utilis�s par le bloc.";
                  " En supposant qu''un bloc a 3 modes et que chaque";
                  " mode peut prendre deux valeurs, alors le bloc";
                  " peut avoir jusqu''� 2^3=8 modes.";
                  " ";
                  " Taille : longueur du registre des modes.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'equations',...
                 [" Utilis� dans les cas des blocs implicites.";
                  " Structure de donn�es de type {modelica} qui";
                  " contient des descriptions relatives au code modelica.";
                  " Cette liste contient quatre entr�es :";
                  " ";
                  "  -- model : une cha�ne de caract�res donnant le nom du fichier";
                  "            de la fonction modelica.";
                  " ";
                  "  -- inputs : un vecteur colonne de cha�nes de caract�res donnant";
                  "             les noms des variables utilis�es comme entr�es.";
                  " ";
                  "  -- outputs : un vecteur colonne de cha�nes de caract�res donnant";
                  "              les noms des variables utilis�es comme sorties.";
                  " ";
                  "  -- parameters : une liste � deux entr�es. La premi�re est un vecteur";
                  "                 de cha�nes de caract�res contenant les noms des variables";
                  "                 modelica utilis�es en tant que param�tres et la deuxi�me";
                  "                 une liste contenant les valeurs de ces param�tres.";
                  "                 Les noms des �tats modelica peuvent aussi �tre inform�s";
                  "                 via {parameters}. Dans ce cas une troisi�me entr�e est";
                  "                 utilis�e pour diff�rencier les param�tres des �tats.";
                  "                 Par exemple :";
                  "                    mo.parameters=list([''C'',''v''],list(C,v),[0,1])"
                  "                 signifie que ''C'' est un param�tre(0) de valeur C, et";
                  "                 que ''v'' est une variable d''�tat(1) avec une valeur";
                  "                 initiale v.";
                  " ";
                  " Taille : 5.";
                  " Type : liste scilab."])

  //****** scicos_graphics ******/
  //*****************************/
  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'graphics',...
                 [" Liste Scilab contenant les informations"
                  " sur les donn�es graphiques du bloc."
                  " "
                  " Size : 14.";
                  " Type : scilab list."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'orig',...
                 [" Un vecteur [xo,yo], o� xo est la coordonn�e sur";
                  " l''axe des abscisses de l''origine du bloc et yo est";
                  " la coordonn�e sur l''axe des ordonn�es.";
                  " ";
                  " [xo,yo] sont les coordonn�es en bas � gauche du";
                  " contours du bloc.";
                  " ";
                  " Taille : 2.";
                  " Type : vecteur ligne de nombres r�els."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'sz',...
                 [" Un vecteur [w,h], o� w est la largeur et";
                  " h la hauteur du bloc.";
                  " ";
                  " Taille : 2.";
                  " Type : vecteur ligne de nombres r�els."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'flip',...
                 [" D�finit l''orientation du bloc. Si vrai alors les ports";
                  " d''entr�e sont sur la gauche du bloc et les ports de sortie";
                  " sur la droite. Si faux alors les ports d''entr�e sont sur "
                  " la droite et ceux de sortie sur la gauche.";
                  " ";
                  " Taille : 1.";
                  " Type : bool�en."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'theta',...
                 [" D�finit l''angle de l''objet Scicos.";
                  " Cette valeur est en degr�s et est inclut";
                  " dans [-360,360]."
                  " ";
                  " ";
                  " Taille : 1.";
                  " Type : nombre r�el."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'exprs',...
                 [" Des cha�nes de caract�res incluant des expressions";
                  " formelles utilis�es dans la bo�te de dialogue.";
                  " ";
                  " Taille : Nombre d''expressions formelles.";
                  " Type : Vecteur colonne de cha�nes de caract�res."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pin',...
                 [" Un vecteur. pin(i) est le num�ro du lien";
                  " connect� � l''entr�e r�guli�re i, ou 0 si";
                  " le port est connect� � aucun lien.";
                  " ";
                  " ";
                  " Taille : nombres de ports r�guliers d''entr�e.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pout',...
                 [" Un vecteur. pout(i) est le num�ro du lien";
                  " connect� � la sortie r�guli�re i, ou 0 si";
                  " le port est connect� � aucun lien.";
                  " ";
                  " ";
                  " Taille : nombres de ports r�guliers de sortie.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pein',...
                 [" Un vecteur. pein(i) est le num�ro du lien";
                  " connect� � l''entr�e �v�nementielle i, ou 0 si";
                  " le port est connect� � aucun lien.";
                  " ";
                  " ";
                  " Taille : nombres de ports �v�nementiel d''entr�e.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'peout',...
                 [" Un vecteur. peout(i) est le num�ro du lien";
                  " connect� � la sortie �v�nementielle i, ou 0 si";
                  " le port est connect� � aucun lien.";
                  " ";
                  " ";
                  " Taille : nombres de ports �v�nementiel de sortie.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'gr_i',...
                 [" Cha�nes de caract�re qui contiennent les expression";
                  " graphiques pour personnaliser l''aspect graphique des";
                  " blocs. Ce champ peut �tre renseign� par le sous-menu";
                  " {Icon}.";
                  " ";
                  " Taille : -.";
                  " Type : Vecteur colonne de cha�nes de caract�res."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'id',...
                 [" Une cha�ne qui contient un identifiant pour";
                  " le bloc. Celui ci est affich�� en dessous du";
                  " bloc dans le diagramme.";
                  " ";
                  " Taille : 1.";
                  " Type : cha�nes de caract�res."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'in_implicit',...
                 [" Un vecteur de cha�nes de caract�res contenant ''E'' ou ''I''.";
                  " ''E'' et ''I'' correspondent respectivement � explicite ou";
                  " implicite. Ce vecteur indique donc la nature de chaque ports.";
                  " Pour les blocs classiques, celui ci est soit vide ou bien �gal";
                  " � ''E''.";
                  " ";
                  " Taille : vide ou bien le nombre de ports r�guliers d''entr�e.";
                  " Type : vecteur colonne de cha�nes de caract�res."])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'out_implicit',...
                 [" Un vecteur de cha�nes de caract�res contenant ''E'' ou ''I''.";
                  " ''E'' et ''I'' correspondent respectivement � explicite ou";
                  " implicite. Ce vecteur indique donc la nature de chaque ports.";
                  " Pour les blocs classiques, celui ci est soit vide ou bien �gal";
                  " � ''E''.";
                  " ";
                  " Taille : vide ou bien le nombre de ports r�guliers de sorties.";
                  " Type : vecteur colonne de cha�nes de caract�res."])

  //****** scicos_link ******/
  //*************************/
  %scs_help_link=scicos_help(%scs_help_link,...
                 'link',...
                 [" ";
                  " Size : 8.";
                  " Type : liste scilab."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'xx',...
                 [" Un vecteur de coordonn�es des abscisses x ";
                  " d''un lien. Un lien est d�finit comme "
                  " une polyline.";
                  " ";
                  " Taille : nombre total de points du liens.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'yy',...
                 [" Un vecteur de coordonn�es des ordonn�es y";
                  " d''un lien. Un lien est d�finit comme";
                  " une polyline.";
                  " ";
                  " Taille : nombre total de points du liens.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'id',...
                 [" Une cha�ne de caract�res d�finissant";
                  " l''identification du lien.";
                  " ";
                  " Taille : 1.";
                  " Type : cha�ne de caract�res."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'thick',...
                 [" Un vecteur de taille deux d�finissant"
                  " l''�paisseur du lien.";
                  " ";
                  " Taille : 2.";
                  " Type : vecteur ligne de nombre entiers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'ct',...
                 [" La premi�re entr�e de ce vecteur d�signe la couleur";
                  " et la deuxi�me la nature du lien. La valeur de cette";
                  " derni�re entr�e est 1 pour un lien r�gulier, -1 pour";
                  " un lien d''activation et 2 pour un lien implicite.";
                  " ";
                  " Taille : 2.";
                  " Type : vecteur ligne de nombre entiers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'from',...
                 [" Un vecteur de taille 3 qui designe un num�ro de";
                  " bloc, un num�ro de port et un type de port (0 pour";
                  " une sortie et 1 pour une entr�e) concernant l''origine";
                  " du lien. Notez que la troisi�me entr�e peut �tre 1 si";
                  " le lien est implicite. Sinon il est z�ro.";
                  " ";
                  " Taille : 3.";
                  " Type : vecteur ligne de nombre entiers."])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'to',...
                 [" Un vecteur de taille 3 qui designe un num�ro de";
                  " bloc, un num�ro de port et un type de port (0 pour";
                  " une sortie et 1 pour une entr�e) concernant la destination";
                  " du lien. Notez que la troisi�me entr�e peut �tre 1 si";
                  " le lien est implicite. Sinon il est z�ro.";
                  " ";
                  " Taille : 3.";
                  " Type : vecteur ligne de nombre entiers."])

  //****** scicos_state ******/
  //**************************/
  %scs_help_state=scicos_help(%scs_help_state,...
                 'state',...
                  [" Une liste typ�e scilab de type {xcs}. Cette";
                   " liste contient tous les �tats du diagramme qui";
                   " vont �voluer pendant la simulation.";
                   " state contient {x}, {z}, {oz}, {iz}, "
                   " {tevts}, {evtspt}, {pointi} et {outtb}."
                   " ";
                   " Taille : 9.";
                   " Type : tlist scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'x',...
                 [" Le registre des �tats continus qui est obtenu";
                  " en concatenant tous les �tats continus de tous";
                  " les blocs."
                  " ";
                  " Taille : total des tailles de tous les registres des �tats continus.";
                  " Type : vecteur colonne de nombres r�els."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'z',...
                 [" Le registre des �tats discrets qui est obtenu";
                  " en concatenant tous les �tats discrets de tous";
                  " les blocs."
                  " ";
                  " Taille : total des tailles de tous les registres des �tats discrets.";
                  " Type : vecteur colonne de nombres r�els."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'oz',...
                 [" La liste des �tats objets discrets obtenue";
                  " par la concat�nation de tous les �tats objets";
                  " de tous les blocs.";
                  " ";
                  " Taille : total des tailles de toutes les listes des �tats objet.";
                  " Type : liste scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'iz',...
                 [" Vecteur de taille �gale au nombre de blocs.";
                  " Ce vecteur est utilis� pour enregistrer les"
                  " pointeurs des registres de travail (work)"
                  " Si un bloc fait une allocation du registre work"
                  " � l''initialisation (flag=4) alors le pointeur"
                  " associ� est enregistr� ici."
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de nombres r�els."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'tevts',...
                 [" Un vecteur de taille �gale au nombre";
                  " de sources d''activation. C''est une table"
                  " contenant les dates des �v�nements"
                  " programm�s dans {evtspt}."
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'evtspt',...
                 [" Un vecteur de taille �gale au nombre";
                  " de sources d''activation. C''est une table"
                  " d''�v�nements."
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'pointi',...
                 [" Le num�ro du prochain �v�nement.";
                  " ";
                  " Taille : 1.";
                  " Type : entier."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'outtb',...
                 [" Liste scilab contenant tous les registres";
                  " de sortie des blocs. Chaque �l�ments de cette"
                  " liste contient des donn�es typ�es matricielles."
                  " ";
                  " Taille : nombre de ports r�guliers de sortie.";
                  " Type : liste scilab contenant des matrices."])

  //****** scicos_sim ******/
  //************************/
  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'sim',...
                 [" Une liste typ�e scilab de type {scs}. Cette";
                  " liste contient des tableaux statiques issus"
                  " du r�sultat de la compilation. Ces tableaux"
                  " n''�voluent pas pendant la simulation."
                  " ";
                  " Taille : 33.";
                  " Type : tlist scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funs',...
                 [" Une liste qui contient les noms des";
                  " fonctions de calculs ou des fonctions";
                  " scilab.";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'xptr',...
                 [" Un vecteur pointant vers le registre des �tats";
                  " continus {x}. Le registre des �tats continus";
                  " du bloc {i} peut-�tre retrouv� par :";
                  " {state.x(sim.xptr(i):sim.xptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zptr',...
                 [" Un vecteur pointant vers le registre des �tats";
                  " discret {z}. Le registre des �tats discrets";
                  " du bloc {i} peut-�tre retrouv� par :";
                  " {state.z(sim.zptr(i):sim.zptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ozptr',...
                 [" Un vecteur pointant vers le registre des �tats";
                  " objets discret {oz}. Le registre des �tats objets";
                  " du bloc {i} peut-�tre retrouv� par :";
                  " {state.oz(sim.ozptr(i):sim.ozptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zcptr',...
                 [" Un vecteur pointant vers le registre des";
                  " d�tections de surfaces. Ce vecteur indique";
                  " par bloc le nombre de passages � z�ro utilis�.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inpptr',...
                 [" (sim.inpptr(i+1)-sim.inpptr(i)) donne le nombre";
                  " de ports r�guliers d''entr�e du bloc i.";
                  " inpptr(i) pointe vers le d�but des ports d''entr�e"
                  " du bloc i et les connections � ces ports peuvent"
                  " retrouv�es par la table d''indirection {inplnk}."
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outptr',...
                 [" (sim.outptr(i+1)-sim.outptr(i)) donne le nombre";
                  " de ports r�guliers de sortie du bloc i.";
                  " outptr(i) pointe vers le d�but des ports de sortie"
                  " du bloc i et les connections � ces ports peuvent"
                  " retrouv�es par la table d''indirection {outlnk}."
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inplnk',...
                 [" (cpr.sim.inplnk(cpr.sim.inpptr(i)-1+j))";
                  " est l''index du lien connect� � l''entr�e j";
                  " du bloc i, o� j va de 1 jusqu''�";
                  " (cpr.sim.inpptr(i+1)-cpr.sim.inpptr(i)).";
                  " ";
                  " Taille : nombre total de port r�guliers d''entr�e.";
                  " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outlnk',...
                 [" (cpr.sim.outlnk(cpr.sim.outptr(i)-1+j))";
                  " est l''index du lien connect� � la sortie j";
                  " du bloc i, o� j va de 1 jusqu''�";
                  " (cpr.sim.outptr(i+1)-cpr.sim.outptr(i)).";
                  " ";
                  " Taille : nombre total de port r�guliers de sortie.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpar',...
                 [" Vecteur des param�tres r�els qui est";
                  " obtenu en concatenant les registres"
                  " des param�tres r�els de tous les blocs."
                  " ";
                  " Taille : nombre total de param�tres r�els.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpptr',...
                 [" Un vecteur pointant vers le registre des";
                  " param�tres r�els {rpar}. Les param�tres "
                  " r�els du bloc i peuvent �tre retrouv�s par :";
                  " {sim.rpar(sim.rpptr(i):sim.rpptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipar',...
                 [" Vecteur des param�tres entiers qui est";
                  " obtenu en concatenant les registres"
                  " des param�tres entiers de tous les blocs."
                  " ";
                  " Taille : nombre total de param�tres entiers.";
                  " Type : vecteur colonne de nombre r�els."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipptr',...
                 [" Un vecteur pointant vers le registre des";
                  " param�tres entier {ipar}. Les param�tres "
                  " entiers du bloc i peuvent �tre retrouv�s par :";
                  " {sim.ipar(sim.ipptr(i):sim.ipptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opar',...
                 [" Liste des param�tres objets qui est";
                  " obtenue en concatenant les listes"
                  " des param�tres objets de tous les blocs."
                  " ";
                  " Taille : nombre total de param�tres objets.";
                  " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opptr',...
                 [" Un vecteur pointant vers la liste des";
                  " param�tres objets {opar}. Les param�tres "
                  " entiers du bloc i peuvent �tre retrouv�s par :";
                  " {sim.opar(sim.opptr(i):sim.opptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'clkptr',...
                 [" Un vecteur pointant vers les ports ";
                  " d''activation de sortie."
                  " (cpr.sim.clkptr(i):cpr.sim.clkptr(i+1)-1)"
                  " donne les num�ros des ports de sortie"
                  " �v�nementiel du bloc i."
                  " "
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordptr',...
                 [" Un vecteur pointant vers {ordclk}";
                  " (cpr.sim.ordptr(i):cpr.sim.ordptr(i+1)-1)"
                  " pointe vers la zone de ordclk qui"
                  " indique les num�ros des blocs activ�s par"
                  " le port �v�nementiel num�rot�s i."
                  " ";
                  " Taille : nombre de source d''activations + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'execlk',...
                 ["Non utilis�.";
                  " ";
                  " Taille : ";
                  " Type : "])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordclk',...
                 [" Une matrice associ� aux blocs activ�s par";
                  " �v�nements. La premi�re colonne contient";
                  " les num�ros des blocs et la deuxi�me le"
                  " code de l''�v�nement par lequel le bloc"
                  " sera appel� pendant la simulation."
                  " ";
                  " Taille : nombre total de bloc somm� par source d''activations.";
                  " Type : matrice de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'cord',...
                 [" Une matrice associ� aux blocs activ�s en";
                  " permanence. La premi�re colonne contient";
                  " les num�ros des blocs et la deuxi�me le"
                  " code de l''�v�nement par lequel le bloc"
                  " sera appel� pendant la simulation."
                  " ";
                  " Taille : ncord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'oord',...
                 [" Sous-ensemble de {cord}. Les blocs de cette";
                  " matrice ont des sorties qui affectent le "
                  " calcul des �tats continus.";
                  "  ";
                  " Taille : noord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zord',...
                 [" Sous-ensemble de {cord}. Les blocs de cette";
                  " matrice ont des sorties qui affectent le "
                  " calcul des d�tections de surface.";
                  "  ";
                  " Taille : nzord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'critev',...
                 [" Un vecteur de taille �gale au nombre";
                  " d''activation. Ce vecteur est compos� de";
                  " 0 et de 1 qui indiquent si des �v�nements";
                  " sont critiques dans le sens o� le solveur";
                  " doit red�marrer � froid.";
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nb',...
                 [" Nombre de blocs. Notez que le nombre de";
                  " blocs peut �tre diff�rent du nombre";
                  " original de blocs dans le diagramme";
                  " car la fonction {c_pass2} du compilateur";
                  " peut dupliquer des blocs synchrones.";
                  " "
                  " Taille : 1.";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ztyp',...
                 [" Un vecteur de taille �gale au nombre de blocs.";
                  " Une entr�e �gale � 1 indique que le bloc poss�de";
                  " une d�tection de surface."
                  " ";
                  " Taille : nombre de source d''activations + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nblk',...
                 ["Pas utilis�. Mis � {nb}.";
                  " ";
                  " Taille : 1.";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ndcblk',...
                 ["Pas utilis�.";
                  " ";
                  " Taille : -.";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'subscr',...
                 ["Pas utilis�.";
                  " ";
                  " Taille : 0.";
                  " Type : vide."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funtyp',...
                 [" Un vecteur de taille �gale au nombre de blocs";
                  " qui indique le type de la fonction de calcul.";
                  " Le type de bloc peut aller de 0 � 5.";
                  " Les blocs courants de Scicos sont les blocs";
                  " de type 4 (bloc C) et de type 5 (bloc scilab).";
                  " Les anciens blocs restent compatibles.";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'iord',...
                 [" Une matrice qui associe les blocs � activer ";
                  " au d�but de la simulation. Cel� inclue les";
                  " blocs {Constante} et les blocs activ�s en";
                  " permanence.";
                  " ";
                  " Taille : niord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'labels',...
                 [" Un vecteur de cha�ne de caract�res de";
                  " taille �gale au nombre de blocs "
                  " contenant les labels des blocs.";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de cha�nes de caract�res."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'modptr',...
                 [" Un vecteur pointant sur les modes des blocs.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])
  end
  //==========================================================

  //Build final Scicos Help list =============================

  //** update scs_help_scilst lists
  %scs_help_scilst.diagram=%scs_help_diagram;   //** diagr
  %scs_help_scilst.params=%scs_help_params;     //** params
  %scs_help_scilst.model=%scs_help_model;       //** model
  %scs_help_scilst.graphics=%scs_help_graphics; //** graphics
  %scs_help_scilst.link=%scs_help_link;         //** link
  %scs_help_scilst.state=%scs_help_state;       //** state
  %scs_help_scilst.sim=%scs_help_sim;           //** sim
  %scs_help_scilst.block=%scs_help_block;       //** block
  %scs_help_scilst.cpr=%scs_help_cpr;       //** cpr

  %scicos_help=tlist(['sch','menu','scilst'],..
                      %scs_help_menu,%scs_help_scilst);
  //==========================================================

  //** This is the END, the END my friend". By The Doors, Apocalypse Now.
endfunction
