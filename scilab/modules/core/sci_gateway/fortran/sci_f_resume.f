c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intresume
      include 'stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      parameter (iif=1,iwhile=2,iselect=3)
      integer pt0,r,prev
      integer semi,comma,eol
      integer resume(nsiz)
c     count is used to count top variables used by current "for" or "select" clauses
      integer count
c
      data resume/505155099,673713686,nz2*673720360/
      data eol/99/,semi/43/
      data comma/52/
c
      if(rhs.eq.0) then
         call error(46)
         return
      endif

      call ref2val
      if(rhs.ne.lhs) then
         call error(41)
         return
      endif
c

c     Determine the  calling context (macro, exec or execstr)
      count=0
      pt0=pt+1
 10   pt0=pt0-1
      if(pt0.le.0) then
c     .  main -> resume
c     .  [x,y,...]=resume(a,b,..) just behave has [x,y,...]=(a,b,..)
         return
      endif
c
c     count temporary variables created on top of stack by for, select,..
      if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &     (rstk(pt0).eq.805.and.ids(1,pt0).eq.iselect).or.
     &     (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1

      if(int(rstk(pt0)/100).ne.5) goto 10

c     pause -> resume
c     macro -> resume
c     exec -> resume
c     execstr -> resume

      if (rstk(pt0).eq.501) then
c     .  compiled_macro -> resume
c     .  exec(compiled_macro) -> resume
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
      elseif(rstk(pt0).eq.502.or.rstk(pt0).eq.503) then
c     .  uncompiled_macro -> resume
c     .  exec(uncompiled_macro) -> resume
c     .  execstr -> resume
c     .  exec -> resume
         if(rstk(pt0-1).eq.902) then
c     .     exec -> resume
            mrhs=rhs
c     .     Locate end of the lhs argument list (703 set by parse)
            do ip0=pt0,pt
               if(rstk(ip0).eq.703) goto 20
            enddo
 20         ip0=ip0-mrhs+1
            rhs=0
            do  i=1,mrhs
               call stackp(ids(1,ip0+mrhs-i),0)
            enddo
c     .        remove top variables associated with unstacked contexts  (for, select,..)
            top=top-count
            pt=pt0
            goto 999
         elseif(rstk(pt0-1).eq.903.or.rstk(pt0-1).eq.706) then
c     .     execstr -> resume
c     .     check execstr calling context
            ip0=pt0+1
 153        pt0=pt0-1
            if(pt0.le.0) return
            if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &           (rstk(pt0).eq.805.and.ids(1,pt0).eq.iselect).or.
     &           (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1
            if(rstk(pt0).lt.501.or.rstk(pt0).gt.503) goto 153

c     .     compiled_macro -> execstr -> resume (501)
c     .     exec(compiled_macro) -> execstr -> resume (501)
c     .     exec(file) -> execstr -> resume (503)
c     .     execstr -> execstr -> resume (503)
c     .     pause -> execstr -> resume (503)
            prev=rstk(pt0-1)
            if(rstk(pt0).eq.503.and.rio.eq.rte.and.paus.ne.0) then
c     .        pause -> execstr -> resume
c     .        check against syntax like [a,b(1,2)]=resume(3,1)
               do  i=1,rhs
                  if (pstk(ip0+rhs-i).ne.0) then
                     buf='Index not allowed in the lhs of resume'
                     call error(997)
                     return
                  endif
               enddo

c     .        remove execstr context
               k=lpt(1)-(13+nsiz)
               lpt(1)=lin(k+1)
               macr=macr-1

c     .        remove pause context temporarily to save variables in the
c     .        calling (pause) context
               k=lpt(1)-(13+nsiz)
               lpt1=lpt(1)
               lpt(1)=lin(k+1)
               bot=lin(k+5)
               paus=paus-1
c     .        save resumed variables in the calling context
               mrhs=rhs
               rhs=0
               do  i=1,mrhs
                  call stackp(ids(1,ip0+mrhs-i),0)
               enddo
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
            if(prev.eq.902.or.prev.eq.903.or.prev.eq.909) then
c     .        exec(compiled_macro) -> execstr -> resume (prev=909)
c     .        exec(file) -> execstr -> resume (prev=902)
c     .        execstr -> execstr -> resume (prev=903)
c     .        returned data are stored in the current scope
c     .        check against syntax like [a,b(1,2)]=resume(3,1)
               do  i=1,rhs
                  if (pstk(ip0+rhs-i).ne.0) then
                     buf='Index not allowed in the lhs of resume'
                     call error(997)
                     return
                  endif
               enddo

c     .           remove execstr context
               k=lpt(1)-(13+nsiz)
               lpt(1)=lin(k+1)
               macr=macr-1
               mrhs=rhs
               rhs=0
               do  i=1,mrhs
                  call stackp(ids(1,ip0+mrhs-i),0)
               enddo
c     .        remove top variables associated with unstacked contexts  (for, select,..)
               top=top-count-1
               pt=pt0
               goto 999
            else
c     .        compiled_macro -> execstr -> resume (501)
c     .        variables are returned in the calling context of the macro (see macro.f code for details)
               k = lpt(1) - (13+nsiz)
               lpt(1)=lin(k+1)
               macr=macr-1

c     .        get location of lhs var names
               lvar=pt-3
               rstk(pt0)=504
               pstk(pt0+1)=lvar
               pstk(pt0+2)=count+1
            endif
         else
c     .     resume not called by an execstr (simple case)
c     .     pause -> resume
c     .     macro -> resume

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
               pstk(pt0+1)=pt
               pstk(pt0+2)=count
            elseif (rstk(pt0-1).eq.701.or.rstk(pt0-1).eq.604) then
c     .       main  -> pause -> resume  (701)
c     .       macro(uncompiled) -> pause -> resume (701)
c     .       macro(compiled) -> pause -> resume (604)

               k=lpt(1)-(13+nsiz)
               bot=lin(k+5)
               if(macr.ne.0.or.paus.ne.0) then
                  lpts=lpt(1)
                  lpt(1)=lin(k+1)
               endif
               mrhs=rhs
               rhs=0
               paus=paus-1
               do  i=1,mrhs
                  call stackp(ids(1,pt),0)
                  pt=pt-1
               enddo
               paus=paus+1
               if(macr.ne.0.or.paus.ne.0) then
                  lpt(1)=lpts
               endif
               lin(k+5)=bot
               top=top-count
            else
c     .        macro(uncompiled) -> resume (306)
               pstk(pt0+1)=pt
               pstk(pt0+2)=count
            endif

         endif

      elseif(rio.eq.rte) then
c     .  pause -> resume (still useful case?)
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
      call putid(ids(1,pt+1),resume)
      call error(72)
 999  return
      end

