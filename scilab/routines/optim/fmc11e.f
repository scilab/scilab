      subroutine fmc11e(a,n,z,w,ir)
c   multiply a vector z by the inverse of the factors given in a
      implicit double precision (a-h,o-z)
      dimensiona(*),z(n),w(n)
      if(ir.lt.n)return
      w(1)=z(1)
      if(n.gt.1)goto400
      z(1)=z(1)/a(1)
      return
  400 continue
      do 402 i=2,n
      ij=i
      i1=i-1
      v=z(i)
      do 401 j=1,i1
      v=v-a(ij)*z(j)
  401 ij=ij+n-j
      w(i)=v
  402 z(i)=v
      z(n)=z(n)/a(ij)
      np=n+1
      do 411 nip=2,n
      i=np-nip
      ii=ij-nip
      v=z(i)/a(ii)
      ip=i+1
      ij=ii
      do 410 j=ip,n
      ii=ii+1
  410 v=v-a(ii)*z(j)
  411 z(i)=v
      return
      end
