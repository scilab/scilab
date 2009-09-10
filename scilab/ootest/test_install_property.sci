function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	function test
		disp(this.y);
	endfunction
endclass

disp("Expected result:");
disp("42");
disp("5");
disp("Error: slot y not found");

disp("------- Running test -----------");

x = Test.%new();
x.%install_property("x", "public", 42);
x.%install_property("y", "protected", 5);
disp(x.x);
x.test();
disp(x.y);
