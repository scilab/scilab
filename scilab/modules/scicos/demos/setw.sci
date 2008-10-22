function y=setw(x)
 mu=size(x,1)
 y(1:mu,1:mu)=0
 for j=1:mu
     for i=1:mu-j+1
	 y(i,j)=x(i+j-1)
     end
 end
endfunction