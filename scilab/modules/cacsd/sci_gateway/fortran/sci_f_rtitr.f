c     --------------------------
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
c
      subroutine scirtitr(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      double precision eps
      integer job,ido
      integer iadr,sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(top+1-rhs+lhs.ge.bot) then
         call error(18)
         return
      endif
      eps=0.1*sqrt(stk(leps))
c
      err=0
c
c
c      systeme defini par sa representatio rationnelle n./d
c
c     y=rtitr(n,d,u [,u0,y0])
c
c     n  matrice polynomes  nd*nu
c     d  matrice polynomes  nd*nd
c     u  matrice scalaire   nu*ido
c     u0 matrice scalaire   nu*maxigd
c     y0 matrice scalaire   nd*maxigd
c     y  matrice scalaire   nd*ido
c
c     maxign= le plus grand degre de polynomes de matrice n
c     maxigd= le plus grand degre de polynomes de matrice d
c
c     verification des quantites de parametres d entree et de sortie
c
      if (lhs .ne. 1) then
         call error(39)
         return
      endif
      if (rhs .ne. 3 .and. rhs .ne. 5) then
         call error(39)
         return
      endif
      lw=lstk(top+1)
      job=1
      if (rhs.eq.5) then
        iy0=iadr(lstk(top))
        if(istk(iy0).ne.1) then
           err=5
           call error(53)
           return
        endif
        if(istk(iy0+3).ne.0) then
           err=5
           call error(52)
           return
        endif
c
        iu0=iadr(lstk(top-1))
        if(istk(iu0).ne.1) then
           err=4
           call error(53)
           return
        endif
        if(istk(iu0+3).ne.0) then
           err=4
           call error(52)
           return
        endif
        top=top-2
        job=2
      endif
c
      in=iadr(lstk(top-2))
      if(istk(in).gt.2) then
         err=1
         call error(54)
         return
      endif
      if(istk(in+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      nx=istk(in+1)
      n1=istk(in+2)
      if(istk(in).eq.1) then
        maxign=0
        ln=sadr(in+4)
                        else
        ilpn=in+8
        call mpdegr(istk(ilpn),nx,maxign,nx,n1)
        ln=sadr(in+9+nx*n1)
      endif
c
      id=iadr(lstk(top-1))
      if(istk(id).gt.2) then
         err=2
         call error(54)
         return
      endif
      if(istk(id+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      nd=istk(id+1)
      if(nd.ne.istk(id+2)) then
         err=2
         call error(20)
         return
      endif
      if(istk(id).eq.1) then
        maxigd=0
        ld=sadr(id+4)
                        else
        ilpd=id+8
        call mpdegr(istk(ilpd),nd,maxigd,nd,nd)
        ld=sadr(id+9+nd*nd)
      endif
c
      iu=iadr(lstk(top))
      if(istk(iu).ne.1) then
         err=3
         call error(54)
         return
      endif
      if(istk(iu+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      top=top-2
      nu=istk(iu+1)
      mu=istk(iu+2)
c
      if(nd.ne.nx.or.n1.ne.nu) then
         call error(60)
         return
      endif
c
      ido=mu+maxigd-maxign
c
      lu=sadr (iu+4)
      lr=lw
c
      lw=lr+nd*ido
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      ly0=lw
      lu0=lw
      if (rhs .eq. 5) then
         if (istk(iy0+1).ne.istk(id+2).and.istk(iy0+1).ne.0) then
            call error(60)
            return
         endif
         if (istk(iy0+2) .ne. maxigd) then
            call error(60)
            return
         endif
         ly0=sadr(iy0+4)
c
         if (istk(iu0+1).ne.istk(in+2).and.istk(iu0+1).ne.0) then
            call error(60)
            return
         endif
         if (istk(iu0+2) .ne. maxigd) then
            call error(60)
            return
         endif
         lu0=sadr(iu0+4)
      endif
c
      if(nd.eq.1.and.nu.eq.1) then
c     cas siso
      call rtitr(1,1,mu,stk(ln),1,maxign,stk(ld),1,maxigd,stk(lu0),
     &           stk(lu),1,stk(ly0),stk(lr),1,job,iw,stk(lw),ierr)
c
      else
c     cas general
      ld1=lw
      ln1=ld1+nd*nd*(maxigd+1)*(istk(id)-1)
      lw=ln1+nd*nu*(maxign+1)*(istk(in)-1)
      ilw=iadr(lw+nd)
      err=sadr(ilw+nd)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     conversion matrice de polynome --> polynome matriciel
      if(istk(id).eq.2) then
      call dmp2pm(stk(ld),istk(ilpd),nd,stk(ld1),maxigd,nd,nd)
                        else
      ld1=ld
      endif
      if(istk(in).eq.2) then
      call dmp2pm(stk(ln),istk(ilpn),nd,stk(ln1),maxign,nd,nu)
                        else
      ln1=ln
      endif
c
      call rtitr(nu,nd,mu,stk(ln1),nd,maxign,stk(ld1),nd,maxigd,
     &           stk(lu0),stk(lu),nu,stk(ly0),stk(lr),nd,job,
     &           istk(ilw),stk(lw),ierr)
      endif
c
      if (ierr.eq.1)then
c     matrice est quasi singuliere ou mal normalisee
         write(buf(1:13),'(1pd13.4)') stk(lw)
         call msgs(5,0)
      endif
      if (ierr .eq. 2) then
         call error(19)
         return
      endif
c
c     rangement du resultat
c
      iy=iadr(lstk(top))
      istk(iy)=1
      istk(iy+1)=nd
      istk(iy+2)=ido
      istk(iy+3)=0
      ly=sadr(iy+4)
      call unsfdcopy(ido*nd,stk(lr),1,stk(ly),1)
      lstk(top+1)=ly+nd*ido
99    return
      end
c     ----------------------------
