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
    case 1
      ct=varargin(1);
      if and(size(ct) == [1,1]) then
        [c(1),c(2),c(3),c(4),c(5),c(6)] = datevec(ct);
   	c(3) = 1;
      else
        error('D must be scalar.');
      end   
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

  // Determine the week day for first day of the month.
  k = modulo(datenum(c(1),c(2),c(3))+5,7)+1;
    
  // Determine number of days in the month.
  dpm = [31 28 31 30 31 30 31 31 30 31 30 31];
  d = dpm(c(2));
  
  if (c(2) == 2) & ((modulo(c(1),4) == 0 & modulo(c(1),100) ~= 0) |  modulo(c(1),400) == 0) then
    d = 29;
  end
  // Fix in the matrix.
  x = zeros(7,6);
  x(k:k+d-1) = 1:d;
  cal = x';

  mths = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';
          'Aug';'Sep';'Oct';'Nov';'Dec'];
  mth = mths(c(:,2),:);
  
  listcal=list(sprintf('%s %d',mth,c(1)),'   S      M      Tu     W      Th     F      S',cal);
endfunction
//------------------------------------------------------------------------
