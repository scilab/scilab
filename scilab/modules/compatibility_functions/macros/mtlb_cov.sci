function Y=mtlb_cov(A,B,C)
// Copyright INRIA
// Emulation function for Matlab cov()
// F.Belahcene

[lhs,rhs]=argn()
if rhs==1 
	if size(A,1)==1 & size(A,2)==1 
		Y = mvvacov(A)
	elseif size(A,1)==1 & size(A,2)>1
		N = size(A,2)
		Y = (N/(N-1))*mvvacov(A')
	elseif size(A,1)>1
		N = size(A,1)
	 	Y = (N/(N-1))*mvvacov(A)
	end
elseif rhs==2 
	if  prod(size(B,"*"))==1 & and(B==1)
		if size(A,1)==1 & size(A,2)==1 
			Y = mvvacov(A)
		elseif size(A,1)==1 & size(A,2)>1
			Y = mvvacov(A')
		elseif size(A,1)>1
		 	Y = mvvacov(A)
		end
	elseif prod(size(B,"*"))==1 & and(B==0)
		if size(A,1)==1 & size(A,2)==1 
			Y = mvvacov(A)
		elseif size(A,1)==1 & size(A,2)>1
			N = size(A,2)
			Y = (N/(N-1))*mvvacov(A')
		elseif size(A,1)>1
			N = size(A,1)
		 	Y = (N/(N-1))*mvvacov(A)
		end
	elseif prod(size(B,"*"))==1
		B = matrix(B,-1,1)
		A = matrix(A,-1,1)
		Y = mvvacov([A,B])
	elseif prod(size(B,"*"))>1
		N = prod(size(B,"*"))
		B = matrix(B,-1,1)
		A = matrix(A,-1,1)
		Y = (N/N-1)*mvvacov([A,B])
	end
elseif rhs==3
	B = matrix(B,-1,1)
	A = matrix(A,-1,1)
	if  C==1
		Y = mvvacov([A,B])
	elseif C==0
		N = prod(size(B,"*"))
		if N<>1
			Y = (N/(N-1))*mvvacov([A,B])	
		else
			Y = mvvacov([A,B])
		end
	end
end
endfunction