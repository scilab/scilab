        integer function lrow2(i,m)
c%calling sequence
c        integer function lrow2(i,m)
c% purpose
c      this routine is only to be called from syhsc
c%
        integer i,k,m
        k=i-3
        if(i.le.2) k=0
        lrow2=2*m-k
        if(i.le.2) lrow2=2*m
        return
        end
