function texte = standard_document(objet, k)
//
// standard_document - documentation d'un bloc Scicos
//
// Copyright INRIA

  type_objet = typeof(objet)
  //
  select type_objet

    case 'Block' then
    //- Initialisations 
    modele = objet.model
    graphique = objet.graphics
    macro = objet.gui
    //
    fonction = modele.sim
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
      elseif fonction(2) <10000 then
	language = string(fonction(2))+'  (dynamic link or...)'
      else
	language = string(fonction(2))+'  (implicit bloc)'
      end
    else
      language = '0 (Scilab function type Scicos 2.2)'
    end
    //

    if modele.blocktype == 'l' then
      typ= 'synchro'
    elseif modele.blocktype == 'z'
      typ= 'zero-crossing'
    elseif modele.blocktype == 'm'
      typ = 'memory'
    else
      typ = 'regular'
    end
    //
    if modele.dep_ut(1) then
      dependance_u = 'yes'
    else
      dependance_u = 'no'

    end

    if modele.dep_ut(2) then
      dependance_t = 'yes'
    else
      dependance_t = 'no'
    end
    //
    identification = graphique.id

    //- Informations generales 

    if modele.sim=='super'|modele.sim=='csuper' then
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
	       'Permanently active         : '+dependance_t]
      if %cpr<>list()&needcompile<>4 then
	cor = %cpr.cor
	corinv = %cpr.corinv
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
    for i = 1 : min(size(modele.in,'*'),size(graphique.pin,'*'))
      tableau = [tableau; 'Regular input', string(i),..
		 string(modele.in(i)), string(graphique.pin(i))]
    end

    //- Sorties standard
    for i = 1 : min(size(modele.out,'*'),size(graphique.pout,'*'))
      tableau = [tableau; 'Regular output', string(i),..
		 string(modele.out(i)), string(graphique.pout(i))]
    end
    //- Entrees evenements 
    for i = 1 : min(size(modele.evtin,'*'),size(graphique.pein,'*'))
      tableau = [tableau; 'Event input', string(i),..
		 string(modele.evtin(i)), string(graphique.pein(i))]
    end
    //- Sorties evenements 
    for i = 1 : min(size(modele.evtout,'*'),size(graphique.peout,'*'))
      tableau = [tableau; 'Event output', string(i),..
		 string(modele.evtout(i)), string(graphique.peout(i))]
    end
    //
    texte = [texte; 'Input / output'; 
	     '--------------';
	     ' '
	     tabule(tableau); ' ']
    // Documentation
    documentation=objet.doc
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

    //= Liaisons 
    case 'Link' then
    //- Initialisation 
    identification = objet.id
    if objet.ct(2) == 1 then
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
    
    from=objet.from
    if %cpr<>list() then
      if sous_type == 'Regular Link' then 
	while %t
	  if scs_m.objs(from(1)).model.sim=='lsplit' then
	    #link=scs_m.objs(from(1)).graphics.pin
	    from=scs_m.objs(#link).from
	  else
	    break
	  end
	end

	cor = %cpr.cor
	path=list()
	for kp=1:size(super_path,'*'),path(kp)=super_path(kp);end
	path($+1)=from(1)
	ind=cor(path)
	if type(ind)==1 then
	  kl=%cpr.sim('outlnk')(%cpr.sim('outptr')(ind)+(from(2)-1))
	  beg=%cpr.sim('lnkptr')(kl)
	  fin=%cpr.sim('lnkptr')(kl+1)-1
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
	       'From', string(objet.from(1:2)); 
	       'to', string(objet.to(1:2))] 
    texte = [texte; 
	     'Connections'; 
	     '-----------';' ' 
	     tabule(tableau); ' ']
    //
  else
    texte=[]
  end
endfunction
