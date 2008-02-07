examples = [ 'ex1f.sce' ; 'ex2f.sce' ;
             'ex3f.sce' ; 'ex4f.sce' ;
						];
             
for i=1:size(examples,'*')
   disp('-----------------------------------');
   disp('Example : ' + examples(i));
   disp('-----------------------------------');
   exec(examples(i));
   
end             





