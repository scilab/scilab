mode (-1)

//getf(envq_proc+'affich_result.sci')
//affich_date()

disp("")
disp("")
disp("------------------------------{ DEBUT TEST SUITE NON REGRESSION Version 3.0 DEBUT }");
disp("")
disp("")

lines(0);
// dans la variable num_test mettre tous les numéros de bugs corrigés dans la version 3.0
num_test = [654,656,790,793,55,71,461,464,467,470,474,475,476,477,480,487,488,498,502,650,681,683]

for k = num_test
   exec('bug' + string(k) + '.sci')
end

disp("")
disp("") 
disp("------------------------------{ FIN TEST SUITE NON REGRESSION Version 3.0 FIN }");
disp("")
disp("")
//affich_date()

