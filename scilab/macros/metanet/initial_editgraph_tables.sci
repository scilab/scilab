function [Menus,Shorts]=initial_editgraph_tables()
//Copyright INRIA
//Author : Serge Steer 2002

//Editgraph Menu definitions
  Menus=list(['Graph','New','Zoom','Replot','SaveAs','Save','Load',..
	      'Options','Settings','Export','Quit'],..
	     ['Edit','NewNode','NewArc','Move','Move Region',..
	      'Copy Region To ClipBoard','Paste','Delete',..
	      'Delete Region','Properties','Default names','Undo'],..
	     ["Compute","Shortest path"])
  Shorts=['d','Delete'; 
	  'x','Delete Region'
	  'c',"Copy Region To ClipBoard"
	  'm','Move';      
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
