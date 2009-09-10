function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(cls) x = 5;
	property x = 42;
	function(cls) nil = hello
		disp("Hello from class Test");
		nil = disp(this.x);
	endfunction
	
	function nil = hello
		disp("Hello from Test instance");
		nil = disp(this.x);
	endfunction
endclass

disp("Expected result:");
disp("Hello from class Test");
disp("5");
disp("Hello from Test instance");
disp("42");

disp("------- Running test -----------");

Test.hello();
Test.%new().hello();
