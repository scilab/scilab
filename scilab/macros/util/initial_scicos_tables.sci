function [scicos_pal,%scicos_menu,%scicos_short]=initial_scicos_tables()
// Define scicos palettes of blocks
scicos_pal=['Inputs_Outputs','SCI/macros/scicos/Inputs_Outputs.cosf'
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
['Object','Open/Set','Resize','Icon','Icon Editor','Color','Label','Get Info','Identification','Documentation'],..
['Misc','Window','Background color','Default link colors','ID fonts','Aspect',..
	'Add color','Shortcuts','Zoom in','Zoom out','Help','Calc']);

%scicos_short=[['d','c','m','u','f','o','s','i','r','l','q'];
['Delete','Copy','Move','Undo','Flip','Open/Set',..
    'Save','Get Info','Replot','Link','Quit']]'

