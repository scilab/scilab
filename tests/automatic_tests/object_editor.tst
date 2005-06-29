getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/object_editor_data.ref','r');
// Below is an example of the appearance of such an interface with objects of type Axes.

// No images for now
xdel_run(winsid());

mclose(%U);
