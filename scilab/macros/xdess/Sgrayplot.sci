function []=Sgrayplot(x,y,z, strf, rect, nax, zminmax, colminmax, mesh, colout)
// 
// PURPOSE
//    Like grayplot but the function fec is used to smooth the 
//    result assuming that the underlying function is linear on 
//    a set of triangles built from the grid (here with n1=5, n2=3):
//             _____________
//             | /| /| /| /|
//             |/_|/_|/_|/_| 
//             | /| /| /| /| 
//             |/_|/_|/_|/_|
//
// Copyright INRIA
// Modified by Bruno Pincon (14 oct 04) to have named argument working
// Some new modifs (Bruno Pincon, Feb 2005, demo + some checking + cleaning + add
// mesh and colout optionnal args)

	[lhs,rhs] = argn();
	 
	if rhs == 0 then   // demo
		title_demo = [
			'';
			'Demo of Sgrayplot()';
			'========================================';
			''];
		
		s_mat = ["t=-%pi:0.1:%pi";
			"m=sin(t)''*cos(t)";
			"xbasc()";
			"xset(''colormap'',jetcolormap(64))";
			"colorbar(-1,1)"
			"Sgrayplot(t,t,m,strf=''041'',zminmax=[-1,1])";
			"xtitle(''Sgrayplot demo f(x,y)=sin(x)*cos(y) on [-pi,pi]x[-pi,pi]'')"]
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	elseif rhs < 3 then
		error("bad number of input arguments")
	end

   // some checks
   if ~(type(x)==1 & isreal(x) & type(y)==1 & isreal(y) & type(z)==1 & isreal(z)) then 
      error("three first arguments must be real")
   end
   nx = length(x); ny = length(y); [p,q] = size(z)
   if p ~= nx | q ~= ny then
      error("third argument has incompatible dimensions with the two first")
   end
   
   // parsing the optionnal args
   opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"]
   opt_arg_seq = []
   for opt_arg = opt_arg_list
      if exists(opt_arg,"local") then
	 opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
      end
   end
   
   // build the datas for fec
   [noe_x,noe_y] = ndgrid(x,y)
   nbtri = 2*(p-1)*(q-1)
   num = (1:p*(q-1))'; num(p*(1:q-1)) = []; num1 = num+1
   connect =[(1:nbtri)' , [num   num1   num+p;...
	                   num1  num1+p num+p]  ,  zeros(nbtri,1)]
   
   // then plot
   if opt_arg_seq == [] then
      fec(noe_x,noe_y,connect,z)
   else
      execstr("fec(noe_x,noe_y,connect,z"+opt_arg_seq+")")
   end
endfunction
