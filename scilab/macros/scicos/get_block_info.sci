function txt=get_block_info(scs_m,k)
// Copyright INRIA
txt=[]
o=scs_m(k)
select o(1)
case 'Block' then
  txt = standard_document(o,k)
  txt=[txt;' ']
  
  if o(3)(1)=='super'|o(3)(1)=='csuper' then
    objet=o(3)(8)
    infos = objet(1)
    if size(infos(2),'*')==2 then 
      txt = [txt;'Super Block Documentation: '+infos(2)(2)+'/'+infos(2)(1)]
    else
      txt = [txt;'Super Block Documentation: '+infos(2)(1)]
    end
    if size(infos)>=10 then 
      txt=[txt;get_info(infos(10))],
    end
    // information on components
    boutons = ['Yes', 'No']
    ligne_1 = list('Blocks', 2, boutons)
    ligne_2 = list('Links', 2, boutons)
    ligne_3 = list('Nodes', 2, boutons)
    ligne_4 = list('Others', 2, boutons)
    ligne_5 = list('Super Blocks', 2, boutons)
    //
    titre = ['This is a super block, Select additional '
	'informations you want to get on it''s'
	'components']
    reponse = x_choices(titre, list(ligne_1, ligne_2, ligne_3, ligne_4, ligne_5))
    if reponse <>[] then
      filtre = (reponse == 1)
      if or(filtre) then
	txt=[txt;
	    'Informations on selected components'
	    '----------------------------------';' ']
      end
      for k = 2 : size(objet)
	o1=objet(k)
	ok=%f
	if o1(1)=='Block' then
	  ok=filtre(1)|..
	     filtre(5)&(o1(3)(1)=='super'|o1(3)(1)=='csuper')|..
	     filtre(4)&is_split(o1)
	else  
	  ok=((o1(1)=='Link')&filtre(2))|((o1(1)=='Text')&filtre(4))
	end
	if ok then
	  txt=[txt;indent(get_block_info(objet,k))]
	end
      end
    end
  else
    execstr('texte_2 = '+o(5)+'(''show'', o)')
    if texte_2==[] then
      texte_2='No  documentation available for the parameters of this block'
    end
    txt=[txt;
	' '
	'Block Parameters';
	'----------------'
	' '
	texte_2]
  end
case 'Link' then
  txt = standard_document(o,k)
case 'Text' then
  txt = standard_document(o,k)
case 'Deleted' then
  txt=[]
end
