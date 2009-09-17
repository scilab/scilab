function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(private) x = 5;
	property z = 3;
	
	function nil = test
		nil = [];
		disp(this.x);
		disp(this.y);
	endfunction
endclass

class Test2 < Test
	property(protected) x = 4;
	property(protected) y = 2;
	
	function nil = test2
		nil = [];
		disp(this.x);
		disp(this.y);
	endfunction
endclass

disp("Expected result:");
disp("5");
disp("2");
disp("4");
disp("2");
disp("3");
disp("Error: slot y found but not visible from this context");

disp("------- Running test -----------");

x = Test2.%new();

x.test();
x.test2();
disp(x.%get("z"));
disp(x.%get("y"));
