function N=%r_i_hm(varargin)
  //author Serge Steer INRIA
  M=varargin($);
  N=varargin($-1);//inserted matrix
//  M(i,...)=N
  Den=ones(M);
  M(varargin(1:$-2))=N.num  
  Den(varargin(1:$-2))=N.den
  N.num=M
  N.den=Den

endfunction
