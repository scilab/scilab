// Allan CORNET INRIA 2004
// clipboard Copy and paste strings to and from system clipboard.
function s=clipboard(varargin)

if MSDOS then
	rhs=argn(2)

	if ( (rhs == 1) & (varargin(1) == 'paste') ) then
		s=ClipBoard(varargin(1));
	else
		if ( (rhs == 2) & (varargin(1) == 'copy') ) then
			param=sci2exp(varargin(2));
			ClipBoard(varargin(1),param);
			abort;
		else
			if ( (rhs == 2) & (varargin(1) == 'do') ) then
				if ( (varargin(2) == 'copy') | (varargin(2) == 'paste') | (varargin(2) == 'empty') ) then
					ClipBoard(varargin(1),varargin(2));
					abort;
				else
					error('invalid parameter(s)');
				end
			else
 				error('invalid parameter(s)');
			end
		end
	end
else
	error('Only for Windows');
end

endfunction
