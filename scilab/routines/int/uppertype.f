      integer function uppertype(ita,itb)
      if(ita.eq.0.or.itb.eq.0) then
         it=0
      else
         it=min(int(ita/10),int(itb/10))*10+max(mod(ita,10),mod(itb,10))
      endif
      uppertype=it
      return
      end
