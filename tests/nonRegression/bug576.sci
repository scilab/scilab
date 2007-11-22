// <-- Non-regression test for bug 576 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=576
//
// <-- Short Description -->
//    input() function of scilab cvs doesn't work like scilab 2.7.x
//    when the ouput argument is empty, the ouput is not detected
//    as an empty result ?


// exec("bug576.sce")

disp("press enter");
chemin=input('?','s');

isempty(chemin)

if isempty(chemin) == %F then
affich_result(%T,576);
else
affich_result(%T,576);
end;

