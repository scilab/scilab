       subroutine  fcalc10(ii,m1,n1,db,m2,n2,rea,m3,n3)
       integer ii(*) 
       double precision db(*) 
       real rea(*) 
       do 10 i=1,m1*n1
          ii(i) =i + ii(i) 
 10    continue
       do 20 i=1,m2*n2
          db(i) = dble(i) + db(i) 
 20    continue
       do 30 i=1,m3*n3
          rea(i) = real(i) + rea(i) 
 30    continue
       return
       end










