        integer function irow2(i,m)
c%calling sequence
c        integer function irow2(i,m)
c
c%purpose
c      this routine is only to be called from syhsc
c%
        integer i,k,m
        irow2=2*(i-1)*m
        k=(i-4)*(i-3)/2
        if(i.le.2) k=0
        irow2=irow2-k
        return
        end
