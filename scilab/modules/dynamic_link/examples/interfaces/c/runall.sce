// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

examples = [ 'ex1c.sce' ;
             'ex2c.sce' ;
             'ex3c.sce' ;
             'ex4c.sce' ;
             'ex5c.sce' ; 
             'ex6c.sce' ; 
             'ex7c.sce' ;
             'ex8c.sce' ;
             'ex9c.sce' ; 
             'ex10c.sce' ;
             'ex11c.sce' ;
             'ex12c.sce' ;
             'ex13c.sce' ;
             'ex14c.sce' ;
             'ex15c.sce' ];
             
for i=1:size(examples,'*')
   disp('-----------------------------------');
   disp('Example : ' + examples(i));
   disp('-----------------------------------');
   exec(examples(i));
   
end             



