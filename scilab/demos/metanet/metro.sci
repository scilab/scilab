function []=metro()
//Scilab program for the travel by the tube in Paris
//needs the file 'node2station' giving the fit between nodes and the names
//of the stations
//needs the execution of the program 'metrostart' for the data of the graph
// Copyright INRIA
  inin=file('open',SCI+'/demos/metanet/node2station','old');
  mamat=read(inin,388,1,'(a40)');
  file('close',inin);
  La=['Abbesses','Alesia','Alexandre Dumas','Alfort-Ecole Veterinaire',..
      'Alma-Marceau','Anatole-France','Anvers','Argentine','Arts et Metiers',..
      'Auber','Aubervilliers-Pantin','Avron'];
  Lb=['Balard','Barbes-Rochechouart','Bastille','Bel air','Belleville','Berault',..
      'Bercy','Billancourt','Bir-Hakeim','Blanche','Boissiere','Bolivar','Bonne-Nouvelle',..
      'Botzaris','Boucicaut','Boulets - Montreuil','Boulevard Massena','Boulevard Victor',..
      'Boulogne - Jean Jaures','Boulogne - Pont de saint cloud','Bourse','Breguet-Sabin',..
      'Brochant','Buttes-Chaumont','Buzenval'];
  Lc=['Cadet','Cambronne','Campo-Formio','Cardinal Lemoine','Carrefour Pleyel',..
      'Censier-Daubenton','Chambre des deputes','Champ-de-Mars','champs-Elysees Clemenceau',..
      'Chardon Lagache','Charenton-Ecoles','Ch. de Gaulle-Etoile','Charles Michels','Charonne',..
      'Chateau d eau','Chateau de Vincennes','Chateau Landon','Chateau-Rouge','Chatelet',..
      'Chatelet-les-Halles','Chatillon-Montrouge','Chaussee d Antin','Chemin Vert','Chevaleret',..
      'Cite','Cite Universitaire','Colonel Fabien','Commerce','Concorde'];
  Lcc=['Convention','Corentin-Cariou','Corentin-Celton','Corvisart','Courcelles','Couronnes',..
       'Creteil-l Echat','Creteil Prefecture','Creteil Universite','Crimee','Croix de Chavaux'];
  Ld=['Danube','Daumesnil','Denfert-Rochereau','Dugommier','Dupleix','Duroc'];
  Le=['Ecole Militaire','Edgar Quinet','Eglise d Auteuil','Eglise de Pantin','Emile Zola',..
      'Etienne Marcel','Europe','Exelmans'];
  Lf=['Faidherbe-Chaligny','Falguiere','Felix Faure','Filles du Calvaire','Fort d Aubervilliers',..
      'Franklin D Roosevelt'];
  Lg=['Gabriel Peri','Gaite','Gallieni','Gambetta','Gare d Austerlitz','Gare de l Est',..
      'Gare de lyon','Gare du Nord','Garibaldi','Gentilly','George V','Glaciere','Goncourt','Guy Moquet'];
  Lh=['Havre Caumartin','Hoche','Hotel de Ville'];
  Li=['Iena','Invalides','Issy Plaine','Issy Ville','Ivry'];
  Lj=['Jacques Bonsergent','Jasmin','Jaures','Javel','Jourdain','Jules Joffrin','Jussieu'];
  Lk=['Kleber'];
  Ll=['La Chapelle','La Defense','La Fourche','Lamarck-Caulaincourt','La Motte Piquet',..
      'La Muette','Laplace','La Plaine Voyageurs','Latour Maubourg','Laumiere','Ledru Rollin',..
      'Le Kremlin Bicetre','Le Peletier','Les Gobelins','Les Halles','Les Sablons','Liberte',..
      'Liege','Louis Blanc','Louise Michel','Lourmel','Louvre','Luxembourg'];
  Lm=['Mabillon','Madeleine','Mairie de clichy','Mairie de Montreuil','Mairie de St Ouen',..
      'Mairie des lilas','Mairie d Issy','Mairie d Ivry','Maison Blanche',..
      'Maison Alfort Les Julliottes','Maison Alfort Stade','Malakoff Plateau de Vanves',..
      'Malakoff Rue Etienne Dolet','Malesherbes','Maraichers','Marcadet Poissonniers',..
      'Marcel Sembat','Marx Dormoy','Maubert Mutualite','Menilmontant','Michel Ange Auteuil',..
      'Michel Ange Molitor','Michel Bizot','Mirabeau','Miromesnil','Monceau','Monge','Montgallet'];
  Lmm=['Montparnasse Bienvenue','Mouton Duvernet'];
  Ln=['Nation','Nationale','Notre Dame de Lorette','Notre dame des Champs'];
  Lo=['Oberkampf','Odeon','Opera','Ourcq'];
  Lp=['Palais Royal','Parmentier','Passy','Pasteur','Pelleport','Pereire','Pere Lachaise','Pernety',..
      'Philippe Auguste','Picpus','Pierre Curie','Pigalle','Place de Clichy','Place des Fetes',..
      'Place d Italie','Plaisance','Poissoniere','Pont de l Alma','Pont de Levallois Becon',..
      'Pont de Neuilly','Pont de Sevres','Pont Marie','Pont Neuf','Porte Dauphine','Porte d Auteuil',..
      'Porte de Bagnolet','Porte de Champeret','Porte de Charenton','Porte de Choisy','Porte de Clichy',..
      'Porte de Clignancourt'];
  Lpp=['Porte de la Chapelle','Porte de la Vilette','Porte de Montreuil','Porte de Pantin',..
       'Porte de St Cloud','Porte de St Ouen','Porte des Lilas','Porte de Vanves','Porte de Versailles',..
       'Porte de Vincennes','Porte d Italie','Porte d Ivry','Porte Doree','Porte d Orleans','Porte Maillot',..
       'Port Royal','Pre St Gervais','Pyramides','Pyrenees'];
  Lq=['Quai de la Gare','Quai de la Rapee','Quai d Orsay','Quatre Septembre'];
  Lr=['Rambuteau','Ranelagh','Raspail','Reaumur Sebastopol','Rennes','Republique','Reuilly Diderot',..
      'Richard Lenoir','Richelieu Drouot','Riquet','Robespierre','Rome','Rue de la Pompe',..
      'Rue du Bac','Rue Montmartre'];
  Ls=['Saint Ambroise','Saint Augustin','Saint Denis Basilique','Saint Denis Porte de Paris',..
      'Saint Fargeau','Saint Francois Xavier','Saint Georges','Saint Germain des Pres','Saint Jacques',..
      'Saint Lazare','Saint Mande Tourelle','Saint Marcel','Saint Maur','Saint Michel','Saint Paul',..
      'Saint Philippe du Roule','Saint Placide','Saint Sebastien Froissard','Saint Sulpice','Segur',..
      'Sentier','Sevres Babylone','Sevres Lecourbe','Simplon','Solferino','Stalingrad',..
      'Strasbourg St Denis','Sully Morland'];
  Lt=['Telegraphe','Temple','Ternes','Tolbiac','Trinite','Trocadero','Tuileries'];
  Lv=['Vaneau','Varenne','Vaugirard','Vavin','Victor Hugo','Villiers','Vincennes','Volontaires','Voltaire'];
  Lw=['Wagram'];
  Lz=['INRIA-Rocquencourt'];
  Lstation=[Lz,La,Lb,Lc,Lcc,Ld,Le,Lf,Lg,Lh,Li,Lj,Lk,Ll,Lm,Lmm,Ln,Lo,Lp,Lpp,Lq,Lr,Ls,Lt,Lv,Lw];

  LA=[175,70,229,371,167,188,141,117,304,-1,368,227];
  LB=[52,140,212,206,251,349,198,24,14,143,112,269,154,264,50,226,-1,-1,2,1,158,237,146,268,228];
  LC=[138,26,102,83,363,80,184,-1,165,8,205,114,10,225,245,350,246,177,88,-1,111,302,220,196,87,-1,250,48,163,59];
  LCC=[276,57,74,122,253,374,376,375,275,356];
  LD=[263,296,68,199,13,44];
  LE=[54,66,6,370,11,379,131,21];
  LF=[210,62,49,239,369,300];
  LG=[367,105,325,324,287,244,213,280,361,-1,180,73,252,185];
  LH=[133,273,331];
  LI=[28,169,-1,-1,-1];
  LJ=[243,19,249,9,267,179,82];
  LK=[113];
  LL=[281,-1,145,176,12,18,-1,-1,101,270,211,194,137,79,236,119,204,148,336,130,51,181,-1];
  LM=[92,314,366,357,362,353,56,198,77,373,372,109,110,125,232,178,23,278,84,254,5,3,201,7,301,123,81,377];
  LMM=[47,69];
  LN=[299,195,173,96];
  LO=[310,85,135,271];
  LP=[161,256,15,45,259,127,323,106,230,207,192,142,144,341,75,107,139,168,189,120,25,89,91,116,4,258,129,203,190,187,186];
  LPP=[279,277,233,272,22,360,352,108,58,234,78,191,202,71',118,-1,262,160,266];
  LQ=[197,214,-1,159];
  LR=[235,20,67,156,97,308,347,222,149,274,355,147,17,99,152];
  LS=[223,171,365,364,260,63,174,93,72,132,348,216,255,86,219,378,95,221,94,41,157,98,42,380,100,340,155,217];
  LT=[282,381,121,76,172,16,162];
  LV=[104,64,60,65,115,124,-1,61,224];
  LW=[126];
  LZ=[-2];

  Lnode=[LZ,LA,LB,LC,LCC,LD,LE,LF,LG,LH,LI,LJ,LK,LL,LM,LMM,LN,LO,LP,LPP,LQ,LR,LS,LT,LV,LW];

  L1=[1;2];
  L2=[21;22;23;24;25];
  iter=1;win2=[]
  while iter==1,
    changename=[];TT=[];
    Ldepart=0;
    Depart=108;Arrivee=108;Gr_typ='null';
    while (Lnode(Depart)==-1|Lnode(Arrivee)==-1),
      Lnode(1)=-2;
      Depart=x_choose(Lstation,'Choose the station of departure');
      Arrivee=x_choose(Lstation,'Choose the station of arrival');
      if(Depart==0|Arrivee==0) then return  end;
	if (Lnode(Depart)==-1|Lnode(Arrivee)==-1) then
	  x_message(['You have chosen an RER station to the suburbs'; 
		     'The RER net will be added later']);
	end;
	if (Lnode(Depart)==-2) then
	  x_message(['Are you sure it''s time to leave?';
		     'If yes, you go to the entrance of the campus.';
		     'You take the INRIA shuttle to one of the 3 railway stations of Versailles.';
		     'Then you take the train to Paris (15-20 minutes).']), 
	  Lnode(Depart)=-1;
	end;
	rep=0;
	if (Lnode(Arrivee)==-2) then 
	  rep=x_message(['You have chosen to visit a nice place.';
			 'It is a little bit complicated to reach the campus.';
			 'If you are a tourist you may be interested to know';
			 'that the buildings of the campus have been designed';
			 'by the American troups in Europe after the second world war';
			 'and you may prefer to visit the palace of Versailles.';
			 'In both cases the first part of the travel is the same.';
			 'If you want to know, click on Travel button'],..
			['Travel','Cancel']); 
	end
	if rep==2 then Lnode(Arrivee)=-1;end
	if rep==1 then 
	  x_message(['INRIA-Rocquencourt is located near Versailles.';
		     'From Paris you have to take the train to Versailles and then the INRIA shuttle.';
		     'You have 3 possibilities from Paris:';
		     '- starting from Saint-Lazare station you reach the Versailles-Rive Droite station';
		     '- starting from Invalides station you reach the Versailles-Rive Gauche station';
		     '- starting from Montparnasse-Bienvenue station you reach the Versailles-Chantiers station']);	
	  Lnode(Arrivee)=-1;  
	end
      end
      g=load_graph(SCI+'/demos/metanet/paris');
      gg=load_graph(SCI+'/demos/metanet/paris2');
      gg.default_font_size=8;
      gg.node_y=abs(gg.node_y-max(gg.node_y))
      win=show_graph(gg,'rep');
      la1=g('node_name');lp1=g('node_type');ls1=g('node_x');
      NodeN=lp1(Lnode(Depart)+1)-lp1(Lnode(Depart))+1;
      duration=g('edge_length');tail=g('tail');
      i=1;
      Ldepart(1)=Lnode(Depart);
      for j=1:NodeN,
	if duration(nodes_2_path([Lnode(Depart),ls1(lp1(Lnode(Depart))+j)],g))==4 then
	  Ldepart(i+1)=ls1(lp1(Lnode(Depart))+j);
	  i=i+1;
	end
      end
      NodeN=size(Ldepart);	
      MinTemps=200;
      for k=1:NodeN(1),
        p=shortest_path(Ldepart(k),Lnode(Arrivee),g);
	Etime=size(p);nbarc=Etime(2);
	temps=0;
	Change=0;
	i=1;
	
	temps=duration(p(1))+temps;
	if duration(p(1))==4 then temps=temps-4;end 
	for j=2:(nbarc-1),
	  temps=duration(p(j))+temps;	
	  if duration(p(j))==4 then
	    Change=Change+1;
	    ll=tail(p(j));
	    changename(Change)=mamat(ll);
	  end		
	end

	temps=duration(p(nbarc))+temps;
	if duration(p(nbarc))==4 then temps=temps-4;end; 

	if (temps<MinTemps) then
	  EndTemps=temps;
	  EndP=p;
	  EndChange=Change;
	end,
      end;
      //show_arcs(EndP)
      ttt=path_2_nodes(EndP,g);
      ppp=nodes_2_path(ttt,gg);
      //show_arcs(ppp);
      //for i=1:5, show_nodes(ttt);show_arcs(ppp);end;
      show_arcs(ppp);show_nodes(ttt,'sup');
      name=g('node_name');
      road=name(ttt)';
      kk=size(road);
      clic='.'; v=[1];
      for i=1:kk(1), 
	clac=part(road(i),v); 
	if clac == clic then 
	  route(i)=road(i); 
	else 
	  il=evstr(road(i)); route(i)= mamat(il); 
	end
      end
      j=1;road=[];
      for i=1:kk(1), 
	clac=part(route(i),v); 
	if clac == clic then 
	  road(j)=route(i); 
	  TT(j)=ttt(i);j=j+1;
	end
      end
      kk=size(road);routef=[];
      routef(1)=road(1);TTT(1)=TT(1);j=2;
      for i=2:kk(1), 
	daor=road(i); 
	if daor <> road(i-1) then 
	  routef(j)=road(i); 
	  TTT(j)=TT(i);j=j+1;
	end
      end
      w=string(win)
      execstr(['global EGdata_'+w
	       'EGdata_'+w+'.GraphList.node_label(TTT)=matrix(mamat(TTT),1,-1)'
	       'EGdata_'+w+'.NodeId=4'])
      show_nodes(TTT,'sup')

      xpause(1d6)
      execstr('EGdata_'+w+'.GraphList.node_label(TTT)='' ''')

      x_message(['Duration of the travel (in minutes): '+string(EndTemps) ; 
		 ' ' ;
		 'The number of changes is: '+string(EndChange);
		 string(changename);' '  ;
		 'You will go thru the following stations:';
		 string(routef);]);
      road=[];route=[];routef=[];changename=[];
      iter=tk_choose(['yes','no'],'Another travel?');
      TTT=[];routef=[];road=[];
    end
    seteventhandler("")
    ge_do_quit(%f)
    xdel(win)
endfunction

