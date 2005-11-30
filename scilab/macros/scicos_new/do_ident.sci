function scs_m = do_ident(scs_m)
// Copyright INRIA
  if Select==[] then
    xc=%pt(1);yc=%pt(2);
    k=getobj(scs_m,[xc;yc])
    if k==[] then return,end
  else
    k=Select(:,1)';%pt=[]
  end  
  if size(k,'*')>1|%win<>Select(1,2) then
    message("Only one block can be selected in current window for this o"+...
	    "peration.")
    Cmenu=[];%pt=[];return
  end
  

  numero_objet=k
  scs_m_save=scs_m

  objet = scs_m.objs(numero_objet)
  type_objet = typeof(objet)

  //
  if type_objet == 'Block' then

    identification = objet.graphics.id
    if identification == [] then
      identification = emptystr()
    end
    //
    texte_1 = 'Set Block identification'
    texte_2 = 'ID'
    [ok, identification] = getvalue(texte_1, texte_2, list('str', 1), ..
				    identification)
    if ok then
      drawobj(objet)
      objet.graphics.id = stripblanks(identification)
      drawobj(objet)
      scs_m.objs(numero_objet) = objet
    end
  elseif type_objet == 'Link' then
    identification = objet.id
    if identification == [] then
      identification = emptystr()
    end
    texte_1 = 'Set link Identification'
    texte_2 = 'ID'
    [ok, identification] = getvalue(texte_1, texte_2, ..
				    list('str', 1),identification)
    //
    if ok then
      identification = stripblanks(identification)
      c_links = connected_links(scs_m,numero_objet)
      //- set identification to all connected links
      //
      for numero = c_links
	drawobj(scs_m.objs(numero))
	scs_m.objs(numero).id = identification
	drawobj(scs_m.objs(numero))
      end
    end				
  else
    x_message('It is impossible to set ID for this type of object')
  end
  //
  if pixmap then
    xset('wshow')
  end
  if ok then [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t),end
endfunction
