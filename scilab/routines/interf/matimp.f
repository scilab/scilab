      subroutine matimp
c ====================================================================
c     impl dassl dasrt : simulation  de systeme algebrico-differentiel
c ====================================================================
c     Copyright INRIA
      INCLUDE '../stack.h'
c     impl     dassl     dasrt
c     1         2          3
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matimp '//buf(1:4))
      endif
c     
      goto(10,100,1000) fin
      return
c     impl part 
 10   call sciimpl("impl")
      return
 100  call dassli("dassl")
      return
 1000 call dasrti("dasrt")
      return
      end
      
      subroutine sciimpl(fname)
c     ==================================================
      INCLUDE '../stack.h'
c
      character*(*) fname
      integer iadr,sadr
c
      double precision atol,rtol,t0,t1
      integer topk,topw
      logical jaco,achaud
      external bresid,badd,bj2
      external fres,fadda,fj2
      integer gettype
      logical getexternal,getrvect,vcopyobj
      logical checkrhs,checklhs,getrmat,cremat,getscalar
      logical typej,typea,typer,getsmat,vectsize
      character*(nlgh+1) namres,namadd,namjac
      character*1 strf
      common/cjac/namjac
      external setfres,setfadda,setfj2
      common/ierode/iero
c     
      data atol/1.d-7/,rtol/1.d-9/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,6,10)) return
C     XXXXXX : pour l'instant 
      if (.not.checklhs(fname,1,3)) return
c     ---------------------------------
      iero=0
      topk=top
      topw=top+1
      iskip=1
      mf=20
c     first argument (check for string )
c     ----------------------------
      if(gettype(topk-rhs+1).eq.10) then
         iskip=0
         if(.not.getsmat(fname,topk,topk-rhs+1,
     $        m1,n1,1,1,lr1,nlr1))return
         call cvstr(1,istk(lr1),strf,1)
         if ( strf.eq.'a') mf = 10
         if ( strf.eq.'s') mf = 20
         if(strf.ne.'a'.and.strf.ne.'s') then
            call error(42)
            return
         endif
      endif
c     Initial condition : y0 arg 2 - iskip
c     -------------------
      kynew=topk-rhs+2-iskip
      if(.not.getrvect(fname,topk,kynew,ny,my,ly))return
      neq=ny*my
c     
c     Initial derivative condition : y0dot arg 3 - iskip
c     -------------------
      kydtop=topk-rhs+3-iskip
      if(.not.getrvect(fname,topk,kydtop,nyd,myd,lyd))return
      if(.not.vectsize(fname,topk,kydtop,ny*my)) return
c     t0 arg 4 - iskip
c     ----------------------------
      kttop=topk-rhs+4-iskip 
      if(.not.getscalar(fname,topk,kttop,lr4))return
      t0=stk(lr4)
c     t1 arg 5 - iskip
c     ---------------------------
      kt1=topk-rhs+5-iskip 
      if(.not.getrmat(fname,topk,kt1,m5,n5,lt5))return
      nn=m5*n5
c     checking variable rtol (number 6 - iskip)
c     -----------------------------------
      itype = gettype(topk-rhs+6-iskip)
      if ( itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,latol,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(latol)=atol
         stk(lrtol)=rtol
         na=1
         nr=1
         iskip = iskip+2
         goto 11105
      endif
      kr=top-rhs+6-iskip
      if(.not.getrvect(fname,topk,kr,m6,n6,lrtol))return
      nr = m6*n6
      if(nr.ne.1.and.nr.ne.neq) then
         err= 6-iskip 
         call error(89)
         return
      endif

c     checking variable rtol (number 7 - iskip )
c     --------------------------------
      itype = gettype(topk-rhs+7-iskip)
      if (itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(lrtol)=rtol
         nr=1
         iskip = iskip +1
         goto 11105
      endif
      kr= top-rhs+7-iskip
      if(.not.getrvect(fname,topk,kr,m7,n7,latol))return
      na = m7*n7
      if(na.ne.1.and.na.ne.neq) then
         err= 7-iskip 
         call error(89)
         return
      endif
c     ----------------------------------
11105 if(nr.eq.1.and.na.eq.1) itol=1
      if(nr.eq.1.and.na.gt.1) itol=2
      if(nr.gt.1.and.na.eq.1) itol=3
      if(nr.gt.1.and.na.gt.1) itol=4
c     les externaux : res,adda et jac 
c     -----------------------------------
c     checking variable res (number 8 - iskip )
      kres=topk-rhs+8-iskip
      typer=.false.
      if (.not.getexternal(fname,topk,kres,namres,typer,
     $     setfres)) return

c     checking variable number 9 - iskip
c     -----------------------------
      kadd=topk-rhs+9-iskip
      if (.not.getexternal(fname,topk,kadd,namadd,typea,
     $     setfadda)) return
      if ( typea.neqv.typer) then 
         buf = fname // ': res and adda must have same type '
         call error(999)
      endif
c     checking variable number 10 - iskip
c     -----------------------------
      achaud=gettype(topk).eq.1
      kjac=topk-rhs+10-iskip
      if ( kjac.eq.topk.or.(achaud.and.kjac.eq.topk-2)) then 
         if (.not.getexternal(fname,topk,kjac,namjac,typej,
     $        setfj2)) return
         mf = mf+1
         jaco=.true.
      else
         jaco=.false.
         typej=.false.
         mf = mf + 2
      endif

c     other parameters 
c     -----------------
      itask=1
      istate=1
      iopt=0
c     hot restart case 
c     hot restart is detected when last argument is a matrix
c     ---------------------
C     space for result 
      if(.not.cremat(fname,topw,0,neq,nn,lres,lc)) return
      kresu=topw
      topw=topw+1
      nsizd=219
      nsizi=41
      if(achaud) then
c        iwork 
         if (.not.vcopyobj(fname,topk,topw)) return
         topw=topw+1
         if(.not.getrmat(fname,topk,topw-1,ml,nl,lci))return
         liwp= ml*nl
         ilc=iadr(lci)
         do 400 k= liwp -nsizi+1 , liwp
            write(06,*) k,'avant sauv iw',stk(lci+k-1)
 400     continue
c        rwork 
         if (.not.vcopyobj(fname,topk-1,topw)) return
         topw=topw+1
         if(.not.getrmat(fname,topk,topw-1,ml,nl,lrwp))return
         ilrw = ml*nl
         istate=2
c        restauration des commons
         lsavs=lrwp+ilrw- nsizd
         lsavi=lci+liwp- nsizi
         liwp1=liwp- nsizi
         call rscom1(stk(lsavs),stk(lsavi))
c        restauration du tableau entier
c        the end was used to restore the common 
         do 40 k=1,liwp
            istk(ilc+k-1)=int(stk(lci+k-1))
 40      continue
         
      else
c         ----create Work space 
         ilrw=0
         if(mf.gt.10) ilrw=22+16*neq+neq*neq
         if(mf.gt.20) ilrw=22+9*neq+neq*neq
         liwp=20+neq
         if(lhs.gt.1) then
            ilrw=ilrw+nsizd
            liwp=liwp+nsizi
         endif
         if(.not.cremat(fname,topw,0,1,liwp,li,lc)) return
         topw=topw+1
         ilc=iadr(li)
         do 1 k=1,liwp
            istk(ilc+k-1) =0
 1       continue
         if(.not.cremat(fname,topw,0,1,ilrw,lrwp,lc)) return
         do 11 k=1,liwp
            stk(lrwp+k-1) =0
 11      continue
         topw=topw+1
      endif
      if(jaco) then
         top=topw
         lw=lstk(top)
         ilw1=iadr(lw)
         istk(ilw1)=3
         istk(ilw1+1)=ilw1+4
         istk(ilw1+2)=ilw1+8
         istk(ilw1+3)=ilw1+12
         istk(ilw1+4)=kres
         istk(ilw1+5)=kttop
         istk(ilw1+6)=kynew
         istk(ilw1+7)=kydtop
         istk(ilw1+8)=kadd
         istk(ilw1+9)=kttop
         istk(ilw1+10)=kynew
         istk(ilw1+11)=kydtop
         istk(ilw1+12)=kjac
         istk(ilw1+13)=kttop
         istk(ilw1+14)=kynew
         istk(ilw1+15)=kydtop
         lstk(top+1)=sadr(ilw1+17)
      else
         top=topw
         lw=lstk(top)
         ilw1=iadr(lw)
         istk(ilw1)=2
         istk(ilw1+1)=ilw1+3
         istk(ilw1+2)=ilw1+7
         istk(ilw1+3)=kres
         istk(ilw1+4)=kttop
         istk(ilw1+5)=kynew
         istk(ilw1+6)=kydtop
         istk(ilw1+7)=kadd
         istk(ilw1+8)=kttop
         istk(ilw1+9)=kynew
         istk(ilw1+10)=kydtop
         lstk(top+1)=sadr(ilw1+11)
      endif
c     
      call xsetf(1)
      call xsetun(wte)
c     
c     appel de l'integrateur
      do 50 k=1,nn
         t1=stk(lt5 +k-1)
c        test sur le type des fonctions fournies
         if(typea) then
            if(typej) then
c     f fortran j fortran
               call lsodi(fres,fadda,fj2,neq,stk(ly),stk(lyd),t0,t1,
     1              itol,stk(lrtol),stk(latol),itask,istate,iopt,
     2              stk(lrwp),ilrw,istk(ilc),liwp,mf)
            else
c     f fortran j macro 
               call lsodi(fres,fadda,bj2,neq,stk(ly),stk(lyd),t0,t1,
     1              itol,stk(lrtol),stk(latol),itask,istate,iopt,
     2              stk(lrwp),ilrw,istk(ilc),liwp,mf)
            endif
         else
            if(typej) then
c     f macro j fortran
               call lsodi(bresid,badd,fj2,neq,stk(ly),stk(lyd),t0,t1,
     1              itol,stk(lrtol),stk(latol),itask,istate,iopt,
     2              stk(lrwp),ilrw,istk(ilc),liwp,mf)
            else
c     f macro j macro
               call lsodi(bresid,badd,bj2,neq,stk(ly),stk(lyd),t0,t1,
     1              itol,stk(lrtol),stk(latol),itask,istate,iopt,
     2              stk(lrwp),ilrw,istk(ilc),liwp,mf)
            endif
         endif
         if(err.gt.0) return
         if(istate.lt.0) then
            call error(24)
            return
         endif
         call unsfdcopy(neq,stk(ly),1,stk(lres+(k-1)*neq),1)
 50   continue
      top= topk-rhs+1
      call copyobj(fname,kresu,topk-rhs+1)
      if(lhs.eq.1) return
c     w
      if (lhs.ne.3) then 
         buf = fname // ' lhs can only be 1 or 2 '
         call error(999)
      endif
      top=top+1
      if (.not.cremat(fname,top,0,1,ilrw,lr,lc)) return
      call unsfdcopy(ilrw-nsizd,stk(lrwp),1,stk(lr),1)
      lsvs=lr+ilrw-nsizd
c     iw
      top=top+1
      if (.not.cremat(fname,top,0,1,liwp,lr,lc)) return
      do 60 k=1,liwp-nsizi
         stk(lr+k-1)=dble(istk(ilc+k-1))
 60   continue
      lsvi=lr+liwp-nsizi
      call svcom1(stk(lsvs),stk(lsvi))
      return
      end

      subroutine dassli(fname)
      character*(*) fname
c     ============================================
      INCLUDE '../stack.h'
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
c     .     info or jac ? get list size to decide
            il6=iadr(lstk(kjac))
            if (istk(il6).lt.0)  il6=istk(il6+1)
            if (istk(il6+1).eq.2) then
c     .        jac
               isres=13
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
      m8e3 =istk(il8e3+2)*istk(il8e3+2)
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
      m8e4 =istk(il8e4+2)*istk(il8e4+2)
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
      m8e5 =istk(il8e5+2)*istk(il8e5+2)
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
         istk(iadr(liwork+1))=mu
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
            call ddassl(bresd,n1,t0,stk(l1),stk(lydot),
     &           stk(lyri),info,stk(lrtol),stk(latol),idid,
     &           stk(lrwork),lrw,istk(iadr(liwork)),liw,stk(lw15),
     &           istk(il17),bjacd)
         endif
         if(err.gt.0)  return
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

