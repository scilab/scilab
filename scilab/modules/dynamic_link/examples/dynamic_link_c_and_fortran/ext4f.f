c This file is released under the 3-clause BSD license. See COPYING-BSD.

      subroutine ext4f(n,a,b,c)
c     example 4 (reading a chain)
c     -->link('ext4f.o','ext4f');
c     -->a=[1,2,3];b=[4,5,6];n=3;yes='yes'
c     -->c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     -->c=sin(a)+cos(b)
c     -->yes='no'
c     -->c=a+b
c     -->clear yes  --> undefined variable : yes

      double precision a(*),b(*),c(*)
      integer chmax
      logical creadchain
      parameter (chmax=10)
      character ch*(chmax)

c     If chain named yes exists reads it in ch else return
      lch=chmax
      if(.not.creadchain('yes'//char(0),lch,ch)) return
c     *********************************     
      if(ch(1:lch).eq.'yes') then
         do 1 k=1,n
            c(k)=sin(a(k))+cos(b(k))
 1       continue
      else
         do 2 k=1,n
            c(k)=a(k)+b(k)
 2       continue
      endif
      return
      end
      
