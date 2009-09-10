function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	function(cls) hello
		disp("Hello");
	endfunction
endclass

disp("Expected result:");
disp("Hello");
disp("Error: slot hello not found");

disp("------- Running test -----------");

Test.hello();
Test.%remove_slot("hello");
Test.hello();
