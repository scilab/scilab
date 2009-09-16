function nil = disp(message)
	message
	nil = [];
endfunction

function hello()
	disp(this.x);
endfunction

class Test
	property x = 5
	
	function hello()
		disp(this.x);
		hello();
	endfunction
endclass

disp("Expected result:");
disp("5");
disp("Undefined variable: a");

disp("------- Running test -----------");

x = Test.%new();
x.hello();
