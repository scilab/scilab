// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14397 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14397
//
// <-- Short Description -->
// mgeti() did not read correctly uint64 or int64 integers > 2^52
//

// Read/Write int64 & uint64 with relative accuracy better than %eps = 1/2^52
// --------------------------------------------------------------------------
n = 10;
b = grand(64,n,"uin",0,1);
p = uint64(2).^ndgrid(0:63,1:n);
x0 = sum(b.*p, "r");

for endian = ["l" "b"]
    for usign = ["u" ""]
        // usign+"l"+endian
        binfile = TMPDIR+"/mgetiTestInt64.dat";
        idF = mopen(binfile, "w+b");
        x = x0;
        if usign==""
            x = int64(x);
        end
        mput(x,usign+"l"+endian)

        mseek(0);
        xr = mgeti(n, usign+"l"+endian);
        mclose(idF);
        assert_checkequal(xr, x);
    end
end
