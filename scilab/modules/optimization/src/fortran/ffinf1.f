      subroutine ffinf1 (n,nv,jc,xpr,p,s)
      implicit double precision (a-h,o-z)
      dimension jc(nv),p(*),s(n),xpr(nv)
c
c          cette subroutine calcule s = sigma xpr(.)*p(.)
c          sachant que les xpr ont ete calcules par fprf2
c
      do 920 i=1,n
      ps=0.
      do 910 k=1,nv
      j=jc(k)-1
      if(j.eq.0) go to 910
      nij=(j-1)*n+i
      ps=ps+xpr(k)*p(nij)
  910 continue
  920 s(i)=ps
      return
      end
