function txt=get_block_info(scs_m,k,ksave)
// Copyright INRIA
  if argn(2)>2 then super_path;super_path($+1)=ksave,end
txt=[]
o=scs_m.objs(k)
ksave=k //pour creer super_path
//select o(1)
select typeof(o)
case 'Block' then
  txt = standard_document(o,k)
  txt=[txt;' ']
  
  if o.model.sim=='super'|o.model.sim=='csuper' then
    objet=o.model.rpar
    infos = o.model.rpar.props
    if size(infos.title,'*')==2 then 
      txt = [txt;
	     'Super Block Documentation: '+infos.title(2)+'/'+..
	     infos.title(1)]
    else
      txt = [txt;'Super Block Documentation: '+infos.title(1)]
    end
    txt=[txt;get_info(infos.doc)],

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
      for k = 1 : size(objet.objs)
	o1=objet.objs(k)
	ok=%f
	if typeof(o1)=='Block' then
	  ok=filtre(1)|..
             filtre(5)&(o1.model.sim=='super'|o1.model.sim=='csuper')|..
	     filtre(4)&is_split(o1)
           
	else  
          ok=((typeof(o1)=='Link')&filtre(2))|((typeof(o1)=='Text')&filtre(4))
	end
	if ok then
	  txt=[txt;indent(get_block_info(objet,k,ksave))]
	end
      end
    end
  else
    execstr('texte_2 = '+o.gui+'(''show'', o)')
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
endfunction
