      subroutine indxg(il,siz,ilr,mi,mx,lw,iopt1)
c!Purpose
c     Converts a scilab index variable to a vector of indices
c!Calling sequence
c     subroutine indxg(il,siz,ilr,mi,lw,iopt)
c     il    : beginning of a  a scilab variable structure. 
c     siz   : integer, matrix size, used for implicits index descriptions
c     ilr   : adress of first elment of resulting vector of indices in
c            istk
c     mi    : size of resulting vector of indices 
c     mx    : maximum value of resulting vector of indices
c     lw    : pointer to free space in stk (modified by execution)
c     iopt1 : flag with decimal form n+10*i
c            if n==0 null indices are accepted
c            else null indices are rejected
c            if i==0 
c               implicit indices ":" gives a vector istk(ilr)=1:siz, mi=siz,mx=siz
c            else 
c               implicit indice ":" gives mi=-1,mx=siz
c!

c     Copyright INRIA
      include '../stack.h'
      integer siz,iopt1,iopt
      double precision e1,v(3)
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c
c     
      impl=iopt1/10
      iopt=iopt1-10*impl
c
      if(istk(il).lt.0) il=istk(il+1)

      if(istk(il).eq.1.or.istk(il).eq.8) then
c     Index is a vector of scalars
         m=istk(il+1)
         n=istk(il+2)
 
         if(m.ge.1) then
c     .     general case
            l=sadr(il+4)
            ilr=iadr(lw)
            lw=sadr(ilr+m*n)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            if (istk(il).eq.1) then
               if(istk(il+3).ne.0) then 
                  call error(21)
                  return
               endif
               call entier(m*n,stk(l),istk(ilr))
            else
               call tpconv(istk(il+3),4,m*n,istk(il+4),1,istk(ilr),1)
            endif
            mi=m*n
            mx=0
            do 05 i=0,m*n-1
               if(iopt.eq.1.and.istk(ilr+i).le.0) then
                  call error(21)
                  return
               else
                  mx=max(mx,istk(ilr+i))
               endif
 05         continue
         elseif(m.eq.0) then
c     .     index is []
            ilr=il
            mi=0
            mx=0
         elseif(m.eq.-1) then
c     .     index is : 
            ilr=iadr(lw)
            if(impl.eq.0) then
               if(siz.gt.0) then
                  lw=sadr(ilr+siz)
                  err=lw-lstk(bot)
                  if(err.gt.0) then
                     call error(17)
                     return
                  endif
                  do 10 i=1,siz
                     istk(ilr-1+i)=i
 10               continue
               endif
               mi=siz
            else
               mi=-1
            endif
            mx=siz
         endif
      elseif (istk(il).eq.2) then
c     .  Index is a vector of polynomial
         m=istk(il+1)
         n=istk(il+2)
         if(istk(il+3).ne.0) then 
            call error(21)
            return
         endif
         mi=m*n
         l=sadr(il+9+mi)
         lr=lw
         ilr=iadr(lr)
         lw=lr+mi
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c     .  evaluate it for siz
         e1=siz
         call ddmpev(stk(l),istk(il+8),1,e1,stk(lr),1,1,mi)
         call entier(mi,stk(lr),istk(ilr))
         lw=sadr(ilr+mi)
         mx=0
         do 15 i=0,mi-1
            if(istk(ilr+i).le.0) then
               call error(21)
               return
            else
               mx=max(mx,istk(ilr+i))
            endif
 15      continue
      elseif (istk(il).eq.129) then
c     .  Index is an implicit polynomial vector (beg:step:end)
         e1=siz
         l=sadr(il+12)
         
         call ddmpev(stk(l),istk(il+8),1,e1,v,1,1,3)
         ideb=v(1)
         ipas=v(2)
         ifin=v(3)
c     sign used to avoid integer overflow
         if(ipas.eq.0.or.(ifin-ideb)*sign(1,ipas).lt.0) then
            mi=0
            mx=0
         else
            if(ipas.lt.0.and.ifin.le.0.or.ipas.gt.0.and.ideb.le.0) then
               call error(21)
               return
            endif
            mi=int((abs(ifin-ideb)+1)/abs(ipas))

            ilr=iadr(lw)
            lw=sadr(ilr+mi+1)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            k=0
            do 20 i=ideb,ifin,ipas
               istk(ilr+k)=i
               k=k+1
 20         continue
            mi=k
            if(ipas.gt.0) then
               mx=istk(ilr-1+mi)
            else
               mx=istk(ilr)
            endif
         endif
      elseif (istk(il).eq.4) then
c     .  index is a boolean vector
         m=istk(il+1)
         n=istk(il+2)
c         if(m*n.ne.siz) then 
c            call error(21)
c            return
c         endif
         ilr=iadr(lw)
         lw=sadr(ilr+m*n)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         mi=0
         do 30 i=1,m*n
            if(istk(il+2+i).eq.1) then
               istk(ilr+mi)=i
               mi=mi+1
            endif
 30      continue
         if(mi.eq.0) then
            mx=0
         else
            mx=istk(ilr-1+mi)
         endif
         lw=sadr(ilr+mi)
      elseif (istk(il).eq.6) then
c     .  index is a boolean vector
         m=istk(il+1)
         n=istk(il+2)
         nel=istk(il+4)

         ir=il+5
         ic=ir+m
         ilr=iadr(lw)
         ilw=ilr+nel
         mx=nel
         mi=0
         if (nel.gt.0) then
            lw=sadr(ilw+nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            do 35 i=0,m-1
               if(istk(ir).gt.0) then
                  do 31 kk=0,istk(ir)-1
                     istk(ilr+mi)=1+i+(istk(ic+kk)-1)*m
                     mi=mi+1
 31               continue
                  ic=ic+istk(ir)
               endif
               ir=ir+1
 35         continue
            call isort1(istk(ilr),nel,istk(ilw),1)
         endif
         lw=sadr(ilr+nel)
      else
         call error(21)
         return
      endif
      return
      end


      subroutine indxgc(il,siz,ilr,mi,mx,lw)
c!Purpose
c     Converts a scilab index variable to the complementary vector of indices
c!Calling sequence
c     subroutine indxg(il,siz,ilr,mi,lw)
c     il   : beginning of a scilab variable structure. 
c     siz  : integer, matrix size, used for implicits index descriptions
c     ilr  : adress of first elment of resulting vector of indices in
c            istk
c     mi   : size of resulting vector of indices 
c     mx   : maximum value of resulting vector of indices
c     lw   : pointer to free space in stk (modified by execution)
c!

*     modification by Bruno so as to use a faster algorithm (7 May 2002)

      implicit none
      include '../stack.h'
      integer il, siz, ilr, mi, mx, lw

      integer i, k, ilc
      
      integer l, iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      call indxg(il,siz,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      ilc=iadr(lw)
      lw=sadr(ilc+siz)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(mi.eq.0) then
         do i=1,siz
            istk(ilc+i-1)=i
         enddo
         mx=istk(ilc+siz-1)
         mi=siz
      else

*     computes complement (part of the code modified by Bruno)
*
*     given 
*       1/ a "vector" w of mi indices stored from istk(ilr)
*          so that w=[istk(ilr), ....., istk(ilr+mi-1)] is this vector
*       2/ the "vector" v of indices v=[1,2,..., siz]
*
*     computes the vector v minus w
*     this new vector is stored from istk(ilc) and its final number of
*     components will be stored in mi

         do i = 0, siz-1
            istk(ilc+i) = 1
         end do

         do i = 0, mi-1
            k = istk(ilr+i)
            if (k .le. siz) istk(ilc+k-1) = 0
         end do
            
         k = 0
         do i = 1, siz
            if (istk(ilc+i-1) .eq. 1) then
               istk(ilc+k) = i
               k = k+1
            end if
         end do
         
         mi = k
         mx=istk(ilc-1+k)
      endif
      ilr=ilc
      lw=sadr(ilr+mi)

      return
      end
