function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property x
	
	function %constructor
		this.x = 200;
	endfunction
	
	function(getter) val = x
		val = value / 100;
	endfunction
endclass

class Test2 < Test
	property y
	
	function %constructor
		this.y = 1;
		super.%constructor();
	endfunction
	
	function(setter) newVal = y(val)
		newVal = val * 100;
	endfunction
endclass

disp("Expected result:");
disp("2");
disp("3");
disp("100");
disp("500");

disp("------- Running test -----------");

x = Test2.%new();

disp(x.x);
x.x = 300;
disp(x.x);
disp(x.y);
x.y = 5;
disp(x.y);
