      subroutine dasrti(fname)
c ====================================================================
C     dasrt 
c ====================================================================
c
c     Copyright INRIA/ENPC 
      INCLUDE '../stack.h'
c
      character*(*) fname
      character*(nlgh+1) namjac
      common/cjac/namjac
      integer iadr,sadr,gettype
c
      double precision atol,rtol,t0
      integer info(15),topk,topw
      logical hotstart,type,getexternal,getrvect
      logical checkrhs,checklhs,getrmat,cremat,getscalar
      double precision tout,tstop,maxstep,stepin
      character*(nlgh+1) namer,namej,names
      common /dassln/ namer,namej,names
      external bresd,bjacd,bsurfd
      external setfresd,setfjacd,setfsurfd
      common/ierode/iero
c     
      data atol/1.d-7/,rtol/1.d-9/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c     SCILAB function : dasrt
c     --------------------------
c     [y0,nvs,[,hotdata]]=dasrt(y0,t0,t1[,atol,rtol],res[,jac],nh,h,info
c     [,hotdata])
      iero=0
      maxord=5
      lbuf = 1
      topk=top
      topw=top+1
      lw = lstk(topw)
      l0 = lstk(top+1-rhs)
      if (.not.checkrhs(fname,6,11)) return
      if (.not.checklhs(fname,2,3)) return
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
c     ----------------------------
      kt0=top-rhs+2
      if(.not.getscalar(fname,topk,kt0,lr2))return
      t0=stk(lr2)
c     checking variable t1 (number 3)
c     -------------------------------
      if(.not.getrmat(fname,topk,top-rhs+3,m3,n3,l3))return
      nt=m3*n3
c     
c     checking variable atol (number 4)
c     --------------------------------
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
         goto 1105
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
         goto 1105
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
c     --------------------------------
 1105 kres=top-rhs+6-iskip
      if (.not.getexternal(fname,topk,kres,namer,type,
     $     setfresd)) return

c     checking variable number 7
c     -----------------------------
      kjac=top-rhs+7-iskip
      if(kjac.gt.top) then
         iskip=iskip+1
         info(5)=0
      else
         is7 = gettype(kjac)
         if(is7.eq.15) then
c     .     info or jac ? get list size to decide
            il7=iadr(lstk(kjac))
            if (istk(il7).lt.0)  il7=istk(il7+1)
            if (istk(il7+1).eq.2) then
c     .        jac
               is7=13
            endif
         endif
         if((is7.ne.10).and.(is7.ne.11).and.(is7.ne.13)) then
            iskip=iskip+1
            info(5)=0
         else
            info(5)=1
            if (.not.getexternal(fname,topk,kjac,namej,type,
     $           setfjacd)) return
         endif
      endif
c     DASRT nh,h
c     checking variable number 8
c     -----------------------------
      if(.not.getscalar(fname,topk,top-rhs+8-iskip,lr8))return
      nh=int(stk(lr8))
c     checking variable number 9
      ksurf=top-rhs+9-iskip
      if (.not.getexternal(fname,topk,ksurf,names,type,
     $        setfsurfd)) return
c     
c     checking variable info (number 10)
c     ------------------------------------
      kinfo = top-rhs+10-iskip
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
      il10 = iadr(lstk(top-rhs+10-iskip))
      if (istk(il10) .ne. 15) then
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
      n10=istk(il10+1)
      l10=sadr(il10+n10+3)
c     
c     --   subvariable tstop(info) --
      il10e1=iadr(l10+istk(il10+1+1)-1)
      l10e1 = sadr(il10e1+4)
      m10e1 = istk(il10e1+1)*istk(il10e1+2)
      if(m10e1.eq.0) then
         info(4)=0
      else
         info(4)=1
         tstop=stk(l10e1)
      endif
      
c     
c     --   subvariable imode(info) --
      il10e2=iadr(l10+istk(il10+1+2)-1)
      l10e2 = sadr(il10e2+4)
      info(3)=stk(l10e2)
      
c     
c     --   subvariable band(info) --
      il10e3=iadr(l10+istk(il10+1+3)-1)
      m10e3 =istk(il10e3+2)*istk(il10e3+2)
      l10e3 = sadr(il10e3+4)
      if(m10e3.eq.0) then
         info(6)=0
      elseif(m10e3.eq.2) then
         info(6)=1
         ml=stk(l10e3)
         mu=stk(l10e3+1)
      else
         err=10-iskip
         call error(89)
         return
      endif
c     
c     --   subvariable maxstep(info) --
      il10e4=iadr(l10+istk(il10+1+4)-1)
      m10e4 =istk(il10e4+2)*istk(il10e4+2)
      l10e4 = sadr(il10e4+4)
      if(m10e4.eq.0) then
         info(7)=0
      else
         info(7)=1
         maxstep=stk(l10e4)
      endif
      
c     
c     --   subvariable stepin(info) --
      il10e5=iadr(l10+istk(il10+1+5)-1)
      m10e5 =istk(il10e5+2)*istk(il10e5+2)
      l10e5 = sadr(il10e5+4)
      if(m10e5.eq.0) then
         info(8)=0
      else
         info(8)=1
         stepin=stk(l10e5)
      endif
      
c     
c     --   subvariable nonneg(info) --
      il10e6=iadr(l10+istk(il10+1+6)-1)
      l10e6 = sadr(il10e6+4)
      info(10)=stk(l10e6)
c     
c     --   subvariable isest(info) --
      il10e7=iadr(l10+istk(il10+1+7)-1)
      l10e7 = sadr(il10e7+4)
      isest=stk(l10e7)
      if(isest.eq.1) info(11)=1
      
      
 10   hotstart=.false.
      if(rhs.eq.11-iskip) then
         hotstart=.true.
c     
c     checking variable hotdata (number 11)
c     --------------------------------------
         
         il11 = iadr(lstk(top-rhs+11-iskip))
         if (istk(il11) .ne. 1) then
            err = 11-iskip
            call error(53)
            return
         endif
         n11 = istk(il11+1)*istk(il11+2)
         lhot = sadr(il11+4)
      elseif(rhs.ne.10-iskip) then
         call error(39)
         return
      endif
c     --------------------Work Tables 
      if (.not.cremat(fname,topw,0,1,1,lw15,lc)) return
      topw=topw+1      
      if (.not.cremat(fname,topw,0,1,1,lw17,lc)) return
      topw=topw+1      
      il17=iadr(lw17)
c     dasrt needs more
      if (.not.cremat(fname,topw,0,1,nh,lgr,lc)) return
      topw=topw+1      
      lgroot=iadr(lgr)
c     
      if(info(6).eq.0) then
C     for the full (dense) JACOBIAN case 
         lrw = 50+(maxord+4)*neq+neq**2+3*nh
      elseif(info(5).eq.1) then
C     for the banded user-defined JACOBIAN case
         lrw=50+(maxord+4)*neq+(2*ml+mu+1)*neq+3*nh
      elseif(info(5).eq.0) then
C     for the banded finite-difference-generated JACOBIAN case
         lrw = 50+(maxord+4)*neq+(2*ml+mu+1)*neq+2*(neq/(ml+mu+1)+1)+
     $        3*nh
      endif
      liw=20+neq
      if(.not.hotstart) then
         if (.not.cremat(fname,topw,0,1,lrw,lrwork,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,sadr(liw)+1,liwork,lc)) return
         topw=topw+1
      else
         if(lrw+liw.gt.n11) then
            err=11-iskip
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
         istk(iadr(liwork+1))=mu
      endif
c     structure d'info pour les externals
      top=topw
      lw=lstk(top)
      ilext=iadr(lw)
      istk(ilext)=3
      istk(ilext+1)=ilext+5
      istk(ilext+2)=ilext+9
      istk(ilext+3)=ilext+13
      istk(ilext+4)=ilext+16
      istk(ilext+5)=kres
      istk(ilext+6)=neq
      istk(ilext+7)=kt0
      istk(ilext+8)=ky
      istk(ilext+9)=kjac
      istk(ilext+10)=neq
      istk(ilext+11)=kt0
      istk(ilext+12)=ky
      istk(ilext+13)=ksurf
      istk(ilext+14)=kt0
      istk(ilext+15)=ky
c     istk(ilext+16)=ky
      lw=sadr(ilext)+16
      
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
      do 1120 i=0,nt-1
         tout=stk(l3+i)
c     
 1115    k=k+1
         lyri=lyri+(2*n1+1)
         lw=lyri+(2*n1+1)
         lstk(top+1)=lw
         margin=(k-1)*(2*n1+1)+4
         lw1=lw+margin
         if(lhs.eq.3) lw1=lw1+4+lrw+liw
         if(lw1-lstk(bot).gt.0) then
c     not enough memory
            call msgstxt('Not enough memory to go further')
            k=k-1
            goto 1125
         endif
         if (tout .eq. t0) then
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            t0=tout
            goto 1120            
         else
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            call ddasrt(bresd,n1,t0,stk(l1),stk(lydot),
     &           stk(lyri),info,stk(lrtol),stk(latol),idid,
     &           stk(lrwork),lrw,istk(iadr(liwork)),liw,stk(lw15),
     &           istk(il17),bjacd,bsurfd,nh,istk(lgroot))
C     SUBROUTINE DDASRT (RES,NEQ,T,Y,YPRIME,TOUT,
C     *  INFO,RTOL,ATOL,IDID,RWORK,LRW,IWORK,LIW,RPAR,IPAR,JAC,
C     *  G,NG,JROOT)
         endif
         if(err.gt.0)  return
         if(idid.eq.1) then
C     A step was successfully taken in the intermediate-output mode. 
C     The code has not yet reached TOUT.
            stk(lyri)=t0
            info(1)=1
            goto 1115
            
         elseif(idid.eq.2) then
C     The integration to TSTOP was successfully completed (T=TSTOP)
            goto 1125
            
         elseif(idid.eq.3) then
C     The integration to TOUT was successfully completed (T=TOUT) by 
C     stepping past TOUT. Y,ydot are obtained by interpolation.
            t0=tout
            info(1)=1
            goto 1120
         elseif(idid.eq.4) then
C     one or more root found
            stk(lyri)=t0
C     stk(lrw+41)
            goto 1125 
         elseif(idid.eq.-1) then
C     A large amount of work has been expended (About 500 steps)
            call msgstxt('to many steps necessary to reached next '//
     &           'required time discretization point')
            call msgstxt('Change discretisation of time vector t '//
     &           'or decrease accuracy')
            stk(lyri)=t0
            goto 1125
         elseif(idid.eq.-2) then
C     The error tolerances are too stringent.
            t0=tout
            info(1)=1
            goto 1115
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
            goto 1125
         elseif(idid.eq.-7) then
C     The corrector could not converge.
            call msgstxt('May be inaccurate or ill-conditioned '//
     &           'JACOBIAN')
            goto 1125
         elseif(idid.eq.-8) then
C     The matrix of partial derivatives is singular.
            buf='The matrix of partial derivatives is singular'//
     &           'Some of your equations may be redundant'
            call error(9986)
            return
         elseif(idid.eq.-9) then
C     The corrector could not converge. there were repeated error 
c     test failures in this step.
            call msgstxt('Either ill-posed problem or '//
     &           'discontinuity or singularity encountered')
            goto 1125
         elseif(idid.eq.-10) then
            call msgstxt('external ''res'' return many times'//
     &           'with ires=-1')
            goto 1125
         elseif(idid.eq.-11) then
C     IRES equal to -2 was encountered  and control is being returned to the
C     calling program.
            buf='error in external ''res'' '
            call error(9989)
            return
         elseif(idid.eq.-12) then
C     DDASSL failed to compute the initial YPRIME.
            buf='dassrt failed to compute the initial Ydot.'
            call error(9990)
            return
         elseif(idid.eq.-33) then
C     The code has encountered trouble from which
C     it cannot recover. A message is printed
C     explaining the trouble and control is returned
C     to the calling program. For example, this occurs
C     when invalid input is detected.
            call msgstxt('dassrt encountered trouble')
            goto 1125
         endif
         t0=tout
         info(1)=1
 1120 continue
c     
 1125 top=topk-rhs
      mv=lw0-l0
c     
c     Variable de sortie: y0
c     
      top=top+1
      if(k.eq.0) istk(ilyr+1)=0
      istk(ilyr+2)=k
      lw=lyr+(2*n1+1)*k
      lstk(top+1)=lw-mv
c     
c     Variable de sortie: roots
c     
      top=top+1
      ilw=iadr(lw)
      err=lw+4+nh+1-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=1
      istk(ilw+2)=1
      istk(ilw+3)=0
      l=sadr(ilw+4)
      stk(l)=t0
      kkk=1
      do 1153 i=0,nh-1
         if(istk(lgroot+i).ne.0) then
            l=l+1
            kkk=kkk+1
            istk(ilw+2)=istk(ilw+2)+1
            stk(l)=i+1
         endif
 1153 continue
      lw=l+1
      lstk(top+1)=lw-mv
      if(lhs.eq.2) goto 1150
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
 1150 call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)      
      return
      end	

