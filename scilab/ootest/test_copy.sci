function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(private) x = 5
	
	function m_this = get_this
		disp(this.x);
		m_this = this;
	endfunction
endclass

disp("Expected result:");
disp("5");
disp("Error: slot x found but not visible from this context");

disp("------- Running test -----------");

x = Test.%new();
disp(x.get_this().x);

