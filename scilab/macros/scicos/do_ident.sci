function scs_m = do_ident(scs_m)
// Copyright INRIA
while %t
  [btn,xc,yc,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  k=getobj(scs_m,[xc;yc])
  if k<>[] then break,end
end
numero_objet=k
scs_m_save=scs_m

TYPE_OBJET=1;MODELE=3;
IDENTIFICATION=15
IDENTIFICATION_LIAISON=5

objet = scs_m(numero_objet)
type_objet = objet(TYPE_OBJET)

//
if type_objet == 'Block' then

  if size(objet(MODELE)) < IDENTIFICATION then  //compatibility
    for ks=size(objet(MODELE))+1:IDENTIFICATION
      objet(MODELE)(ks)=[]
    end
  end
  
  identification = objet(MODELE)(IDENTIFICATION)
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
    objet(MODELE)(IDENTIFICATION) = stripblanks(identification)
    drawobj(objet)
    scs_m(numero_objet) = objet
  end
elseif type_objet == 'Link' then
  identification = objet(IDENTIFICATION_LIAISON)
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
      drawobj(scs_m(numero))
      scs_m(numero)(IDENTIFICATION_LIAISON) = identification
      drawobj(scs_m(numero))
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
