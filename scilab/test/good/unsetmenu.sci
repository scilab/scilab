// Allan CORNET INRIA 2005 
function unsetmenu(varargin)
  [lhs,rhs]=argn(0);
  
  select rhs,
    case 1 then unsetmenu_old(varargin(1)),
    case 2 then unsetmenu_old(varargin(1),varargin(2)),
    case 3 then unsetmenu_old(varargin(1),varargin(2),varargin(3)),
    else error("invalid parameters"),
  end
  
endfunction
