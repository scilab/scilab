mode (-1)

affich_date()
disp("")
disp("")
disp("------------------------------{ DEBUT TEST SUITE NON REGRESSION Version 2.7.1 DEBUT }");
disp("")
disp("")

// dans la variable num_test mettre tous les numéros de bugs corrigés dans la version .7.1
num_test = [1,6,21,22,24,25]

for k = num_test
   exec('bug' + string(k) + '.sci')
end

disp("")
disp("") 
disp("------------------------------{ FIN TEST SUITE NON REGRESSION Version 2.7.1 FIN }");
disp("")
disp("")
affich_date()

