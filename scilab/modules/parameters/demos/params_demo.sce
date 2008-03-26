lines(0);

test_list = init_param();

test_list = add_param(test_list,'min');
test_list = add_param(test_list,'max');

printf('is_param(test_list,''min'') = %f\n',1.0*is_param(test_list,'min'));
printf('is_param(test_list,''max'') = %f\n',1.0*is_param(test_list,'max'));
printf('is_param(test_list,''mex'') = %f\n',1.0*is_param(test_list,'mex'));

test_list = set_param(test_list,'min',[0 0 0]);
test_list = set_param(test_list,'max',[1 1 1]);

printf('get_param(test_list,''min'') = '); disp(get_param(test_list,'min'));
printf('get_param(test_list,''max'') = '); disp(get_param(test_list,'max'));
printf('get_param(test_list,''mex'') = '); disp(get_param(test_list,'mex'));

printf('removing the parameter ''min'' from test_list \n');
printf('list of parameters name before removing\n');
disp(list_param(test_list));

test_list = remove_param(test_list,'min');

printf('list of parameters name after removing\n');
disp(list_param(test_list));

printf('get_param(test_list,''min'') = '); disp(get_param(test_list,'min'));

printf('typeof(test_list) = %s\n', typeof(test_list));

A = [];

printf('Add a parameter to a non plist. Before add_param: '); disp(A);

[A,err] = add_param(A,'min');

printf('After add_param: '); disp(A);
printf('Error = %f\n',1*err);
