      integer function  memused(it,mn)
      integer mn,it
      if(it.ne.0) then
         memused=int((mn*mod(it,10))/4)+1
      else
         memused=mn*2
      endif
      end
