// checks arguments compatibility

check_properties_1([1;2;3;4],[3,4,5,6],[6;7;8;9],90,rand(5,5))

// first argument can have different types

chprop2([1,2,2]);
chprop2("foo");

// overload case

deff("[]=%sp_chprop2(sp)","disp(''sparse overloaded'')");
chprop2(sparse([1,2,3]));

// tests which give an error message with check_properties_1

try
    check_properties_1([1;2;3;4]',[3,4,5,6],[6;7;8;9],90,rand(5,5))
catch
    disp(lasterror());
end

try
    check_properties_1([1;2;3;4],[3,4,5,6]',[6;7;8;9],90,rand(5,5))
catch
    disp(lasterror());
end

try
    check_properties_1([1;2;3;4],[3,4,5,6],[6;7;8;9]',90,rand(5,5))
catch
    disp(lasterror());
end

try
    check_properties_1([1;2;3;4],[3,4,5,6],[6;7;8;9],[],rand(5,5))
catch
    disp(lasterror());
end

try
    check_properties_1([1;2;3;4],[3,4,5,6],[6;7;8;9],90,rand(4,4))
catch
    disp(lasterror());
end
