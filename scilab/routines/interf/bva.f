      subroutine bva(fname)
c
C      implicit undefined (a-z)
c     Copyright ENPC (Jean-Philippe Chancelier )
c     -----------------------------------------------------
      include '../stack.h'
      character*(*) fname
      character*(nlgh+1)   efsub,edfsub,egsub,edgsub,eguess
      integer    kfsub,kdfsub,kgsub,kdgsub,kguess,topk
      external   fsub,dfsub,gsub,dgsub,dguess
      external setfcolgu,setfcoldg,setfcolg,setfcoldf
      external setfcolf
      integer    mstar,ncomp,io
      double precision aleft,aright
      integer iadr,lr,iflag,mf,nf,lfixpnt,mtol,ntol,ltol,l
      integer mltol,nltol,lltol,iero,mipar,nipar,lipar,mzeta,nzeta,lzeta
      integer mm,mn,lrm,i,lispace,lspace,lc,ki,kz,kx,lr1,lc1
      integer mres,nres,lres
      integer itfsub,itdfsub,itgsub,itdgsub,itguess,gettype
      logical type,getexternal,getrmat,cremat,getscalar
      common/iercol/iero
C     External names 
      common / colname / efsub,edfsub,egsub,edgsub,eguess
C     External Position in stack and arguments model position in stack
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
C     Type of externals 
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      common / icolnew/  ncomp,mstar
c
      iadr(l)=l+l-1

c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' bva '//buf(1:4))
      endif
c
c     fin  1
c         bvode
c
c     z=bvode(res,ncomp,m,aleft,aright,zeta,ipar,ltol,tol,fixpnt,...
c	fsub1,dfsub1,gsub1,dgsub1,guess1)
c
c     Interface for the colnew program for boundary values problem.
      type=.false.
      topk=top
      kguess=top
c     guess1 external
      itguess= gettype(top)
      if (.not.getexternal(fname,topk,top,eguess,type,
     $     setfcolgu)) return
      top=top-1
c     dgsub1 external
      itdgsub=gettype(top)
      kdgsub=top
      if (.not.getexternal(fname,topk,top,edgsub,type,
     $     setfcoldg)) return
      top=top-1
c     gsub1 external
      itgsub=gettype(top)
      kgsub=top
      if (.not.getexternal(fname,topk,top,egsub,type,
     $     setfcolg)) return
      top=top-1
c     dfsub1 external
      itdfsub=gettype(top)
      kdfsub=top
      if (.not.getexternal(fname,topk,top,edfsub,type,
     $     setfcoldf)) return
      top=top-1
c     fsub1 external
      itfsub=gettype(top)
      kfsub=top
      if (.not.getexternal(fname,topk,top,efsub,type,
     $     setfcolf)) return
c      write(06,*) 'args',itfsub,itdfsub,itgsub,itdgsub,itguess
      top=top-1
c     fixpnt
      if (.not.getrmat(fname,topk,top,mf,nf,lfixpnt))  return
      top=top-1
c     tol
      if (.not.getrmat(fname,topk,top,mtol,ntol,ltol))  return
      top=top-1
c     ltol
      if (.not.getrmat(fname,topk,top,mltol,nltol,lltol))  return
      call entier(mltol*nltol,stk(lltol),istk(iadr(lltol)))
      top=top-1
c     ipar  
      if (.not.getrmat(fname,topk,top,mipar,nipar,lipar))  return
      if(mipar*nipar.lt.11) then 
c     .  bvode: ipar dimensioned at least 11
         call error(251) 
      endif
      ilipar=iadr(lipar)
      call entier(mipar*nipar,stk(lipar),istk(ilipar))
c
      if(istk(ilipar+3).ne.mltol*nltol) then 
C     .  bvode: ltol must be of size ipar(4)
         call error(252) 
      endif
      if(istk(ilipar+10).ne.mf*nf.and.istk(ilipar+10).ne.0) then 
c     .  bvode: fixpnt must be of size ipar(11)
         call error(253) 
      endif
      top=top-1
c     zeta 
      if (.not.getrmat(fname,topk,top,mzeta,nzeta,lzeta))  return
      top=top-1
c     aright  
      if (.not.getscalar(fname,topk,top,lr))  return
      aright=stk(lr)
      top=top-1
c     aleft
      if (.not.getscalar(fname,topk,top,lr))  return
      aleft=stk(lr)
      top=top-1
c     m
      if (.not. getrmat(fname,topk,top,mm,mn,lrm)) return 
      call entier(mm*mn,stk(lrm),istk(iadr(lrm)))
      mstar=0
      do 10 i=1,mm*mn
         mstar=mstar+ istk(iadr(lrm)+i-1)
 10   continue
      top=top-1
c     ncomp
      if (.not.getscalar(fname,topk,top,lr))  return
      ncomp=int(stk(lr))
      if(ncomp.gt.20) then 
c     .  bvode: ncomp < 20 requested 
         call error(254) 
      endif
      if(mm*mn.ne.ncomp) then 
c     .  bvode: m must be of size ncomp
         call error(255) 
      endif
      if(mstar.gt.40) then 
c     .  bvode: sum(m must be less than 40
         call error(256) 
      endif

      top=top-1
c     res
      if (.not.getrmat(fname,topk,top,mres,nres,lres))  return
c
c     create working arrays
      top=topk+1
      if (.not.cremat(fname,top,0,1,istk(iadr(lipar)+6-1),lispace,lc)) 
     $     return
      top=top+1
      if (.not.cremat(fname,top,0,1,istk(iadr(lipar)+5-1),lspace,lc)) 
     $     return
C     Modele des arguments des external x scalaire z vecteur 
      top=top+1
      ki=top
      kx=top
      if (.not.cremat(fname,top,0,1,1,lr,lc)) return
      top=top+1
      kz=top
      if (.not.cremat(fname,top,0,mstar,1,lr,lc)) return
      iero=0
      call colnew (ncomp,istk(iadr(lrm)),aleft,aright,stk(lzeta),
     $     istk(iadr(lipar)),istk(iadr(lltol)), stk(ltol),stk(lfixpnt),
     $     istk(iadr(lispace)), stk(lspace), iflag, fsub, 
     $             dfsub, gsub, dgsub, dguess) 
      if(err.gt.0.or.err1.gt.0) return
      if(iero.gt.0) then
         call error(24)
         Return
      endif
      if ( iflag.ne.1) then 
         goto (101,102,103,104) iflag+4
 101     call error(258)
         return 
 102     call error(24)
         return
 103     call error(259)
         return
 104     call error(260)
         return
      endif
      top=top+1
      if (.not.cremat(fname,top,0,mstar,mres*nres,lr,lc)) return
         do 20 i=1,mres*nres
            call appsln(stk(lres+i-1),stk(lr+(i-1)*mstar),stk(lspace),
     $           istk(iadr(lispace)))
 20      continue
      top=topk-rhs+1
      if (.not.cremat(fname,top,0,mstar,mres*nres,lr1,lc1)) return
      call unsfdcopy(mstar*mres*nres,stk(lr),1,stk(lr1),1)
      return
      end

