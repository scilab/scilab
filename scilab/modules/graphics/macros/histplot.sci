function histplot(n,data,style,strf,leg,rect,nax,logflag,frameflag,axesflag,normalization)
// histplot(n,data,<opt_arg_seq>)
// draws histogram of entries in  data put into n classes
// 
// histplot(xi,data,<opt_arg_seq>)
// generates the histogram of entries in data put into classes
// [xi(1),xi(2)], (xi(k) xi(k+1)], k=2,..,n.
// xi's are assumed st. increasing (this point is verified now).
//
// optionnal args: 
//     1/ the same than for a plot2d:
//        style,strf,leg,rect,nax,logflag,frameflag,axesflag
//     2/ normalization flag (default value %t). When true the
//        histogram is normalized so that to approach a density:
//          xmax
//         /
//         |  h(x) dx = 1   (true if xmin <= min(data) and max(data) <= xmax) 
//         /
//         xmin
//
// Example : enter histplot()
// 
// Copyright INRIA
// modifs to use dsearch (Bruno Pincon 10/12/2001)
// others modifs from Bruno (feb 2005):
//    - may be a cleaner and shorter way to deal with optionnal arg ?
//    - now the histgram is drawn using only one polyline
//      (so properties (color, thickness,...) are easier 
//       to change with new graphics).
//    - removed computation of nax and rect if they are not
//      passed (let plot2d doing it)
//    - modify a little the demo
//    - add some checking on n|x and data
//
	[lhs,rhs]=argn()
	if rhs == 0 then   // demo
		
		title_demo = [
			'';
			'Demo of histplot()';
			'========================================';
			''];
		
		s_mat=[
			'histplot([-4.5:0.25:4.5],rand(1,20000,''n''),style=2);';
			'deff(''[y]=f(x)'',''y=exp(-x.*x/2)/sqrt(2*%pi);'');';
			'x=-4.5:0.125:4.5;x=x'';plot2d(x,f(x),26,'"000'");';
			'titre= ''macro histplot : (normalized) histogram plot'';';
			'xtitle(titre,''C (Classes)'',''N(C)/(Nmax length(C))'');';
			'legends([''gaussian random sample histogram'' ''exact gaussian density''],[2 26],1)'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end
   
   if rhs < 2 then
      error("wrong number of input arguments")
   end
   
   if ~(type(n) == 1 & isreal(n) & type(data) == 1 & isreal(data)) then
      error("first and second input arguments must be real") 
   end
   
   // this is the only specific optionnal argument for histplot
   if ~exists('normalization','local') then, normalization=%t,end
   
   // now parse optionnal arguments to be sent to plot2d
   opt_arg_seq = []
   opt_arg_list = ["style","strf","leg","rect","nax","logflag","frameflag","axesflag"]
   for opt_arg = opt_arg_list
      if exists(opt_arg,"local") then
	    opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
      end
   end
   
   p = length(data)
   if length(n) == 1 then  // the number of classes is provided
      if n~=floor(n) | n < 1 then
	 error("n must be a positive integer")
      end
      x = linspace(min(data), max(data), n+1);
   else                    // the classes are provided
      x = matrix(n,1,-1)   // force row form
      if min(diff(x)) <= 0 then
	 error("first input arg must have its components in increasing order")
      end
      n = length(x)-1
   end
   
   [ind , y] = dsearch(data, x)
   
   if normalization then y=y ./ (p*(x(2:$)-x(1:$-1))),end 
   
   // now form the polyline  
   //    X = [x1 x1 x2 x2 x2 x3 x3 x3  x4 ...   xn xn+1 xn+1]'
   //    Y = [0  y1 y1 0  y2 y2 0  y3  y3 ... 0 yn yn   0 ]'
   X = [x(1);x(1);matrix([1;1;1]*x(2:n),-1,1);x(n+1);x(n+1)] 
   Y = [matrix([0;1;1]*y,-1,1);0]

   if opt_arg_seq == [] then
      plot2d(X,Y)
   else
      execstr('plot2d(X,Y'+opt_arg_seq+")")
   end

endfunction
