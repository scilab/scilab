//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
function listcal = calendar(varargin)
  lhs=argn(1);   
  rhs=argn(2);
  
  c=[0,0,0];
  
    select rhs
    case 0
      ct=getdate();
      c=[ct(1),ct(2),1]
    break
    case 2
      Y=varargin(1);
      M=varargin(2);
      if (size(Y) == [1,1]) & (size(M) == [1,1]) then
      else
        error('Y and M must be scalars.');
      end
      if (M < 1) | (M > 12) then error('M must contain values between 1 and 12.'),end
      c=[Y,M,1];
    break
  else
     error('Number of parameters incorrect.');
  end

  
  mths = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';
          'Aug';'Sep';'Oct';'Nov';'Dec'];
          
  mth = mths(c(:,2),:);
  cal=Calendar(c(2),c(1));
  listcal=list(sprintf('%s %d',mth,c(1)),'   M      Tu     W      Th     F     Sat     Sun',cal);
endfunction
//------------------------------------------------------------------------
