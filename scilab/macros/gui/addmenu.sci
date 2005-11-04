// Allan CORNET INRIA 2005 
function addmenu(varargin)
  [lhs,rhs]=argn(0);
  
  select rhs,
    case 1 then addmenu_old(varargin(1)),
    case 2 then addmenu_old(varargin(1),varargin(2)),
    case 3 then addmenu_old(varargin(1),varargin(2),varargin(3)),
    case 4 then addmenu_old(varargin(1),varargin(2),varargin(3),varargin(4)),
    else error("invalid parameters"),
  end

endfunction
