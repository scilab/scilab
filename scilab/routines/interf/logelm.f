      subroutine logelm
c ================================== ( Inria    ) =============
c evaluation des fonctions elementaires sur les booleens
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'

c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' logelm '//buf(1:4))
      endif

c     functions/fin
c     1      2     3    4
c   find   bool2s  or  and
c
c
c
      goto (10,20,30,40) fin
 10   call intsfind
      return
 20   call intsbool2s
      return
 30   call intor('or')
      return
 40   call intand('and')
      end
c
      subroutine intsfind
      include '../stack.h'

      double precision tv
c
      logical ref
      integer sadr,iadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
c

   10 if(rhs.ne.1) then
         call error(39)
         return
      endif
      if(lhs.gt.2) then
         call error(39)
         return
      endif

      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).eq.1) then
c     argument is a standard matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         l1=sadr(il1+4)
         if(ref) then
            err=sadr(ilr+4)+mn1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il1),1,istk(ilr),1)
         endif
         lr=sadr(ilr+4)
         l=lr
         if(mn1.gt.0) then
            do 11 k=0,mn1-1
               if(stk(l1+k).ne.0.0d0) then
                  stk(l)=float(k+1)
                  l=l+1
               endif
 11         continue
            nt=l-lr
         else
            nt=0
         endif
      elseif(istk(il1).eq.4) then
c     argument is a full boolean matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         if(.not.ref) then
            il=max(il1+3+mn1,iadr(lstk(top)+mn1*lhs)+8)
            err=sadr(il+mn1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(mn1,istk(il1+3),1,istk(il),1)
         else
            il=il1+3
         endif
         istk(ilr)=1
         lr=sadr(ilr+4)
         if(mn1.gt.0) then
            l=lr
            do 13 k=0,mn1-1
               if(istk(il+k).ne.1) goto 13
               stk(l)=float(k+1)
               l=l+1
 13         continue
            nt=l-lr
         else
            nt=0
         endif
      elseif(istk(il1).eq.6) then
c     argument is a sparse boolean matrix
         goto 20
      elseif(istk(il1).eq.5) then
c     argument is a sparse  matrix
         goto 20
      else
         call putfunnam('find',top)
         fun=-1
         return
      endif
      istk(ilr+1)=min(1,nt)
      istk(ilr+2)=nt
      istk(ilr+3)=0
      lstk(top+1)=lr+nt
      if(lhs.eq.1) goto 999
      top=top+1
      il2=iadr(lstk(top))
      istk(il2)=1
      istk(il2+1)=min(1,nt)
      istk(il2+2)=nt
      istk(il2+3)=0
      l2=sadr(il2+4)
      lstk(top+1)=l2+nt
      if(nt.eq.0) goto 999
      do 15 k=0,nt-1
         stk(l2+k)=float(int((stk(lr+k)-1.0d0)/m1)+1)
         stk(lr+k)=stk(lr+k)-(stk(l2+k)-1.0d+0)*m1
 15   continue
      goto 999
c
 20   continue
c     sparse matrix find
      m1=istk(il1+1)
      n1=istk(il1+2)
      nel1=istk(il1+4)
c

      if(.not.ref) then
         li=sadr(il1+4)
         ilj=iadr(li+nel1)
         lj=sadr(ilj+4)
         lw=max(lw,lj+nel1)
         ilr1=iadr(lw)
         lw=sadr(ilr1+m1+nel1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m1+nel1,istk(il1+5),1,istk(ilr1),1)
         call int2db(nel1,istk(ilr1+m1),1,stk(lj),1)
      else
         li=sadr(ilr+4)
         ilj=iadr(li+nel1)
         lj=sadr(ilj+4)
         lw=max(lw,lj+nel1)
         ilr1=il1+5
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call int2db(nel1,istk(ilr1+m1),1,stk(lj),1)
      endif
      i1=0
      do 30 i=0,m1-1
         if(istk(ilr1+i).ne.0) then
            tv=i+1
            call dset(istk(ilr1+i),tv,stk(li+i1),1)
            i1=i1+istk(ilr1+i)
         endif
 30   continue
      istk(ilr)=1
      istk(ilr+1)=1
      istk(ilr+2)=nel1
      istk(ilr+3)=0
      lstk(top+1)=li+nel1
      if(lhs.eq.1) then
         do 31 i=0,nel1-1
            stk(li+i)=stk(li+i)+(stk(lj+i)-1.0d0)*m1
 31      continue
      else
         top=top+1
         istk(ilj)=1
         istk(ilj+1)=1
         istk(ilj+2)=nel1
         istk(ilj+3)=0
         lstk(top+1)=lj+nel1
      endif
      goto 999
         
c
  999 return
      end

      subroutine intsbool2s
      include '../stack.h'

      logical ref
      integer sadr,iadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
c

   10 if(rhs.ne.1) then
         call error(39)
         return
      endif
      if(lhs.ne.1) then
         call error(39)
         return
      endif

      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      mn1=istk(il1+1)*istk(il1+2)

      if(istk(il1).eq.4) then

c     argument is a full boolean matrix
         lr=sadr(ilr+4)
         err=lr+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         do 13 k=mn1-1,0,-1
            stk(lr+k)=istk(il1+3+k)
 13      continue
         istk(ilr)=1
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         lstk(top+1)=lr+mn1
      elseif(istk(il1).eq.6) then
c     argument is a sparse boolean matrix
         m1=istk(il1+1)
         n1=istk(il1+2)
         nel1=istk(il1+4)
c   
         if(ref) then
            err=sadr(ilr+5+m1+nel1)+nel1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
         endif
         lj=sadr(ilr+5+m1+nel1)
         call dset(nel1,1.0d0,stk(lj),1)
         istk(ilr)=5
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         istk(ilr+4)=nel1
         lstk(top+1)=lj+nel1
      elseif(istk(il1).eq.1) then
         if(mn1.eq.0) then
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=lr
         else
            l1=sadr(il1+4)
            lr=sadr(ilr+4)
            if(ref) then
               err=lr+mn1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            do 20 k=mn1-1,0,-1
               if(stk(l1+k).ne.0.0d0) then
                  stk(lr+k)=1.0d0
               else
                  stk(lr+k)=0.0d0
               endif
 20         continue
            istk(ilr)=1
            istk(ilr+1)=istk(il1+1)
            istk(ilr+2)=istk(il1+2)
            istk(ilr+3)=0
            lstk(top+1)=lr+mn1
         endif
      elseif(istk(il1).eq.5) then
c     argument is a sparse matrix
         m1=istk(il1+1)
         n1=istk(il1+2)
         nel1=istk(il1+4)
c
         if(ref) then
            err=sadr(ilr+5+m1+nel1)+nel1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
         endif
         lj=sadr(ilr+5+m1+nel1)
         call dset(nel1,1.0d0,stk(lj),1)
         istk(ilr)=5
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         istk(ilr+4)=nel1
         lstk(top+1)=lj+nel1
      else
         call putfunnam('bool2s',top)
         fun=-1
         return
      endif
      end

      
