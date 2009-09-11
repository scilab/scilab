function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	function hello
		disp("Hello");
	endfunction
endclass

disp("Expected result:");
disp("Hello");
disp("Error: slot hello not found");

disp("------- Running test -----------");

x = Test.%new();
x.hello();
Test.%remove_instance_slot("hello");
x.hello();
