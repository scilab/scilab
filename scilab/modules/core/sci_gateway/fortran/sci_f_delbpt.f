      subroutine intdelbpt
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Francois VOGEL, May      2004 - Bug 718 fixed
c Francois VOGEL, January  2005 - Bug 1187 fixed
c Francois VOGEL, February 2005 - Request 156 fixed - delbpt()
c Francois VOGEL, February 2005 - Vector argument now allowed
c Bruno PINCON,   = = = = = = = - slight pbs of goto (most removed)
c Francois VOGEL, August   2007 - Bug 2474 fixed
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,getrvect,checkval,eqid
      integer topk
      logical dupbpt
c
      topk=top

      rhs=max(0,rhs)
      if(.not.checklhs('delbpt',1,1)) return
      if(.not.checkrhs('delbpt',0,2)) return

c check line number(s) argument
      if(rhs.eq.2) then
         if(.not.getrvect('delbpt',topk,top,m,n,lv)) return
         nbp=m*n
         do i=1,nbp
            lnb=stk(lv+i-1)
            if (dble(lnb).ne.stk(lv+i-1).or.lnb.lt.1) then
               buf='Breakpoint line number must be a positive integer'
               call error(9997)
               return
            endif
         enddo
c        silently remove duplicate line numbers in vector argument
         nbpnew = 1
         do i = 1, nbp-1
c           look if the bpt number stk(lv+i) is already in stk(lv+0:nbpnew-1)
            dupbpt = .false.
            j = 0
            do while ( .not.dupbpt .and. j.lt.nbpnew )
               dupbpt = stk(lv+i).eq.stk(lv+j)
               j = j+1
            enddo
            if ( .not.dupbpt ) then
               stk(lv+nbpnew) = stk(lv+i)
               nbpnew = nbpnew+1
            endif
         enddo
         nbp = nbpnew

         lnb=stk(lv)
         top=top-1
         if(nmacs.eq.0) goto 360
      endif

      if(rhs.gt.0) then
c        rhs is 1 or 2 (macro name is provided)
         if(.not.getsmat('delbpt',topk,top,m,n,1,1,l,n1)) return
         if(.not.checkval('delbpt',m*n,1) ) return
         call namstr(id,istk(l),n1,0)
         do kmac=1,nmacs
            if(eqid(macnms(1,kmac),id)) goto 355
         enddo
         goto 360 ! if the specified macro don't have breakpoint(s) or don't exist: return silently

      else ! rhs = 0: delete all bpt of all functions
         nmacs = 0
         lgptrs(1) = 0   ! est-ce nécessaire ?  
         top=top+1
         goto 360
      endif

 355  continue
      if (rhs.eq.1 ) then
c        all the breakpoints of the specified macro are removed
         if(kmac.lt.nmacs) then
            l0=lgptrs(kmac+1)
            call icopy(lgptrs(nmacs+1)-l0 ,bptlg(l0),1,
     $           bptlg(lgptrs(kmac)),1)
            do kk=kmac,nmacs-1
               call icopy(nsiz,macnms(1,kk+1),1,macnms(1,kk),1)
               lgptrs(kk+1)=lgptrs(kk)+lgptrs(kk+2)-lgptrs(kk+1)
            enddo
            lgptrs(nmacs+1)=0
         else
            lgptrs(nmacs+1)=0
         endif
         nmacs=nmacs-1

      else ! rhs = 2 here
c        only the specified breakpoint(s) is (are) removed
         nbpr=0
c        forget about bpts requested - nbpr keeps track of the
c        number of removed breakpoints
         do i=0,nbp-1
            lnb=stk(lv+i)
            kk1=lgptrs(kmac)-1
            do kk=lgptrs(kmac),lgptrs(kmac+1)-1-nbpr
               if(bptlg(kk).ne.lnb) then
                  kk1=kk1+1
                  bptlg(kk1)=bptlg(kk)
               else
                  nbpr=nbpr+1
               endif
            enddo
         enddo
c    if no match was found, return
         if(nbpr.eq.0) goto 360
c    shift end of breakpoints list - no hole allowed
         if(kmac.lt.nmacs) then
            l0=lgptrs(kmac+1)
            do kk=kmac+1,nmacs
               call icopy(lgptrs(kk+1)-l0,bptlg(l0),1,bptlg(l0-nbpr),1)
               l0=lgptrs(kk+1)
               lgptrs(kk)=lgptrs(kk)-nbpr
            enddo
         endif
         lgptrs(nmacs+1)=lgptrs(nmacs+1)-nbpr
         lgptrs(nmacs+2)=0
c    shift end of macro names array in case all the bpts
c    of a macro were removed - forget name of macros w/o bpt
         if(lgptrs(kmac+1).eq.lgptrs(kmac)) then
            if(kmac.lt.nmacs) then
               do kk=kmac,nmacs-1
                  call icopy(nsiz,macnms(1,kk+1),1,macnms(1,kk),1)
                  lgptrs(kk)=lgptrs(kk+1)
               enddo
            endif
            lgptrs(nmacs)=lgptrs(nmacs+1)
            lgptrs(nmacs+1)=0
            nmacs=nmacs-1
         endif
      endif

c the end...
 360  continue
      call objvide('delbpt',top)
      return
      end
