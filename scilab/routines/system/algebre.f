c     Copyright INRIA
      integer function basetype(l)
      integer l
      basetype=mod(l,100)
      return
      end

      integer function algtype(m1,m2)
      integer m1,m2
      if (m1.eq.0) then
         algtype=m2
      else
         algtype=900+100*m1+m2
      endif
      return
      end

c      integer function algcode(l)
c      integer l
c      integer typecode
c      if (l.le.999) then
c         algcode=0
c      else
c         algcode=(l-900-typecode(l))/100
c      endif
c      return
c      end

      double precision function addid(l)
      integer l
      if (l.eq.1) then
         addid=-(10000.3**10000.3)
      elseif (l.eq.2) then
         addid=(10000.3**10000.3)
      else 
         addid=0.0d0
      endif
      return
      end

      double precision function peak(l)
      integer l
         if (l.eq.2) then
            peak=-(10000.3**10000.3)
         else
            peak=(10000.3**10000.3)
         endif
      return
      end

