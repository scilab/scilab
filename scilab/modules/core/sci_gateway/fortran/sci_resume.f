c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intresume
      include 'stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      parameter (iif=1,iwhile=2,iselect=3)
      integer pt0,count,r
      integer semi,comma,eol
      integer resume(nsiz)
c
      data resume/505155099,673713686,nz2*673720360/
      data eol/99/,semi/43/
      data comma/52/
c
      call ref2val
      if(rhs.ne.lhs) then
         call error(41)
         return
      endif
c
      count=0
      pt0=pt+1
 151  pt0=pt0-1
      if(pt0.le.0) return
         
      if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &     (rstk(pt0).eq.805.and.ids(1,pt0).eq.iselect).or.
     &     (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1
      if(int(rstk(pt0)/100).ne.5) goto 151
c
      if (rstk(pt0).eq.501) then
c     resume in a compiled macro
         lc=pstk(pt)
         ids(1,pt0+1)=lc
         pstk(pt0+2)=count
         l=lc+3
         do i=1,lhs
            if(istk(l+nsiz).ne.0) then
               call error(79)
               return
            endif
            l=l+nsiz+1
         enddo
      elseif(rstk(pt0).eq.502) then
c     resume in an uncompiled macro or an exec or an execstr
         if(rstk(pt0-1).eq.903.or.rstk(pt0-1).eq.706) then
c     .  in an execstr, check execstr calling context
            ip0=pt0+1
c            pt0=pt0-2
 153        pt0=pt0-1
            if(pt0.le.0) return
            if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &           (rstk(pt0).eq.805.and.ids(1,pt0).eq.iselect).or.
     &           (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1
            if(rstk(pt0).lt.501.or.rstk(pt0).gt.503) goto 153
c     .     check against syntax like [a,b(1,2)]=resume(3,1)

            if(rstk(pt0).eq.503.and.rio.eq.rte.and.paus.ne.0) then
c     .     resume called by execstr under pause
c     .        check against syntax like [a,b(1,2)]=resume(3,1)
               do  i=1,rhs
                  if (pstk(ip0+rhs-i).ne.0) then
                     buf='Index not allowed in the lhs of resume'
                     call error(997)
                     return
                  endif
               enddo

               mrhs=rhs
               rhs=0

c     .        remove execstr context 
               k=lpt(1)-(13+nsiz)
               lpt(1)=lin(k+1)
               macr=macr-1

c     .        remove pause context temporarily to save variables in the
c     .        calling context
               k=lpt(1)-(13+nsiz)
               lpt1=lpt(1)
               lpt(1)=lin(k+1)
               bot=lin(k+5)
               paus=paus-1
c     .        save resumed variables in the calling context
               do 154 i=1,mrhs
                  call stackp(ids(1,ip0+mrhs-i),0)
 154           continue
c     .        recreate  pause context (the pause will be finished  normally)
               lpt(1)=lpt1
               paus=paus+1
               lin(k+5)=bot
c     .        remove top variables associated with unstacked contexts  (for, select,..)
               top=top-count
               pt=pt0
               goto 999
            elseif(paus.ne.0.and.rstk(pt0).eq.201) then
c     .        ???
               r=rstk(pt0-4)
               if (r.eq.701.or.r.eq.604) goto 156
            endif

c    .      resume in an execstr, simulate a resume in the calling macro
c    .      see macro.f code for details 
            k = lpt(1) - (13+nsiz)
            lpt(1)=lin(k+1)
            macr=macr-1

c     .     get location of lhs var names
            lvar=pt-3
            rstk(pt0)=504
            pstk(pt0+1)=lvar
            pstk(pt0+2)=count+1
         else
c     .     resume in an uncompiled macro or syncexec
            if(rstk(pt-1).ne.201
     &           .or.rstk(pt-2).ne.101
     &           .or.rstk(pt-3).ne.703
     &           .or.(sym.ne.semi.and.sym.ne.comma.and.sym.ne.eol)) 
     &           goto 156
            pt=pt-3
c     .     check against syntax like [a,b(1,2)]=resume(3,1)
            do  i=1,rhs
               if (pstk(pt+1-i).ne.0) then
                  call error(79)
                  return
               endif
            enddo
            if (rstk(pt0-1).eq.1002) then
c     .        syncexec case
               mrhs=rhs
               rhs=0
               do  i=1,mrhs
                  call stackp(ids(1,pt),0)
                  pt=pt-1
               enddo
            endif
            pstk(pt0+1)=pt
            pstk(pt0+2)=count
         endif
      elseif(rio.eq.rte) then
c     resume in a pause
         if(rstk(pt-1).ne.201
     &        .or.rstk(pt-2).ne.101
     &        .or.rstk(pt-3).ne.703
     &        .or.(sym.ne.semi.and.sym.ne.comma.and.sym.ne.eol)) 
     &         goto 156
         pt=pt-3
c     .  check against syntax like [a,b(1,2)]=resume(3,1)
         do  i=1,rhs
            if (pstk(pt+1-i).ne.0) then
               call error(79)
               return
            endif
         enddo
         k=lpt(1)-(13+nsiz)
         bot=lin(k+5)
         if(macr.ne.0.or.paus.ne.0) then
            lpts=lpt(1)
            lpt(1)=lin(k+1)
         endif
         mrhs=rhs
         rhs=0
         paus=paus-1
         do 155 i=1,mrhs
            call stackp(ids(1,pt),0)
            pt=pt-1
 155     continue
         paus=paus+1
         if(macr.ne.0.or.paus.ne.0) then
            lpt(1)=lpts
         endif
         lin(k+5)=bot
         top=top-count
      else
         goto 156
      endif
      pt=pt0
      goto 999
 156  continue
      call putid(ids(1,pt),resume)
      call error(72)
 999  return
      end

