
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Warning : order !!
// list of  : filename , module

listmacrostoload = ['listfiles','fileio'
  '%c_a_c','overloading';
  'mputl','fileio';
  'genlib','functions';
  ];

dim = size(listmacrostoload);

for i = 1:dim(1)
 clear(listmacrostoload(i,1));
 exec(SCI+'/modules/'+listmacrostoload(i,2)+'/macros/'+listmacrostoload(i,1)+'.sci');
end
clear i;
clear dim;
// ------------------------------------