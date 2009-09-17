function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(cls) x = 200
	
	function(getter,cls) val = x
		val = value / 100;
	endfunction
endclass

class Test2 < Test
	property(cls) y = 100
	
	function(setter,cls) newVal = y(val)
		newVal = val * 100;
	endfunction
endclass

disp("Expected result:");
disp("2");
disp("3");
disp("100");
disp("500");

disp("------- Running test -----------");

disp(Test2.x);
Test2.x = 300;
disp(Test2.x);
disp(Test2.y);
Test2.y = 5;
disp(Test2.y);
