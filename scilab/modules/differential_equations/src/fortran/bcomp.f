c     ====================================       
      subroutine bcomp (neq, t, x, xdot)
c     ====================================       
C      implicit undefined (a-z)
      double precision t, x, xdot
      dimension x(4), xdot(4)
      real br(1000),pas,ppr,ppk,ppa,ppb,ppm,pps,ppl,
     $     h(2),l(2),k(2),xe(2),f(2,2),g(2),ue
      integer nn 
      common / bcompc/ br,pas,ppr,ppk,ppa,ppb,ppm,pps,ppl,
     $     h,l,k,xe,f,g,ue,nn
      real u ,brui,y
      integer nnn,neq
      
      nnn=int(min(1+t/pas,dble(nn)))
      
      if ( nnn.le.0 .or. nnn.gt.nn) then 
         write(06,*) ' depassement ds vbruit'
      endif
      
      brui=br(nnn)
      u=ue- k(1)*(x(3)-xe(1))-k(2)*(x(4)-xe(2))
      y=h(1)*x(1)+h(2)*x(2)+brui
C     xdot=compet(t,x,u) composantes 1 et 2 
      xdot(1) = ppr*x(1)*(1-x(1)/ppk) - u*ppa*x(1)*x(2)
      xdot(2) = pps*x(2)*(1-x(2)/ppl) - u*ppb*x(1)*x(2)
C     observer 
      xdot(3) = f(1,1)*(x(3)-xe(1))+f(1,2)*(x(4)-xe(2))
      xdot(4) = f(2,1)*(x(3)-xe(1))+f(2,2)*(x(4)-xe(2))
      xdot(3) = xdot(3) + g(1)*(u-ue) - l(1)*(h(1)*x(3)+h(2)*x(4)-y)
      xdot(4) = xdot(4) + g(2)*(u-ue) - l(2)*(h(1)*x(3)+h(2)*x(4)-y)
      return
      end 
c     ====================================       

