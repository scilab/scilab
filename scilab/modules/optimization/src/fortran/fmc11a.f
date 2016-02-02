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
      subroutine fmc11a(a,n,z,sig,w,ir,mk,eps)
      implicit double precision (a-h,o-z)
      dimension a(*),z(n),w(n)
c   update factors given in a by   sig*z*ztranspose
      if(n.gt.1)goto1
      a(1)=a(1)+sig *z(1)**2
      ir=1
      if(a(1).gt.0.d0)return
      a(1)=0.d0
      ir=0
      return
    1 continue
      np=n+1
      if(sig.gt.0.d0)goto40
      if(sig.eq.0.d0.or.ir.eq.0)return
      ti=1.d0/sig
      ij=1
      if(mk.eq.0)goto10
      do 7 i=1,n
      if(a(ij).ne.0.d0)ti=ti+w(i)**2/a(ij)
    7 ij=ij+np-i
      goto20
   10 continue
      do 11 i=1,n
   11 w(i)=z(i)
      do 15 i=1,n
      ip=i+1
      v=w(i)
      if(a(ij).gt.0.d0)goto12
      w(i)=0.d0
      ij=ij+np-i
      goto15
   12 continue
      ti=ti+v**2/a(ij)
      if(i.eq.n)goto14
      do 13 j=ip,n
      ij=ij+1
   13 w(j)=w(j)-v*a(ij)
   14 ij=ij+1
   15 continue
   20 continue
      if(ir.le.0 )goto21
      if(ti.gt.0.d0)goto22
      if ((mk-1) .le. 0) then 
         goto 40
      else
         goto 23
      endif
   21 ti=0.d0
      ir=-ir-1
      goto23
   22 ti=eps/sig
      if(eps.eq.0.d0)ir=ir-1
   23 continue
      mm=1
      tim=ti
      do 30 i=1,n
      j=np-i
      ij=ij-i
      if(a(ij).ne.0.d0)tim=ti-w(j)**2/a(ij)
      w(j)=ti
   30 ti=tim
      goto41
   40 continue
      mm=0
      tim=1.d0/sig
   41 continue
      ij=1
      do 66 i=1,n
      ip=i+1
      v=z(i)
      if(a(ij).gt.0.d0)goto53
      if(ir.gt.0 .or.sig.lt.0.d0.or.v.eq.0.d0)goto52
      ir=1-ir
      a(ij)=v**2/tim
      if(i.eq.n)return
      do 51 j=ip,n
      ij=ij+1
   51 a(ij)=z(j)/v
      return
   52 continue
      ti=tim
      ij=ij+np-i
      goto66
   53 continue
      al=v/a(ij)
      if (nm .le. 0) then
         goto 54
      else
         goto 55
      endif
   54 ti=tim+v*al
      goto56
   55 ti=w(i)
   56 continue
      r=ti/tim
      a(ij)=a(ij)*r
      if(r.eq.0.d0)goto70
      if(i.eq.n)goto70
      b=al/ti
      if(r.gt.4.d0)goto62
      do 61 j=ip,n
      ij=ij+1
      z(j)=z(j)-v*a(ij)
   61 a(ij)=a(ij)+b*z(j)
      goto64
   62 gm=tim/ti
      do 63 j=ip,n
      ij=ij+1
      y=a(ij)
      a(ij)=b*z(j)+y*gm
   63 z(j)=z(j)-v*y
   64 continue
      tim=ti
      ij=ij+1
   66 continue
   70 continue
      if(ir.lt.0)ir=-ir
      return
      end
