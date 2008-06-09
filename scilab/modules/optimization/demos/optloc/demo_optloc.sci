//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_optloc()
  stacksize(1D7)
  n=20; // the number of points

  //The coordinates of the points
  Alpha=round(99*rand(n,1))+1;
  Beta=round(99*rand(n,1))+1;
  my_handle             = scf(100001);
  clf(my_handle,"reset");
  drawlater();
  plot(Alpha,Beta,'*')
  e=gce();e=e.children;e.mark_size=10;
  l=legend(_('Customer and service locations'))
  drawnow()
  realtimeinit(0.1);for k=1:10,realtime(k),end

  // costs to establish  services 
  C=100*ones(n,1); // all costs are equal

  [X1,Y1]=optloc(Alpha,Beta,C);

  kf=find(Y1>0);
  drawlater();
  plot(Alpha(kf),Beta(kf),'ro')
  e=gce();e=e.children;e.mark_size=10;
  delete(l)
  l=legend([_('Customer locations');
  msprintf(_('Service %d locations'),1)])
  drawnow()
  [ic,jc]=find(X1>0);
  xsegs([Alpha(ic) Alpha(jc)]',[Beta(ic),Beta(jc)]',12)

  C=800*ones(n,1); // all costs are equal

  [X1,Y1]=optloc(Alpha,Beta,C);
  kf=find(Y1>0);

  drawlater();
  plot(Alpha(kf),Beta(kf),'go')
  e=gce();e=e.children;e.mark_size=14;
  delete(l)
  l=legend([_('Customer locations');
	    msprintf(_('Service %d locations'),1);
	    msprintf(_('Service %d locations'),2)])
  drawnow()
  [ic,jc]=find(X1>0);
  xsegs([Alpha(ic) Alpha(jc)]',[Beta(ic),Beta(jc)]',15)
  realtimeinit(0.1);for k=1:30,realtime(k),end
endfunction

function [X,Y]=optloc(Alpha,Beta,C)
  n=size(Alpha,'*')
  if n<>size(Beta,'*')| n<>size(C,'*') then 
    error(msprintf(_('%s: incompatible argument dimensions'),'optloc'))
  end
  // make columns
  Alpha=Alpha(:);Beta=Beta(:);C=C(:);
  // compute distance between points
  D=sqrt((Alpha*ones(1,n)-ones(n,1)*Alpha').^2+..
	 (Beta *ones(1,n)-ones(n,1)*Beta' ).^2);
  // Comptuation of linpro input arguments
  // ======================================

  // The criterion
  //  ---               ---  --- 
  //  \		      \    \   
  //Z= >(C(j)*Y(j)) +    >    > D(i,j)*X(i,j)
  //  /		      /    /   
  //  ---j	      ---i ---j

  // Z = p'*x  x=[X(:);Y(:)]

  p=[D(:);C(:)];

  // Boundary constraints
  //  X(i,j)>=0
  //  Y(j)<=1  
  //  Y(j)>=0
  // Y(j)<=1  and Y(j)>=0 are used in place of    Y(j)=0 ou Y(j)=1

  ci=[zeros(n*n,1);  //X(i,j)>=0
      zeros(n,1)  ]; //Y(j)>=0

  cs=[ones(n*n,1)    //X(i,j)<=1
      ones(n,1)  ];  //Y(j)<=1

  // Inequality constraints 
  //  X(i,j)<=Y(j)   ==>  X(i,j)-Y(j)<=0

  // c*x<=b
  b2=zeros(n*n,1);
  A2=[eye(n*n,n*n),-eye(n,n).*.ones(n,1)];  // X(i,j)-Y(j)
  
  //Equality constraints 
  //  ---           
  //  \		  
  //   > X(i,j) = 1 
  //  /		  
  //  ---j	

  b1=ones(n,1);
  A1=[ones(1,n).*.eye(n,n) zeros(n,n)];

  //resolution
  x0=zeros(n*n+n,1);
  [x,lagr,f]=linpro(p,[A1;A2],[b1;b2],ci,cs,n)
  if max(abs(x-round(x)))>1.d-6 then 
    warning(msprintf(_('%s: non integer solution found'),'optloc'))
  end
  x=round(x)
  X=matrix(x(1:n^2),n,n)
  Y=x(n^2+1:$)
endfunction
