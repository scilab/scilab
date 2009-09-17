function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	property(cls) x;
	property(cls,protected) y;
	property(cls,private) z;
	
	function(cls) a
	endfunction
	function(cls,protected) b
	endfunction
	function(cls,private) c
	endfunction
endclass

disp("Expected result:");
disp("%get method public");
disp("%set method public");
disp("%new method public");
disp("%slots_list method public");
disp("%install_method method public");
disp("%install_property method public");
disp("%remove_slot method public");
disp("%instance_slots_list method public");
disp("%install_instance_method method public");
disp("%install_instance_property method public");
disp("%remove_instance_slot method public");
disp("a method public");
disp("b method protected");
disp("c method private");
disp("name property public");
disp("super property public");
disp("x property public");
disp("y property protected");
disp("z property private");

disp("------- Running test -----------");

[N,T,V] = Test.%slots_list()
