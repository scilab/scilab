// test non reg de huynh olivier
mode (-1)

//getf(envq_proc+'affich_result.sci')
//affich_date()

//exec('E:/cvsCompile/affich_result.sci');chdir('E:/testNonReg/nonreg_3_Olivier/');exec('Loop_nonreg_3b.sce'); 
//exec('/home/huynh/poubelle/cvsCompile/affich_result.sci');chdir('/home/huynh/poubelle/testNonReg/nonreg_3_Olivier/') 
//
//
//quit

lines(0);

disp("");
disp("");
disp("------------------------------{ DEBUT TEST SUITE NON REGRESSION Version 3.0 DEBUT }");
disp("");
disp("");
// a ajouter quand corrrigé: 654,656,
// dans la variable num_test mettre tous les numéros de bugs corrigés dans la version 3.0
num_test = [ 577,584];
//num_test = [525,528,534,542,545,546,549,553,557,558,564,566,568,569,570,575, 584,604,608,610,612,623,632,625];
//,649
for k = num_test
   exec('bug' + string(k) + '.sci');
end

disp("");
disp("") ;
disp("------------------------------{ FIN TEST SUITE NON REGRESSION Version 3.0 FIN }");
disp("");
disp("");
//affich_date()

