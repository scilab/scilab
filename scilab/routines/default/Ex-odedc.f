      subroutine fexcd(jflag,nc,nd,t,y,ydp)
c     input variables jflag, nc, nd, t, y
c     jflag=0 or 1
c     nc = # of continuous states yc
c     nd = # of discrete states yd
c     t = time
c     y = state variable = [yc; yd] 
c     output variable = ydp
c     
c     iflag=0  >> external routine must 
c     load ydp(1:nc) with ydot=d/dt ( yc(t) )
c     derivative of continuous state 
c     iflag=1  >> external routine must
c     load ydp(1:nd) with yplus= yd(t+)  
c     update of discrete state
c     here y=[yc;yd] has dimension nc+nd=3+2 
c     
c     Example:
c     1/ call this fexcd: 
c     y0c=[1;2;3]; y0d=[1;-1]; nd=2; t0=0; t=1:10;
c     y=odedc([y0c;y0d],nd,[5,0],t0,t,'fexcd')
c     
c     2/ using dynamic link: 
c     link('fexcd.o','fexcd')
c     y0c=[1;2;3]; y0d=[1;-1]; nd=2; t0=0; t=1:10;
c     y=odedc([y0c;y0d],nd,[5,0],t0,t,'fexcd')
c     
c     3/ passing a parameter to fexcd routine:     
c     y=odedc([y0c;y0d],nd,[5,0],t0,t,list('fexcd',param))
c     param can be retrieved in fexcd by:
c     param(1)=y(nc+nd+1) , param(2)=y(nc+nd+2) etc 
c     with this calling sequence y is a nc+nd+np vector
c     where np=dimension of scilab variable param
c     
c     Copyright INRIA
      double precision t, y, ydp
      dimension y(*), ydp(*)
      if(jflag.eq.0) then
         ydp(1) = y(4)
         ydp(2) = y(5)
         ydp(3) = 0.d0
      elseif(jflag.eq.1) then
         ydp(1)=-y(4)
         ydp(2)=-y(5)
      endif
      return
      end

c     The odedc example in the manual:

      subroutine fcd(jflag,nc,nd,t,x,xdp)
c     set dimensions of u,v,y
      double precision u(1),v(1),y(1)
c     
      double precision t,x(*),xdp(*)
      if (jflag.eq.0) then
c     fc(t,xc,e(t)-hd(t,xd)
c     u=e(t)-hd(t,xd)
         call  finput(t,v)
         call hd(t,x(nc+1),u)
         u(1)=v(1)-u(1)
c     u(2)=v(2)-u(2)  ....
         call  fc(t,x,u,xdp)
      elseif(jflag.eq.1) then
         call hc(t,x,y)
         call fd(x(nc+1),y,xdp)
      endif
      return
      end

      subroutine fc(t,xc,u,xdot)
      double precision t,xc(*),xdot(*),u(*)
c     A=[-10,2,3;4,-10,6;7,8,-10];B=[1;1;1]
      xdot(1)=-10*xc(1)+2*xc(2)+3*xc(3)+u(1)
      xdot(2)=4*xc(1)-10*xc(2)+6*xc(3)+u(1)
      xdot(3)=7*xc(1)+8*xc(2)-10*xc(3)+u(1)
      return
      end

      subroutine hc(t,x,y)
      double precision t,x(*),y(*)
      y(1)=x(1)+x(2)+x(3)
      return
      end

      subroutine fd(xd,y,xp)
c     Ad=[0.5,1;0,0.05] Bd=[1;1]
      double precision xd(*),y(*),xp(*)
      xp(1)=0.5D0*xd(1)+xd(2)+y(1)
      xp(2)=0*xd(1)+0.05D0*xd(2)+y(1)
      return
      end

      subroutine hd(t,xd,u)
      double precision t,xd(*),u(*)
      u(1)=xd(1)+xd(2)
      return
      end

c     The odedc example in the manual:
c     It is assumed here that scilab variables
c     A,B,C  and  Ad,Bd,Cd   exist.
c     
      subroutine fcd1(jflag,nc,nd,t,x,xdp)
c     
c     iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
c     iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
c     
c     set here dimensions of u,v,y
      double precision u(1),v(1),y(1)
c     
      double precision t,x(*),xdp(*)
      if (jflag.eq.0) then
c     xcd=fc1(t,xc,u)
c     u=e(t)-hd1(t,xd)
         call  finput(t,v)
         call hd1(t,x(nc+1),u)
         u(1)=v(1)-u(1)
c     u(2)=v(2)-u(2)  ....
         call  fc1(t,x,u,xdp)
      elseif(jflag.eq.1) then
c     xcd=fd1(xd,y)     xd=[x(nc+1),x(nc+2),...]
c     y=hc1(t,xc)
         call hc1(t,x,y)
         call fd1(x(nc+1),y,xdp)
      endif
      return
      end

      subroutine fc1(t,xc,u,xdot)
c     xdot=A*xc+B*u
c     A and B real scilab matrices
      double precision t,xc(*),xdot(*),u(*)
      include '../stack.h'
      call matptr('A'//char(0),m,n,la)
c     call dset(m,0.0d0,xdot,1)
c     call dgemm('n','n',m,1,m,1.0d0,stk(la),m,xc,m,1.0d0,xdot,m)
      call brdmmul(stk(la),m,xc,m,xdot,m,m,m,1)
      call matptr('B'//char(0),m,nb,lb)
      call dgemm('n','n',m,1,nb,1.0d0,stk(lb),m,u,1,1.0d0,xdot,m)
      end

      subroutine hc1(t,x,y)
      double precision t,x(*),y(*)
      include '../stack.h'
c     y=C*x
      call matptr('C'//char(0),m,n,lc)      
      call brdmmul(stk(lc),m,x,m,y,m,m,n,1)
      end

      subroutine fd1(xd,y,xp)
c     xp=Ad*xd + Bd*y
      double precision xd(*),y(*),xp(*)
      include '../stack.h'
      call matptr('Ad'//char(0),m,n,la)
      call brdmmul(stk(la),m,xd,m,xp,m,m,m,1)
      call matptr('Bd'//char(0),m,nb,lb)
      call dgemm('n','n',m,1,nb,1.0d0,stk(lb),m,y,1,1.0d0,xp,m)
      end


      subroutine hd1(t,xd,u)
      double precision t,xd(*),u(*)
c     u=Cd*xd
      include '../stack.h'
c     y=C*x
      call matptr('Cd'//char(0),m,n,lc)      
      call brdmmul(stk(lc),m,xd,m,u,m,m,n,1)
      end

      subroutine finput(t,v)
      double precision t,v(1)
      v(1)=sin(3*t)
      end

c     dot(x)=A x + B u with u= (0,1) step function 
      subroutine phis(jflag,nc,nd,t,x,xdp)
c     
c     iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
c     iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
c     
      double precision t,x(*),xdp(*)
      if (jflag.eq.0) then
c     dot(x)=A*x+B*xd
         call  sbrc(t,x,xdp)
      elseif(jflag.eq.1) then
c     xd=1-xd
         xdp(1)=1.d0-x(nc+1)
      endif
      end


c     dot(x)=A x + B u with u= piecewise triangular function 
      subroutine phit(jflag,nc,nd,t,x,xdp)
c     
c     iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
c     iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
c     
      double precision t,x(*),xdp(*)
      if (jflag.eq.0) then
c     dot(x1c)=A*x1c+B*x2c
c     dot(x2c)=xd
         call  sbrc(t,x,xdp)
         xdp(nc)=x(nc+1)
      elseif(jflag.eq.1) then
c     xd=-xd
         xdp(1)=-x(nc+1)
      endif
      end

      subroutine sbrc(t,x,xdot)
c     xdot=A*x1+B*x2
c     A and B real scilab matrices
      double precision t,x(*),xdot(*)
      include '../stack.h'
      call matptr('A'//char(0),m,n,la)
      call brdmmul(stk(la),m,x,m,xdot,m,m,m,1)
      call matptr('B'//char(0),m,nb,lb)
      call dgemm('n','n',m,1,nb,1.0d0,stk(lb),m,x(m+1),1,1.0d0,xdot,m)
      end

      subroutine brdmmul(a,na,b,nb,c,nc,l,m,n)
c     Copyright INRIA
      double precision a(*),b(*),c(*)
      double precision ddot
      integer na,nb,nc,l,m,n
      integer i,j,ib,ic
c     
      ib=1
      ic=0
      do 30 j=1,n
         do 20 i=1,l
            c(ic+i)=ddot(m,a(i),na,b(ib),1)
 20      continue
         ic=ic+nc
         ib=ib+nb
 30   continue
      return
      end





