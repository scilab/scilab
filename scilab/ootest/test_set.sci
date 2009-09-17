function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(protected) x = 4;
	property y = 2;
endclass

disp("Expected result:");
disp("2");
disp("5");
disp("Error: slot x found but not visible from this context");

disp("------- Running test -----------");

x = Test.%new();
disp(x.y);
x.%set("y", 5);
disp(x.y);
disp(x.x);
