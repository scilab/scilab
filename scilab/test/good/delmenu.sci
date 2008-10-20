// Allan CORNET INRIA 2005 
function delmenu(varargin)
  [lhs,rhs]=argn(0);
  
  select rhs,
    case 1 then delmenu_old(varargin(1)),
    case 2 then delmenu_old(varargin(1),varargin(2)),
    else error("invalid parameters"),
  end

endfunction
