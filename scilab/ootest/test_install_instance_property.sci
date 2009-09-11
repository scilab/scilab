function nil = disp(message)
	message
	nil = [];
endfunction

class Test
endclass

disp("Expected result:");
disp("42");

disp("------- Running test -----------");

x = Test.%new();
Test.%install_instance_property("x", "public", 42);
disp(x.x);
