      subroutine floqua(b,bprim,c,cprim,eps,ex,la2,lp2,ma,
     &     mm,n,or,phibar,phir,pile,piv,
     &     predw,sufval,type,waqc,flag)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),or(ma),ex(ma)
      dimension b(ma),c(ma),bprim(ma),cprim(ma)
      dimension predw(n),type(ma),pile(n)
      doubleprecision piv(n),sufval(n),ref,waqc(ma),phir(ma),delta,eps
      doubleprecision phibar(ma)
      precis=nint((log(eps)/log(2.))+.5)
      cmax=0
      flag=1
      do 50 u=1,ma
         if(iabs(b(u)).gt.cmax)cmax=iabs(b(u))
         if(iabs(c(u)).gt.cmax)cmax=iabs(c(u))
 50   continue
      ref=(alog(float(cmax))/alog(2.))+.5
      pbar=nint(ref)
      p=pbar
      do 60 u=1,ma
         phir(u)=0
 60   continue
      do 70 i=1,n
         piv(i)=0
 70   continue
 100  continue
      delta=2**float(p)
      do 120 u=1,ma
         if(p.gt.0)go to 118
         bprim(u)=b(u)
         cprim(u)=c(u)
         go to 120
 118     bb=b(u)/delta
         bprim(u)=bb*delta
         cc=c(u)/delta
         cc=delta*cc
         if(cc.lt.c(u))cc=cc+delta
         cprim(u)=cc
 120  continue
      call kiltq(b,c,coderr,delta,ex,la2,lp2,ma,mm,n,
     &     or,phibar,phir,pile,piv,predw,sufval,type,waqc)
      if (coderr .eq. 1) then
         call erro('internal dimension error')
         return
      endif
      if (coderr .eq. 2) then
         call out('no feasible solution')
         flag=0
         return
      endif
      if(p.eq.precis) return
      p=p-1
      go to 100
      end
