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
        error(gettext('errors','time_error_20'));
      end
      if (M < 1) | (M > 12) then error(gettext('errors','time_error_21'));,end
      c=[Y,M,1];
    break
  else
     error(gettext('errors','time_error_22'));
  end

  
  mths = [gettext('messages','time_message_17'); ..
	gettext('messages','time_message_18'); ..
	gettext('messages','time_message_19'); ..
	gettext('messages','time_message_20'); ..
	gettext('messages','time_message_21'); ..
	gettext('messages','time_message_22'); ..
	gettext('messages','time_message_23'); ..
	gettext('messages','time_message_24'); ..
	gettext('messages','time_message_25'); ..
	gettext('messages','time_message_26'); ..
	gettext('messages','time_message_27'); ..
	gettext('messages','time_message_28')];
          
  mth = mths(c(:,2),:);
  cal=Calendar(c(2),c(1));
  listcal=list(sprintf('%s %d',mth,c(1)),gettext('messages','time_message_29'),cal);
endfunction
//------------------------------------------------------------------------
