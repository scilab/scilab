c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine dassli(fname)
      character*(*) fname
c     ============================================
      INCLUDE 'stack.h'
c
      integer iadr,sadr
      integer topk,topw, info(15),gettype
      logical hotstart,getexternal,getrvect,type
      logical checkrhs,checklhs,getrmat,cremat,getscalar
      double precision tout,tstop,maxstep,stepin
      double precision atol,rtol,t0
      character*(nlgh+1) namer,namej,names
      character*(nlgh+1) namjac
      external bresd,bjacd
      external setfresd,setfjacd

      common /dassln/ namer,namej,names
      common/ierode/iero
      common/cjac/namjac
c     
      data atol/1.d-7/,rtol/1.d-9/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     SCILAB function : dassl
c     --------------------------
c     [y0 [,hotdata]]=dassl(y0,t0,t1 [,atol,rtol],res [,jac],info..
c     [,hotdata])
      iero=0
      maxord=5
      lbuf = 1
      topk=top
      topw=top+1
      lw = lstk(topw)
      l0 = lstk(top+1-rhs)
      if (.not.checkrhs(fname,4,9)) return
      if (.not.checklhs(fname,1,2)) return
c     checking variable y0 (number 1)
c     -------------------------------
      ky=top-rhs+1
      if(.not.getrmat(fname,topk,ky,n1,m1,l1))return
      neq=n1
      lydot=l1+n1
      info(11)=0
      if (m1 .eq.1) then
         if (.not.cremat(fname,topw,0,n1,1,lydot,lc)) return
         topw=topw+1
         info(11)=1
         call dset(n1,0.0d0,stk(lydot),1)
      elseif(m1.ne.2) then
         err = 1
         call error(89)
         return
      else 
         il1 = iadr(lstk(top-rhs+1))
         istk(il1+2)=1
      endif
c     checking variable t0 (number 2)
c     -------------------------------
      kt0=top-rhs+2
      if(.not.getscalar(fname,topk,kt0,lr2))return
      t0=stk(lr2)
c     checking variable t1 (number 3)
c     -------------------------------
      if(.not.getrmat(fname,topk,top-rhs+3,m3,n3,l3))return
      nt=m3*n3
c     
c     checking variable atol (number 4)
c     -------------------------------
      iskip=0
      itype = gettype(top-rhs+4)
      if ( itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,latol,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(latol)=atol
         stk(lrtol)=rtol
         info(2)=0
         iskip=iskip+2
         goto 105
      endif
      if(.not.getrvect(fname,topk,top-rhs+4,m4,n4,latol))return
      m4 = m4*n4
c     checking variable rtol (number 5)
c     --------------------------------
      itype = gettype(top-rhs+5)
      if (itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(lrtol)=lrtol
         info(2)=0
         iskip=iskip+1
         goto 105
      endif
      if(.not.getrvect(fname,topk,top-rhs+5,m5,n5,lrtol))return
      m5 = m5*n5
      if(m5.ne.m4) then
         call error(60)
         return
      endif
      if(m5.eq.1) then
         info(2)=0
      else
         info(2)=1
      endif
c     checking variable res (number 6)
c     
 105  kres=top-rhs+6-iskip
      if (.not.getexternal(fname,topk,kres,namer,type,
     $     setfresd)) return
c     checking variable jac (number 7)
c     
      kjac=top-rhs+7-iskip
      if(kjac.gt.top) then
         iskip=iskip+1
         info(5)=0
      else
         isres=gettype(kjac)
         if(isres.eq.15) then
c     .     info or jac ? get list first element type to decide
            il6=iadr(lstk(kjac))
            if (istk(il6).lt.0)  il6=istk(il6+1)
            nelt=istk(il6+1)
            l61=sadr(il6+3+nelt)
            if (abs(istk(iadr(l61))).eq.11.or.
     $           abs(istk(iadr(l61))).eq.13) then
c     .        jac
               isres=istk(iadr(l61))
            endif
         endif
            
         if((isres.ne.10).and.(isres.ne.11).and.(isres.ne.13)) then
            iskip=iskip+1
            info(5)=0
         else
            info(5)=1
            if (.not.getexternal(fname,topk,kjac,namej,type,
     $           setfjacd)) return
         endif
      endif
c     
c     checking variable info (number 8)
c     ---------------------------------
      kinfo=top-rhs+8-iskip
      if (kinfo.gt.top) then
         info(4)=0
         info(3)=0
         info(6)=0
         info(7)=0
         info(8)=0
         info(10)=0
         info(11)=0
         iskip=iskip+1
         goto 10
      endif
      il8 = iadr(lstk(top-rhs+8-iskip))
      if (istk(il8) .ne. 15) then
c     default info values
         info(4)=0
         info(3)=0
         info(6)=0
         info(7)=0
         info(8)=0
         info(10)=0
         info(11)=0
         iskip=iskip+1
         goto 10
      endif
      n8=istk(il8+1)
      l8=sadr(il8+n8+3)
c     
c     --   subvariable tstop(info) --
      il8e1=iadr(l8+istk(il8+1+1)-1)
      l8e1 = sadr(il8e1+4)
      m8e1 = istk(il8e1+1)*istk(il8e1+2)
      if(m8e1.eq.0) then
         info(4)=0
      else
         info(4)=1
         tstop=stk(l8e1)
      endif
      
c     
c     --   subvariable imode(info) --
      il8e2=iadr(l8+istk(il8+1+2)-1)
      l8e2 = sadr(il8e2+4)
      info(3)=stk(l8e2)
      
c     
c     --   subvariable band(info) --
      il8e3=iadr(l8+istk(il8+1+3)-1)
      m8e3 =istk(il8e3+1)*istk(il8e3+2)
      l8e3 = sadr(il8e3+4)
      if(m8e3.eq.0) then
         info(6)=0
      elseif(m8e3.eq.2) then
         info(6)=1
         ml=stk(l8e3)
         mu=stk(l8e3+1)
      else
         err=8-iskip
         call error(89)
         return
      endif
c     
c     --   subvariable maxstep(info) --
      il8e4=iadr(l8+istk(il8+1+4)-1)
      m8e4 =istk(il8e4+1)*istk(il8e4+2)
      l8e4 = sadr(il8e4+4)
      if(m8e4.eq.0) then
         info(7)=0
      else
         info(7)=1
         maxstep=stk(l8e4)
      endif
      
c     
c     --   subvariable stepin(info) --
      il8e5=iadr(l8+istk(il8+1+5)-1)
      m8e5 =istk(il8e5+1)*istk(il8e5+2)
      l8e5 = sadr(il8e5+4)
      if(m8e5.eq.0) then
         info(8)=0
      else
         info(8)=1
         stepin=stk(l8e5)
      endif
      
c     
c     --   subvariable nonneg(info) --
      il8e6=iadr(l8+istk(il8+1+6)-1)
      l8e6 = sadr(il8e6+4)
      info(10)=stk(l8e6)
c     
c     --   subvariable isest(info) --
      il8e7=iadr(l8+istk(il8+1+7)-1)
      l8e7 = sadr(il8e7+4)
      isest=stk(l8e7)
      if(isest.eq.1) info(11)=1
      
      
 10   hotstart=.false.
      if(rhs.eq.9-iskip) then
         hotstart=.true.
c     
c     checking variable hotdata (number 9)
c     
         il9 = iadr(lstk(top-rhs+9-iskip))
         if (istk(il9) .ne. 1) then
            err = 9-iskip
            call error(53)
            return
         endif
         n9 = istk(il9+1)*istk(il9+2)
         lhot = sadr(il9+4)
      elseif(rhs.ne.8-iskip) then
         call error(39)
         return
      endif
c     
c     --------------------Work Tables 
      if (.not.cremat(fname,topw,0,1,1,lw15,lc)) return
      topw=topw+1      
      if (.not.cremat(fname,topw,0,1,1,lw17,lc)) return
      topw=topw+1      
      il17=iadr(lw17)
      if(info(6).eq.0) then
C     for the full (dense) JACOBIAN case 
         lrw = 40+(maxord+4)*neq+neq**2
      elseif(info(5).eq.1) then
C     for the banded user-defined JACOBIAN case
         lrw=40+(maxord+4)*neq+(2*ml+mu+1)*neq
      elseif(info(5).eq.0) then
C     for the banded finite-difference-generated JACOBIAN case
         lrw = 40+(maxord+4)*neq+(2*ml+mu+1)*neq+2*(neq/(ml+mu+1)+1)
      endif
      liw=20+neq
      if(.not.hotstart) then
         if (.not.cremat(fname,topw,0,1,lrw,lrwork,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,sadr(liw)+1,liwork,lc)) return
         topw=topw+1
      else
         if(lrw+liw.gt.n9) then
            err=9-iskip
            call error(89)
            return
         endif
         lrwork=lhot
         liwork=lhot+lrw
         call entier(liw,stk(liwork),istk(iadr(liwork)))
      endif
c     
      if(info(4).eq.1) then
         stk(lrwork)=tstop
      endif
      if(info(7).eq.1) then
         stk(lrwork+1)=maxstep
      endif
      if(info(8).eq.1) then
         stk(lrwork+2)=stepin
      endif
      if(info(6).eq.1) then
         istk(iadr(liwork))=ml
         istk(iadr(liwork)+1)=mu
      endif
c     structure d'info pour les externals
      top=topw
      lw=lstk(top)
      ilext=iadr(lw)
      istk(ilext)=2
      istk(ilext+1)=ilext+4
      istk(ilext+2)=ilext+8
      istk(ilext+3)=ilext+12
      istk(ilext+4)=kres
      istk(ilext+5)=neq
      istk(ilext+6)=kt0
      istk(ilext+7)=ky
      istk(ilext+8)=kjac
      istk(ilext+9)=neq
      istk(ilext+10)=kt0
      istk(ilext+11)=ky
      lw=sadr(ilext)+12
      lw0=lw
      ilyr=iadr(lw)
      istk(ilyr)=1
      istk(ilyr+1)=2*n1+1
      istk(ilyr+3)=0
      lyr=sadr(ilyr+4)
      lyri=lyr-(2*n1+1)
      k=0
      info(1)=0
      if(hotstart) info(1)=1
      info(9)=0
      do 120 i=0,nt-1
         tout=stk(l3+i)
c     
 115     k=k+1
         lyri=lyri+(2*n1+1)
         lw=lyri+(2*n1+1)
         lstk(top+1)=lw
         margin=(k-1)*(2*n1+1)+4
         lw1=lw+margin
         if(lhs.eq.2) lw1=lw1+4+lrw+liw
         if(lw1-lstk(bot).gt.0) then
c     not enough memory
            call msgstxt('Not enough memory to go further')
            k=k-1
            goto 125
         endif
         if (tout .eq. t0) then
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            t0=tout
            goto 120            
         else
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            call dassl(bresd,n1,t0,stk(l1),stk(lydot),
     &           stk(lyri),info,stk(lrtol),stk(latol),idid,
     &           stk(lrwork),lrw,istk(iadr(liwork)),liw,stk(lw15),
     &           istk(il17),bjacd)
         endif
         if(err.gt.0.or.err1.gt.0)  return
         if(idid.eq.1) then
C     A step was successfully taken in the intermediate-output mode. 
C     The code has not yet reached TOUT.
            stk(lyri)=t0
            info(1)=1
            goto 115
            
         elseif(idid.eq.2) then
C     The integration to TSTOP was successfully completed (T=TSTOP)
            goto 125
            
         elseif(idid.eq.3) then
C     The integration to TOUT was successfully completed (T=TOUT) by 
C     stepping past TOUT. Y,ydot are obtained by interpolation.
            t0=tout
            info(1)=1
            goto 120
            
         elseif(idid.eq.-1) then
C     A large amount of work has been expended (About 500 steps)
            call msgstxt('to many steps necessary to reached next '//
     &           'required time discretization point')
            call msgstxt('Change discretisation of time vector t '//
     &           'or decrease accuracy')
            stk(lyri)=t0
            goto 125
         elseif(idid.eq.-2) then
C     The error tolerances are too stringent.
            t0=tout
            info(1)=1
            goto 115
c     buf='The error tolerances are too stringent'
c     call error(9982)
c     return
         elseif(idid.eq.-3) then
C     The local error test cannot be satisfied because you specified 
C     a zero component in ATOL and the corresponding computed solution
C     component is zero. Thus, a pure relative error test is impossible 
C     for this component.
            buf='atol and computed test value are zero'
            call error(9983)
            return
         elseif(idid.eq.-6) then
C     repeated error test failures on the last attempted step.
            call msgstxt('A singularity in the solution '//
     &           'may be present')
            goto 125
         elseif(idid.eq.-7) then
C     The corrector could not converge.
            call msgstxt('May be inaccurate or ill-conditioned '//
     &           'JACOBIAN')
            goto 125
         elseif(idid.eq.-8) then
C     The matrix of partial derivatives is singular.
            buf='Singular partial derivatives matrix'//
     &           ' (may be redundant equations)'
            call error(9986)
            return
         elseif(idid.eq.-9) then
C     The corrector could not converge. there were repeated error 
c     test failures in this step.
            call msgstxt('Either ill-posed problem or '//
     &           'discontinuity or singularity encountered')
            goto 125
         elseif(idid.eq.-10) then
            call msgstxt('external ''res'' return many times'//
     &           ' with ires=-1')
            goto 125
         elseif(idid.eq.-11) then
C     IRES equal to -2 was encountered  and control is being returned to the
C     calling program.
            buf='error in external ''res'' '
            call error(9989)
            return
         elseif(idid.eq.-12) then
C     DDASSL failed to compute the initial YPRIME.
            buf='dassl failed to compute the initial Ydot.'
            call error(9990)
            return
         elseif(idid.eq.-33) then
C     The code has encountered trouble from which
C     it cannot recover. A message is printed
C     explaining the trouble and control is returned
C     to the calling program. For example, this occurs
C     when invalid input is detected.
            call msgstxt('dassl encountered trouble')
            goto 125
         endif
         t0=tout
         info(1)=1
 120  continue
c     
 125  top=topk-rhs
      mv=lw0-l0
c     
c     Variable de sortie: y0
c     
      top=top+1
      if(k.eq.0) istk(ilyr+1)=0
      istk(ilyr+2)=k
      lw=lyr+(2*n1+1)*k
      lstk(top+1)=lw-mv
      if(lhs.eq.1) goto 150
      
c     
c     Variable de sortie: rwork
c     
      top=top+1
      ilw=iadr(lw)
      err=lw+4+lrw+liw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=lrw+liw
      istk(ilw+2)=1
      istk(ilw+3)=0
      lw=sadr(ilw+4)
      call unsfdcopy(lrw,stk(lrwork),1,stk(lw),1)
      call int2db(liw,istk(iadr(liwork)),1,stk(lw+lrw),1)
      lw=lw+lrw+liw
      lstk(top+1)=lw-mv
c     
c     Remise en place de la pile
 150  call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return
      end
c     ============================================
