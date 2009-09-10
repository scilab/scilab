function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property x = 42;
	function nil = hello
		nil = disp(this.x);
	endfunction
endclass

disp("Expected result:");
disp("42");
disp("51");

disp("------- Running test -----------");

x = Test.%new();
m = x.hello;
m();
x.x = 51;
m();
