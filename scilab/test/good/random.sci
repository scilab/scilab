function [y]=RndInt(m,n,imin,imax) 
	// discrete uniform random number 
	//-------------------------------
	// Copyright ENPC 
	y=rand(m,n,'uniform')
	y=int(floor(y*(imax+1-imin)+ imin ));
endfunction 


function [y]=RndDisc(m,n,x,p)
	// discrete law random number
	// sum p_i delta_{x_i}
	//-------------------------------
	p1=[0,p];p1=cumsum(p1);
	y=rand(m,n,'uniform')
	N=prod(size(x));
	res=0*ones(m*n);
	for i=1:N,z=0*ones(m*n,1),id=find( p1(i) <= y & y < p1(i+1) ),
		z(id)=x(i)*ones(prod(size(id))),res=res+z;
	end
	y=matrix(res,m,n);
endfunction 

function [y]=Binomial(m,n,pb,nb) 
	// Binomial law (p,N) 
	// P{X=n} = C_N^n p^n (1-p)^(N-n)
	//----------------------------------
	res=[];
	// we use blocks of size 100 to avoid overflows 
	ntir=100;
	ntirc=ntir;
	y=rand(ntir,nb,'uniform');
	indy= find( y < pb);
	y=0*ones(y);
	y(indy)=1;
	y=sum(y,'c')
	res=[res;y];
	while ( ntirc < m*n )
		y=rand(ntir,nb,'uniform');
		indy= find(y< pb);
		y=0*ones(y);
		y(indy)=1;
		y=sum(y,'c')
		res=[res;y];
		ntirc=ntirc+ntir;
	end
	y=matrix(res(1:m*n),m,n);
endfunction 

function [y]=Geom(m,n,p) 
	// P(0)= 0 P(i) = p*(1-p)^{n-1} P(inf)=0
	// E = 1/p ; sig2= (1-p)/p^2
	//--------------------------------------
	if p >= 1 then write(%io(2),'p must be < 1');end
	y=0*ones(m,n)
	for i=1:m*n,
		samples=1
		z=rand(1,1,'uniform');
		while( z < 1-p) ,z=rand(1,1,'uniform'); samples=samples+1;end
		y(i)= samples;
	end
	y=matrix(y,m,n)
endfunction 


function [y]=Poisson(m,n,pmean) 
	// P{n} = exp(-lambda)lambda^n/n!
	// pmean =lambda 
	//----------------------------
	y=0*ones(m,n)
	bound= exp(-pmean);
	for i=1:m*n, 
	count=0
	lprod=1
	while( lprod >= bound), lprod=lprod*rand(1,1,'uniform');
	count=count+1;end
	y(i)=count-1;
	end
	y=matrix(y,m,n)
endfunction 

function [y]=Exp(m,n,lambda)
	// lambda exp(-lambda x) x>=0 
	// ---------------------------
	y=(-1/lambda)* log(rand(m,n,'uniform'));
endfunction 

function [y]=Weibull(m,n,Alpha,Beta)
	//------------------------------- 
	y=rand(m,n,'uniform')
	y= (Beta*( - log(1-y)))^(1/Alpha)
endfunction 

function [y]=HyperGeom(m,n,Mean,var)
	//------------------------------- 
	z = var / (Mean * Mean);
	pP = 0.5 * (1.0 - sqrt((z - 1.0) / ( z + 1.0 )));
	y=rand(m,n,'uniform')
	zz=find( y > pP) ;
	y=pP*ones(y);
	y(zz) = (1-pP)*ones(zz);
	y1=rand(m,n,'uniform')
	y=-Mean * log(y1) ./ (2.0 * y) ;
endfunction 

function [y]=Erlang(m,n,pMean,pVariance) 
	//------------------------------- 
	k = int( (pMean * pMean ) / pVariance + 0.5 );
	if (k <= 0) then k=1;end
	a = k / pMean;
	// we use blocks of size 100 to avoid overflows 
	res=[];
	ntir=100;
	ntirc=ntir;
	y=rand(ntir,k,'uniform');
	y= -log(prod(y,'r'))/a;
	res=[res;y];
	while ( ntirc < m*n )
	y=rand(ntir,k,'uniform');
	y= -log(prod(y,'r'))/a;
	res=[res;y];
	ntirc=ntirc+ntir;
	end 
	y=matrix(res(1:m*n),m,n);
endfunction 

function [y]=RndPerm(n) 
	//------------------------------- 
	// a uniform random permutation of (1:n)
	y=rand(1,n,'uniform')
	[us,z]=sort(y);
endfunction 

