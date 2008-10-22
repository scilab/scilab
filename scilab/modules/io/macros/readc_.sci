// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [c]=readc_(unit)
//Syntaxes : c=readc_(unit)
//           c=readc_()
//
//readc_ reads a character string 
//This macro allows one to interrupt an exec file without pause;
//the exec file stops until carrige return is made.
//!
//

[lhs,rhs]=argn(0);
if rhs<=0 then unit=%io(1); end;
c=read(unit,1,1,'(a)');
endfunction
