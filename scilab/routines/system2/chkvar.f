      logical function chkvar(v1,v2)
c     compare polynomials formal names
c     Copyright INRIA
      integer v1(4),v2(4)
      logical test
      
      if(v1(1).eq.0.or.v2(1).eq.0) then
         test=.true.
      else
         test=.false.
         do 10 i=1,4
            if(v1(i).ne.v2(i)) goto 20
 10      continue
         test=.true.
      endif
 20   chkvar=test
      return
      end
      
     
      
