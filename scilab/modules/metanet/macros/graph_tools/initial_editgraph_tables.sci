
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Menus,Shorts]=initial_editgraph_tables()

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
