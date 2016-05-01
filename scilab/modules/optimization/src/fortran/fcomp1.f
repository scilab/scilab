c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine fcomp1(indic2,ibloc,indi,h,g,d,w,w1,n,nr,ncs,
     &dga,delta,prop,acc,scale)
c
      implicit double precision (a-h,o-z)
      dimension ibloc(n),indi(n),h(*),g(n),d(n),
     &w(n),w1(n),scale(n)
c
      ncs=0
      if(nr.eq.n) return
      zm=0.d0
      if(indic2.eq.1) go to 900
      delta=0.d0
      nh=nr*(nr+1)/2
      nrr=n-nr
      call fmlag1(n,nr,h,d,w)
      do 500 i=1,n
      ibi=ibloc(i)
      if(ibi.eq.0) go to 500
      gi=g(i)
      inc=indi(i)
      inc1=inc-1
      inr=inc-nr
      winc=w(inc)
      dmu=winc+gi
      am=dmin1(dabs(gi),dabs(dmu))
      if(2.d0*dabs(winc).ge.am) go to 500
      if(ibi.eq.-1.and.dmu.ge.0.d0) go to 500
      if(ibi.eq.1.and.dmu.le.0.d0) go to 500
      dmu=dabs(dmu)
      if(dmu*scale(i).le.acc) go to 500
      dmu1=dmu*dmu
      k=inr
      nh1=(inc1)*(n+1)-(inc1)*inc/2+1
      z=h(nh1)
      if(nr.eq.0) go to 350
      do 200 j=1,nr
      w1(j)=h(nh+k)
  200 k=k+nrr
      call fmc11e(h,nr,w1,w1,nr)
      k=inr
      do 250 j=1,nr
      z=z-w1(j)*h(nh+k)
  250 k=k+nrr
  350 dmu1=dmu1/z
      if(dmu1.le.delta) go to 500
      delta=dmu1
      ncs=i
      zm=dmu
  500 continue
      if(ncs.eq.0) return
      if(delta.le.-prop*dga)ncs=0
      return
  900 do 910 i=1,n
      ibi=ibloc(i)
      if(ibi.eq.0) go to 910
      dmu=g(i)
      if(ibi.eq.-1.and.dmu.ge.0.d0) go to 910
      if(ibi.eq.1.and.dmu.le.0.d0) go to 910
      dmu=dabs(dmu)*scale(i)
      if(dmu.le.zm) go to 910
      zm=dmu
      ncs=i
  910 continue
      if(zm.le.acc) ncs=0
      return
      end
