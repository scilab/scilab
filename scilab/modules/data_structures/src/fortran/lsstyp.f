      subroutine lsstyp(ivt,job)
c     set the type
      integer ivt(*)
      character*(*) job
      if(job.eq.'size') then
         ivt(1)=23
      else
         ivt(1)=10
         ivt(2)=1
         ivt(3)=7
         ivt(4)=0
         ivt(5)=1
         ivt(6)=4
         ivt(7)=5
         ivt(8)=6
         ivt(9)=7
         ivt(10)=8
         ivt(11)=10
         ivt(12)=12
         ivt(13)=21
         ivt(14)=28
         ivt(15)=28
         ivt(16)=-10
         ivt(17)=-11
         ivt(18)=-12
         ivt(19)=-13
         ivt(20)=-33
         ivt(21)=0
         ivt(22)=13
         ivt(23)=29
      endif
      return
      end

