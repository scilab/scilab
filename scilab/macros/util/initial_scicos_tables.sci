function [scicos_pal,%scicos_menu,%scicos_short,%scicos_help,%scicos_display_mode]=...
    initial_scicos_tables()
  
%scicos_display_mode=0  // =1 if pixmap to be used for display

// Define scicos palettes of blocks
scicos_pal=['Sources','SCI/macros/scicos/Sources.cosf'
      'Sinks','SCI/macros/scicos/Sinks.cosf';
      'Linear','SCI/macros/scicos/Linear.cosf';
      'Non_linear','SCI/macros/scicos/Non_linear.cosf';
      'Events','SCI/macros/scicos/Events.cosf';
      'Treshold','SCI/macros/scicos/Treshold.cosf';
      'Others','SCI/macros/scicos/Others.cosf';
      'Branching','SCI/macros/scicos/Branching.cosf'];
// define graphics mode for Scicos (with or without scroll bar) 

//Scicos Menu definitions
 %scicos_menu=list(['Diagram','Replot','New','Region to Super Block','Purge','Rename','Save','Save As',..
	'Load','Load as Palette','Save as Palette','Save as Interf Func',..
	'Set Diagram Info','Navigator','Export','Export All','Quit'],..
['Edit','Palettes','Context','Smart Move','Move','Copy','Copy Region',..
	'Replace','Align','Link','Delete','Delete Region','Add new block',..
	'Flip','Undo','Pal editor'],..
['Simulate','Setup','Compile','Eval','Run'],..
['Object','Open/Set','Resize','Icon','Icon Editor','Color','Label','Get Info','Identification','Documentation','Code Generation'],..
['Misc','Background color','Default link colors','ID fonts','Aspect',..
	'Add color','Shortcuts','Zoom in','Zoom out','Help','Calc']);

%scicos_short=[['d','c','m','u','f','o','s','i','r','l','q'];
['Delete','Copy','Move','Undo','Flip','Open/Set',..
    'Save','Get Info','Replot','Link','Quit']]'

%scicos_help=tlist(..
 ['sch','Window','Background color','Default link colors','ID font','3D aspect','Add color',..
 'Focus','Shift','Zoom in','Zoom out','Help','Calc','Palettes','Context','Smart Move',..
 'Move','Copy','Copy Region','Replace','Align','Link','Delete','Delete Region',..
 'Add new block','Flip','Undo','Setup','Compile','Eval','Run','Replot','New',..
 'Region to Super Block','Purge','Rename','Save','Save As','Load','Load as Palette',..
 'Save as Palette','Save as Interf. Func.','Set Diagram Info','Navigator','Quit','Open/Set',..
 'Resize','Icon','Color','Label','Get Info','Identification','Documentation','Code Generation'],..
 [' In the active editor Scicos window, clicking on the ';
 ' Window menu item invokes a dialog box that allows you to change ';
 ' window dimensions'],..
 [' This menu allows to change the background and defaukt foreground';' colors'],..
 [' This menu allows to change the default color for regular ';' and event links'],..
 [' This menu allows to change the font used to write the block';
 ' identifications (see ""Set block ID"" menu)'],..
 [' This menu allows to select 3D shape for blocks and ';' associated parameters'],..
 [' This menu allows to add new color to the diagram private';
 ' color map. Added colors are stored in the diagram data structure'],..
 [' Focus menu allows to select a zone (click left, drag zone, click';
 ' to select) which is focused on';
 ' To change focus back use ""Zoom in"" menu'],..
 [' To shift the diagram to left, right, up or down,';
 ' select this menu item, then click on the point you want ';
 ' to appear in the middle of the graphics window. '],..
 [' When you select this menu item the diagram is zoomed in ';'by a factor of 10%'],..
 [' When you select this menu item the diagram is zoomed out ';'by a factor of 10%'],..
 [' To get help on an object or menu buttons,';
 ' select first Help menu item and then on ';
 ' the selected object or menu item.'],..
 [' When you select this menu item you switch Scilab to ';
 ' the pause mode (see the help on pause).';
 ' In the Scilab main window and you may enter Scilab instructions';
 ' to compute whatever you want.';
 ' to go back to Scicos you need enter the ""return"" or';
 ' ""[...]],=return(...)"" Scilab instruction.';
 ' ';
 ' If you use ""[...]=return(...)"" Scilab instruction take care';
 ' not to modify Scicos variables such as ""scs_m"",""scs_gc"",';
 ' ""menus"",""datam"",...';
 ' ';
 ' If you have modified scicos graphic window you may retore it ';
 ' using the Scicos ""Replot"" menu.'],..
 ' Select the Palettes menu item to open a predefined palette.',..
 [' When you select this menu item you get a dialogue to';
 ' enter scilab instructions for defining symbolic scicos parameters';
 '  used in block definitions or to do whatever you want';
 ' ';
 ' These instructions will be evaluated each time the diagram ';
 ' is loaded.';
 ' ';
 ' If you  change the value of a symbolic scicos parameters in ';
 ' the contextyou can either click on the block(s) that use this';
 ' variable or on the Eval menu item to update actual block parameter';
 ' value.'],..
 [' To move a block in  the active editor Scicos window';
 ' or in edited palette keeping horizontal and vertical';
 ' links, select first the ""Smart Move"" menu item, ';
 ' then click on the selected block, link segment or link';
 ' corner, drag the mouse to the desired new  position ';
 ' and click left again to fix the position.';
 ' ';
 ' Right click cancel the move action'],..
 [' To move a block in  the active editor Scicos window';
 ' or in edited palette,';
 ' select first the Move menu item, ';
 ' then click on the selected block, link segment or link';
 ' corner, drag the mouse to the desired new block position ';
 ' and click left again to fix the position.';
 ' ';
 ' Right click cancel the move action'],..
 ['To copy a block in the active editor Scicos window';
 ' select first the Copy menu item, then';
 ' click (with left button) on the to-be-copied block';
 ' in Scicos windows or in a palette) ,  and';
 ' finally click left where you want the copy';
 ' be placed in the active editor Scicos window.';
 ' ';
 ' The lower left corner of the block is placed';
 ' at the selected point.';
 ' This menu remains active until user choose an other one';
 ' ';
 ' Right click cancel the copy action'],..
 ['To copy a region in the active editor Scicos window';
 ' select first the Copy menu item, then';
 ' click (with right button) on a corner of the desired';
 ' region (in Scicos windows or in a palette), drag to ';
 ' define the region, click to fix the region  and';
 ' finally click left where you want the copy.';
 ' to be placed in the active editor Scicos window.';
 ' NOTE: If source diagram is big be patient, region selection ';
 ' may take a while.';
 ' ';
 ' The lower left corner of the block is placed';
 ' at the selected point.';
 ' ';
 ' Right click cancel the copy action'],..
 [' To replace a block in the active editor Scicos window';
 ' select first the Replace menu item, then';
 ' select the replacement block (in Scicos window or in a';
 ' palette), and  finally click on the to-be-replaced block'],..
 [' To obtain nice diagrams, you can align ports of';
 ' different blocks, vertically and horizontally.';
 ' Select first  the Align menu item, then click on the first';
 ' port and finally on the second port.';
 ' The block corresponding to the second port is moved.';
 ' ';
 ' A connected block cannot be aligned.'],..
 [' To connect an output port to an input port,';
 ' select first  the Link menu item, then click on the output';
 ' port, drag, click left on each intermediate points';
 ' and finally click left on the input port.';
 ' ';
 ' To split a link, select first  the Link menu item,';
 ' then click left on the link where the split should be placed,';
 ' drag, click left on each intermediate points';
 ' and finally click left on the input port.';
 ' ';
 ' Right click cancel the link action';
 ' ';
 ' Only one link can go from and to a port.';
 ' Link color can be changed directly by clicking';
 ' on the link.';
 ' ';
 ' This menu remains active until user choose an other one'],..
 ['To delete  blocks or a links, select first the Delete';
 ' menu item, then click successively on the selected objects';
 '(with left button).';
 ' ';
 ' When you delete a block all links connected to it';
 ' are deleted as well.';
 ' ';
 ' This menu remains active until user choose an other one'],..
 ['To delete a blocks in  a region, select first  the Delete Region';
 ' menu item, then click  on a corner of the ';
 ' desired region, drag to define the region, and click left to ';
 ' fix the region. All connected links will be destroyed as';
 ' well';
 ' ';
 ' Right click instead of left cancels the delete  action'],..
 [' To add a newly defined block to the current palette or diagram';
 ' select first this menu item, A dialog box will popup ';
 ' asking for the name of the GUI function associated ';
 ' with the block. If this function is not already loaded';
 ' it was search in the current directory. The user may then';
 ' click at the desired position of the block icon '],..
 [' To reverse the positions of the (regular) inputs';
 ' and outputs of a block placed on its sides,';
 ' select the Flip menu item first and then click on the';
 ' selected block. This does not affect the order,';
 ' nor the position of the input and output event';
 ' ports which are numbered from left to right.';
 ' ';
 ' A connected block cannot be flipped.'],..
 [' Select the Undo menu item to undo the last edit operation.';
 ' It is not possible to undo more!'],..
 [' In the main Scicos window, clicking on the Setup menu item';
 ' invokes a dialog box that allows you to change ';
 ' integration parameters: ';
 '   *final integration time';
 '   *absolute and relative error tolerances';
 '   *time tolerance (the smallest time interval for which ';
 '         the ode solver is used to update continuous states)';
 '   *deltat : the maximum time increase realized by a single';
 '         call to the ode solver'],..
 [' select the Compile menu item to compile the block diagram.';
 ' This menu item need never be used since compilation is';
 ' performed automatically, if necessary, before';
 ' the beginning of every simulation (Run menu item).';
 ' ';
 ' Normally, a new compilation is not needed if only';
 ' system parameters and internal states are modified.';
 ' In some cases however these modifications are not';
 ' correctly updated and a manual compilation may be';
 ' needed before a Restart or a Continue.';
 ' Please report if you encounter such a case.'],..
 [' All dialogs user answers may be scilab instructions';
 ' they are evaluated immediatly and stored as character strings.';
 ' select this menu item to have them re-evaluated according to';
 ' new values of underlying scilab variables. ';
 ' ';
 ' These underlying scilab variables may be user global variables';
 ' defined before scicos was launch, They may also be defined in';
 ' by the scicos context (see Context menu item)'],..
 [' select the Run menu item to start the simulation.';
 ' If the system has already been simulated, a';
 ' dialog box appears where you can choose to Continue,';
 ' Restart or End the simulation.';
 ' ';
 ' You may interrupt the simulation by clicking on the ';
 ' ""stop"" button, change any of the block parameters';
 ' and continue the simulation with the new values.'],..
 [' Select the Replot menu item to replot the content of';
 ' the graphics window. Graphics window stores complete';
 ' history of the editing session in memory.';
 ' ';
 ' Replot is usefull for ''cleaning'' this memory.'],..
 [' Clicking on the New menu item loads an empty diagram in the';
 ' active editor Scicos window. If the previous content of the';
 ' window is not saved, it will be lost.'],..
 [' This menu allows to transform a rectangular region of the';
 ' current diagram by a super block.';
 ' Click  on a corner of the region , drag an click left to';
 ' fix the region (left click cancels selection)';
 ' ';
 ' Region is replaced by a super block ans links are redrawn'],..
 [' select the Purge menu item to get a clean data structure:';
 ' If diagram has been hugely modified many deleted blocks';
 ' may remain in the data structure. It may be  usefull to';
 ' suppress then before saving.'],..
 [' This menu allows to change the diagram name. An editable';'  dialog box opens.'],..
 [' select the save menu item to save the block diagram';
 ' in a binary file already selected by a previous';
 ' select the Save As menu item. If you select this';
 ' menu item and you have never clicked on the Save As';
 ' menu item, the diagram is saved in the current directory';
 ' as <window_name>.cos where <window_name> is the name';
 ' of the window appearing on top of the window (usually';
 ' Untitled or Super Block).'],..
 [' select the Save As menu item to save the block diagram';
 ' or palette in a  file. A dialog box allows choosing ';
 ' the file which must have a .cos or .cosf extension. The diagram';
 ' takes the name of the file (without the extension).';
 ' ';
 ' If extension is "".cosf"" an ascii  formatted save is performed';
 ' instead of binary save. Formatted save is slower than regular ';
 ' save but has the advantage that the generated file is system ';
 ' independent (usefull for exchanging data on different computers)'],..
 [' select the Load menu item to load an ascii or binary file';
 ' containing a saved block diagram or palette.';
 ' A dialog box allows user choosing the file.'],..
 [' select the Load menu item to load an ascii or binary file';
 ' containing a saved block diagram as a palette.';
 ' A dialog box allows user choosing the file.'],..
 [' select the Save as Palette menu item to save the block diagram';
 ' as a palette in a  file. A dialog box allows choosing ';
 ' the file which must have a .cos or .cosf extension. The palette';
 ' takes the name of the file (without the extension).';
 ' ';
 ' If extension is "".cosf"" an ascii  formatted save is performed';
 ' instead of binary save. It may take a while';
 ' ';
 ' .scilab user file is updated if necessary'],..
 [' Select ""the Save as Interf. Func."" menu item to save the ';
 ' diagram as a new Scicos block. A Scilab function is generated';
 ' and saved in a  file with "".sci"" extension. File name and path';
 ' are to be set in a ""File menu"" dialog.'],..
 [' This menu allows to set users diagram informations';
 ' these infos are stored in the diagram data structure';
 ' and may be used as diagram user documentation';
 ' ';
 ' information format may be redefined by user '],..
 [' This experimental menu opens a graphic window with a tree ';
 ' representation of the super blocks hierarchy. Each node ';
 ' represents a superblock.';
 ' ';
 ' Navigator window is usefull to open directly a super-block';
 ' every where in the hierarchy.'],..
 [' Click on the Exit menu item to close current diagram. ';
 ' If current diagram is not a Super block Exit menu item ';
 ' leave Scicos and return to Scilab session. Save your diagram ';
 ' or palette before leaving.';
 ' ';
 ' File/Close menu as the same effect'],..
 [' To change the parameters of a regular block or link, ';
 ' to open a super block, select first ';
 ' this menu item, click next on the desired object.';
 ' A dialog or edition window appears';
 ' that allows you to modify object';
 ' ';
 ' It is also possible to select a super block to open clicking';
 ' on a node of the ""Navigator"" window'],..
 [' To change the size of a block , select first this menu item,';
 ' click next on the desired block. A dialog appear that allows ';
 ' you to change the width and/or height of the block shape.'],..
 [' To change the icon of a block, select first this menu item,';
 ' click next on the desired block. A dialog appear that allows ';
 ' you to enter scilab instructions used to draw the icon';
 ' ';
 ' You may use the icon_edit function to generate the scilab';
 ' instructions'],..
 [' To change the background color of an object, select first ';
 ' this menu item, click next on the desired object. A dialog appear';
 ' that allows you to choose the desired color'],..
 [' To add a label to block, select first this menu item, click next';
 ' on the desired block. A dialog appear that allows you to enter ';
 ' the desired label.';
 ' labels are used to import data from a block in an other one'],..
 [' This menu allows to get information on an object and on ';
 ' its connection with other diagram objects.';
 ' ';
 ' Select this menu and click on an object';
 ' This menu remains selected'],..
 [' This menu allows to set an identificator to a link or a block ';
 ' block identificators are drawn under the block icon. Super blocks';
 ' input/output ports identificators are replicated over the block';
 ' shape ports. Links identificators are not displayed';
 ' ';
 ' Selecting this menu and clicking on a block or links opens an';
 ' editable dialog box'],..
 [' This menu allows to set or get documentation for a block ';
 ' ';
 ' Selecting this menu and clicking on a block  opens an';
 ' editable dialog box'],..
 [' This menu allows to generate the simulation code associated with' 
  ' a discrete time Superblock'
  ''
  ' The code generation is obtained simply by selecting this menu and  '
  ' then the desired superblock.'
  ' '
  ' If the superblock statisfies the required conditions, a dialog box'
  ' pops up to ask for a block name, a directory where to put the'
  ' generated files and for optional libraries requested by the linker.'
  ' '
  ' Given this informations the code is generated, compiled and linked with '
  ' Scilab. The superblock is automatically replaced by a new block which '
  ' implements the generated code. It is then possible to run the modified '
  ' diagram.'])
endfunction
