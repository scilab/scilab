function [Menus,Shorts]=initial_editgraph_tables()
//Copyright INRIA
//Author : Serge Steer 2002

//Editgraph Menu definitions
  Menus=list(['Graph','New','Zoom','Replot','Find Node','Find Arc','SaveAs','Save','Load',..
	      'Options','Settings','Information','Export','Quit'],..
	     ['Edit','NewNode','NewArc','Move Node','Move Region',..
	      'Copy Region To ClipBoard','Paste','Delete',..
	      'Delete Region','Properties','Give default names','Undo'],..
	     ["Compute","Shortest path"],..
	     ["Help"])
  Shorts=['d','Delete'; 
	  'x','Delete Region'
	  'c',"Copy Region To ClipBoard"
	  'm','Move Node';      
	  'u','Undo';      
	  'n','NewNode';   
	  'a','NewArc';    
	  's','Save';      
	  'r','Replot';    
	  'q','Quit';  
	  'p','Properties';
	  'v','Paste'
	  'o','Options'
	  'l','Load']
endfunction
