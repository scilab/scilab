// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 
function show_pca(lambda,facpr,N)
//used to visualize Principal component analysis results.
// It includes  drawing of
//"correlations circle",  i.e. in the  horizontal axis the
//correlation   values  r(c1;xj)   and  in   the  vertical
//r(c2;xj). It is an extension of the pca function.  
  
//References: Saporta, Gilbert, Probabilites,  Analyse des
//Donnees et Statistique, Editions Technip, Paris, 1990.
//
//Extracted out of pca, author: carlos klimann

  if argn(2)<3 then N=[1 2],end
  if size(N,'*')<>2 then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: Size %s expected.\n"),"show_pca",2,"2x1")),
  end,

  colx=size(facpr,2)
  if max(N)>colx then 
    error(msprintf(gettext("%s: Graph demand out of bounds.\n"),"pca"))
  end
  
  rango=find(lambda(:,1)<=size(facpr,2)*%eps*lambda(1,1),1)
  if  rango==[] then rango=size(lambda,1),end

   
  rc = (ones(colx,1)* sqrt((lambda(N,1))')) .* facpr(:,N) ;
  ra = [1:rango]';
  if ( rango <= 1 ) then return, end
  
  clf()
  subplot(121),a1=gca();
  plot2d(-rc(ra,1),rc(ra,2),style=-10);
  legend('(r(c1,xj),r(c2,xj)');
  a1.x_location="middle";
  a1.y_location = "middle";
  a1.isoview='on';
  a1.grid=[31 31];
  blue=color('blue')
  for k=1:rango,
    xstring(rc(k,1),rc(k,2),'X'+string(k));
    e=gce();e.foreground=blue;
  end
  title(gettext("Correlations Circle"));
  
  //
  subplot(122),a2=gca();
  plot2d3([0;ra;rango+1]',[0; lambda(ra,2);0]);
  plot2d(ra,lambda(ra,2),style=9);
  a2.grid=[31 31];
  plot2d3([0;ra;rango+1]',[0; lambda(ra,2);0])
  plot2d(ra,lambda(ra,2),style=9)
  for k=1:rango,
    xstring(k,0,'l'+string(k)),
    e=gce();e.font_style=1
  end
  title(gettext("Eigenvalues (in percent)"))
  ylabel('%')
endfunction
