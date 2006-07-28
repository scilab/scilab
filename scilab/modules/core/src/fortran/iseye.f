C/MEMBR ADD NAME=ISEYE,SSI=0
c     Copyright INRIA
      logical function iseye(ib)
      integer ib(5),eye(5)
      data eye/14,34,14,41,42/
      iseye=.false.
      do 10 i=1,5
      if (ib(i).ne.eye(i)) return
 10   continue
      iseye=.true.
      return
      end
