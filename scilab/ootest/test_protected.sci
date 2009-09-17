function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(protected) x = 2
	property(protected) y = 42
	
	function nil = hello2
		disp(this.x);
		disp(this.y);
		nil = [];
	endfunction
endclass

class Test2 < Test
	property y = 37
	function nil = hello
		disp(this.x);
		disp(this.y);
		nil = [];
	endfunction
endclass

disp("Expected result:");
disp("2");
disp("37");
disp("2");
disp("37");
disp("37");
disp("Error: slot x found but not visible from this context");

disp("------- Running test -----------");

x = Test2.%new();
x.hello();
x.hello2();
disp(x.y);
x.y
disp(x.x);
