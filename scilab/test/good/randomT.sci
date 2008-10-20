function [z]=RndIntT(n)
	//------------------------------- OK
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	imin=-10;
	imax=10;
	y=RndInt(1,n,-10,10);
	i=imin-2:imax+2;
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	plot2d3("onn",i',z'/n,[1,2],,,[-12,0,12,0.1]);
	i1=(imin:imax)';
	plot2d1("onn",i1,ones(i1)/prod(size(imin:imax)),[-2,5]);
	legend(['Simulation';'Theory']);
	drawnow();
endfunction


function [z]=RndDiscT(n)
	//------------------------------- OK
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	x=[1,3,4,6,10,12];
	pr=[0.1,0.2,0.3,0.2,0.1,0.1];
	y=RndDisc(1,n,x,pr);
	i=0:13
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	plot2d3("onn",i',z'/n,[1,3],,,[0,0,14,0.5]);
	plot2d1("onn",x',pr',[-2,6]);
	xtitle('Simulation of a discrete random variable');
	BackgroundColorId = color(232,230,200);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	legend(['Simulation';'Theory']);
	drawnow();
endfunction


function [zt]=BinomialT(n)
	//------------------------------- OK
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	prb=0.5;
	N=10;
	y=Binomial(1,n,prb,N);
	i=0:10;
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	plot2d3("onn",i',z'/n,[1,3]);
	deff('[y]=fact(n)','y=prod(1:n)');
	deff('[z]=C(N,n)','z= fact(N)/(fact(n)*fact(N-n))');
	i=0:N; 
	zt=[];
	for j=i, zt=[zt, C(N,j)*prb^j*(1-prb)^(N-j)];end
	plot2d1("onn",i',zt',[-2,6]);
	xtitle('Simulation of a binomial random variable');
	BackgroundColorId = color(232,230,200);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	legend(['Simulation';'Theory']);
	drawnow();
endfunction 


function []=GeomT(n)
	//------------------------------- OK
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end
	pr=0.2
	y=Geom(1,n,pr)
	N=20
	i=0:N;
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	plot2d3("onn",i',z'/n,[1,3]);
	zt=[0];for i1=1:N; zt=[zt,pr*(1-pr)^(i1-1)];end
	plot2d1("onn",i',zt',[-2,6]);
	xtitle('Simulation of a geometric random variable');
	BackgroundColorId = color(232,230,200);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	legend(['Simulation';'Theory']);
	drawnow();
endfunction


function [z]=PoissonT(n) 
	//------------------------------- OK
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=1000;end 
	pmean=3;
	y=Poisson(1,n,pmean);
	N=20;
	i=0:N;
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	plot2d3("onn",i',z'/n,1);
	deff('[y]=fact(n)','if n==0 then y=1;else y=n*fact(n-1);end');
	zt=[];for i1=0:N; zt=[zt, exp(-pmean) *pmean^i1/fact(i1)];end
	plot2d1("onn",i',zt',[-2,6]);
	xtitle('Simulation of a poisson random variable');
	BackgroundColorId = color(232,230,200);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	legend(['Simulation';'Theory']);
	drawnow();
endfunction


function []=ExpT(n)
	//------------------------------- OK
	// lambda exp(-lambda x) x>=0 
	// ---------------------------
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=1000;end 
	lambda=3;
	y=Exp(1,n,lambda);
	drawlater();
	histplot([0:0.1:10],y,[1,1],'051',' ',[0,0,5,3]);
	deff('[y]=f(x)','y=lambda*exp(-lambda*x);');
	x=[0:0.1:10]';plot2d(x,f(x),1,"000");
	titre= 'macro histplot : Histogram plot';
	xtitle(titre,'Classes','N(C)/Nmax');
	BackgroundColorId = color(232,230,200);
	PolylineFillColorId = color(179,179,0);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	current_axe.children(2).children.fill_mode = 'on';
	current_axe.children(2).children.background = PolylineFillColorId;
	current_axe.grid = [-1 PolylineFillColorId];
	drawnow();
endfunction


function []=WeibullT(n)
	//------------------------------- 
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	y=Weibull(1,n,1,2)
	drawlater();
	histplot(20,y,[1,1],'061');
	xtitle('Simulation of a Weibull random variable');
	BackgroundColorId = color(232,230,200);
	PolylineFillColorId = color(179,179,0);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	current_axe.children.children.fill_mode = 'on';
	current_axe.children.children.background = PolylineFillColorId;
	current_axe.grid = [-1 PolylineFillColorId];
	drawnow();
endfunction 


function []=HyperGeomT(n)
	//------------------------------- 
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	y=HyperGeom(1,n,1,10)
	drawlater();
	histplot([0:0.25:10],y,[1,1],'061',' ',[0,0,10,0.4]);
	xtitle('Simulation of a hyper geometric random variable');
	BackgroundColorId = color(232,230,200);
	PolylineFillColorId = color(179,179,0);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	current_axe.children.children.fill_mode = 'on';
	current_axe.children.children.background = PolylineFillColorId;
	current_axe.grid = [-1 PolylineFillColorId];
	drawnow();
endfunction 


function [y]=ErlangT(n)
	//-------------------------------
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end 
	y=Erlang(1,n,10,1);
	drawlater();
	histplot(20,y,[1,1],'061');
	xtitle('Simulation of a Erlang random variable');
	BackgroundColorId = color(232,230,200);
	PolylineFillColorId = color(179,179,0);
	current_axe = gca();current_axe.title.font_size = 3;
	current_axe.background = BackgroundColorId;
	current_axe.children.children.fill_mode = 'on';
	current_axe.children.children.background = PolylineFillColorId;
	current_axe.grid = [-1 PolylineFillColorId];
	drawnow();
endfunction 
