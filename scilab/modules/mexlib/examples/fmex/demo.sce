
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Allan CORNET 
// INRIA 2007

exec(SCI+'/modules/mexlib/examples/fmex/builder.sce');
exec(SCI+'/modules/mexlib/examples/fmex/loader.sce');

[a,b]=mexf1(1,2);

if (b~=2) then 
  disp('problem with this example');
else
  disp('ok');
end
