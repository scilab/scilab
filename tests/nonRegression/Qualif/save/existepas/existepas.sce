mode (-1)

disp("")
disp("")
disp("------------------------------{ DEBUT TEST SUITE AUTREREP DEBUT }");
disp("")
disp("")

// dans la variable num_test mettre tous les numéros de bugs corrigés dans la version .7.1
//


// existepas

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

errcatch(-1 , 'stop') 
exec("oubaouba_marsupilami.sce");

quit;

disp("")
disp("") 
disp("------------------------------{ FIN TEST SUITE AUTREREP FIN }");
disp("")
disp("")

