// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
// Copyright (C) 2021 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

//handle
handle1 = gcf();
handle2 = gca();

assert_checkequal(handle1 == handle1, %t);
assert_checkequal(handle1 == handle2, %f);

delete(handle1);

// With type 0:
L = list(,2);
uitree = uiCreateTree(uiCreateNode('Root', 'path\rootImage.jpg', 'rootCallback'));
for object = list(gdf(), uitree)
    assert_checkfalse(object == L(1));
    assert_checkfalse(object == null());
    assert_checkfalse(null() == object);
    assert_checkfalse(L(1) == object);
end
