      integer function scierr()
c     returns 1 if  scilab error indicator err is positive and 0 in the
c     other cases
c     

c     Copyright INRIA
      include "../stack.h"
      if(err.gt.0) then
         scierr=1
      elseif(err1.gt.0.or.err2.gt.0) then
         scierr=2
      else
         scierr=0
      endif
      return
      end
