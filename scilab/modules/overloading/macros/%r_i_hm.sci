function N=%r_i_hm(varargin)
  //author Serge Steer INRIA
  M=varargin($);
  N=varargin($-1);//inserted matrix
  //  M(i,...)=N,  M hypermatrix; N rational matrix
  Num=M;
  Den=ones(M);
  Num(varargin(1:$-2))=N.num
  Den(varargin(1:$-2))=N.den
 
  N.num=Num;
  N.den=Den;
endfunction
