// checks arguments compatibility 

ex8c_1([1;2;3;4],[3,4,5,6],[6;7;8;9],90,rand(5,5))

// first argument can have different types 

ex8c_2([1,2,2]);
ex8c_2('foo');

// overload case 

deff('[]=%sp_ex8c_2(sp)','disp(''sparse overloaded'')');
ex8c_2(sparse([1,2,3]));

