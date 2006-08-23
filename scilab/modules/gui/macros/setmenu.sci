// Allan CORNET INRIA 2005 
function setmenu(varargin)
  [lhs,rhs]=argn(0);
  
  select rhs,
    case 1 then setmenu_old(varargin(1)),
    case 2 then setmenu_old(varargin(1),varargin(2)),
    case 3 then setmenu_old(varargin(1),varargin(2),varargin(3)),
    else error("invalid parameters"),
  end

endfunction
