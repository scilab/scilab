      subroutine dgsub(ii,z,dg)
c ======================================================================
C     Soft and Fortrans coded externals for colnew 
c ======================================================================
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr,sadr
      common/iercol/iero
      double precision z(*), dg(*)
      common / icolnew/  ncomp,mstar
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      integer ii
      logical allowptr
      data mlhs/1/,mrhs/2/

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' dgsub  top:'//buf(1:4))
      endif

      if(itdgsub.eq.10) then
c       Fortran case 
        call fcoldg(ii,z,dg)
        return
      endif
c     external is a Scilab function
c+ 
c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1

c     Putting Fortran arguments on Scilab stack 

      call ftob(dble(ii),1,ki)
      if(err.gt.0.or.err1.gt.0) return
      call ftob(z,mstar,kz)
      if(err.gt.0.or.err1.gt.0) return
c+    
      if(itdgsub.ne.15) then
         fin=lstk(kdgsub)
      else
         ils=iadr(lstk(kdgsub))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include '../callinter.h'
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
C     Scilab to Fortran convertion 
      call btof(dg,mstar)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end


      subroutine gsub(ii,z,g)
c ======================================================================
C     Soft and Fortrans coded externals for colnew 
c ======================================================================
      INCLUDE '../stack.h'
      integer iadr,sadr
      common/iercol/iero
      double precision z(*), g(*)
      common / icolnew/  ncomp,mstar
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      logical allowptr
      integer ii

      data mlhs/1/,mrhs/2/

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' gsub  top:'//buf(1:4))
      endif

      if(itgsub.eq.10) then
c       Fortran case 
        call fcolg(ii,z,g)
        return
      endif


c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1


c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(dble(ii),1,ki)
      if(err.gt.0.or.err1.gt.0) return
      call ftob(z,mstar,kz)
      if(err.gt.0.or.err1.gt.0) return
c+    
      if(itgsub.ne.15) then
         fin=lstk(kgsub)
      else
         ils=iadr(lstk(kgsub))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include '../callinter.h'
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
C     Scilab to Fortran convertion 
      call btof(g,1)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
c+    
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end

      subroutine dfsub(x,z,df)
c ======================================================================
C     Soft and Fortrans coded externals for colnew 
c ======================================================================
      INCLUDE '../stack.h'
      integer iadr,sadr
      common/iercol/iero
      double precision z(*), df(*),x
      common / icolnew/  ncomp,mstar
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      logical allowptr

      data mlhs/1/,mrhs/2/

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' dfsub  top:'//buf(1:4))
      endif

      if(itdfsub.eq.10) then
c       Fortran case 
        call fcoldf(x,z,df)
        return
      endif

c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1


c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(x,1,kx)
      if(err.gt.0.or.err1.gt.0) return
      call ftob(z,mstar,kz)
      if(err.gt.0.or.err1.gt.0) return
c+    
      if(itdfsub.ne.15) then
         fin=lstk(kdfsub)
      else
         ils=iadr(lstk(kdfsub))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include '../callinter.h'
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
C     Scilab to Fortran convertion 
      call btof(df,mstar*ncomp)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
c+    
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end


      subroutine fsub(x,z,f)
c ======================================================================
C     Soft and Fortrans coded externals for colnew 
c ======================================================================
      INCLUDE '../stack.h'
      integer iadr,sadr
      common/iercol/iero
      double precision z(*), f(*),x
      common / icolnew/  ncomp,mstar
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      logical allowptr

      data mlhs/1/,mrhs/2/

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' fsub  top:'//buf(1:4))
      endif

      if(itfsub.eq.10) then
c       Fortran case 
        call fcolf(x,z,f)
        return
      endif
c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1


c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(x,1,kx)
      if(err.gt.0.or.err1.gt.0) return
      call ftob(z,mstar,kz)
      if(err.gt.0.or.err1.gt.0) return
c+    
      if(itfsub.ne.15) then
         fin=lstk(kfsub)
      else
         ils=iadr(lstk(kfsub))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include '../callinter.h'
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
C     Scilab to Fortran convertion 
      call btof(f,ncomp)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
c+    
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end

      subroutine dguess(x,z,dmval)
c ======================================================================
C     Soft and Fortrans coded externals for colnew 
c ======================================================================
      INCLUDE '../stack.h'
      integer iadr,sadr
      common/iercol/iero
      double precision z(*), dmval(*),x
      common / icolnew/  ncomp,mstar
      common / coladr / kfsub,kdfsub,kgsub,kdgsub,kguess,kx,ki,kz
      common / coltyp / itfsub,itdfsub,itgsub,itdgsub,itguess
      logical allowptr

      data mlhs/2/,mrhs/1/

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' guess  top:'//buf(1:4))
      endif

      if(itguess.eq.10) then
c       Fortran case 
        call fcolgu(x,z,dmval)
        return
      endif
c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1


c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(x,1,kx)
      if(err.gt.0.or.err1.gt.0) return
c+    
      if(itguess.ne.15) then
         fin=lstk(kguess)
      else
         ils=iadr(lstk(kguess))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include '../callinter.h'
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
C     Scilab to Fortran convertion 
      call btof(dmval,ncomp)
      if(err.gt.0.or.err1.gt.0) return
      call btof(z,mstar)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
c+    
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end

