c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     =======================================
      subroutine scicorr(fname)
c     implicit undefined (a-z)
      include 'stack.h'
      character*(*) fname
      logical checkrhs,getrmat,getscalar
      logical cremat,getexternal,getmat
      integer m1,n1,lr1,m2,n2,lr2,lr,top2, iadr,sadr
      integer topk,topw,gettype
      double precision yyy
c
      logical fort,type
      integer fini,update
c
      external dgetx,dgety,setdgetx,setdgety
      external bgetx,bgety
      character*(nlgh+1) namex,namey
      common / corrname / namex,namey
      common / corradr  / kgxtop,kgytop,ksec,kisc
      common / corrtyp /  itxcorr,itycorr
      common/  iercorr /iero
c
      data fini/15/,update/30/
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     calcul de correlations
      iero=0
      topk=top
      topw=top+1
      top2=top-rhs+1
      il1=iadr(lstk(top2))
      if(istk(il1).eq.10) then
         ich=il1+5+istk(il1+1)*istk(il1+2)
         ich=istk(ich)
         if(ich.eq.fini) goto 41
         if(ich.eq.update) goto 42
      endif
C     usual case [cov,mean]=corr(x,[y],nlags) 
C     ------------------------------------------
      if (.not.checkrhs(fname,2,3)) return
C     getting nlags (last argument)
      if(.not.getscalar(fname,topk,top,lr))return
      lag=int(stk(lr))
      if (.not.cremat(fname,topk+1,0,1,lag,lcxy,lc)) return
      if (.not.cremat(fname,topk+2,0,1,2,lrxy,lc)) return

      if(rhs.eq.3) then
         top=top-1
         if (.not.getrmat(fname,topk,top,m2,n2,lr2)) return
         ny= m2*n2
         top=top-1
         if (.not.getrmat(fname,topk,top,m1,n1,lr1)) return
         nx= m1*n1
         if(nx.ne.ny) then
            call error(60)
            return
         endif
      endif
      if(rhs.eq.2) then
         top=top-1
         if (.not.getrmat(fname,topk,top,m1,n1,lr1)) return
         nx= m1*n1
         lr2=lr1
      endif
      call tscccf(stk(lr1),stk(lr2),nx,stk(lcxy),stk(lrxy),
     $     lag,ierr)
      if(ierr.eq.-1) then
         buf='too many coefficients are required'
         call error(999)
         return
      endif
      top=topk-rhs+1
      call copyobj(fname,topk+1,top)
      if(lhs.eq.2) then
         top=top+1
         if (.not.cremat(fname,top,0,1,rhs-1,lr,lc)) return
         call unsfdcopy(rhs-1,stk(lrxy),1,stk(lr),1)
      endif
      return
C     ----------------------
C     Special case using fft 
C       [cov,mean]=corr('fft',xmacro,[ymacro],n,sect)
C     ----------------------
 41   continue
c     methode de la fft
      if (.not.checkrhs(fname,4,5)) return
c     -------last argument (sect)
      if(.not.getscalar(fname,topk,top,lr))return
      lag=int(stk(lr))
      mm=2*lag
c     --------total size 
      kntop=top-1
      if(.not.getscalar(fname,topk,kntop,lr))return
      n=int(stk(lr))
c     -------external for xmacro 
      kgxtop= top-rhs+2 
      itxcorr=gettype(kgxtop)
      if (.not.getexternal(fname,topk,kgxtop,namex,fort,
     $     setdgetx)) return
      mode=2
c     -------external for ymacro 
      if(rhs.eq.5) then
         kgytop= top-rhs+3
         itycorr=gettype(kgytop)
         if (.not.getexternal(fname,topk,kgytop,namey,type,
     $     setdgety)) return
         if( type.neqv.fort) then
            err=3
            call error(55)
            return
         endif
         mode=3
      endif
c     Working areas 
      if (.not.cremat(fname,top+1,0,1,mm,lxa,lc)) return
      if (.not.cremat(fname,top+2,0,1,mm,lxr,lc)) return
      if (.not.cremat(fname,top+3,0,1,mm,lxi,lc)) return
      if (.not.cremat(fname,top+4,0,1,sadr(mm),lzr,lc)) return
      if (.not.cremat(fname,top+5,0,1,sadr(mm),lzi,lc)) return
      top=top+6
      ksec=top
      if (.not.cremat(fname,ksec,0,1,1,lr,lc))return
      kisc=top+1
      if (.not.cremat(fname,kisc,0,1,1,lr,lc))return
      if(fort) then
         call cmpse2(mm,n,mode,dgetx,dgety,stk(lxa),stk(lxr),
     1        stk(lxi),stk(lzr),stk(lzi),ierr)
         if(ierr.gt.0) then
            buf='fft call : needs power of two!'
            call error(999)
            return
         endif
      else
         call cmpse2(mm,n,mode,bgetx,bgety,stk(lxa),stk(lxr),
     1        stk(lxi),stk(lzr),stk(lzi),ierr)
         if(err.gt.0.or.err1.gt.0) return
         if(ierr.gt.0) then
            buf='fft call : needs a power of 2'
            call error(999)
            return
         endif
      endif
      top=topk-rhs+1
      if (.not.cremat(fname,top,0,1,lag,lr,lc)) return
      call unsfdcopy(lag,stk(lxa),1,stk(lr),1)
      if(lhs.eq.1) return
      top=top+1
      if (.not.cremat(fname,top,0,1,rhs-3,lr,lc)) return
      call unsfdcopy(rhs-3,stk(lxr),1,stk(lr),1)
      return
 42   continue
C     ------------------------
C     special case with Updates 
C     [w,xu]=corr('updt',x1,[y1],w0) etc....
      ichaud=0
      mode=0
c     -------------x1 value 
      top2=top2+1
      if (.not.getrmat(fname,topk,top2,mx,nx,lx)) return
      mnx=mx*nx
c     --------------y1 value or w0
      top2=top2+1
      if (.not.getmat(fname,topk,top2,it,my,ny,ly,lyc)) return 
      mny=ny*my
      if(mny.eq.mnx) then
         mode=1
         if ( it.eq.1) then 
            buf= fname // ' y1 should be a real matrix'
            call error(999)
            return
         endif
      endif
c     --------------third or second argument 
      if(mode.eq.1) top2=top2+1
      if (.not.getmat(fname,topk,top2,it,mz,nz,lzr,lzi)) return 
      mfft=mz*nz
C     if w0 is real we add space for storing an imag part 
      if (it.eq.0) then
         if (.not.cremat(fname,topw,0,1,mfft,lzi,lc)) return
         topw=topw+1
         call dset(mfft,0.d0,stk(lzi),1)
      endif
      if(mode.eq.1.and.rhs.eq.5) ichaud=1
      if(mode.eq.0.and.rhs.eq.4) ichaud=1
c     -----------last argument + working areas 
      if (.not.cremat(fname,topw,1,1,mfft,lwr,lwi)) return
      call dset(2*mfft,0.d0,stk(lwr),1)
      if(ichaud.eq.1) then
         top2=top2+1
         if (.not.getmat(fname,topk,top2,it,mx0,nx0,
     $        lxr,lxi))   return 
         nbx=mx0*nx0
         if ( nbx.gt.mfft) then 
            buf = fname // 'last argument has wrong size '
            call error(999)
            return
         endif
         call unsfdcopy(nbx,stk(lxr),1,stk(lwr),1)
      endif
C     ---------calling the function 
      if(mode.eq.1) then
         call cmpse3(mfft,mnx,mode,stk(lx),stk(ly),stk(lwr),stk(lwi),
     1        stk(lzr),stk(lzi),ierr,ichaud,nbx)
      endif
      if(mode.eq.0) then
         call cmpse3(mfft,mnx,mode,stk(lx),yyy,stk(lwr),stk(lwi),
     1        stk(lzr),stk(lzi),ierr,ichaud,nbx)
      endif
C     ---------back to Scilab
      if(ierr.gt.0) then
         buf='fft call: needs a power of 2'
         call error(999)
         return
      endif
      if (lhs.eq.2) then 
C        keep this in a safe place before the copy for lhs1
         call unsfdcopy(nbx,stk(lx+mnx-nbx),1,stk(lwr),1)
      endif
      if(lhs.ge.1) then
         top=topk-rhs+1
         if (.not.cremat(fname,top,1,mz,nz,lres,lresi)) return
         call unsfdcopy(mfft,stk(lzr),1,stk(lres),1)
         call unsfdcopy(mfft,stk(lzi),1,stk(lresi),1)
      endif
      if(lhs.eq.2) then
         top=top+1
         if (.not.cremat(fname,top,0,1,nbx,lres,lresi)) return
         call unsfdcopy(nbx,stk(lwr),1,stk(lres),1)         
      endif
      return
      end
c     =======================================


