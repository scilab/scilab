//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

a=amell(10,0.1);
if a > 3.693 then pause,end
if a < 3.692 then pause,end
if or(size(amell([1:10], 0.1)) <> [1 10]) then pause,end
if or(amell([1:10], 1)<> 0) then pause,end
