      double precision function infinity(x)
c Purpose
c     Return infinity value	
c Calling sequence
c     a = infinity(0.0d)
c     Copyright INRIA
      double precision x
      
      infinity = 1.0d0/x
      
      return 
      end      
