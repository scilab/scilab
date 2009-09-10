function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(private) x = 2
	property(private) y = 42
	
	function nil = hello
		disp(this.x);
		disp(this.y);
		nil = [];
	endfunction
endclass

class Test2 < Test
	property y = 37
endclass

disp("Expected result:");
disp("2");
disp("42");
disp("37");
disp("Error: slot x found but not visible from this context");

disp("------- Running test -----------");

x = Test2.%new();
x.hello();
x.y
x.x
