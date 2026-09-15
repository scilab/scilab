folder =  "SCI/modules/helptools/data/configuration";

//macros
macro_list = completion("", "macros");
macro_list(part(macro_list, 1) == "%") = [];
macro_list(part(macro_list, 1) == "#") = [];

tmp = convstr(macro_list, "l");
[tmp, ij] = gsort(tmp, "lr", "i");
macro_list = macro_list(ij);
mputl(macro_list, fullfile(folder, "scilab_macros.txt"));

//builtins
func_list = completion("", "functions");
must_list = completion("", "mustBe");
func_list = [func_list;must_list];

func_list(part(func_list, 1) == "%") = [];
func_list(part(func_list, 1) == "!") = [];


func_list = gsort(func_list, "lr", "i");
tmp = convstr(func_list, "l");
[tmp, ij] = gsort(tmp, "lr", "i");
func_list = func_list(ij);

mputl(func_list, fullfile(folder, "scilab_primitives.txt"));

[_, n] = lasterror();
exit(n);
