      subroutine lstops
c ==================================
c     elementary operations on lists
c ==================================
c
c     Copyright INRIA
      include '../stack.h'
c
c
      integer insert,extrac
      data insert/2/,extrac/3/

c     handle recursion
      if(rstk(pt).eq.403.or.rstk(pt).eq.405) goto 10
      if(rstk(pt).eq.406.or.rstk(pt).eq.407) goto 50
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lstops '//buf(1:4))
      endif
c

      if(fin.eq.extrac) goto 10
      if(fin.eq.insert) goto 50
c
c     undefined operations (look for function-defined operation)
      icall = 0
      fun   = 0
      fin   = -fin
      return
c
c     extraction
 10   continue
      call intl_e()
      return
c
c     insertion
 50   continue
      call intl_i()
      if(err.gt.0) return
c      if(rstk(pt).eq.407) goto 50
      return
c
      end

      integer function strpos(ptr,ns,chars,str,n)
      integer ptr(ns+1),ns,chars(*),str(n),n
      do 10 i=1,ns
         i1=ptr(i)
         i2=ptr(i+1)
         if(i2-i1.eq.n) then
            do 05 j=1,n
               if(str(j).ne.chars(i1-1+j)) goto 10
 05         continue
            strpos=i
            return
         endif
 10   continue
      strpos=0
      return
      end



