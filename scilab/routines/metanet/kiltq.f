      subroutine kiltq(b,c,coderf,delta,ex,la2,lp2,ma,mm,n,
     &     or,phibar,phir,pile,piv,predw,sufval,type,waqc)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),or(ma),ex(ma)
      dimension b(ma),c(ma),type(ma)
      dimension pile(n),predw(n)
      doubleprecision piv(n),sufval(n),waqc(ma)
      doubleprecision infr,delta,phir(ma),gamma1,gamma2,teta
      doubleprecision ep,eps,eps1,eps2,zero,ref,min,phibar(ma)
      icon=0
      zero=0.00001
      infr=10.d6
 100  continue
      do 160 u=1,ma
         type(u)=11
         if(phir(u).le.c(u))go to 110
         type(u)=10
         go to 160
 110     if(phir(u).ge.b(u))go to 115
         type(u)=9
         go to 160
 115     if(c(u).ne.b(u)) goto 120
         type(u)=8
         goto 160
 120     teta=piv(ex(u))-piv(or(u))
         gamma1=waqc(u)*(phir(u)+delta*.5-phibar(u))
         gamma2=waqc(u)*(phir(u)-delta*.5-phibar(u))
         if(abs(phir(u)-b(u)).ge.zero) go to 130
         if((teta-gamma1).gt.zero)type(u)=6
         if((gamma1-teta).gt.zero)type(u)=1
         if(abs(teta-gamma1).le.zero)type(u)=4
         go to 160
 130     if(abs(phir(u)-c(u)).ge.zero)go to 140
         if((teta-gamma2).gt.zero)type(u)=3
         if((gamma2-teta).gt.zero)type(u)=7
         if(abs(teta-gamma2).le.zero)type(u)=5
         go to 160
 140     if((teta-gamma1).gt.zero)type(u)=6
         if((gamma2-teta).gt.zero)type(u)=7
         if(abs(teta-gamma2).le.zero)type(u)=51
         if(abs(teta-gamma1).le.zero)type(u)=41
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
 310  i1=ex(u0)
      j1=or(u0)
      goto 330
 320  continue
      j1=ex(u0)
      i1=or(u0)
 330  continue
      call mintyq(b,c,coderf,delta,ex,i1,infr,j1,la2,lp2,ma,
     &     mm,n,or,phibar,phir,pile,piv,predw,
     &     sufval,type,u0,waqc)
      icon=icon+1
      if(icon .gt. ma+1) then
         coderf=2
         return
      endif
      if(coderf.ne.0) return
      if(predw(i1).le.zero) go to 100
      eps1 = infr
      eps2 = infr
      jj=i1
      goto 403
 400  continue
      if(jj.eq.i1)goto 420
 403  continue
      u=predw(jj)
      teta=piv(ex(u))-piv(or(u))
      if(jj.eq.ex(u)) go to 405
      go to 410
 405  if(type(u).ne.9)go to 406
      if(teta.gt.(waqc(u)*(b(u)+delta*.5-phibar(u))))go to 406
      eps1=min(eps1,(b(u)-phir(u)))
      go to 409
 406  k=1
 407  ref=waqc(u)*((k-1/2)*delta+b(u)-phibar(u))+zero
      if(teta.le.ref)go to 408
      k=k+1
      go to 407
 408  ep=k*delta+b(u)-phir(u)
      eps1=min(eps1,ep)
 409  jj=or(u)
      goto 400
 410  continue
      if(type(u).ne.10)go to 412
      if(teta.lt.(waqc(u)*(c(u)-delta*.5-phibar(u))))goto 412
      eps2=min(eps2,(phir(u)-c(u)))
      goto 415
 412  k=1
 413  ref=waqc(u)*((k-1/2)*delta+b(u)-phibar(u))+zero
      if(teta.lt.ref)go to 414
      k=k+1
      goto 413
 414  ep=phir(u)-(k-1)*delta-b(u)
      eps2=min(eps2,ep)
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
      if(jj.eq.or(u)) go to 440
      phir(u)=phir(u)+eps
      jj=or(u)
      goto 430
 440  continue
      phir(u)=phir(u)-eps
      jj=ex(u)
      goto 430
 450  continue
      goto 100
      end
