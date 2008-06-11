c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intl_e()
c =============================================================
c     extraction pour les list tlist, mlist
c =============================================================
c
c     Copyright INRIA
      include 'stack.h'
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
      if(rstk(pt).eq.404) goto 36


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
c     .        arg2(...)(i,j,..)(...)
               if(istk(il2i).ne.9) then
c     .            too many indices in index list
                   call error(21)
                   return
                endif
            endif
c     .     arg2(...)(i,j,..) a matrix single index,
c     .     use standard extraction to finish the job

c     .     copy index at the top of the stack
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
c     .  end of arg1 list (index list) reached
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
c     .     syntax is arg2.xxx
            fin=-fin
            top=top2
            rhs1=rhs
            return
         else
c     .     syntax is arg2(...).xxx(...)
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
c
c     standard matrix extraction
      call createref(il2i,0,vol2)
c
 34   fin=3
c     back to allops for  standard extraction
      if (ptover(1,psiz)) return
      icall=4
      ids(1,pt)=icount
      ids(2,pt)=m1
      ids(4,pt)=lhs

      rstk(pt)=405
c     *call* allops
      return
 35   continue
      icall=0

      if(err1.ne.0) then
         pt=pt-1
         return
      endif
      icount=ids(1,pt)
      m1=ids(2,pt)
      lhs=ids(4,pt)
      pt=pt-1
      if (m1.le.icount) then
c     .  nothing more to do here but to move results at its correct location
         top=top-2-lhs
         do ii=1,lhs
            top=top+1
            vol=lstk(top+3)-lstk(top+2)
            call unsfdcopy(vol,stk(lstk(top+2)),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+vol
         enddo
         fin=0
         return
      endif

c     m1 > icount handle, ... case
c     finish extraction using overloading

c     build new index list using the remaining entries of the
c     original index list
      ilind=iadr(lstk(top-2))
      if(istk(ilind).lt.0) ilind=iadr(istk(ilind+1))
      nlist=istk(ilind+1)
      ll=sadr(ilind+3+nlist)
      if(m1-icount.eq.1) then
         il1i=iadr(ll+istk(ilind+1+icount+1)-1)
         if (istk(il1i).ne.15) then
            vol=istk(ilind+2+icount+1)-istk(ilind+1+icount+1)
            call copyvar(il1i,vol)
            rhs=2
         else
c     .     transform index list(i,j,...) in the list in sequence of variables
c     .     at the top of the stack
            m1i=istk(il1i+1)
            ll1=sadr(il1i+m1i+3)
            call unsfdcopy(istk(il1i+m1i+2)-1,stk(ll1),1,
     $           stk(lstk(top+1)),1)
            do  k1=1,m1i
               top=top+1
               lstk(top+1)=lstk(top)+istk(il1i+2+k1)-istk(il1i+1+k1)
            enddo
            rhs=1+m1i
            m2i=istk(il2i+1)
         endif
      else
         do i=1,m1-icount
            ilindi=iadr(ll+istk(ilind+1+icount+i)-1)
            vol=istk(ilind+2+icount+i)-istk(ilind+1+icount+i)
            call copyvar(ilindi,vol)
         enddo
         if(m1-icount.gt.1) call mklist(m1-icount)
         rhs=2
      endif
c     create a reference on the handle
      call createref1(top-rhs+1)
      if (ptover(1,psiz)) return
      fun=0
      fin=3
      icall=4
      rstk(pt)=404
c     *call* allops
      return

 36   continue
      pt=pt-1
c     move results at its correct location
      vol=lstk(top+1)-lstk(top)
      call unsfdcopy(vol,stk(lstk(top)),1,stk(lstk(top-3)),1)
      top=top-3
      lstk(top+1)=lstk(top)+vol
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

