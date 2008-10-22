// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_waitforbuttonpress(tree)
// M2SCI function
// Conversion function for Matlab waitforbuttonpress()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

btn=gettempvar()
xc=gettempvar()
yc=gettempvar()
win=gettempvar()
str=gettempvar()

insert(Equal(list(btn,xc,yc,win,str),Funcall("xclick",1,list(),list())))

comp=Operation(">",list(btn,Cste(64)),list())

tree=Funcall("bool2s",1,list(comp),tree.lhs)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
