	subroutine cusum(n,w)
c    Utility fct: cumulated sum
c	Copyright INRIA
	double precision w(*),t
	t=0.0d0
	do 1 k=1,n
	w(k)=t+w(k)
	t=w(k) 
 1	continue
	end

