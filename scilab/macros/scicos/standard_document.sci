function texte = standard_document(objet, k)
//
// standard_document - documentation d'un bloc Scicos
//
// Copyright INRIA
MODELE=3
GRAPHIQUE=2
MACRO=5
FONCTION=1
DEPENDANCE=12
TYPE=10
IDENTIFICATION=15
#ENTREES=2
$ENTREES=5
#SORTIES=3
$SORTIES=6
#ENTREES_EVT=4
#SORTIES_EVT=5
$ENTREES_EVT=7
$SORTIES_EVT=8
//liens
IDENTIFICATION_LIAISON=5
TYPE_COULEUR=7
OBJET_ORIGINE=8
OBJET_DESTINATION=9
//
type_objet = objet(1)
//
select type_objet

case 'Block' then
  //- Initialisations 

  modele = objet(MODELE)
      graphique = objet(GRAPHIQUE)
    macro = objet(MACRO)
  //
  fonction = modele(FONCTION)
  if prod(size(fonction)) > 1 then
    if fonction(2) == 0 then
      language = '0 (Scilab function type Scicos 2.2)'
    elseif fonction(2) == 1  then
      language = '1 (Fortran or C code)'
    elseif fonction(2) == 2 then
      language = '2 (C code)'
    elseif fonction(2) == 3 then
      language = '3 (Scilab function)'
    elseif fonction(2) < 0  then
      language = '<0 (synchro block)'
    else
      language = '>3  (dynamic link or...)'
    end
  else
    language = '0 (Scilab function type Scicos 2.2)'
  end
  //
  if modele(TYPE) == 'l' then
    typ= 'synchro'
  elseif modele(TYPE) == 'z'
    typ= 'zero-crossing'
  elseif modele(TYPE) == 'm'
    typ = 'memory'
  else
    typ = 'regular'
  end
  //
  if modele(DEPENDANCE)(1) then
dependance_u = 'yes'
else
dependance_u = 'no'

end
if modele(DEPENDANCE)(2) then
dependance_t = 'yes'
else
dependance_t = 'no'
end
//
if size(modele) >= IDENTIFICATION then
  identification = modele(IDENTIFICATION)
else
  identification = emptystr()
end

//- Informations generales 
if modele(1)=='super'|modele(1)=='csuper' then
  texte = ['General Information'; 
	'--------------------';' '
	'object type                : Super Block';   ..
	'Identification             : '+identification; ..
	'Object number in diagram   : '+string(k); ' ';  ..
	'Drawing function           : '+macro;' ']
else
  texte = ['General Information'; 
         '--------------------';' '
	 'object type                : bloc standard';   ..
	 'Identification             : '+identification; ..
	 'Object number in diagram   : '+string(k); ' ';  ..
	 'Drawing function           : '+macro;           ..
	 'Simulation function        : '+fonction(1);     ..
	 'Simulation Function type   : '+language;' ';    ..
	 'Bloc type                  : '+typ;             ..
         'Direct feed through        : '+dependance_u;    ..
         'Time varying               : '+dependance_t]
  if %cpr<>list()&needcompile<>4 then
    cor = %cpr(3)
    corinv = %cpr(4)
    path=list()
    for kp=1:size(super_path,'*'),path(kp)=super_path(kp);end
    path($+1)=k
    ind=cor(path)
    if ind>0&ind<=size(corinv) then
      txt = ['Compiled structure Index   : '+string(cor(path)); ' ']
    else
      txt = ['Compiled structure Index   : suppressed'; ' ']
    end
  else
    txt = ['Compiled structure Index   : Not available';' ']
  end
  texte=[texte;txt]
end
//- Entrees / sorties 

tableau = ['Port type', 'Number', 'Size', 'Link'; '-', '-', '-', '-']
//- Entrees standard

for i = 1 : min(size(modele(#ENTREES),'*'),size(graphique($ENTREES),'*'))
  tableau = [tableau; 'Regular input', string(i),	..
	  string(modele(#ENTREES)(i)), string(graphique($ENTREES)(i))]
end

//- Sorties standard
for i = 1 : min(size(modele(#SORTIES),'*'),size(graphique($SORTIES),'*'))
  tableau = [tableau; 'Regular output', string(i),	..
	  string(modele(#SORTIES)(i)), string(graphique($SORTIES)(i))]
end
//- Entrees evenements 
for i = 1 : min(size(modele(#ENTREES_EVT),'*'),size(graphique($ENTREES_EVT),'*'))
  tableau = [tableau; 'Event input', string(i),	..
	  string(modele(#ENTREES_EVT)(i)), string(graphique($ENTREES_EVT)(i))]
end
//- Sorties evenements 
for i = 1 : min(size(modele(#SORTIES_EVT),'*'),size(graphique($ENTREES_EVT),'*'))
  tableau = [tableau; 'Event output', string(i),	..
	  string(modele(#SORTIES_EVT)(i)), string(graphique($SORTIES_EVT)(i))]
end
//
texte = [texte; 'Input / output'; 
                '--------------';
		' '
		 tabule(tableau); ' ']
// Documentation
if size(modele)>=14 then
  documentation=modele(14)
  if type(documentation)==15 then
    if size(documentation)>=2 then
      funname=documentation(1);doc=documentation(2)
      if type(funname)==10 then 
	ierr=execstr('docfun='+funname,'errcatch')
	if ierr<>0 then
	  x_message('function '+funname+' not found')
	  return
	end
      else
	docfun=funname
      end
      ierr=execstr('doc=docfun(''get'',doc)','errcatch')
      if ierr==0&doc<>[] then
	texte = [texte; 'Documentation'; 
	                '-------------';
			' '
			doc; ' ']
      end
    end
  end
end

//= Liaisons 
case 'Link' then
  //- Initialisation 
  identification = objet(IDENTIFICATION_LIAISON)
  if objet(TYPE_COULEUR)(2) == 1 then
    sous_type = 'Regular Link'
  else
    sous_type = 'Event link'
  end
  //- Informations generales 
  texte = ['General informations'; 
           '--------------------';' '
	  'Object type                : '+sous_type;
	  'Object Identification      : '+identification'; 
	  'Object number in diagram   : '+string(k); ' ']
  
  from=objet(OBJET_ORIGINE)
  if %cpr<>list() then
    if sous_type == 'Regular Link' then 
      while %t
	if scs_m(from(1))(3)(1)=='lsplit' then
	  #link=scs_m(from(1))(2)(5)
	  from=scs_m(#link)(OBJET_ORIGINE)
	else
	  break
	end
      end

      cor = %cpr(3)
      path=list()
      for kp=1:size(super_path,'*'),path(kp)=super_path(kp);end
      path($+1)=from(1)
      ind=cor(path)
      if type(ind)==1 then
	kl=%cpr(2)('outlnk')(%cpr(2)('outptr')(ind)+(from(2)-1))
	beg=%cpr(2)('lnkptr')(kl)
	fin=%cpr(2)('lnkptr')(kl+1)-1
	txt = ['Compiled link memory zone  : ['+..
		    string(beg)+','+string(fin)+']'; ' ']
      else
	txt = ['Compiled link memory zone  : Not available';' ']
      end
    end
  else
    txt = ['Compiled link memory zone  : Not available';' ']
  end
  texte=[texte;txt]   
      
  //- Connexions 

  tableau = [' ', 'Block', 'Port' ; '-', '-', '-'; 
	  'From', string(objet(OBJET_ORIGINE)); 
	  'to', string(objet(OBJET_DESTINATION))]
  //
  texte = [texte; 
          'Connections'; 
          '-----------';' ' 
	  tabule(tableau); ' ']
  //
else
  texte=[]
end
