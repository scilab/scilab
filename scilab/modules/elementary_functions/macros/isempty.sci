function answ=isempty(m)
	
	// Copyright INRIA . 1998  1
	
	m_type = type(m);
	
	if( (type(m) >= 11) & (type(m) <= 13) | (type(m) >= 128) ) then
		error(246);
	end
	
	select m_type
	case 10
		// matrix of character string
		answ = ( max(length(m)) == 0 );
		
	case 15
		// list
		answ = %t;
		for i=1:size(m),
			answ = (answ & isempty(m(i)));
		end;
		
	case 16
		// typed list
		answ = %t;
		for i=1:size(m),
			answ = (answ & isempty(m(i)));
		end;
	
	case 17
		// mlist
		answ = %f;
		
	else
		answ = size(m,'*')==0;
	end
	
endfunction
