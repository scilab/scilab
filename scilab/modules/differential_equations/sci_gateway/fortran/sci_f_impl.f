c 
c  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c  Copyright (C) INRIA
c  
c  This file must be used under the terms of the CeCILL.
c  This source file is licensed as described in the file COPYING, which
c  you should have received as part of this distribution.  The terms
c  are also available at    
c  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c 
c 
   		subroutine sciimpl(fname)
c     ==================================================
      INCLUDE 'stack.h'
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
c     
      data atol/1.d-7/,rtol/1.d-9/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,6,10)) return
C     XXXXXX : pour l'instant 
      if (.not.checklhs(fname,1,3)) return
c     ---------------------------------
      ierror=0
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
         if(err.gt.0.or.err1.gt.0) return
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
