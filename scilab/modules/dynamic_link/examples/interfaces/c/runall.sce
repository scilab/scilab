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



