      subroutine ddpowe(v,p,rr,ri,ierr,iscmpl)
c!purpose
c     computes v^p with p and v double precision
c!calling sequence
c     subroutine ddpowe(v,p,rr,ri,ierr,iscmpl)
c     integer ierr
c     double precision v,p,rr,ri
c
c     rr   : result's real part
c     ri   : result's imaginary part
c     ierr : error flag
c            ierr=0 if ok
c            ierr=1 if 0**0
c            ierr=2 if  0**k with k<0
c     iscmpl :
c            iscmpl=0 if result is real
c            iscmpl=1 if result is complex
c!origin
c Serge Steer INRIA 1996
c!
c     Copyright INRIA
      integer ierr
      double precision v,p,sr,si,rr,ri,infinity
c     
      ierr=0
      iscmpl=0
c     
      if(dble(int(p)).eq.p) then 
         call dipowe(v,int(p),rr,ierr)
         ri=0.0d0
      else
         if(v.gt.0.0d+0) then
            rr=v**p
            ri=0.0d0
         elseif(v.lt.0.0d+0) then
            call wlog(v,0.0d0,sr,si)
            sr=exp(sr*p)
            si=si*p
            rr=sr*cos(si)
            ri=sr*sin(si)
            iscmpl=1
         elseif(v.eq.0.d0) then
            if(p.lt.0.0d+0) then
               ri=0.0d0
               rr=infinity(ri)
               ierr=2
            elseif(p.eq.0.0d+0) then
c               ierr=1
               rr=1.0d0
               ri=0.0d0
            elseif(p.gt.0.d0) then
               rr=0.0d0
               ri=0.0d0
            else
c              p is nan
               rr = p
               ri = 0.d0
            endif
         else
c           v is nan
            rr = v
            ri = 0.d0
         endif
      endif
c     
      return
      end
