c     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c     Copyright (C) INRIA
c     
c     This file must be used under the terms of the CeCILL.
c     This source file is licensed as described in the file COPYING,
c     which
c     you should have received as part of this distribution.  The terms
c     are also available at    
c     http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine macro
c     
      include 'stack.h'
c     
      parameter (nz2=nsiz-2,nz3=nsiz-3)
      double precision val
      integer eol
      logical eqid,ptover,vargin,vargout,exec,vcopyobj
      integer blank,r,ival(2),ptr,top1,count,iadr,wmacn
      integer varargin(nsiz),varargout(nsiz),id(nsiz)
      integer scivarindex
      integer p
      equivalence (ival(1),val)
      data blank/40/,eol/99/
      data varargin/169544223,387059739,nz2*673720360/
      data varargout/169544223,504893467,673720349,nz3*673720360/
c     
c     
      iadr(l)=l+l-1
c     
      r=rstk(pt)
      if (ddt .eq. 4) then
         write(buf(1:18),'(2i4,i6)') pt,r,fin
         call basout(io,wte,' macro  pt:'//buf(1:4)//' rstk(pt):'//
     &        buf(5:8)//' fin:'//buf(9:14))
      endif
c     
      ir=r/100
      if(ir.ne.5) goto 10
      goto(40,40,60,40),r-500
      goto 99
c     
 10   continue

c     check for interpreted pause(701),compiled pause(604) or exec(902)
      if(r.eq.902.or.r.eq.701.or.r.eq.604) goto 50

c     initialize macro or execstr execution
c------------------------------------------
c     
      ilk=iadr(fin)
      wmacn=0
c     
      if(istk(ilk).eq.10) then
c     an execstr
         exec=.false.
         vargout=.false.
         mrhs=0
         rhs=0
         mlhs=0
         l=ilk+5+istk(ilk+1)*istk(ilk+2)
         lf=l+istk(ilk+4+istk(ilk+1)*istk(ilk+2))-1
         last=isiz
         if(macr.ne.0.or.paus.ne.0) then
            k=lpt(1)-(13+nsiz)
            last=lin(k+5)
         endif
      else
c     a macro
         exec=rstk(pt).eq.909.or.rstk(pt).eq.903
         if(nmacs.gt.0.and.comp(1).eq.0) then
            if(rstk(pt).eq.602) then
c     .        see run to explain the following
               lcn=pstk(pt)-4-(nsiz+3)+1
               call putid(id,istk(lcn))
            elseif(rstk(pt).eq.707) then
c     .        function without argument list
               call putid(id,syn)
            else
               call putid(id,ids(1,pt))
            endif
         endif

         if(comp(1).eq.0) then
c     .    find the index of the macro in the stack et memorize it for breakpoints
            wmacn=scivarindex(fin)
         endif

         l=ilk+1
         if(exec) then
            vargout=.false.
            last=bbot
            if(macr.gt.0.or.paus.gt.0) then
               k=lpt(1) - (13+nsiz)
               last=lin(k+5)
            endif

            mlhs=istk(l)
            l=l + nsiz*mlhs + 1
            mrhs=istk(l)
            l=l+1
            l = l + nsiz*mrhs + 1
            rhs=0
            lhs=0
            goto 17
c     .    exec(macro)
         endif
         mrhs=0
         last=bot
c     set output variable name
         mlhs=istk(l)
         l0=l
         l=l+nsiz*mlhs+1
         vargout=.false.
         if(mlhs.gt.0) vargout=eqid(istk(l-nsiz),varargout)
         if(mlhs.lt.lhs.and..not.vargout) then
            if(mlhs.ne.0.or.lhs.gt.1) then
               lhs=mlhs
               pstk(pt)=l0+1
               call error(59)
               return
            endif
         endif

c     set input variable name
         mrhs=istk(l)
         l=l+1
         l1=l+nsiz*(rhs-1)
         l = l + nsiz*mrhs + 1
         lf= l+istk(l-1)+1
         if(mrhs.eq.0.and.rhs.le.1) then
            if(comp(1).ne.0)  rhs=0
         endif
         vargin=.false.
         if(mrhs.gt.0) vargin=eqid(istk(l-nsiz-1),varargin)
         if(mrhs.lt.rhs.and..not.vargin) then
            pstk(pt)=l1-(rhs-1)*nsiz
            rhs=mrhs
            call error(58)
            return
         endif
      endif
c     save line pointers
 17   k = lpt(6)
      if(k+13+nsiz.gt.lsiz) then
         call error(108)
         return
      endif
      lin(k+1) = lpt(1)
      lin(k+2) = lpt(2)
      lin(k+3) = lpt(3)
      lin(k+4) = lpt(4)
      lin(k+5) = last
      lin(k+6) = ilk
      lin(k+7) = l
      val = stk(lstk(isiz))
      lin(k+8) = ival(1)
      lin(k+9) = ival(2)
      lin(k+10)=char1
      lin(k+11)=sym
      call putid(lin(k+12),syn)
      lin(k+12+nsiz)=lct(8)
      lpt(1) = k + 13+nsiz
c     
      if ( ptover(1,psiz-1)) return
      ids(1,pt) = rhs
      ids(2,pt) = lhs
      ids(3,pt) = lf
      if(vargout) then
         ids(4,pt)=1
      else
         ids(4,pt)=0
      endif
      ids(4,pt)=ids(4,pt)+2*wmac
      wmac=wmacn
      pstk(pt)=lct(4)
c     
      macr=macr+1
c     set line pointers
      k=lpt(1)
      lin(k) = eol
      lpt(6) = k
      lpt(4) = lpt(1)
      lpt(3) = lpt(1)
      lpt(2) = lpt(1)
c     c_ex lct(1) = 0
      if(.not.exec) then
         if (ddt .ne. 2) lct(4)=-1
      else
         lct(4)=pstk(pt-1)
      endif
      char1 = blank
      lin(lpt(4)+1)=blank
c     
c     save input variables

c     next line should be suppressed, but when no arg given rhs is -1
c     and not 0
      irhs=max(rhs,0)
      if(comp(1).eq.0.and..not.exec) then
         if( mrhs.gt.0) then
            if(vargin.and.irhs.ge.mrhs-1) then
               call mklist(irhs-mrhs+1)
               infstk(top)=0
               irhs=mrhs
               l1=l-nsiz-1
            endif
            mrhs=irhs
            rhs=0
            do 20 j=1,mrhs
               if(infstk(top).eq.1) then
c     .           named variable
                  call stackp(idstk(1,top),0)
               elseif(infstk(top).eq.2) then
c     .           global variable : create a local copy and save it
c     .           instead of the original one, to avoid side effects if
C     .           the same global variable is used inside the macro
                  kg=istk(iadr(lstk(top))+2)
                  if (.not.vcopyobj(' ',kg,top)) return
                  call stackp(istk(l1),0)
               else
                  call stackp(istk(l1),0)
               endif
               l1=l1-nsiz
 20         continue
         endif
         if(vargout) then
            call mklist(0)
            call stackp(varargout,0)
         endif
      endif
c     
      ids(6,pt)=toperr
      toperr=top
      if(istk(ilk).eq.13) then
         lct(8)=1
         rstk(pt)=501
c     next line for forced rhs
         ids(5,pt)=0
         icall=6
c     *call* run
      else
         lct(8)=1
         rstk(pt)=502
c     pstk(pt)=0
         icall=7
         sym=eol
c     *call* parse
      endif
      go to 99
c     
 40   continue
c     terminate macro or execstr execution
c-----------------------------------------
c     handle errcatch
      exec=rstk(pt-1).eq.909.or.rstk(pt-1).eq.903
      if(errct.ne.0.and.errpt.ge.pt.and..not.exec) then
c     .  looking for a previous execstr(....,'errcatch') see intexecstr
         p=pt
 401     continue
         if (rstk(p).eq.903) then
C     .     execstr(....,'errcatch') level found
            err1=0
            err2=0
            catch=0
            errct=0
            errpt=0
            if (p.gt.0) then
               if (ids(1,p).eq.1) errct=-900001
               errpt=p
            endif
         elseif (rstk(p).eq.808) then
C     .     try level found
            errct=-900001
            errpt=p
         elseif (p-1.gt.0) then
            p=p-1
            goto 401
         endif
      endif
      

      lhsr=lhs
c     
      lct(4)=pstk(pt)
      rhs=ids(1,pt)
      lhs=ids(2,pt)
      lct(4)=pstk(pt)
      vargout=mod(ids(4,pt),2).eq.1
      wmac=ids(4,pt)/2
c     restaure  pointers
      k = lpt(1) - (13+nsiz)
      ilk=lin(k+6)
      char1=lin(k+10)
      sym=lin(k+11)
      call putid(syn,lin(k+12))
c     
      if (err1.gt.0.and.catch.eq.0) then
         top=toperr
         toperr=ids(6,pt)
         macr=macr-1
         if (.not.exec) bot=lin(k+5)
         goto 48
      endif

      toperr=ids(6,pt)
      
c     
      if(comp(1).ne.0) then
         comp(2)=comp(1)
         comp(1)=0
         macr=macr-1
         goto 47
      endif


      if(istk(ilk).ne.10.and..not.exec) then
c     .  copy output variables at the top of the stack
         l0=ilk+1
c     .  set output variable name
         mlhs=istk(l0)
         if(mlhs.eq.0.and.lhs.le.1) lhs=0

         if(vargout.and.lhs.ge.mlhs) then
            lhs1=mlhs-1
         else
            lhs1=lhs
         endif

         l0=l0+1
         if(lhs1.gt.0) then
            mrhs=rhs
            rhs=0
            do 41 i=1,lhs1
               fin=0
               call stackg(istk(l0))
               if(fin.eq.0) then
                  if(err1.le.0) then
c     .              call error only if it has not been already called
                     call putid(ids(1,pt+1),istk(l0))
                     call error(4)
                     if(err.gt.0) return
                  endif
               endif
               l0=l0+nsiz
 41         continue
            rhs=mrhs
         endif
         if(lhs1.lt.lhs) then
c     .   extract required output variables out of varargout
            nv=lhs-mlhs+1
            call stackgl(istk(l0),nv)
            if(err.gt.0) return
            l0=l0+nsiz
         endif
      endif
c     
      macr=macr-1
      if(istk(ilk).eq.10.or.exec) goto 48
      bot=lin(k+5)
      if(lhsr.ne.0) then
c     .   handle variables returned by resume
         lpt1 = lpt(1)
         lpt(1)=lin(k+1)
         top1=top
         top=top-lhs
         count=0
c     
         if(rstk(pt).eq.501) then
c     .     resume in a "compiled" macro
            count=pstk(pt+2)
            lc=ids(1,pt+1)
            if(istk(lc).eq.29) then
               lc=lc+3
               i27=0
            elseif(istk(lc).eq.1) then
c     .        retained for 2.7 and earlier version compatibility (old affectation)
               lc=lc+1
               i27=1
            endif
c     .     preserve names stored in the macro if macro is moved
            if(pt+lhsr.gt.psiz) then
               call error(26)
               return
            endif
            do 43 i=1,lhsr
               call putid(ids(1,pt+i),istk(lc))
               lc=lc+nsiz+1
c     .        retained for 2.7 and earlier version compatibility (old affectation)
               if(i27.ne.0.and.istk(lc-1).eq.22) lc=lc+2
 43         continue
            do 44 i=1,lhsr
               call stackp(ids(1,pt+i),0)
               if(err.gt.0) return
 44         continue
         else
c     .     resume in "uncompiled" macros 
c     .     and in execstr call in a macro (rstk(pt)==504 see sci_resume)

            ptr=pstk(pt+1)
            count=pstk(pt+2)
            lpt1s=lpt(1)
c     .      reset lpt(1) for error recovery
            if(rstk(pt).eq.504) lpt(1)=lpt1
            do 45 i=1,lhsr
               call stackp(ids(1,ptr),0)
               if(err.gt.0) return
               if(err1.gt.0) then
                  goto 48
               endif
               ptr=ptr-1
 45         continue
            lpt(1)=lpt1s
         endif
c     
c     .  remove top variables relatives to for or select if any
         top=top-count
c     
         if(lhs.gt.0) then
c     .     move macro output variables at the top of the stack
            top1=top1-lhs
            if(top1.ne.top) then
               do 46 i=1,lhs
                  top1=top1+1
                  top=top+1
                  call unsfdcopy(lstk(top1+1)-lstk(top1),stk(lstk(top1))
     $                 ,1,stk(lstk(top)),1)
                  lstk(top+1)=lstk(top)+lstk(top1+1)-lstk(top1)
 46            continue
            else
               top=top+lhs
            endif
         endif
      endif
c     
 47   continue
      if(lhs.eq.0) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
      endif
 48   pt=pt-1
      lct(8)=lin(k+12+nsiz)
      lpt(1)=lin(k+1)
      lpt(2)=lin(k+2)
      lpt(3)=lin(k+3)
      lpt(4)=lin(k+4)
      lpt(6)=k
      ival(1)=lin(k+8)
      ival(2)=lin(k+9)
      stk(lstk(isiz))=val
      return
c     
c     exec
 50   continue
      k = lpt(6)
      if(k+13+nsiz.gt.lsiz) then
         call error(26)
         return
      endif
      lin(k+1) = lpt(1)
      lin(k+2) = lpt(2)
      lin(k+3) = lpt(3)
      lin(k+4) = lpt(4)
      lin(k+5) = bot0
c     two following lines set information necessary for  stackp to know 
c     current macro context
      if(macr.ge.1) lin(k+5)=lin(lpt(1)-(8+nsiz))
      if(rio.eq.rte) lin(k+5)=bot

      lin(k+6) = lct(4)
c     following lines allows to distinguish between macro (<>0) and
c     exec (=0)
      lin(k+7)=0
c     
      lin(k+10)=char1
      lin(k+11)=sym
      lin(k+12+nsiz)=lct(8)
      lpt(1) = k + (13+nsiz)
      lpt(6) = lpt(1)
      if(lct(4).le.-10) fin=-lct(4)-11
      lct(4) = fin
      if(rio.eq.rte) paus=paus+1
      sym = eol
      if ( ptover(1,psiz)) return
      lct(8)=0
      rstk(pt)=503
      pstk(pt)=wmac
      
      if(r.eq.701.or.r.eq.604) then 
c     .  disable error recovery mode , for pause only (is it mandatory?)
         ids(2,pt)=errct
         ids(3,pt)=err2
         ids(4,pt)=err1
         ids(5,pt)=errpt
         ids(6,pt)=catch
         err1=0
         catch=0
         errct=-1
      endif
      wmac=0
      icall=7
c     *call parse*
      go to 99
c     
c     fin exec
 60   continue
      k = lpt(1) - (13+nsiz)
      lpt(1) = lin(k+1)
      lpt(2) = lin(k+2)
      lpt(3) = lin(k+3)
      lpt(4) = lin(k+4)
      lct(4) = lin(k+6)
      lpt(6) = k
      char1=lin(k+10)
      sym=lin(k+11)
      lct(8)=lin(k+12+nsiz)
      if(rio.eq.rte.and.paus.gt.0) then
         bot=lin(k+5)
         paus=paus-1
      endif
      wmac=pstk(pt)
      r=rstk(pt-1)
      if(r.eq.701.or.r.eq.604) then 
c     restore current error recovery modes
         errct=ids(2,pt)
         err2=ids(3,pt)
         err1=ids(4,pt)
         errpt=ids(5,pt)
         catch=ids(6,pt)
      endif

      pt=pt-1
      go to 99
c     
 99   continue
      return
      end

      integer function scivarindex(l)
      include 'stack.h'
      integer k0,k1,k2,l

      if(l.ge.lstk(bot)) then
         k0=bot
         k2=isiz
      elseif(l.le.lstk(top+1)) then
         k0=1
         k2=top+1
      else
         scivarindex=0
         return
      endif

c     dichotomy search
 10   continue
      if (k2-k0.le.1) goto 20
      k1=int((k2-k0)/2)+k0
      if(l.ge.lstk(k1)) then
         k0=k1
      else
         k2=k1-1
      endif
      goto 10

 20   if (l.ge.lstk(k2)) then
         scivarindex=k2
      else
         scivarindex=k0
      endif
      return
      end

