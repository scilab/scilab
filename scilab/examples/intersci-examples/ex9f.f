
       subroutine fcalc9(a,b,c,lc,d,nd)
       integer a(*),d(*)
       double precision b(*)
       character c*(*) 
       do 10 i=1,nd
          d(i)=2*d(i)
 10    continue
       do 20 i=1,lc
          if (c(i:i).eq.'u') c(i:i)='o'
 20    continue
       return
       end
