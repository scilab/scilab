function []=boucle(fch,abruit,xdim,npts,farrow)
// Copyright INRIA
[lhs,rhs]=argn(0);
ncnl=lines();
if rhs<=4,farrow='f';end;
if rhs<=3,
  // interactive dialog for  integration points 
  if ~isdef('bcl_npts');bcl_npts=['100';'0.1'];end;
  rep=x_mdialog('Requested points and step ',...
       	    ['n points';'step'],bcl_npts);
  if rep <> [] then bcl_npts=rep;end
  npts=evstr(bcl_npts');
else 
  bcl_npts=npts;
end
if rhs <= 2,
  // interactive dialog for graphics boundaries
  if ~isdef('bcl_xdim');bcl_xdim=['-1';'-1';'1';'1'];end
  rep=x_mdialog('Graphic boundaries',...
            ['xmin';'ymin';'xmax';'ymax'],...
            bcl_xdim);
  if rep<>[] ;bcl_xdim=rep;end
  xdim=evstr(bcl_xdim');
else 
  bcl_xdim= xdim;
end 
// Test on the frame 
if xdim(3) <= xdim(1),
write(%io(2),'Error:  xmin < xmax '),lines(ncnl(1));return,end
if xdim(4) <= xdim(2),
write(%io(2),'Error:  ymin < ymax '),lines(ncnl(1));return,end

if rhs <=1,abruit=0.0;end
tcal=npts(2)*(0:npts(1))

br=sqrt(abruit)*rand(1,npts(1)+1,'normal');
if type(fch)==10;
// Sending constants to a hard coded program 
// if the system is a string (it is supposed to be the bcomp system)
  idisp=0;
  pp_c=[ppr,ppk,ppa,ppb,ppm,pps,ppl]
  if npts(1)> 1000 then 
	write(%io(2),'Error: the number of requested points ') 
	write(%io(2),'must be smaller than 1000 for hard coded example');
	npts(2)=mini(npts(2),1000);
  end
  fort('icomp',xe,1,'r',ue,2,'r',f,3,'r',g,4,'r',h,5,'r',...
      k,6,'r',l,7,'r',br,8,'r',npts(2),9,'r',npts(1),10,'i',...
      pp_c,11,'r',idisp,12,'i','sort');
end
xset("window",0);xselect();xclear();
if isdef('xe');
  plot2d([xe(1)],[xe(2)],[-2,-4],"111",...
      'equilibrium point for ue='+string(ue),xdim);
else 
  plot2d([xdim(1)],[xdim(3)],[0],"111"," ",xdim);
end
ylast=(1/2)*[xdim(3)+xdim(1),xdim(4)+xdim(2)]';
ylast=[ylast;ylast];
// Boucle sur les points de depart
  go_on=1
  while go_on==1,
       ftest=1;
       while ftest==1,
	  xset("window",0);xselect();
	  n=x_choose(['New initial points (x0 and xchap0)';'Continue ode';'Quit'],"Choose ");
	  n=n-1;
          if n==-1,
	    go_on=0;lines(ncnl(1));
	    [bcl_xdim,bcl_npts]=resume(bcl_xdim,bcl_npts);
	  end
          if n==2,
	    go_on=0;lines(ncnl(1));
	    [bcl_xdim,bcl_npts]=resume(bcl_xdim,bcl_npts);
	  end
	  if n==0,
	    while %t
	      [i,x,y]=xclick(); 
	      if i==-100 then return,end
	      if or(i==[0 1 2 3 4 5]) then break,end
	    end
	    x0=[x,y];
	    while %t
	      [i,x,y]=xclick(); 
	      if i==-100 then return,end
	      if or(i==[0 1 2 3 4 5]) then break,end
	    end
	    xchap0=[x,y];
	    fullx0=[x0,xchap0]';
	  end;
          if n==1,fullx0=ylast;end;
          if type(fch)==10,
             ftest=desorb1(fullx0,npts,fch,farrow,xdim);
          else
	    ftest=desorb1(fullx0,npts,list(fch,abruit,...
					   npts(2),npts(1)),farrow,xdim);
          end
          if ftest==1;x_message('Initial value out of boundaries'),end
       end
  end
lines(ncnl(1));
[bcl_xdim,bcl_npts]=resume(bcl_xdim,bcl_npts);

endfunction
function [res]=desorb1(x0,n1,fch,farrow,xdim);
//[res]=desorb1(x0,n1,fch,farrow,xdim);
//!
res=0
write(%io(2),'Computing')
tcal=n1(2)*(0:n1(1))
ylast=x0;
xxx=ode(x0,0,tcal,fch);
ylast=xxx(:,$);
[nn1,nn2]=size(tcal);
comcom=-k*(xxx(3:4,:)-xe*ones(1,nn2));
//dessin de l'evolution conjointe de la deuxieme
//composante de l'etat et de son estimee (observateur)
xset("window",1);xbasc();
plot2d([tcal;tcal]',xxx([2,4],:)',[1,2],"111",...
       "x2(t) @observer of x2(t)",[0,xdim(2),n1(1)*n1(2),xdim(4)])
xset("window",2);xbasc();
//--- The controller 
plot2d([tcal]',[comcom]',[1],"121",...
       "Linear controller (ecart par rapport a ue)")
xset("window",0);
//--- phase portrait 
plot2d(xxx([1,3],:)',xxx([2,4],:)',[1,2],"111","(x1,x2)@observer ",...
xdim);
[ylast]=resume(ylast);
endfunction
