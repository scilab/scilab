      subroutine powblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     rpar(1) is power
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c     
      if (nrpar.eq.1) then
         do 15 i=1,nu
            if(u(i).lt.0.d0) then
               if (flag.ge.4) return
               flag=-2
               return
            elseif(u(i).eq.0.d0.and.rpar(1).le.0) then
               if (flag.ge.4) return
               flag=-2
               return
            endif
            y(i)=u(i)**rpar(1)
 15      continue
      else
         do 25 i=1,nu
            if(ipar(1).le.0.and.u(i).eq.0.0d0) then
               if (flag.ge.4) return  
               flag=-2
               return
            endif
            y(i)=u(i)**ipar(1)
 25      continue
      endif
      
      end
