// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [rep] = getsymbol(Title)
[lhs,rhs] = argn(0)
if rhs<1 then
  Title = "",
end;
marksIds = ["dot"
	    "plus"
	    "cross"
	    "star"
	    "filled diamond"
	    "diamond"
	    "triangle up"
	    "triangle down"
	    "diamond plus"
	    "circle"
	    "asterisk"
	    "square"
	    "triangle right"
	    "triangle left"
	    "pentagram"]'

fontsSiz = ["08","10","12","14","18","24"];
lmid = list("markId",1,marksIds);
lmsiz = list("marksize",1,fontsSiz);
xch_l = list(lmid,lmsiz);
entval = [lmid(2),lmsiz(2)];
rep = x_choices(Title,xch_l)-[1,1];

endfunction

