      subroutine int3d
C     --------------------------------------------
c     Scilab int3d 
c      implicit undefined (a-z)
c     Copyright INRIA
      include '../stack.h'
      character*(5) fname
      character*(nlgh+1) namef
      integer iero 
      common/ierajf/iero
      external bint3d,fint3d
      double precision epsabs,epsrel,result,abserr
      logical getexternal,getrmat,type ,cremat,checkval,getscalar
      integer topk,lr,katop,kydot,top2,lra,lrb,lc,gettype,top0
c
      integer iadr,sadr
      external setfint3d
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      fname='int3d'
      if(rhs.lt.4) then
         call error(39)
         return
      endif
      type=.false.
      top0=top-rhs+1
      topk=top
      if(rhs.eq.6) then
c      [minpts,maxpts,epsabs,epsrel]
         if (.not.getrmat(fname,topk,top,m1,n1,lr))  return
         if(.not.checkval(fname,m1*n1,4)) return
         minpts=max(int(stk(lr)),0)
         maxpts=max(int(stk(lr+1)),0)
         epsabs=max(stk(lr+2),0.0d0)
         epsrel=max(stk(lr+3),0.0d0)
         if(maxpts.lt.minpts) then
            err= 5
            call error(36)
         endif
         top=top-1
      else
         minpts=0
         maxpts=1000
         epsabs=0.0d0
         epsrel=1.D-5
      endif
c
      if(gettype(top).eq.1) then
         if(.not.getscalar(fname,topk,top,lnum)) return
         numfun=stk(lnum)
         top=top-1
      else
         numfun=1
      endif
      top2=top
      if (.not.getexternal(fname,topk,top,namef,type,setfint3d)) return
      kext=top
      top=top-1
      
      if (.not.getrmat(fname,topk,top,mz,nz,lz))  return
      if(.not.checkval(fname,mz,4)) return

      top=top-1
      if (.not.getrmat(fname,topk,top,my,ny,ly))  return
      if(.not.checkval(fname,my,4)) return
      top=top-1
      if (.not.getrmat(fname,topk,top,mx,nx,lx))  return
      if(.not.checkval(fname,mx,4)) return

      if(.not.checkval(fname,nx,ny)) return
      if(.not.checkval(fname,nx,nz)) return
      numtet=nx

      maxpts=max(maxpts,43*numtet)

      top=top2
c     form matrix ver
      maxsub = 7*(maxpts-43*numtet)/(8*43) + numtet
      lenver=maxsub
      top=top+1
      if (.not.cremat(fname,top,0,3,4*lenver,lver,lc)) return
      call unsfdcopy(4*numtet,stk(lx),1,stk(lver),3)
      call unsfdcopy(4*numtet,stk(ly),1,stk(lver+1),3)
      call unsfdcopy(4*numtet,stk(lz),1,stk(lver+2),3)

c     allocate matrix for result and abserr
      top=top+1
      if (.not.cremat(fname,top,0,numfun,2,lres,lc)) return

c     form arg shape for external
      top=top+1
      kxyz=top
      if (.not.cremat(fname,top,0,3,1,lxyz,lc)) return

c     tableaux de travail 
      mdiv=1
      nw=maxsub*(2*numfun+1) + 7*max(8*mdiv,numtet)*numfun + 1
      top=top+1
      if (.not.cremat(fname,top,0,1,nw,lwork,lc)) return

c     tableau de travail entier necessaire 
      niw=lenver + mdiv
      top=top+1
      if (.not.cremat(fname,top,0,1,iadr(niw)+1,liwork,lc)) return

c
c     external scilab
c
      top=top+1
      ipal=iadr(lstk(top))
      istk(ipal)=1
      istk(ipal+1)=ipal+2
      istk(ipal+2)=kext
      istk(ipal+3)=kxyz
      lstk(top+1)=sadr(ipal+4)

      irestar=0
      if(type) then
         call dcutet(fint3d,numfun,stk(lver),numtet,minpts,maxpts,epsabs
     $        ,epsrel,lenver,nw,irestar,stk(lres),stk(lres+numfun),neval
     $        ,ifail,stk(lwork),stk(liwork))
      else
         call dcutet(bint3d,numfun,stk(lver),numtet,minpts,maxpts,epsabs
     $        ,epsrel,lenver,nw,irestar,stk(lres),stk(lres+numfun),neval
     $        ,ifail,stk(lwork),stk(liwork))
      endif
      if(err.gt.0)return
      if(ifail.gt.0) then
         if(ifail.eq.1) then
c     MAXPTS was too small to obtain the required accuracy
            call msgs(85,0)
         elseif(ifail.eq.3) then
c     volume of one of the  given tetrahedrons is zero
            buf='volume of one of the  given tetrahedrons is zero'
            call error(999)
         else
            buf='inexpected error: please report'
            call error(999)
         endif
      endif
      top=top0
      if (.not.cremat(fname,top,0,numfun,1,lr1,lc)) return
      call unsfdcopy(numfun,stk(lres),1,stk(lr1),1)

      if(lhs.ge.2) then
         top=top+1
         if (.not.cremat(fname,top,0,numfun,1,lr2,lc)) return
         call unsfdcopy(numfun,stk(lres+numfun),1,stk(lr2),1)
      endif
      if(lhs.eq.3) then
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr2,lc)) return
         stk(lr2)=neval
      endif
      return
      end

