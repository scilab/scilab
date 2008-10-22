function type_meth=arbre_decision(delta) 
// Copyright INRIA
// développé par EADS-CCR
// Cette fonction renvoie le type de la mèthode //
// apres le percour du fichier xml en testant   //
// le signe du discriminant                     //
// entrée :                                     //
//   - delat (Double) : discriminant de l'EDP   //
// sortie :                                     //
//   - type_meth (Entier) correspond au code de //
//     la méthode de discrétisation.            //
//----------------------------------------------//
  nom='meth.xml';
  // lecture du fichier xml
  txt_xml=lecture_xml(nom);
  decision=interpret(txt_xml,delta,1)
  // interpretation de la decision
  execstr('deci= ('+decision+')');
  if (deci == "type_meth = 1") then
    type_meth = 1;
  elseif (deci == "type_meth = 3") then
    type_meth = 3;
  else
    type_meth = 2;
  end
endfunction

