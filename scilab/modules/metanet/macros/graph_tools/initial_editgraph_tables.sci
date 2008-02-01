function [Menus,Shorts]=initial_editgraph_tables()
//Copyright INRIA
//Author : Serge Steer 2002

//Editgraph Menu definitions
  Menus=list(['File','New','SaveAs','Save','Load','Export','Quit'],..
	     ['Graph','Add Node Data Field','Add Arc Data Field','Settings'],.. 
	     ['View','Zoom','Replot','Find Node','Find Arc','Options','Information'],..
	     ['Edit','NewNode','NewArc','Move Node','Move Region',..
	      'Copy Region To ClipBoard','Paste','Delete',..
	      'Delete Region','Properties','Give default names','Undo'],..
	     ["Compute","Shortest path","Circuit","Salesman"],..
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
