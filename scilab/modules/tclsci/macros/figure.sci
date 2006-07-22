// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function h=figure(varargin)
	
  Rhs=length(varargin);
  if (Rhs == 0) then
    h=TCL_scf(0);
  else
 		if (Rhs == 1) then
 		  if (IsAScalar(varargin(1))==%T) then
    		h=TCL_scf(varargin(1));
 		  else
 		    error('figure : invalid value type',999);
 		  end
 		else
 		  if (modulo(Rhs,2)==0) then
 		    // nbr params paire
 		    if (IsAScalar(varargin(1))==%T) then
 		      error('figure : invalid value type',999) 
 		    else
 		      h=TCL_scf(0);
					for i=1:2:Rhs
					  if ( type(varargin(i+1)) <> GetUiControlValueType(GetUiControlPropertyName(varargin(i))) ) then
					    if ( GetUiControlValueType(GetUiControlPropertyName(varargin(i))) == 10) then
					      error(string(varargin(i+1))+' Value must be a string.',999);
					    else
					      error(string(varargin(i+1))+' Value must be numeric.',999);
					    end
					  else
					    set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
					  end
					end
 		    end
 		  else
 		    // nbr params impaire
 		    if (IsAScalar(varargin(1))==%T) then
 		      h=TCL_scf(varargin(1));
    		  for i=2:2:Rhs
					   if ( type(varargin(i+1)) <> GetUiControlValueType(GetUiControlPropertyName(varargin(i))) ) then
					     if ( GetUiControlValueType(GetUiControlPropertyName(varargin(i))) == 10) then
					       error(string(varargin(i+1))+' Value must be a string.',999);
					     else
					       error(string(varargin(i+1))+' Value must be numeric.',999);
					     end
					   else
					     set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
					   end
					end
 		    else
 		      error('figure : invalid value type',999) ;
 		    end
 		  end
 		end
  end
  
 endfunction
 // -----------------------------------------------------------
 