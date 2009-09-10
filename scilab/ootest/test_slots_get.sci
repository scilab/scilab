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
disp("a");
disp("b");
disp("c");
disp("name");
disp("super");
disp("x");
disp("y");
disp("z");
disp("a method");
disp("b method");
disp("c method");
disp("name property");
disp("super property");
disp("x property");
disp("y property");
disp("z property");
disp("a method public");
disp("b method protected");
disp("c method private");
disp("name property public");
disp("super property public");
disp("x property public");
disp("y property protected");
disp("z property private");

disp("------- Running test -----------");

N = Test.%slots_list(%f)
[N,T] = Test.%slots_list(%f)
[N,T,V] = Test.%slots_list(%f)
