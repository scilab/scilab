function []=emc2(filename)
	
	// Visualise un resultat de type GNU.*
	// generes par NSCK2E 
	// en general un ensemble de segments 
	// Copyright ENPC
	
	xx=read(filename,-1,2);
	x=xx(:,1);
	y=xx(:,2);
	y1=matrix(y,2,prod(size(y))/2);
	x1=matrix(x,2,prod(size(x))/2);
	rect=[mini(x1),mini(y1),maxi(x1),maxi(y1)];
	plot2d(1,1,[1],"031"," ",rect);
	xsegs(x1,y1);
	
endfunction 

function []=amdbaR(File_name)
	
	// Read a file describing a MESH of finite elements 
	// of triangle type 
	// The file is of amdba type 
	
	[lhs,rhs]=argn(0);
	if rhs == 0 ; File_name='MESH'; end
	unit=file('open',File_name,'old');
	x=read(unit,1,2);
	noeuds=x(1);
	triang=x(2);
	noeul=read(unit,x(1),4);
	trianl=read(unit,x(2),5);
	file('close',unit);
	[noeuds,triang,noeul,trianl]=resume(noeuds,triang,noeul,trianl);
	
endfunction 

function []=meshvisu(col,rect)
	
	// Mesh visualisation 
	// uses global variables 
	
	[lhs,rhs]=argn(0);
	if rhs<=0;col=1;end
	if rhs<=1;rect=[mini(noeul(:,2)),mini(noeul(:,3)),maxi(noeul(:,2)),maxi(noeul(:,3))];end
	if rhs<=2;iso='1';end
	plot2d(1,1,[1],"031"," ",rect);
	xset("clipgrf");
	xx=trianl(:,2:4);
	xx=matrix(xx,prod(size(xx)),1);
	x=noeul(xx,2);
	triang=size(x,'*')/3;
	x=matrix(x,triang,3);
	y=noeul(xx,3);
	y=matrix(y,triang,3);
	x=[x,x(:,1)]';
	y=[y,y(:,1)]';
	xpolys(x,y,col*ones(1,triang));
	xset("clipoff");
	
endfunction 

function []=nvisu(rect)
	
	// Visualisation des noeuds 
	
	[lhs,rhs]=argn(0);
	if rhs==0;rect=[mini(noeul(:,2)),mini(noeul(:,3)),maxi(noeul(:,2)),maxi(noeul(:,3))];end
	plot2d(1,1,[1],"031"," ",rect);
	xset("clipgrf");
	bords=noeul(find(noeul(:,4)>0),:);
	[no,ign]=size(bords);
	for i=1:no
		xstring(bords(i,2),bords(i,3),string(bords(i,4)));
	end
	xset("clipoff");
	
endfunction 

function []=emc2V(i,j,k,sa,FN,rect)
	
	// Visualise un champ de vitesse 
	// la taille des vecteur est constante
	// mais on change la couleur suivant l'intensite du champ
	// Les vecteurs sont lus dans les colonnes i et j du fichier 
	// du fichier FN qui continet k colonnes 
	
	[lhs,rhs]=argn(0);
	plot2d(1,1,[1],"031"," ",rect);
	xset("clipgrf");
	if rhs == 0 ; FN='MESH';end
	unit=file('open',FN,'old')
	resu=read(unit,noeuds,k);
	resu=resu(:,[i,j]);
	nm=[];
	for i=1:noeuds;
		nm1=norm(resu(i,:),2);
		nm=[nm,nm1];
		if nm1<>0,resu(i,:)=resu(i,:)/nm1;end
	end
	nmax=maxi(nm);
	if nmax<>0; nm=nm/nmax;end
	xsegs([noeul(:,2)-(1/sa)*resu(:,1),noeul(:,2)+(1/sa)*resu(:,1)]',...
		[noeul(:,3)-(1/sa)*resu(:,2),noeul(:,3)+(1/sa)*resu(:,2)]',...
		16*ones(nm)-16*nm);
	file('close',unit);
	xset("clipoff");
	file("close",unit);
	
endfunction 

function []=emc2C(i,j,FN,rect)
	
	// iso contour de la fonction lineaire sur les 
	// triangles du maillage on lit les valeurs de la fonction 
	// aux noeuds du maillage sur la ieme colonne 
	// du fichier file qui en contient j 
	
	[lhs,rhs]=argn(0);
	unit=file('open',FN,'old');
	resu=read(unit,-1,j);
	resu=resu(:,i);
	file('close',unit);
	if rhs<=3;rect=[mini(noeul(:,2)),mini(noeul(:,3)),maxi(noeul(:,2)),maxi(noeul(:,3))];end
	fec(noeul(:,2),noeul(:,3),trianl,resu,"131"," ",rect);
	file("close",unit);
	
endfunction 
