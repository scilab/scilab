        integer function irow1(i,m)
c% calling sequence
c        integer function irow1(i,m)
c%purpose
c    this routine is only to be call from syhsc
c%
        integer i,m
        irow1=(i-1)*m-(i-2)*(i-3)/2
        if(i.eq.1) irow1=0
        return
        end
