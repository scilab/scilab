      subroutine eclat(alphi,beta,c,ex,imin,ma,mm,modif,
     &     n,ndim,necl,nnn1,or,pimin,pivw,q,r,sat,x)
      implicit doubleprecision (a-h,o-z)
      dimension pivw(ndim),pimin(ndim)
      integer q(ndim),x(ma),sat(ndim)
      integer alphi(ndim),beta(ndim),or(ma),ex(ma)
      dimension c(ma),r(ndim),imin(ndim)
      logical modif
      doubleprecision infr
      infr=10.d5
      do 125 i=1,nnn1
         k=i
 105     if(alphi(k).eq.necl) goto 115
         if(alphi(k).eq.k) goto 115
         k=alphi(k)
         if(k.eq.0)goto 125
         goto 105
 115     beta(i)=k
 125  continue
      beta(necl)=0
      alphi(necl)=0
 135  do 145 i=1,nnn1
         if(alphi(i).eq.necl) alphi(i)=i
 145  continue
      do 165 i=1,nnn1
         sat(i)=0
 165  continue
      do 175 l=1,ma
         i=or(l)
         j=ex(l)
         ii=beta(i)
         jj=beta(j)
         if(ii.eq.jj) goto 175
         if(x(l).ne.0) sat(ii)=1
         if(x(l).ne.0) sat(jj)=1
 175  continue
      do 205 i=1,nnn1
         r(i)=infr
 205  continue
      do 290 i=1,n
         if(alphi(i).ne.i) goto 215
         r(i)=0
         goto 290
 215     continue
         r(i)=0
         ii=i
 225     continue
         i1=alphi(ii)
         if(i1.eq.ii) goto 265
         r(i)=r(i)+pivw(ii)-pimin(i1)
         if(r(i1).lt.infr) goto 255
         ii=i1
         goto 225
 255     continue
         r(i)=r(i)+r(i1)
 265     continue
         ii=i
 275     continue
         i1=alphi(ii)
         if(i1.ne.ii) goto 290
         if(r(i1).lt.infr)goto 290
         r(i1)=r(ii)-pivw(ii)+pimin(i1)
         ii=i1
         goto 275
 290  continue
      i1=imin(necl)
 300  l=q(i1)
      if(sat(i1).eq.1) goto 320
      i2=or(l)
      i2=beta(i2)
      ii=ex(l)
      ii=beta(ii)
      if(i2.eq.i1)i2=ii
      if(i2.eq.imin(necl)) goto 310
      i1=i2
      goto 300
 310  i1=imin(necl)
      sat(i1)=0
 320  k=i1
 330  l=q(i1)
      i2=or(l)
      i2=beta(i2)
      ii=ex(l)
      ii=beta(ii)
      if(i2.eq.i1) i2=ii
      x(l)=0
      if(i2.eq.k) goto 340
      sat(i2)=1
      l=q(i2)
      i3=or(l)
      i3=beta(i3)
      ii=ex(l)
      ii=beta(ii)
      if(i3.eq.i2) i3=ii
      x(l)=2
      sat(i3)=1
      i1=i3
      goto 330
 340  i1=imin(necl)
 350  l=q(i1)
      q(i1)=0
      i2=or(l)
      i2=beta(i2)
      ii=ex(l)
      ii=beta(ii)
      if(i2.eq.i1)i2=ii
      i1=i2
      if(i1.ne.imin(necl)) goto 350
      modif=.true.
      end
