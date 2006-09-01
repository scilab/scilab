function []=tdinit()

// Copyright INRIA

tit=["bioreactor model (bioreact)";
 "competition model (compet)";
 "system with limit cycle (cycllim)";
 "linear system (linear)";
 "quadratic model (linper)";
 "linear system with a feedback (lincom)";
 "prey predator model (p_p)"]
ii=x_choose(tit," Systems Initialisation ");
// bioreactor
if ~isdef('k');k=2.0;end
if ~isdef('debit');debit=1.0;end
if ~isdef('x2in');x2in=3.0;end
// competition 
if ~isdef('ppr');ppr=1/100 ;end
if ~isdef('ppa');ppa=1/20000;end
if ~isdef('pps');pps=1/200 ;end
if ~isdef('ppb');ppb=1/10000 ;end
if ~isdef('ppk');ppk=1000 ;end
if ~isdef('ppl'); ppl=500;end
if ~isdef('ppm'); ppm=1/100;end
// linear 
if ~isdef('alin'); alin=eye(2,2);end;
// limit cycle 
if ~isdef('qeps'); qeps=0.1;end;
// quadratic  
if ~isdef('q1linper'); q1linper=eye(2,2);end;
if ~isdef('q2linper'); q2linper=eye(2,2);end;
if ~isdef('rlinper'); rlinper=0.0;end;
// linear and feedback
if ~isdef('lic_a'); lic_a=eye(2,2);end;
if ~isdef('lic_b'); lic_b=[1;1];end;
// pray predator 
if ~isdef('p_ppr');p_ppr=1/100 ;end
if ~isdef('p_ppa');p_ppa=1/20000;end
if ~isdef('p_ppm');p_ppm=1/100 ;end
if ~isdef('p_ppb');p_ppb=1/10000 ;end
if ~isdef('p_ppk');p_ppk=1000 ;end
select ii,
case 1 then [k,debit,x2in]=ibio();
case 2 then [ppr,ppa,pps,ppb,ppk,ppl]=icompet();
case 3 then [qeps]=icycl();
case 4 then [alin]=ilinear();
case 5 then [alin,qeps,q1linper,q2linper,rlinper]=ilinp();
case 6 then [lic_a,lic_b]=ilic();
case 7 then [p_ppr,p_ppa,p_ppm,p_ppb,p_ppk]=ip_p();
end
[k,debit,x2in,ppr,ppa,pps,ppb,ppk,ppl,qeps,q1linper,q2linper,...
rlinper,ppm,alin,p_ppr,p_ppa,p_ppm,p_ppb,p_ppk,lic_a,lic_b]= resume(k,debit,x2in,...
ppr,ppa,pps,ppb,ppk,ppl,qeps,...
q1linper,q2linper,rlinper,ppm,alin,p_ppr,p_ppa,p_ppm,p_ppb,p_ppk,lic_a,lic_b)


endfunction
function [k,debit,x2in]=ibio()
// initialisation du bioreactur

// Copyright INRIA

tit=["  bioreactor model initialisation";
   "x(1): biomass concentration ";
   "x(2): sugar concentration"; 
   " ";
   "xdot(1)=mu_td(x(2))*x(1)- debit*x(1)";
   "xdot(2)=-k*mu_td(x(2))*x(1)-debit*x(2)+debit*x2in";
   "mu(x):= x/(1+x)"];
x=x_mdialog(tit,['k';'debit';'x2in'],[string(k);string(debit);string(x2in)]);
k=k;debit=debit;x2in=x2in;
if x<>[] then k=evstr(x(1));debit=evstr(x(2));x2in=evstr(x(3));end


endfunction
function [ppr,ppa,pps,ppb,ppk,ppl]=icompet()

// Copyright INRIA

tit=["  competition model initialisation";
     "xdot(1) = ppr*x(1)*(1-x(1)/ppk) - u*ppa*x(1)*x(2)";
     "xdot(2) = pps*x(2)*(1-x(2)/ppl) - u*ppb*x(1)*x(2)"];

x=x_mdialog(tit,['ppr';'ppa';'pps';'ppb';'ppk';'ppl'],...
	string([ppr;ppa;pps;ppb;ppk;ppl]));
//	['1/100';'1/20000';'1/200';'1/10000';'1000';'500']);
ppr=ppr;ppa=ppa;pps=pps;ppb=ppb;ppk=ppk;ppl=ppl;
if x<>[] then ppr=evstr(x(1));
ppa=evstr(x(2));
pps=evstr(x(3));
ppb=evstr(x(4));
ppk=evstr(x(5));
ppl=evstr(x(6));end


endfunction
function [qeps]=icycl()

// Copyright INRIA

//[qeps]=icycl()
tit=["  system with limit cycle ";
     " xdot=a*x+qeps(1-||x||**2)x";" Enter qeps"];
qeps_r=x_matrix(tit,qeps);
if qeps_r<>[] then qeps=qeps_r;end



endfunction
function [alin]=ilinear()

// Copyright INRIA

rep=x_matrix(['xdot=a*x';'Matrice 2x2 du systeme lineaire'],alin);
if rep<>[] then alin=rep;end


endfunction
function [alin,qeps,q1linper,q2linper,rlinper]=ilinp()

// Copyright INRIA

tit=[" quadratic model ";
     "xdot= a*x+(1/2)*qeps*[(x'')*q1*x;(x'')*q2*x]+r"];
x=x_mdialog(tit,['qeps';'r'],...
	[string(qeps);string(rlinper)]);
qeps=qeps;
rlinper=rlinper;
alin=alin;
q1linper=q1linper;
q2linper=q2linper;
qeps=qeps;
rlinper=rlinper;
if x<>[] then   rlinper=evstr(x(2));
	        qeps=evstr(x(1));
end
rep=x_matrix([tit;'Enter a'],alin);
if rep<>[] then alin=rep;end
rep=x_matrix([tit;'Enter q1linper'],q1linper);
if rep<>[] then q1linper=rep;end
rep=x_matrix([tit;'Enter q2linper'],q2linper);
if rep<>[] then q2linper=rep;end


endfunction
function [lic_a,lic_b]=ilic()

// Copyright INRIA

tit=[" linear system with a feedback ";
	"xdot= a*x +b*(-k*x);"];
rep=x_matrix([tit;"Enter a"],lic_a)
if rep<>[] then lic_a=rep;end
rep=x_matrix([tit;"Enter b"],lic_b)
if rep<>[] then lic_b=rep;end


endfunction
function [p_ppr,p_ppa,p_ppm,p_ppb,p_ppk]=ip_p()

// Copyright INRIA

tit=["  pray predator model initialisation";
     "xdot(1) = p_ppr*x(1)*(1-x(1)/p_ppk) - p_ppa*x(1)*x(2) - u*x(1);"
     "xdot(2) = -p_ppm*x(2)             + p_ppb*x(1)*x(2) - u*x(2);"];
x=x_mdialog(tit,['p_ppr';'p_ppa';'p_ppm';'p_ppb';'p_ppk'],...
	string([p_ppr;p_ppa;p_ppm;p_ppb;p_ppk]));
p_ppr=p_ppr;
p_ppa=p_ppa;
p_ppm=p_ppm;
p_ppb=p_ppb;
p_ppk=p_ppk;
if x<>[] then p_ppr=evstr(x(1));
p_ppa=evstr(x(2));
p_ppm=evstr(x(3));
p_ppb=evstr(x(4));
p_ppk=evstr(x(5));
end
endfunction
