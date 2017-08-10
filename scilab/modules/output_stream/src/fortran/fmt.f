c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine fmt(a,maxc,typ,n1,n2)
c!but
c     ce sous programme determine le format d'ecriture le plus court
c     representant au mieux le nombre positif a
c!liste d'appel
c     subroutine fmt(a,maxc,typ,n1,n2)
c
c     double precision a
c     integer maxc,typ,n1,n2
c
c     a : nombre positif
c     maxc : nombre maximum de caracteres utilisables pour representer
c            a
c     typ : type du format
c           typ=0 le nombre n'est pas representable
c           typ=1 le format est dn1.n2
c           typ=2 le format est fn1.n2
c     n1,n2 : entiers definissant les champs des formats
c
      double precision a,a1,ent,dec,sciround,dlamch
      integer typ,n1,n2,maxc
      logical v
c
C     testing Nans
      v=.false.
      if (isanan(a).eq.1) v=.true.
      if(v) then
         typ=-2
         return
c     testing Inf
      elseif(a.gt.dlamch('o')) then
         typ=-1
         return
      endif
      if(maxc-3.le.0) goto 30
      if(a.lt.1.0d+0) goto 20
      a1=mod(a,10.0d+0)
      ent=a-a1+int(a1)
c      ent=dble(int(a))
      dec=a-ent
      if (abs(1.0d0-dec).le.10.0d0*a*dlamch('e')) then
         ent=ent+1.0d0
         dec=a-ent
      endif


      ndgt=int(log10(ent+0.4))+1
      if(ndgt.lt.0) ndgt=maxc
      if(ndgt.le.maxc-2) goto 10
      if(maxc-7.lt.0) goto 30
      typ=1
      n1=maxc
      n2=maxc-7
      return
   10 n1=ndgt+2
      typ=2
      ndgt=maxc-n1
      n2=0
      a1=dec*10.0d+0**(ndgt+1)
c      dec=(a1-mod(a1,10.0d+0))/10.0d+0
      dec=sciround(a1/10.0d+0)
      if(dec.eq.0.0d+0) return
      n2=ndgt
   11 if(mod(dec,10.0d+0).ne.0.0d+0) goto 12
      n2=n2-1
      dec=dec/10.0d+0
      goto 11
   12 n1=n1+n2
      return
c
   20 ndgt=0
      if(a.eq.0.0d+0) goto 26
      m=1-log10(a)
      ndgt=maxc-3+m

      if(m.gt.maxc-3) goto 25
      dec=sciround(a*10.0d+0**ndgt)
   21 if(mod(dec,10.0d+0).ne.0.0d+0) goto 22
      dec=dec/10.0d+0
      ndgt=ndgt-1
      goto 21
   22 if(ndgt.le.maxc-3) goto 26
      n1=maxc-3
      n2=min(maxc-7,ndgt-m)
      if(n1.ge.n2) goto 26
   25 if(maxc-7.lt.0) goto 26
      typ=1
      n1=maxc
      n2=maxc-7
      return
   26 typ=2
      n2=min(ndgt,maxc-3)
      n1=n2+3
      return
   30 typ=2
      n1=maxc
      n2=0
      return
      end
