      subroutine kilter(b,c,ex,gamma,la2,lp2,ma,
     &     mm,n,or,phimi,pile,piv,predw,sufval,type,flag)
      implicit integer (a-z)
      dimension lp2(*),la2(mm)
      dimension or(ma),ex(ma),b(ma),c(ma)
      doubleprecision gamma(ma),piv(n),sufval(n)
      dimension phimi(ma),type(ma),predw(n),pile(n)
      doubleprecision infr,gammar,teta,eps,eps1,eps2,zero
      n1=n+1
      flag=1
      icon=0
      zero=0.00001
      infr=10.d6
      do 10 u=1,ma
         phimi(u)=0
 10   continue
      do 20 i=1,n
         piv(i)=0.0
 20   continue
 100  continue
      do 160 u=1,ma
         if(phimi(u).le.c(u)) goto 110
         type(u)=10
         goto 160
 110     if(phimi(u).ge.b(u)) goto 115
         type(u)=9
         goto 160
 115     if(c(u).ne.b(u)) goto 120
         type(u)=8
         goto 160
 120     teta=piv(ex(u))-piv(or(u))
         gammar=gamma(u)-teta
         if(abs(gammar).lt.zero) goto 130
         if(gammar.gt.0) goto 145
         if(phimi(u).lt.c(u)) goto 125
         type(u)=3
         goto 160  
 125     type(u)=6
         goto 160
 130     if(phimi(u).eq.c(u)) goto 135
         if(phimi(u).eq.b(u)) goto 140
         type(u)=2
         goto 160
 135     type(u)=5
         goto 160
 140     type(u)=4
         goto 160
 145     if(phimi(u).gt.b(u)) goto 150
         type(u)=1
         goto 160
 150     type(u)=7
 160  continue
      do 210 u=1,ma
         if(type(u).ne.6.and.type(u).ne.7.and.
     &        type(u).ne.9.and.type(u).ne.10) goto 210
         u0=u
         goto 300
 210  continue
      return
 300  continue
      if(type(u0).eq.7.or.type(u0).eq.10) goto 310
      if(type(u0).eq.6.or.type(u0).eq.9) goto 320
 310  continue
      i1=ex(u0)
      j1=or(u0)
      goto 350
 320  continue
      j1=ex(u0)
      i1=or(u0)
 350  continue
      call minty(coderr,ex,gamma,i1,infr,j1,la2,lp2,ma,
     &     mm,n,or,pile,piv,predw,sufval,type,u0)
      icon=icon+1
      if (coderr .eq. 1) then
         call erro('internal dimension error')
         return
      endif
      if (coderr .eq. 2 .or. icon .gt. ma+1) then
         call out('no feasible solution')
         flag=0
         return
      endif
      if(predw(i1).le.zero) goto 100
      eps1 = infr
      eps2 = infr
      jj=i1
      goto 405
 400  continue
      if(jj.eq.i1)goto 420
 405  continue
      u=predw(jj)
      if(jj.eq.ex(u)) goto 407
      goto 410
 407  if(type(u).ne.9)goto 408
      if((piv(ex(u))-piv(or(u))).gt.gamma(u))goto 408
      eps1=min(eps1,dble(b(u)-phimi(u)))
      goto 409
 408  eps1=min(eps1,dble(c(u)-phimi(u)))
 409  jj=or(u)
      goto 400
 410  continue
      if(type(u).ne.10)goto 412
      if((piv(ex(u))-piv(or(u))).lt.gamma(u))goto 412
      eps2=min(eps2,dble(phimi(u)-c(u)))
      goto 415
 412  eps2=min(eps2,dble(phimi(u)-b(u)))
 415  jj=ex(u)
      goto 400
 420  continue
      eps=min(eps1,eps2)
      jj=i1
      goto 435
 430  continue
      if(jj.eq.i1)goto 450
 435  continue
      u=predw(jj)
      if(jj.eq.or(u)) goto 440
      phimi(u)=phimi(u)+eps
      jj=or(u)
      goto 430
 440  continue
      phimi(u)=phimi(u)-eps
      jj=ex(u)
      goto 430
 450  continue
      goto 100
      end
