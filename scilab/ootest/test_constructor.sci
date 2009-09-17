function nil = disp(message)
	message
	nil = [];
endfunction

class TestA
	function %constructor(a)
		disp("Hello from TestA");
		disp(a);
	endfunction
endclass

class TestB < TestA
	function %constructor
		disp("Hello from TestB");
	endfunction
endclass

class TestC < TestA
	function %constructor
		disp("Hello from TestC");
		super.%constructor(5);
	endfunction
endclass

class TestD < TestA
	
endclass

disp("Expected result:");
disp("Hello from TestB");
disp("Hello from TestC");
disp("Hello from TestA");
disp("5");
disp("Hello from TestA");
disp("42");

disp("------- Running test -----------");

TestB.%new();
TestC.%new();
TestD.%new(42);
