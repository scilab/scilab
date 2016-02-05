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
      subroutine calmaj(dh,n,g1,sig,w,ir,mk,epsmc,nfac)
c
      implicit double precision (a-h,o-z)
c     subroutine de qnbd
      dimension dh(*),g1(*),w(*)
      if(nfac.eq.n) go to 50
      nfac1=nfac+1
      nnfac=n-nfac
      n2fac=(nfac*nfac1)/2
      do 10 i=1,n
10    w(i)=g1(i)*sig
      k=n2fac
      if(nfac.eq.0)go to 25
      do 20 j=1,nfac
      do 20 i=nfac1,n
      k=k+1
      dh(k)=dh(k)+g1(i)*w(j)
20    continue
25    k=n2fac+nfac*nnfac
      do 30 j=nfac1,n
      do 30 i=j,n
      k=k+1
      dh(k)=dh(k) + g1(i)*w(j)
30    continue
50    ir=nfac
      if(nfac.eq.0)return
      call majour(dh,g1,w,nfac,sig,ir,mk,epsmc)
      return
      end
