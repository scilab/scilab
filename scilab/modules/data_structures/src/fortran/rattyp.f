c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine rattyp(ivt,job)
c     set the type ['r','num','den','dt']
      integer ivt(*)
c
      character*(*) job
      if(job.eq.'size') then
         ivt(1)=18
      else
         ivt(1)=10
         ivt(2)=1
         ivt(3)=4
         ivt(4)=0
         ivt(5)=1
         ivt(6)=2
         ivt(7)=5
         ivt(8)=8
         ivt(9)=10
         ivt(10)=27
         ivt(11)=23
         ivt(12)=30
         ivt(13)=22
         ivt(14)=13
         ivt(15)=14
         ivt(16)=23
         ivt(17)=13
         ivt(18)=29
      endif
      return
      end
