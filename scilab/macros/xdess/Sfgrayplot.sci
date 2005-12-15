function []=Sfgrayplot(x, y, f, strf, rect, nax, zminmax, colminmax, mesh, colout)

	// PURPOSE
	//    Like fgrayplot but the function fec is used to smooth the 
	//    result assuming that the underlying function is linear on 
	//    a set of triangles built from the grid (here with n1=5, n2=3):
	//             _____________
	//             | /| /| /| /|
	//             |/_|/_|/_|/_| 
	//             | /| /| /| /| 
	//             |/_|/_|/_|/_|
	//
	// Copyright INRIA
	// rewritten by Bruno Pincon (17 april 05) (based of my modifs of Sgrayplot)

	[lhs,rhs]=argn()
	if rhs == 0 then   // demo
		
		title_demo = [
			'';
			'Demo of Sfgrayplot()';
			'========================================';
			''];
		
		s_mat=[
			'deff(''[z]=Surf(x,y)'',''z=x.^3+y'');';
			'xbasc()';
			'xset(""colormap"",jetcolormap(64))';
			'colorbar(-2,2)';
			'Sfgrayplot(-1:0.1:1,-1:0.1:1,Surf,strf=""031"",rect=[-1,-1,1,1]);';
			'xtitle(''Sfgrayplot demo: f(x,y)=x^3+y'')'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	elseif rhs < 3 then
		error("bad number of input arguments");
	end
  
	// some checks
	if ~(type(x)==1 & isreal(x) & type(y)==1 & isreal(y)) then
		error("two first arguments must be real");
	end
	if type(f)~=11 & type(f)~=13 then 
		error("third argument must be a scilab function");
	end
	
	p = length(x); q = length(y);
	
	// parsing the optional args
	opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"];
	opt_arg_seq = [];
	for opt_arg = opt_arg_list
		if exists(opt_arg,"local") then
			opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg;
		end
	end
	
	// build the datas for fec
	z = feval(x,y,f);
	[noe_x,noe_y] = ndgrid(x,y)
	nbtri = 2*(p-1)*(q-1)
	num = (1:p*(q-1))'; num(p*(1:q-1)) = []; num1 = num+1
	connect =[(1:nbtri)' , [num   num1   num+p;...
		num1  num1+p num+p]  ,  zeros(nbtri,1)]
	
	// then plot
	if opt_arg_seq == [] then
		fec(noe_x,noe_y,connect,z);
	else
		execstr("fec(noe_x,noe_y,connect,z"+opt_arg_seq+")");
	end
endfunction
