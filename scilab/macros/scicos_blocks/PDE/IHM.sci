// Copyright INRIA
// développé par EADS-CCR
// ce  script est pour initialiser quelque paramètres de l'IHM et contient //
// des fonctions utiles pour l'IHM
// ------------------------------------------------------------------------//
function do_pdeode()
disp('Tranlate');
endfunction

function do_quitter()
  fin=%f;
  ok=%t;
  [ok,fin]=resume(ok,fin);
endfunction

function do_valider()
  
  // verification du domaine
  if (get(edita,'String') == '' | get(editb,'String') == '') then
      x_message(['veuillez renseigner les valeurs';'du domaine de la discretisation']); 
      fin=%t;
      fin=resume(fin);
  end
    
  // Test de renseignements sur les differents champs de l'IHM
  if (get(check_op1,'Value') == 1) then
    if (get(edita1,'String') == '' | get(editb1,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'    de l''operateur d2u/dt2']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op2,'Value')== 1) then
    if (get(edita2,'String') == '' | get(editb2,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'    de l''operateur d2u/dx2']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op3,'Value')== 1) then
    if (get(edita3,'String') == '' | get(editb3,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'    de l''operateur du/dt']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op4,'Value')== 1) then
    if (get(edita4,'String') == '' | get(editb4,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'    de l''operateur d2u/dtdx']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op5,'Value')== 1) then
    if (get(edita5,'String') == '' | get(editb5,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'    de l''operateur du/dx']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op6,'Value')== 1) then
    if (get(edita6,'String') == '' | get(editb6,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'      de l''operateur u']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op7,'Value')== 1) then
    if (get(edita7,'String') == '' | get(editb7,'String') == '') then
      x_message(['veuillez renseigner les coefficients';'      de l''operateur f']); 
      fin=%t;
      fin=resume(fin);
    end
  end
 
  // verification du renseignement du signe du discriminant non constant
  if (get(check2,'Value')== 1) then 
    if (get(liste_signe,'Value') == 0 ) then
      x_message(['veuillez renseigner le signe';'du discriminant non constant']); 
      fin=%t;
      fin=resume(fin);
    end
  end  
    
  // verification des informations du choix manuel  
  if (get(rad_manuel,'Value')== 1) then     
    if (get(liste_meth,'Value') == 0 ) then
      x_message(['veuillez renseigner le type';'de la methode de discretisation']); 
      fin=%t;
      fin=resume(fin);
    end
    
    if (get(editdegre,'String') == '' ) then
      x_message(['veuillez renseigner le degre';'de la methode de discretisation']); 
      fin=%t;
      fin=resume(fin);
    end
    
    if (get(editpas,'String') == '' ) then
      x_message(['veuillez renseigner le nombre';'de points de discretisation']); 
      fin=%t;
      fin=resume(fin);
    end    
  end
  // verification du renseignement des conditions intiales
  if (get(check_op1,'Value')==1 | get(check_op3,'Value')== 1 | get(check_op4,'Value')== 1) then
    if (get(editCI,'String') == '' ) then
      x_message(['veuillez renseigner la condition';'         initiale u(x,t0)']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  if (get(check_op1,'Value')== 1) then
    if (get(editCI1,'String') == '' ) then
      x_message(['veuillez renseigner la condition';'         initiale du/dx|t0']); 
      fin=%t;
      fin=resume(fin);
    end
  end
  // verification du renseignement des conditions aux limites
  if (get(editCLa,'String') == '' | get(editCLb,'String') == '' ) then
    x_message(['veuillez renseigner les conditions';'         aux limites']);
    fin=%t;
    fin=resume(fin);
  end
  
  fin=%f; // Pour sortir sans problemes de l'IHM
  fin=resume(fin);
endfunction

function add_list_points()
  // Copyright INRIA
  // développé par EADS-CCR
  // cette fonction gere l'ajout //
  // d'un poit de mesure         //
  // ----------------------------//
  if edit_pt<>0&liste_pts<>0 then
    new = get(edit_pt,'String');
    newv=evstr(new);
    if (newv < evstr(get(edita,'String'))-%eps | newv > evstr(get(editb,'String'))+%eps) then
      x_message('le point choisi est en dehors du domaine')
      return;
    end
    if (list_points == '') then
      list_points=[];
    end
    list_points = [list_points  new];
    set(liste_pts,'String',strcat(list_points,'|'));
    list_points=resume(list_points)
  end
endfunction

function del_list_points()
  // Copyright INRIA
  // développé par EADS-CCR
  // cette fonction gère la supprission //
  // d'un poit de mesure         //
  // ----------------------------//
  if edit_pt<>0&liste_pts<>0 then
    del = get(liste_pts,'Value');
    if (list_points == '') then
      list_points=[];
    end
    list_points =[list_points(1:del-1) list_points(del+1:$)];
    set(liste_pts,'String',strcat(list_points,'|'));
    list_points=resume(list_points);
  end
endfunction
  
function visualiser()
  // Copyright INRIA
  // développé par EADS-CCR
  // Cette fonction est pour l'affichage de l'EDP dans l'IHM //
  // selon les opérateurs choisis                            //
  // --------------------------------------------------------//
  
  expression=[];
  if (get(check_op1,'Value')== 1) then 
    expression='('+get(edita1,'String')+')*b1(t)*d2u/dt2';
  end
  if (get(check_op2,'Value')== 1) then 
    if isempty(expression) then
      expression='('+get(edita2,'String')+')*b2(t)*d2u/dx2';      
    else
      expression=expression+'+('+get(edita2,'String')+')*b2(t)*d2u/dx2';
    end
  end
  if (get(check_op3,'Value')== 1) then 
    if isempty(expression) then
      expression='('+get(edita3,'String')+')*b3(t)*du/dt';
    else
      expression=expression+'+('+get(edita3,'String')+')*b3(t)*du/dt';      
    end
  end
  if (get(check_op4,'Value')== 1) then 
    if isempty(expression) then
      expression='('+get(edita4,'String')+')*b4(t)*d2u/dtdx';
    else
      expression=expression+'+('+get(edita4,'String')+')*b4(t)*d2u/dtdx';      
    end
  end
  if (get(check_op5,'Value')== 1) then 
    if isempty(expression) then
      expression='('+get(edita5,'String')+')*b5(t)*du/dx';
    else
      expression=expression+'+('+get(edita5,'String')+')*b5(t)*du/dx';      
    end
  end
  if (get(check_op6,'Value')== 1) then 
    if isempty(expression) then
      expression='('+get(edita6,'String')+')*b6(t)*u';
    else
      expression=expression+'+('+get(edita6,'String')+')*b6(t)*u';
    end
  end
  if (get(check_op7,'Value')== 1) then 
    if isempty(expression) then
      expression='Veuillez selectionner les operateurs';
    else
      expression=expression+'=('+get(edita7,'String')+')*b7(t)';      
    end
  else
    expression=expression+'=0';
  end
  if (expression == '=0') then
    expression='Veuillez selectionner les operateurs';
  end
  sep=['+','*','-',' '];
  expression=cformatlinedp(expression,80,sep);
  if (size(expression,'*')== 1) then
    set(txt_exp,'String',expression(1));
    set(txt_exp2,'String','');
  elseif (size(expression,'*')== 2) then
    set(txt_exp,'String',expression(1));
    set(txt_exp2,'String',expression(2));
  end
endfunction

// Ces fonctions sont les callback des //
// differents rad_buttom pour assurer  //
// l'exlusivité du choix               //
// ----------------------------------- //
function []=execlusif_check1()
  // Copyright INRIA
  // développé par EADS-CCR
  if ( check2) then
    if (get(check2,'Value') == 1) then
      set(check2,'Value','O');
    end
  end
  endfunction
function []=execlusif_check2()  
    // Copyright INRIA
    // développé par EADS-CCR
    if (get(check1,'Value') == 1) then
      set(check1,'Value','O');
    end
endfunction

function []=execlusif_radaut()
  // Copyright INRIA
  // développé par EADS-CCR
  if ( rad_manuel) then
    if (get(rad_manuel,'Value') == 1) then
      set(rad_manuel,'Value','O');
    end
  end
  endfunction
function []=execlusif_radman()
    // Copyright INRIA
    // développé par EADS-CCR
    if (get(rad_automatique,'Value') == 1) then
      set(rad_automatique,'Value','O');
    end
endfunction

function afficher()
  // Copyright INRIA
  // développé par EADS-CCR
  // Cette fonction est pour l'affichage de le pas de maillage //
  // --------------------------------------------------------//
  
  expression= msprintf('%.10g',(evstr(get(editb,'String'))-evstr(get(edita,..
                      'String')))/(evstr(get(editpas,'String'))-1));
  set(txt_pas,'String',expression);
  
endfunction
