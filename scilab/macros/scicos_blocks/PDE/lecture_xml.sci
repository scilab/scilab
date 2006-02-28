function txt_xml=lecture_xml(nom)
// Copyright INRIA
// développé par EADS-CCR
  txt_xml=[];
  vect=mgetl('SCI/macros/scicos_blocks/PDE/'+nom); //lecture du code xml
  for i=1:size(vect,1)
    txt_xml=txt_xml+vect(i)+'$';
  end
endfunction

function decision=interpret(txt_xml,delta,order)
// Copyright INRIA
// développé par EADS-CCR
  suite = [];
  
  //Indice du premier noeud ouvert
  ind=strindex(txt_xml,'<Node');
    //SI il reste un noeud ouvert dans txt_xml
    if ind ~=[] then
    
      //On récupère la définition du noeud
      balise_fin=strindex(txt_xml,'$');
      ind_balise_fin=find(balise_fin>ind(1));
      node_attr=part(txt_xml,ind(1):balise_fin(ind_balise_fin(1))-1);
      ind_str=strindex(node_attr,''"');
      ind_str_cond=strindex(node_attr,'condition = '"');
      ind_str_dec=strindex(node_attr,'decision = '"');
      if ind_str_cond ~= [] then, 
        ind_end=ind_str(find(ind_str>ind_str_cond+12));
        //on stocke la condition sans les guillemets
        condition = 'condi= ('+part(node_attr,ind_str_cond+13:ind_end(1)-1)+')';
      end
      if ind_str_dec ~= [] then, 
        ind_end=ind_str(find(ind_str>ind_str_dec+11));
        //on stock la decision
        decision = part(node_attr,ind_str_dec+11:ind_end(1));
      else
        decision = "";
      end
      
      //Execution de la condition pour stocker le test dans la variable condi
      execstr(condition);
      //Si la condition est vrai
      if (condi) then
        //Si le noeud a des fils on continue avec le bloc fils du noeud courant sinon on s'arrete
        if (strindex(node_attr,'/>') ==[]) then
          suite= extraire_bloc_fils(part(txt_xml,balise_fin(ind_balise_fin(1))+1:length(txt_xml)));
        end
      else
        //Si la condition est fausse on continue avec le reste du fichier
        //sans les fils du noeud courant
        
        //si le noeud a des fils
         if (strindex(node_attr,'/>') ==[]) then
          suite=extraire_reste(part(txt_xml,balise_fin(ind_balise_fin(1))+1:length(txt_xml)));
         else
           //sinon on renvoie tout le reste
           suite=part(txt_xml,balise_fin(ind_balise_fin(1)):length(txt_xml));
         end
      end
      
      //S'il reste des noeuds on continue
      if (suite ~= [] ) then
        if strindex(suite,'<Node')~=[] then
          //disp('On s''occupe maintenant de '+suite);
          decision=interpret(suite,delta,order);
        else
          decision="";
          disp('attention pas de decision');
        end
      end
      
    end
    
endfunction

function  suite= extraire_bloc_fils(txt_xml)
// Copyright INRIA
// développé par EADS-CCR
  profondeur = 0;
  ind_dollar=strindex(txt_xml,'$');
  debut_ligne=1;
  ind_fin=-1;
  i=1;
  
  //Tant qu'il reste des lignes à lire
  //et qu'on a pas trouve la fin des fils du noeud courant
  while (i<size(ind_dollar,'*') & ind_fin ==-1) then
     ligne=part(txt_xml,debut_ligne:ind_dollar(i));
    //Si le noeud lu a des fils
    if (strindex(ligne,'/>')==[]) then
      //Si c'est la fin d'un noeud pere
      if ~(strindex(ligne,'</Node>$')==[]) then
        //Si la profondeur est a 0 on a trouve la fin du noeud initial
        if profondeur == 0 then
          ind_fin=debut_ligne-1;
        //Sinon on ferme un noeud pere interne
        //on décrémente la profondeur
        else
          profondeur = profondeur -1;
        end
      else
      //Sinon il ne reste qu'une possibilité c'est que la ligne 
      //se termine par >$, c'est à dire qu'on a trouve un noeud pere interne
      //donc on incrémente la profondeur
        profondeur = profondeur +1;
      end
    
    end
    debut_ligne= ind_dollar(i)+1;
    i=i+1;
  end
  
  if ind_fin ~= -1 then
    suite=part(txt_xml,1:ind_fin);
  else
    suite=[];
  end
endfunction

function  suite= extraire_reste(txt_xml)
// Copyright INRIA
// développé par EADS-CCR
  profondeur = 0;
  ind_dollar=strindex(txt_xml,'$');
  debut_ligne=1;
  ind_fin=-1;
  i=1;
  
  //Tant qu'il reste des lignes à lire
  //et qu'on a pas trouve la fin des fils du noeud courant
  while (i<size(ind_dollar,'*') & ind_fin ==-1) then
    ligne=part(txt_xml,debut_ligne:ind_dollar(i));
    //Si le noeud lu a des fils
    if (strindex(ligne,'/>')==[]) then
      //Si c'est la fin d'un noeud pere
      if ~(strindex(ligne,'</Node>$')==[]) then
        //Si la profondeur est a 0 on a trouve la fin du noeud initial
        if profondeur == 0 then
          ind_fin=ind_dollar(i)+1;
        //Sinon on ferme un noeud pere interne
        //on décrémente la profondeur
        else
          profondeur = profondeur -1;
        end
      else
      //Sinon il ne reste qu'une possibilité c'est que la ligne 
      //se termine par >$, c'est à dire qu'on a trouve un noeud pere interne
      //donc on incrémente la profondeur
        profondeur = profondeur +1;
      end
    else
    //Le noeud n'a pas de fils on renvoie tout ce qui suit
    
      ind_fin=ind_dollar(i)+1;
    end
    debut_ligne= ind_dollar(i)+1;
    i=i+1;
  end
  if ind_fin ~= -1 then
    suite=part(txt_xml,ind_fin:length(txt_xml));
  else
    suite=[];
  end
endfunction



