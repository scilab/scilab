// bug25

clear all
mode (-1)
//getf ('../fonct_qualif.sci')

correct = %F
I=0; 
while 1 
  printf('Dans le while principal I='+string(I)+'\n')
  while 1 
     break;
  end 
  // les lignes suivantes ne sont jamais executes !!!!!!
  correct = %T
  I=I+1; 	
  if I >= 5 then break; end 
end

affich_result(correct, 25)
clear all

//La raison : c'est un cas non prevu le break est appele par while sans un if dans ce cas le while ne sert a rien.
//C'est donc un bug mais d'importance mineure, je le corrigerais apres l'eventuelle sortie de la 2.7.1
