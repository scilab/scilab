function [al,be,Z]=gspec(A,B)
//For backward compatibility
warning('Obsolete function, replace gspec by spec')
[al,be,Z]=spec(A,B)
endfunction
