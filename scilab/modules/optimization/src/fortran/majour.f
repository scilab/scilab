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
      subroutine majour(hm,hd,dd,n,hno,ir,indic,eps)
c
      implicit double precision (a-h,o-z)
      dimension hm(*),hd(n),dd(n)
      if(n.eq.1) go to 100
c
      np=n+1
      if(hno.gt.0.0d+0) go to 99
c
      if(hno.eq.0.0d+0) go to 999
      if(ir.eq.0) go to 999
      hon=1.0d+0/hno
      ll=1
      if(indic.eq.0) go to 1
c
      do 2 i=1,n
      if(hm(ll).eq.0.0d+0) go to 2 
      hon=hon+dd(i)**2/hm(ll)
 2    ll=ll+np-i
      go to 3
c
 1    continue
      do 4 i=1,n
      dd(i)=hd(i)
 4    continue
      do 5 i=1,n
         iplus=i+1
         del=dd(i)
         if(hm(ll).gt.0.0d+0) go to 6
         dd(i)=0.0d+0
         ll=ll+np-i
         go to 5
 6       continue
         hon=hon+del**2/hm(ll)
         if(i.eq.n) go to 7
         do 8 j=iplus,n
            ll=ll+1
 8          dd(j)=dd(j)-del*hm(ll)
 7          ll=ll+1
 5    continue
c
 3    continue
      if(ir.le.0) go to 9
      if(hon.gt.0.0d+0) go to 10
      if ((indic-1) .le. 0) then 
         goto 99
      else
         goto 11
      endif
 9    continue
      hon=0.0d+0
      ir=-ir-1
      go to 11
 10   hon=eps/hno
      if(eps.eq.0.0d+0)ir=ir-1
 11   continue
      mm=1
      honm=hon
      do 12 i=1,n
      j=np-i
      ll=ll-i
      if(hm(ll).ne.0.0d+0) honm=hon-dd(j)**2/hm(ll)
      dd(j)=hon
 12   hon=honm
      go to 13
c
 99   continue
      mm=0
      honm=1.0d+0/hno
 13   continue
      ll=1
c
      do 98 i=1,n
         iplus=i+1
         del=hd(i)
         if(hm(ll).gt.0.0d+0) go to 14
         if(ir.gt.0) go to 15
         if(hno.lt.0.0d+0) go to 15
         if(del.eq.0.0d+0) go to 15
         ir=1-ir
         hm(ll)=del**2/honm
         if(i.eq.n) go to 999
         do 16 j=iplus,n
            ll=ll+1
 16         hm(ll)=hd(j)/del
         go to 999
 15      continue
         hon=honm
         ll=ll+np-i
         go to 98
 14      continue
         hml=del/hm(ll)
         if (mm .le. 0) then 
            goto 17
         else
            goto 18
         endif
 17      hon=honm+del*hml
         go to 19
 18      hon=dd(i)
 19      continue
         r=hon/honm
         hm(ll)=hm(ll)*r
         if(r.eq.0.0d+0) go to 20
         if(i.eq.n)go to 20
         b=hml/hon
         if(r.gt.4.0d+0) go to 21
         do 22 j=iplus,n
            ll=ll+1
            hd(j)=hd(j)-del*hm(ll)
 22         hm(ll)=hm(ll)+b*hd(j)
         go to 23
 21      gm=honm/hon
         do 24 j=iplus,n
            ll=ll+1
            y=hm(ll)
            hm(ll)=b*hd(j)+y*gm
 24         hd(j)=hd(j)-del*y
 23     continue
        honm=hon
        ll=ll+1
 98   continue
c
 20   continue
      if(ir.lt.0)ir=-ir
      go to 999
 100  continue
      hm(1)=hm(1)+hno *hd(1)**2
      ir=1
      if(hm(1).gt.0.0d+0) go to 999
      hm(1)=0.0d+0
      ir=0
 999  continue
      return
      end
