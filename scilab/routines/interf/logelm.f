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
c     1      2 
c   find   bool2s
c
c
c
      goto (10,20) fin
 10   call intsfind
      return
 20   call intsbool2s
      return
      end
c
      subroutine intsfind
      include '../stack.h'

      double precision tv
c
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

      if(istk(il1).eq.1) then
c     argument is a standard matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         l1=sadr(il1+4)
         l=l1
         if(mn1.gt.0) then
            do 11 k=0,mn1-1
               if(stk(l1+k).ne.0.0d0) then
                  stk(l)=float(k+1)
                  l=l+1
               endif
 11         continue

            nt=l-l1
         else
            nt=0
         endif
      elseif(istk(il1).eq.4) then
c     argument is a full boolean matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         il=max(il1+3+mn1,iadr(lstk(top)+mn1*lhs)+8)
         err=sadr(il+mn1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(mn1,istk(il1+3),1,istk(il),1)
         istk(il1)=1
         l1=sadr(il1+4)
         if(mn1.gt.0) then
            l=l1
            do 13 k=0,mn1-1
               if(istk(il+k).ne.1) goto 13
               stk(l)=float(k+1)
               l=l+1
 13         continue
            nt=l-l1
         else
            nt=0
         endif
      elseif(istk(il1).eq.6.) then
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
      istk(il1+1)=min(1,nt)
      istk(il1+2)=nt
      istk(il1+3)=0
      lstk(top+1)=l1+nt
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
         stk(l2+k)=float(int((stk(l1+k)-1.0d0)/m1)+1)
         stk(l1+k)=stk(l1+k)-(stk(l2+k)-1.0d+0)*m1
 15   continue
      goto 999
c
 20   continue
c     sparse matrix find

      m1=istk(il1+1)
      n1=istk(il1+2)
      nel1=istk(il1+4)
c
      li=sadr(il1+4)
      ilj=iadr(li+nel1)
      lj=sadr(ilj+4)
      lw=max(lw,lj+nel1)
      ilr=iadr(lw)
      lw=sadr(ilr+m1+nel1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(m1+nel1,istk(il1+5),1,istk(ilr),1)
      call int2db(nel1,istk(ilr+m1),1,stk(lj),1)
      i1=0
      do 30 i=0,m1-1
         if(istk(ilr+i).ne.0) then
            tv=i+1
            call dset(istk(ilr+i),tv,stk(li+i1),1)
            i1=i1+istk(ilr+i)
         endif
 30   continue
      istk(il1)=1
      istk(il1+1)=1
      istk(il1+2)=nel1
      istk(il1+3)=0
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
      if(istk(il1).lt.0) il1=istk(il1+1)
      mn1=istk(il1+1)*istk(il1+2)
      ilr=iadr(lstk(top))
      lr=sadr(ilr+4)
      err=lr+mn1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(istk(il1).eq.4) then
c     argument is a full boolean matrix
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
         call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
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
            do 20 k=mn1-1,0,-1
               if(stk(lr+k).ne.0.0d0) then
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
         call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
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


