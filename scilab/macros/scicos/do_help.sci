function do_help()
// Copyright INRIA

while %t do
  [btn,xc,yc,cwin,Cmenu]=cosclick(0)
  if Cmenu<>[] then
    name=Cmenu
    nm=1
    break
  elseif cwin==curwin then 
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
      o=scs_m(k)
      name=o(5)
      nm=0
      break
    end
  elseif or(windows(find(windows(:,1)<0),2)==cwin) then
    kwin=find(windows(:,2)==cwin)
    pal=palettes(-windows(kwin,1))
    k=getobj(pal,[xc;yc])
    if k<>[] then
      o=pal(k)
      name=o(5)
      nm=0
      break
    end
  end
end
if nm==0 then
  help(name)
//  unix_s('$SCI/bin/scilab -help ""'+name+'"" | $SCI/bin/xless &')
  return
end
select name
// Misc menu---------------------------------------------------------
case 'Window' then
 mess=[' In the active editor Scicos window, clicking on the ';
       ' Window menu item invokes a dialog box that allows you to change ';
       ' window dimensions'];
case 'Background color' then    
 mess=[' This menu allows to change the background and defaukt foreground'
       ' colors']
case 'Default link colors' then    
 mess=[' This menu allows to change the default color for regular '
       ' and event links']
case 'ID font' then    
 mess=[' This menu allows to change the font used to write the block'
       ' identifications (see ""Set block ID"" menu)']
case '3D aspect' then   
  mess=[' This menu allows to select 3D shape for blocks and '
          ' associated parameters']
case 'Add color' then         
  mess=[' This menu allows to add new color to the diagram private'
        ' color map. Added colors are stored in the diagram data structure']
case 'Focus' then    
  mess=[' Focus menu allows to select a zone (click left, drag zone, click'
        ' to select) which is focused on';
	' To change focus back use ""Zoom in"" menu']
case 'Shift' then
  mess=[' To shift the diagram to left, right, up or down,';
      ' select this menu item, then click on the point you want ';
      ' to appear in the middle of the graphics window. '];
case 'Zoom in' then
  mess=[' When you select this menu item the diagram is zoomed in '
      'by a factor of 10%']
case 'Zoom out' then  
  mess=[' When you select this menu item the diagram is zoomed out '
      'by a factor of 10%']
case 'Help' then
  mess=[' To get help on an object or menu buttons,';
        ' select first Help menu item and then on ';
	' the selected object or menu item.']
case 'Calc' then 
  mess=[' When you select this menu item you switch Scilab to ';
      ' the pause mode (see the help on pause).';
      ' In the Scilab main window and you may enter Scilab instructions';
      ' to compute whatever you want.';
      ' to go back to Scicos you need enter the ""return"" or';
      ' ""[...]=return(...)"" Scilab instruction.';
      ' '
      ' If you use ""[...]=return(...)"" Scilab instruction take care';
      ' not to modify Scicos variables such as ""scs_m"",""scs_gc"",';
      ' ""menus"",""datam"",...';
      ' '
      ' If you have modified scicos graphic window you may retore it ';
      ' using the Scicos ""Replot"" menu.']

// Edit menu---------------------------------------------------------
case 'Palettes' then
  mess=[' Select the Palettes menu item to open a predefined palette.']

case 'Context' then
  mess=[' When you select this menu item you get a dialogue to';
    ' enter scilab instructions for defining symbolic scicos parameters';
    '  used in block definitions or to do whatever you want';
    ' ';
    ' These instructions will be evaluated each time the diagram ';
    ' is loaded.'
    ' ';
    ' If you  change the value of a symbolic scicos parameters in ';
    ' the contextyou can either click on the block(s) that use this';
    ' variable or on the Eval menu item to update actual block parameter';
    ' value.']

case 'Smart Move' then
  mess=[' To move a block in  the active editor Scicos window';
        ' or in edited palette keeping horizontal and vertical'
        ' links, select first the ""Smart Move"" menu item, '
	' then click on the selected block, link segment or link'
	' corner, drag the mouse to the desired new  position '
	' and click left again to fix the position.'
	' '
	' Right click cancel the move action']

case 'Move (m)' then
  mess=[' To move a block in  the active editor Scicos window';
        ' or in edited palette,'
        ' select first the Move menu item, '
	' then click on the selected block, link segment or link'
	' corner, drag the mouse to the desired new block position '
	' and click left again to fix the position.'
	' '
	' Right click cancel the move action']


case 'Copy (c)' then
  mess=['To copy a block in the active editor Scicos window';
         ' select first the Copy menu item, then'
	 ' click (with left button) on the to-be-copied block'
	 ' in Scicos windows or in a palette) ,  and'
	 ' finally click left where you want the copy';
	 ' be placed in the active editor Scicos window.';
	 ' '
	 ' The lower left corner of the block is placed';
	 ' at the selected point.';
	 ' This menu remains active until user choose an other one';
	 ' '
	 ' Right click cancel the copy action']
     
case 'Copy Region' then
  mess=[ 'To copy a region in the active editor Scicos window';
         ' select first the Copy menu item, then'
	 ' click (with right button) on a corner of the desired';
	 ' region (in Scicos windows or in a palette), drag to '
	 ' define the region, click to fix the region  and'
	 ' finally click left where you want the copy.' 
	 ' to be placed in the active editor Scicos window.';
	 ' NOTE: If source diagram is big be patient, region selection '
	 ' may take a while.'
	 ' '
	 ' The lower left corner of the block is placed';
	 ' at the selected point.';
	 ' '
	 ' Right click cancel the copy action']
     
case 'Replace' then	
 mess=[' To replace a block in the active editor Scicos window';
         ' select first the Replace menu item, then'
	 ' select the replacement block (in Scicos window or in a'
	 ' palette), and  finally click on the to-be-replaced block']
     


case 'Align' then
  mess=[' To obtain nice diagrams, you can align ports of';
        ' different blocks, vertically and horizontally.';
	' Select first  the Align menu item, then click on the first';
	' port and finally on the second port.';
	' The block corresponding to the second port is moved.';
	' '
	' A connected block cannot be aligned.']


case 'Link (l)' then
  mess=[' To connect an output port to an input port,';
        ' select first  the Link menu item, then click on the output';
	' port, drag, click left on each intermediate points'
	' and finally click left on the input port.';
	' '
	' To split a link, select first  the Link menu item,';
	' then click left on the link where the split should be placed,';
	' drag, click left on each intermediate points'
	' and finally click left on the input port.'
	' '
	' Right click cancel the link action'
	' '
	' Only one link can go from and to a port.';
	' Link color can be changed directly by clicking';
	' on the link.'
	' '
	' This menu remains active until user choose an other one']

case 'Delete (d)' then
  mess=['To delete  blocks or a links, select first the Delete'
        ' menu item, then click successively on the selected objects'
	'(with left button).';
	' '
	' When you delete a block all links connected to it';
	' are deleted as well.'
	' '
	' This menu remains active until user choose an other one']
    
case 'Delete Region' then
  mess=['To delete a blocks in  a region, select first  the Delete Region'
        ' menu item, then click  on a corner of the ';
	' desired region, drag to define the region, and click left to ';
	' fix the region. All connected links will be destroyed as';
	' well'
	' '
	' Right click instead of left cancels the delete  action']

case 'Add new block' then 
  mess=[' To add a newly defined block to the current palette or diagram';
      ' select first this menu item, A dialog box will popup ';
      ' asking for the name of the GUI function associated '
      ' with the block. If this function is not already loaded';
      ' it was search in the current directory. The user may then'
      ' click at the desired position of the block icon ']

case 'Flip (f)' then
    mess=[' To reverse the positions of the (regular) inputs'
	  ' and outputs of a block placed on its sides,';
	  ' select the Flip menu item first and then click on the';
	  ' selected block. This does not affect the order,';
	  ' nor the position of the input and output event';
	  ' ports which are numbered from left to right.'
	  ' '
	  ' A connected block cannot be flipped.']


case  'Undo (u)' then
  mess=[' Select the Undo menu item to undo the last edit operation.'
        ' It is not possible to undo more!']


// Simulate menu --------------------------------------------------
case 'Setup' then
  mess=[' In the main Scicos window, clicking on the Setup menu item';
        ' invokes a dialog box that allows you to change ';
	' integration parameters: ';
	'   *final integration time';
	'   *absolute and relative error tolerances' ;
	'   *time tolerance (the smallest time interval for which ';
	'         the ode solver is used to update continuous states)';
        '   *deltat : the maximum time increase realized by a single';
	'         call to the ode solver'];

case 'Compile' then
  mess=[' select the Compile menu item to compile the block diagram.';
        ' This menu item need never be used since compilation is';
	' performed automatically, if necessary, before';
	' the beginning of every simulation (Run menu item).';
	' '
	' Normally, a new compilation is not needed if only';
	' system parameters and internal states are modified.';
        ' In some cases however these modifications are not';
	' correctly updated and a manual compilation may be';
	' needed before a Restart or a Continue.';
	' Please report if you encounter such a case.']
    
case 'Eval' then     
 mess=[' All dialogs user answers may be scilab instructions';
       ' they are evaluated immediatly and stored as character strings.'
       ' select this menu item to have them re-evaluated according to'; 
       ' new values of underlying scilab variables. '
       ' '
       ' These underlying scilab variables may be user global variables'
       ' defined before scicos was launch, They may also be defined in'
       ' by the scicos context (see Context menu item)']

case 'Run' then
  mess=[' select the Run menu item to start the simulation.';
        ' If the system has already been simulated, a';
	' dialog box appears where you can choose to Continue,'
        ' Restart or End the simulation.'
        ' '
	' You may interrupt the simulation by clicking on the '
	' ""stop"" button, change any of the block parameters'
	' and continue the simulation with the new values.']

// Diagram menu ---------------------------------------------------
case 'Replot (r)' then
  mess=[' Select the Replot menu item to replot the content of'
        ' the graphics window. Graphics window stores complete';
	' history of the editing session in memory.';
	' '
	' Replot is usefull for ''cleaning'' this memory.']

case 'New' then
  mess=[' Clicking on the New menu item loads an empty diagram in the';
        ' active editor Scicos window. If the previous content of the';
	' window is not saved, it will be lost.']
case 'Region to Super Block' then    
  mess=[' This menu allows to transform a rectangular region of the'
        ' current diagram by a super block.'
	' Click  on a corner of the region , drag an click left to'
	' fix the region (left click cancels selection)'
	' '
	' Region is replaced by a super block ans links are redrawn']
case 'Purge' then
   mess=[' select the Purge menu item to get a clean data structure:';
         ' If diagram has been hugely modified many deleted blocks';
	 ' may remain in the data structure. It may be  usefull to';
	 ' suppress then before saving.']
case 'Rename' then
   mess=[' This menu allows to change the diagram name. An editable'
         '  dialog box opens.']
   
case 'Save (s)' then
   mess=[' select the save menu item to save the block diagram';
         ' in a binary file already selected by a previous';
	 ' select the Save As menu item. If you select this';
	 ' menu item and you have never clicked on the Save As';
	 ' menu item, the diagram is saved in the current directory';
	 ' as <window_name>.cos where <window_name> is the name';
	 ' of the window appearing on top of the window (usually';
       ' Untitled or Super Block).']
 
case 'Save As' then
  mess=[' select the Save As menu item to save the block diagram';
         ' or palette in a  file. A dialog box allows choosing ';
	 ' the file which must have a .cos or .cosf extension. The diagram';
	 ' takes the name of the file (without the extension).'
	 ' '
	 ' If extension is "".cosf"" an ascii  formatted save is performed'
	 ' instead of binary save. Formatted save is slower than regular ';
	 ' save but has the advantage that the generated file is system ';
         ' independent (usefull for exchanging data on different computers)']
   
case 'Load' then
  mess=[' select the Load menu item to load an ascii or binary file';
	' containing a saved block diagram or palette.'
	' A dialog box allows user choosing the file.']
    
case 'Load as Palette' then
  mess=[' select the Load menu item to load an ascii or binary file';
	' containing a saved block diagram as a palette.'
	' A dialog box allows user choosing the file.']
    

case 'Save as Palette' then
  mess=[' select the Save as Palette menu item to save the block diagram';
         ' as a palette in a  file. A dialog box allows choosing ';
	 ' the file which must have a .cos or .cosf extension. The palette';
	 ' takes the name of the file (without the extension).';
         ' '
	 ' If extension is "".cosf"" an ascii  formatted save is performed'
         ' instead of binary save. It may take a while'
	 ' '
	 ' .scilab user file is updated if necessary']


case 'Save as Interf. Func.' then
   mess=[' Select ""the Save as Interf. Func."" menu item to save the '
         ' diagram as a new Scicos block. A Scilab function is generated'
	 ' and saved in a  file with "".sci"" extension. File name and path'
	 ' are to be set in a ""File menu"" dialog.']
   
case 'Set Diagram Info' then
   mess=[' This menu allows to set users diagram informations'
         ' these infos are stored in the diagram data structure'
	 ' and may be used as diagram user documentation'
	 ' '
	 ' information format may be redefined by user ']

case 'Navigator' then
   mess=[' This experimental menu opens a graphic window with a tree '
         ' representation of the super blocks hierarchy. Each node '
	 ' represents a superblock.'
	 ' '
	 ' Navigator window is usefull to open directly a super-block'
	 ' every where in the hierarchy.']
     
case 'Quit (q)' then
  mess=[' Click on the Exit menu item to close current diagram. '
        ' If current diagram is not a Super block Exit menu item '
        ' leave Scicos and return to Scilab session. Save your diagram ';
	' or palette before leaving.'
	' '
	' File/Close menu as the same effect']
     
//Object menu  --------------------------------------------------------
case 'Open/Set (o)' then     
 mess=[' To change the parameters of a regular block or link, '
       ' to open a super block, select first ';
       ' this menu item, click next on the desired object.'
       ' A dialog or edition window appears';
       ' that allows you to modify object'
       ' '
       ' It is also possible to select a super block to open clicking'
       ' on a node of the ""Navigator"" window'];
   
case 'Resize' then     
 mess=[' To change the size of a block , select first this menu item,';
       ' click next on the desired block. A dialog appear that allows ';
       ' you to change the width and/or height of the block shape.'];
   
case 'Icon' then     
 mess=[' To change the icon of a block, select first this menu item,';
       ' click next on the desired block. A dialog appear that allows ';
       ' you to enter scilab instructions used to draw the icon'
       ' '
       ' You may use the icon_edit function to generate the scilab'
       ' instructions'];
   
   
case 'Color' then     
 mess=[' To change the background color of an object, select first ';
       ' this menu item, click next on the desired object. A dialog appear';
       ' that allows you to choose the desired color'];
   
   
case 'Label' then     
 mess=[' To add a label to block, select first this menu item, click next';
       ' on the desired block. A dialog appear that allows you to enter ';
       ' the desired label.';
       ' labels are used to import data from a block in an other one'];
 
case 'Get Info (i)' then     
 mess=[' This menu allows to get information on an object and on '
       ' its connection with other diagram objects.'
       ' '
       ' Select this menu and click on an object'
       ' This menu remains selected']
case 'Identification' then     
 mess=[' This menu allows to set an identificator to a link or a block '
       ' block identificators are drawn under the block icon. Super blocks'
       ' input/output ports identificators are replicated over the block'
       ' shape ports. Links identificators are not displayed'
       ' '
       ' Selecting this menu and clicking on a block or links opens an'
       ' editable dialog box']
case 'Documentation' then     
 mess=[' This menu allows to set or get documentation for a block '
       ' '
       ' Selecting this menu and clicking on a block  opens an'
       ' editable dialog box']

end
if exists('mess')==0 then
  mess='No help available on this topic. Sorry.';
end
message(mess)












