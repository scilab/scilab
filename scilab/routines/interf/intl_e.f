      subroutine intl_e()
c =============================================================
c     extraction pour les list tlist, mlist
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'
c
c
      integer rhs1,vol1,vol2,vol
      integer op,top1,top2

      integer strpos
      external strpos
      logical ptover
      integer id(nsiz)
      integer iadr,sadr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      icall=0
c
      if(rstk(pt).eq.403) goto 45
      if(rstk(pt).eq.405) goto 35


      rhs1=rhs
      top0=top

      fun=0   
      if(rhs.gt.2) then
         fin=-fin
         return
      endif

      if(rhs.le.0) then
         call error(39)
         return
      endif
      icount=0

c     arg2(arg1)
c     ==========

c     get arg2
 10   continue
      lw=lstk(top+1)
c
      top2=top
c     get arg1
      top=top-1
      top1=top
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))

      if(istk(il1).eq.15.and.istk(il1+1).eq.0) then
c     .  arg2(list())  -->arg2
         il2=iadr(lstk(top2))
         if(istk(il2).lt.0) top2=istk(il2+2)
         call unsfdcopy(lstk(top2+1)-lstk(top2),stk(lstk(top2)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top2+1)-lstk(top2)
         return
      endif

c
      ilind=iadr(lw)

      if(istk(il1).ne.15) then
         lw=lw+1
         m1=1
      else
         m1=istk(il1+1)
         lw=sadr(ilind+m1)
      endif
c     go ahead along the path
      call followpath(top1,top2,il1ir,vol1,il2ir,vol2,istk(ilind),
     $     icount,info,lw)
      if(err.gt.0) return
      if(vol2.eq.0) then
c     empty field found
         err=istk(ilind-1+icount)
         call error(117)
         return
      endif

      il1i=il1ir
      if(istk(il1i).lt.0) il1i=iadr(istk(il1i+1))
      il2i=il2ir
      if(istk(il2i).lt.0) il2i=iadr(istk(il2i+1))
      if(info.eq.2.or.info.eq.4) then
c     .  syntax is arg2(...)(i,j,..)(...)
c     .  matrix index (i,..) syntax
         lstk(top+2)=lw
         if (istk(il1i).eq.15) then
c     .     more than one index
c     .     transform index list(i,j,...) in the list in sequence of variables 
c     .     at the top of the stack
            m1i=istk(il1i+1)
            ll1=sadr(il1i+m1i+3)
            top=top2+1
            call unsfdcopy(istk(il1i+m1i+2)-1,stk(ll1),1,
     $           stk(lstk(top)),1)
            do 16 k1=1,m1i
               lstk(top+1)=lstk(top)+istk(il1i+2+k1)-istk(il1i+1+k1)
               top=top+1
 16         continue
            top=top-1
            rhs=1+m1i
            m2i=istk(il2i+1)
         else
c     .     a matrix  index 
            if(m1.gt.icount.and.info.eq.4) then
c     .        arg2(...)(i,j,..)(...) :too many indices in index list
               call error(21)
               return
            endif
c     .     arg2(...)(i,j,..) a matrix single index, use standard extraction to finish the job
c     .     copy it at the top of the stack (may possible to put a pointer)
            top=top2
            call copyvar(il1ir,vol1)
            rhs=2
         endif
         goto 30
      elseif(info.eq.3) then
c     .  current list index is not a single index (multiple extraction)
         if(icount.ne.m1) then
c     .     only final index handled
            call error(43)
            return
         endif
         if(istk(il1i).eq.10) then
            if(istk(il2i).ne.16.and.istk(il2i).ne.17) then
               call error(21)
               return
            endif
c     .  named indexes
            ilfn=iadr(sadr(il2i+istk(il2i+1)+3))
            nn=istk(ilfn+1)*istk(ilfn+2)
            ilptr=ilfn+5
            lfn=ilfn+5+nn
            ili=iadr(lw)
            lw=sadr(ili+nn)
            nind=istk(il1i+1)*istk(il1i+2)
            do 20 k=1,nind
               nname=istk(il1i+4+k)-istk(il1i+3+k)
               ilname=il1i+4+nind+istk(il1i+3+k)
c     .        look for corresponding index if any
               ipos=strpos(istk(ilptr),nn-1,istk(lfn),istk(ilname),
     $              nname)
               if(ipos.le.0) then
c     .           no such name in the field names 
                  if(icount.eq.1.and.m1.eq.1) then
c     .              syntax is arg2('xxx')
                     fin=-fin
                     top=top2
                     rhs1=rhs
                     return
                  else
c     .              syntax is arg2(...)('xxx')(...)
c     .              set the index
                     top=top2
                     call copyvar(il1ir,vol1)
                     rhs=2
                     goto 30
                  endif
               else
                  istk(ili+k-1)=ipos+1
               endif
 20         continue
            call extractfields(il2i,istk(ili),nind,lw)
         else
            m2i=istk(il2i+1)
            call indxg(il1i,m2i,ili,n,mx,lw,10)
            call extractfields(il2i,istk(ili),n,lw)
         endif
         return
      elseif(info.eq.5) then
c     .  end of arg1 list reached 
c     .  copy the designed sublist at the top of the stack
         top=top-1
         call copyvar(il2ir,vol2)
         return
      elseif(info.eq.6) then
c     .  index is out of bounds
         call error(21)
         return
      elseif(info.eq.1) then
c     .  current index is a name which is not an explicit field name
         if(icount.eq.1.and.m1.eq.1) then
c     .     syntax is arg2('xxx')
            fin=-fin
            top=top2
            rhs1=rhs
            return
         else
c     .     syntax is arg2(...)('xxx')(...)
c     .     set the index
            top=top2
            call copyvar(il1ir,vol1)
            rhs=2
            goto 30
         endif
      endif


 30   continue
c     escape from standard list algorithm to handle  special cases:
c     (matrix/vector extraction, method..)

      if(istk(il2i).ge.15.and.istk(il2i).le.17) goto 40

c     standard matrix extraction
      call createref(il2i,0,vol2)
c
 34   fin=3
c     back to allops for  standard extraction
      if (ptover(1,psiz)) return
      icall=4
      rstk(pt)=405
c     *call* allops
      return
 35   continue
      icall=0
c     move results at its correct location
      if(err1.ne.0) then
         pt=pt-1
         return
      endif
      vol=lstk(top+1)-lstk(top)
      call unsfdcopy(vol,stk(lstk(top)),1,stk(lstk(top-2)),1)
      top=top-2
      lstk(top+1)=lstk(top)+vol
      pt=pt-1
c     nothing more to do here
      fin=0
      return

 40   continue
c     list mlist or tlist coded matrix extraction or other method
c     .  form the sublist
      call copyvar(il2i,vol2)


      if (ptover(1,psiz)) return
      ids(1,pt)=icount
      ids(2,pt)=m1
      ids(4,pt)=lhs
c     only last index may select many lhs elements
      if (icount.ne.m1) lhs=1
      fun=0
      fin=-fin
      rstk(pt)=403
c     *call* macro or matfns
      return

 45   continue
c     restore context
      if(err1.ne.0) then
         pt=pt-1
         return
      endif
      icount=ids(1,pt)
      m1=ids(2,pt)
      lhs=ids(4,pt)
      fin=3
      pt=pt-1
      if(icount.eq.m1) then
c     .  put the result at the top of the stack and return
         vol=lstk(top+1)-lstk(top-lhs+1)
         im=lstk(top-lhs+1)-lstk(top-lhs-1)
        call unsfdcopy(vol,stk(lstk(top-lhs+1)),1,
     $        stk(lstk(top-lhs-1)),1) 
         top=top-2
         do 46 k=1,lhs
            lstk(top-lhs+k+1)=lstk(top-lhs+k+3)-im
 46      continue
         return
      else
c     .  move result ajust after the index
         ll2=lstk(top+1)-lstk(top)
         call unsfdcopy(ll2,stk(lstk(top)),1,stk(lstk(top-1)),1) 
         lstk(top)=lstk(top-1)+ll2
         top=top-1
c     .  goto back to the standard list algorithm
         goto 10
      endif
c
      end

      subroutine forcerhs(n,ierr)
c     this routine allows to change the apparent lhs of a list
c     extraction when it extraction appears in an input argument
c     list. The lhs is changed  from 1 to the number of extracted
c     variables.
c
      integer n,ierr
      include '../stack.h'

      ierr=0
c      write(6,'(''forcerhs'',i5)') n
      if(rstk(pt-1).eq.617) then
c      in a compiled function
         pstk(pt-1)=pstk(pt-1)+n-1
      elseif(rstk(pt-1).eq.501) then
c      in a compiled function retained for 2.4.1  compatiblity
         ids(5,pt-1)=ids(5,pt-1)+n-1
      elseif(pt.ge.4) then
         if(rstk(pt-3).eq.307.or.rstk(pt-3).eq.702) then
            pstk(pt-3)=pstk(pt-3)-1+n
         else
            ierr=1
         endif
      else
         ierr=1
      endif
      return
      end

      subroutine extractfields(illist,ind,n,lw)
c     Copyright INRIA
c
c     given a list beginning at istk(illist), a vector of n indexes ind 
c     extractfields creates n variables corresponding to the n fields
c
c     if n==-1 all fieds are extracted
c

      include '../stack.h'
      integer ind(*)
c     
      integer vol2
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if(n.eq.-1) then
c     .  L(:)
         n=istk(illist+1)
         llist=sadr(illist+3+n)
c     .  Check lhs
         if(lhs.le.1) then
            if(n.ne.lhs) then
               call forcerhs(n,ierr)
               if(ierr.ne.0) then
                  call error(41)
                  return
               endif
            endif
         else
            if(n.ne.lhs) then
               call error(41)
               return
            endif
         endif
c     .  check if it is possible to create n new variables
         if(top+1+n.ge.bot) then
            call error(18)
            return
         endif
c     .  create table of pointers on new variables
         do  31 i=1,n
            vol2=istk(illist+2+i)-istk(illist+1+i)
            if(vol2.eq.0) then
               err=i
               call error(117)
               return
            endif
            lstk(top+1)=lstk(top)+vol2
            top=top+1
 31      continue
         top=top-1
c     .  copy the fields
         l=lstk(top-n+1)
         call unsfdcopy(istk(illist+n+2)-1,stk(llist),1,stk(l),1)
      else
c     .  L([....])
         llist=sadr(illist+3+istk(illist+1))
         if(n.ne.lhs) then
            call forcerhs(n,ierr)
            if(ierr.ne.0) then
               call error(41)
               return
            endif
         endif
c     .  check if it is possible to create n new variables
         if(top+n+1.ge.bot) then
            call error(18)
            return
         endif
c     .  create table of pointers on new variables
         do  52 i=1,n
            k=ind(i)
            if(k.le.0.or.k.gt.istk(illist+1)) then
               call error(21)
               return
            endif
            vol2=istk(illist+2+k)-istk(illist+1+k)
            if(vol2.eq.0) then
               err=k
               call error(117)
               return
            endif
            lstk(top+1)=lstk(top)+vol2
            top=top+1
 52      continue
         top=top-1

c     .  preserve fields adress (newly created variables may 
c     .  overwrite beginining of the given list)
         ill=iadr(max(lw,lstk(top+1)))
         lw=sadr(ill+n)
         err=sadr(ill+n)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         do 54 i=1,n
            k=ind(i)
            istk(ill-1+i)=istk(illist+1+k)+llist-1
 54      continue
c     .  copy the fields
         do 55 i=1,n
            k=top-n+i
            call unsfdcopy(lstk(k+1)-lstk(k),stk(istk(ill-1+i)),1,
     $           stk(lstk(k)),1)
 55      continue
      endif
      return
      end
