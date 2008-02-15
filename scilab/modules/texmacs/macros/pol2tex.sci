// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1989 - INRIA - Serge STEER
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tt]=pol2tex(a)
	
	// Latex source code for the scilab polynomial matrix a.
	// (For use with texprint)
	//

	
	p  = string(coeff(a));
	z  = varn(a);
	lz = length(z);
	
	while part(z,lz) == ' ' then
		lz = lz-1;
	end
	
	z  = part(z,1:lz);
	
	np = prod(size(p));
	ok = " ";
	tt = " ";
	
	//coeff  degree 0
	
	if part(p(1),1)<>'-' then
		if p(1)<>'0' then
			tt = tt+ok+p(1);
			ok = "+";
		end
	else
		tt = tt+p(1);
		ok = "+";
	end
	
	if np==1 then
		if ok==' ' then
			tt='0';
		end
		return;
	end
	
	//coeff  degree 1
	
	if part(p(2),1)<>'-' then
		
		select p(2)
		case "0",
		case "1" then
			tt = tt+ok+z;
			ok = "+";
		else
			tt = tt+ok+p(2)+"\*"+z
			ok = "+";
		end
		
	else
		
		if p(2)=='-1' then
			tt=tt+"-"+z;
		else
			tt=tt+p(2)+"\*"+z;
		end
		
		ok="+";
		
	end
	
	//other coefficients
	for i=3:np
	
		if part(p(i),1)<>'-' then
			select p(i)
			case "0",
			case "1" then
				tt = tt+ok+z+'^{'+string(i-1)+'}';
				ok = "+";
			else
				tt = tt+ok+p(i)+"\*"+z+"^{"+string(i-1)+"}";
				ok = "+";
			end
		else
			ok = "+"
			if p(i)=="-1" then
				tt=tt+"-"+z+"^{"+string(i-1)+"}"
			else
				tt=tt+p(i)+"\*"+z+"^{"+string(i-1)+"}"
			end
		end
		
	end
	
	if ok == " " then
		tt="0";
	end
	
endfunction
