function [z,z1]=col1()
	fixpnt=0
	m=[4]
	ncomp=1
	aleft=1
	aright=2
	zeta=[1,1,2,2]
	ipar=0*ones(1,11)
	ipar(3)=1
	ipar(4)=2
	ipar(5)=2000
	ipar(6)=200
	ipar(7)=1
	ltol=[1,3]
	tol=[1.e-11,1.e-11]
	res=aleft:0.1:aright
	z=bvode(res,ncomp,m,aleft,aright,zeta,ipar,ltol,tol,fixpnt,...
		fsub,dfsub,gsub,dgsub,guess)
	z1=[]
	for x=res,z1=[z1,trusol(x)]; end;
endfunction

function [df]=dfsub(x,z)
	df=[0,0,-6/x**2,-6/x]
endfunction

function [f]=fsub(x,z)
	f=(1 -6*x**2*z(4)-6*x*z(3))/x**3
endfunction

function [g]=gsub(i,z)
	g=[z(1),z(3),z(1),z(3)]
	g=g(i)
endfunction

function [dg]=dgsub(i,z)
	dg=[1,0,0,0;0,0,1,0;1,0,0,0;0,0,1,0]
	dg=dg(i,:)
endfunction

function [z,mpar]=guess(x)
	// unused here
	z=0
	mpar=0
endfunction

function [u]=trusol(x)
	u=0*ones(4,1)
	u(1) = .25* (10.*log(2.)-3.) * (1.-x) +0.5* (1./x+ (3.+x)*log(x) - x)
	u(2) = -.25* (10.*log(2.) - 3.) + .5 * (-1./x/x + log(x) + (3.+x)/x - 1.)
	u(3) = .5 * (2./x**3 + 1./x -3./x/x)
	u(4) = .5 * (-6./x**4 - 1./x/x + 6./x**3)
endfunction

function [z,zf]=col2(flag)
	ipar=0*ones(1,11)
	//     define constants, print a heading.
	Gamma = 1.1d0
	eps = .01d0
	dmu = eps
	eps4mu = eps**4/dmu
	xt = sqrt(2.d0*(Gamma-1.d0)/Gamma)
	//   define no. of differential equations.
	fixpnt=0
	iflag=0
	ncomp = 2
	//   orders
	m=[2,2]
	//   interval ends
	aleft = 0.d0
	aright = 1.d0
	//   locations of side conditions
	zeta=0*ones(1,4)
	zeta(1) = 0.d0
	zeta(2) = 0.d0
	zeta(3) = 1.d0
	zeta(4) = 1.d0
	//   ipar  values
	//   a nonlinear problem
	ipar(1) = 1
	//   4 collocation points per subinterval
	ipar(2) = 4
	//   initial uniform mesh of 10 subintervals
	ipar(3) = 10
	ipar(8) = 0
	//   dimension of real work array  fspace  is 40000
	ipar(5) = 40000
	//   dimension of integer work array  ispace  is 2500
	ipar(6) = 2500
	//   (these dimensions of  fspace  and  ispace
	//    enable  colsys  to use meshes of up to 192 intervals.)
	//   print full output.
	ipar(7) = 1
	//   initial approximation for nonlinear iteration is provided
	//   in  solutn
	ipar(9) = 1
	//   a regular problem
	ipar(10) = 0
	//   no fixed points in the mesh
	ipar(11) = 0
	//   tolerances on  all components
	ipar(4) = 4
	ltol=1:4
	tol=1.e-5*ones(1,4)
	//   call  colsys
	res=aleft:0.05:aright
	if flag==1 then 
		z=bvode(res,ncomp,m,aleft,aright,zeta,ipar,ltol,tol,fixpnt,...
			fsub1,dfsub1,gsub1,dgsub1,guess1)
	else 
		z=bvode(res,ncomp,m,aleft,aright,zeta,ipar,ltol,tol,fixpnt,...
			'cnf','cndf','cng','cndg','cngu')
	end
	zf=[   0.00        0.00000D+00    0.20414D+01    0.10225D-31   -0.90397D+00
		0.05        0.10207D+00    0.20414D+01   -0.45265D-01   -0.90794D+00
		0.10        0.20414D+00    0.20414D+01   -0.90926D-01   -0.91982D+00
		0.15        0.30621D+00    0.20414D+01   -0.13738D+00   -0.93962D+00
		0.20        0.40828D+00    0.20414D+01   -0.18502D+00   -0.96735D+00
		0.25        0.51035D+00    0.20415D+01   -0.23425D+00   -0.10030D+01
		0.30        0.61245D+00    0.20430D+01   -0.28545D+00   -0.10466D+01
		0.35        0.71528D+00    0.21066D+01   -0.33905D+00   -0.10992D+01
		0.40        0.83213D+00   -0.24518D+00   -0.39612D+00   -0.12054D+01
		0.45        0.17751D-01   -0.35755D+01   -0.44540D+00   -0.71354D+00
		0.50        0.22512D-01    0.12361D+00   -0.48036D+00   -0.67007D+00
		0.55        0.25869D-01    0.48526D-01   -0.51169D+00   -0.58108D+00
		0.60        0.28099D-01    0.41111D-01   -0.53831D+00   -0.48234D+00
		0.65        0.29911D-01    0.29912D-01   -0.55981D+00   -0.37641D+00
		0.70        0.30874D-01    0.55520D-02   -0.57587D+00   -0.26595D+00
		0.75        0.30032D-01   -0.45165D-01   -0.58642D+00   -0.15667D+00
		0.80        0.25524D-01   -0.14662D+00   -0.59175D+00   -0.60454D-01
		0.85        0.13751D-01   -0.34695D+00   -0.59307D+00   -0.14009D-02
		0.90       -0.12516D-01   -0.75283D+00   -0.59330D+00   -0.28623D-01
		0.95       -0.69427D-01   -0.16508D+01   -0.59906D+00   -0.24811D+00
		1.00        0.26514D-13    0.11926D+03   -0.62542D+00   -0.88763D+00 ]
	z=z'
	zf=zf(:,2:5)
endfunction

function [z,dmval]=guess1(x)
	cons = Gamma * x * (1.d0-.5d0*x*x)
	dcons = Gamma * (1.d0 - 1.5d0*x*x)
	d2cons = -3.d0 * Gamma * x
	if x > xt then   z=[0,0,-cons,-dcons]
		dmval(2) = -d2cons ;
	else z=[ 2.d0 * x, 2 , -2*x+cons,-2+dcons]
		dmval(2) = d2cons ;
	end
	dmval(1) = 0.d0
endfunction

function [f]=fsub1 (x,z)
	f=[0,0]
	f(1) = z(1)/x/x - z(2)/x + (z(1) - z(3)*(1.d0-z(1)/x) - ...
		Gamma*x*(1.d0-x*x/2.)) / eps4mu
	f(2) = z(3)/x/x - z(4)/x + z(1)*(1.d0-z(1)/2.d0/x) / dmu
endfunction

function [df]=dfsub1 (x, z)
	df=0*ones(2,4)
	df(1,1) = 1.d0/x/x +(1.d0 + z(3)/x) / eps4mu
	df(1,2) = -1.d0/x
	df(1,3) = -(1.d0-z(1)/x) / eps4mu
	df(1,4) = 0.d0
	df(2,1) = (1.d0 - z(1)/x) / dmu
	df(2,2) = 0.d0
	df(2,3) = 1.d0/x/x
	df(2,4) = -1.d0/x
endfunction

function [g]=gsub1(i, z)
	g=[z(1),z(3),z(1), z(4)-0.3d0*z(3)+0.7d0]
	g=g(i)
endfunction

function [dg]=dgsub1 (i, z)
	dg=0*ones(4,1)
	select i 
		case 1 then dg(1)=1
		case 2 then dg(3)=1
		case 3 then dg(1)=1
		case 4 then dg(4)=1,dg(3)=-0.3
	end
endfunction
