function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property x;
	property(protected) y;
	property(private) z;
	
	function a
	endfunction
	function(protected) b
	endfunction
	function(private) c
	endfunction
endclass

disp("Expected result:");
disp("a");
disp("b");
disp("c");
disp("x");
disp("y");
disp("z");
disp("a method");
disp("b method");
disp("c method");
disp("x property");
disp("y property");
disp("z property");
disp("a method public");
disp("b method protected");
disp("c method private");
disp("x property public");
disp("y property protected");
disp("z property private");

disp("------- Running test -----------");

N = Test.%instance_slots_list(%f)
[N,T] = Test.%instance_slots_list(%f)
[N,T,V] = Test.%instance_slots_list(%f)
