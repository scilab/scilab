// =============================================================================
// Copyright INRIA
//
// dec2hex(d) returns the hexadecimal representation corresponding to the
// numbers in vector v.
// =============================================================================

function h=dec2hex(v)

	[nr,nc] = size(v);
	
	if (find(v<0) <> []) | (norm(round(v)-v,1) > norm(round(v),1)*2*%eps) then
		error(msprintf(gettext("%s: Wrong values for first input argument: Non-negative integers expected.\n"),"dec2hex"));
	end
	
	v  = v(:);
	kz = find(v<>0)
	v  = v(kz);
	n  = 1 + fix(log(v)/log(16));
	s  = cumprod([1,16*ones(1,max(n)-1)]);
	z  = "0";
	h  = z(ones(nr*nc,1));
	
	for ii=1:size(v,'*')
		
		nn = n(ii);
		d  = v(ii);
		g  = s(1:nn);
		
		for i = 1:nn
			x=s(nn-i+1)
			g(i) = fix(d/x);
			d = d-g(i)*x;
		end
		
		k         = find(g>9);
		g(k)      = -g(k);
		h(kz(ii)) = code2str(g);
		
	end
	
	h=matrix(h,nr,nc);
	
endfunction
