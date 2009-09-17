function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property x = [1 1 1]
endclass

disp("Expected result:");
disp("[1 2 1]");
disp("[1 1 1]");
disp("[1 2 1 4]");
disp("[1 1 1]");
disp("[1 2 3]");
disp("[1 1 1]");

disp("------- Running test -----------");

x = Test.%new();
y = Test.%new();
x.x(2) = 2;
disp(x.x);
disp(y.x);
x.x(4) = 4;
disp(x.x);
disp(y.x);
x.x = [1 2 3];
disp(x.x);
disp(y.x);
