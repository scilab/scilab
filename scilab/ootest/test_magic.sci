function nil = disp(message)
	message
	nil = [];
endfunction

class Test
	function res = %get_slot(name)
		disp("GET:"+name);
		res = 42;
	endfunction
	
	function %set_slot(name, val)
		disp("SET:"+name);
		disp(val);
	endfunction
endclass

class Test2 < Test
endclass

disp("Expected result:");
disp("GET:foo");
disp("42");
disp("SET:bar");
disp("1");

disp("------- Running test -----------");

x = Test2.%new();
disp(x.foo);
x.bar = 1;