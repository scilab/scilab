      subroutine gdcp2i(n, itab, m)
c!purpose
c       decomposition of an integer n in a base tw0.
c       n=a1+a2*2+a3*2**2+.........+am*2**(m-1).
c!calling sequence
c     subroutine gdcp2i(n,itab,m)
c     integer n,itab,m
c
c        n     : integer to be decomposed (n.le.32767)
c
c        itab  :logical vector of dimension 15.
c               in output: if(a(i-1).ne.0)then itab(i)=.true.
c               else itab(i)=.false.
c
c        m     :the number of itab elements to be consider in output.
c
c!originator
c  j. hanen -september 1978-ensm-nantes.
c!
c
      dimension ipow2(15)
      logical itab(*)
c
      data ipow2(1), ipow2(2), ipow2(3), ipow2(4), ipow2(5),
     * ipow2(6), ipow2(7), ipow2(8), ipow2(9), ipow2(10),
     * ipow2(11), ipow2(12), ipow2(13), ipow2(14), ipow2(15)
     * /16384,8192,4096,2048,1024,512,256,128,64,32,16,8,4,2,1/
c
      m = 0
      k = 15
      nn = abs(n)
      if (nn.gt.32767) nn = mod(nn,32767)
      do 30 i=1,15
         if (nn.lt.ipow2(i)) go to 10
         itab(k) = .true.
         nn = nn - ipow2(i)
         if (m.eq.0) m = k
         go to 20
   10    itab(k) = .false.
   20    k = k - 1
   30 continue
c
      return
      end
