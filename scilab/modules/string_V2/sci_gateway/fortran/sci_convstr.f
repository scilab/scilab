c =========================================
c INRIA
c =========================================  
      subroutine intconvstr(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz)
      logical ref,lgq,cremat
      integer tops
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.2.or.rhs.lt.1) then
         call error(39)
         return
      endif

c
      tops=top
c
      if(rhs.eq.2) then
c     conversion flag
         il1=iadr(lstk(top))
         if(istk(il1).lt.0) il1=iadr(istk(il1+1))
         if(istk(il1).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(il1+1).ne.1.or.istk(il1+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(abs(istk(il1+6)).eq.30) then
            lgq=.true.
         elseif(abs(istk(il1+6)).ne.21) then
            err=2
            call error(36)
            return
         else
            lgq=.false.
         endif
         top=top-1
      else
         lgq=.false.
      endif
c
   51 continue
      il1=iadr(lstk(top))
      
      il=il1
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.10) then
         if(istk(il1).eq.1) then
            if(istk(il+1)*istk(il+2).eq.0) then
c              convstr([])
               lgq=cremat('convstr', top, 0, 0, 0, lr, lc)
               return
            endif
         endif
         top=tops
         fun=-1
         call funnam(ids(1,pt+1),'convstr',il)
         return
      endif

      if(istk(il1).lt.0) then
         rhs=1
         call ref2val
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      if (lgq) goto 53
c
c conversion en minuscule
c
      k=iadr(lstk(top+1))-1
      do 52 i=il1+5,k
         if(istk(i).lt.-35.or.istk(i).gt.-10) goto 52
         istk(i)=-istk(i)
   52 continue
      goto 999
c
c conversion en majuscule
c
   53 continue
      k0=il1+4
      k =k0+m1*n1
      do 55 i=1,m1*n1
        k1=istk(k0+1)-istk(k0)
        k0=k0+1
        do 54 l=1,k1
          k=k+1
          if(istk(k).lt.10.or.istk(k).gt.35) goto 54
          istk(k)=-istk(k)
   54   continue
   55 continue
      goto 999
c
 999  return
      end
c =========================================  
