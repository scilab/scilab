    // before_nonreg_3 tests_suite
    // check old bugs

    mode (-1)
    //getf ('../fonct_qualif.sci')//////////////////////

    //***************************************************
    // bugA
    affich_result((and((1&[1,0,1;1,1,0])==([%T,%F,%T;%T,%T,%F]))), "A");

    //***************************************************
    // bugAA
    affich_result(and((1|[1,0,1;1,1,0])==[%T,%T,%T;%T,%T,%T]), "AA");

    //***************************************************
    // bugB
    files = ['']
    files=listfiles([SCI+'/macros/scicos/*.cosf'])
    affich_result(files <> [''], "B");
    clear files;

    //***************************************************
    // bugC
    affich_result(isnan(1:%nan), "C");

    //***************************************************
    // bugD
     affich_result((and(cumprod([1+1*%i,2+2*%i,3+3*%i])==cumprod([1+1*%i,2+2*%i,3+3*%i],2))), "D")

//La fonction cumprod() donne des résultats érronés pour les complexes.
//
// Exemple :
// -->res1 = cumprod([1+1*%i,2+2*%i,3+3*%i])
//  res1  =
//  
// !   1. + i      2. + 2.i    6. + 6.i !
//  
// Alors que l'on devrait avoir :
// 
//  res1  =
//  
// !   1. + i      4.i  - 12. + 12.i !
// 


    //***************************************************
    // bugE
    [q,r]=qr((1:3)')
     affich_result(((r(2)==0)&(r(3)==0)), "E");
    clear r
    clear q
// La matrice triangulaire supérieure renvoyée par cette fonction contient 
// des valeurs non nulles en dessous de la diagonale :
// 
// -->[Q,R]=qr([1,2,3]')
//  R  =
// ! - 3.7416574 !
// !   4.881-313 ! // !!!!!
// !   6.154-313 ! // !!!!!
// 
// 
//  Q  =
// ! - 0.2672612  - 0.5345225  - 0.8017837 !
// ! - 0.5345225    0.7745419  - 0.3381871 !
// ! - 0.8017837  - 0.3381871    0.4927193 !
// 


    //***************************************************
    // bugF
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

  affich_result(correct, "F")
  clear correct
  //La raison : c'est un cas non prevu le break est appele par while sans un if dans ce cas le while ne sert a rien.

    //***************************************************
    // bugG
  result=execstr("contour2d(1:10,1:10,rand(10,10),[0.1 0.5 0.9])","errcatch","n")
    if result==0 then
       affich_result(%T, "G");
    else
       affich_result(%F, "G");
       disp ("Error n° "+string(result)"+" sur bugG")
       disp (lasterror());
    end
    xdel()
    clear result;
  //
  // Une option de contour2d a ete oubliee dans la version 2.7

    //***************************************************
    // bugH 
    // it is not a bug, it is not yet implemented
    // restart command does not exist with the keyboard
    //result =execstr("restart","errcatch","n")
    //if result==0 then
    //   affich_result(%T, "H");
    //else
    //   affich_result(%F, "H");
    //   disp ("Error n° "+string(result)"+" sur bugH")
    //   disp (lasterror());
    //end
    //clear result;   

    //***************************************************
    // bugI
    result =execstr("comp(beurk)","errcatch","n")
    if result==4 then
       affich_result(%T, "I");
    else
       affich_result(%F, "I");
       disp ("Error n° "+string(result)"+" sur bugI")
       disp (lasterror());
    end
    clear result;   
    
   //***************************************************
   // bugJ
   // crash Scilab

   //   function toto()
   //   N
   //   endfunction

   //  -->N
   //      !--error     4
   //      undefined variable : N

   //   result=execstr('toto()','errcatch')

   //   Error Message:

   //                              !--error    22
   //  recursion problems. Sorry....


   //  warning: recursion problem..., cleared

   //***************************************************
   // bugK

   mode (-1)
   clear all

   x='a'
   z=x($:-1:1)
   affich_result(z=='a', "K")

   clear all

   //***************************************************
