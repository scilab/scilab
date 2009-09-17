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
disp("%constructor method public");
disp("a method public");
disp("b method protected");
disp("c method private");
disp("x property public");
disp("y property protected");
disp("z property private");

disp("------- Running test -----------");

[N,T,V] = Test.%instance_slots_list()
