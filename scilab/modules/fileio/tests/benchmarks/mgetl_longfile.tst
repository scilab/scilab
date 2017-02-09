// ================================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// ================================================================================

//=================================================================================
// Benchmark for mgetl function
//==============================================================================

// <-- BENCH NB RUN : 10 -->
function a = randInt(start, stop, count)
    a = int(rand(count, 1) * ((stop-1) - start) + start);
endfunction

function a = randStr(pool, count)
    s = size(pool, "*");
    i = rand(count, 1) * (s-1) + 1;
    a = matrix(pool(i), -1, 1);
endfunction

filename = fullfile(TMPDIR, "longfile.txt");

COLS3 = ["ALACHUA COUNTY", "BAKER COUNTY", "BAY COUNTY", "BRADFORD COUNTY", "BREVARD COUNTY", "BROWARD COUNTY", "CALHO UN COUNTY", "CHARLOTTE COUNTY", "CITRUS COUNTY", "CLAY COUNTY", "COLLIER COUNTY", "COLUMBIA COUNTY", "DESOTO COUNTY", "DIXIE COUNTY", "DUVAL COUNTY", "ESCAMBIA COUNTY", "FLAGLER COUNTY", "FRANKLIN COUNTY", "GADSDEN COUNTY", "GILCHRIST COUNTY", "GLADES COUNTY", "GULF COUNTY", "HAMILTON COUNTY", "HARDEE COUNTY", "HENDRY COUNTY", ... 
    "HERNANDO COUNTY", "HIGHLANDS COUNTY", "HILLSBOROUGH COUNTY", "HOLMES COUNTY", "INDIAN RIVER COUNTY", "JACKSON COUNTY", "JEFFERSON COUNTY", "LAFAYETTE COUNTY", "LAKE COUNTY", "LEE COUNTY", "LEON COUNTY", "LEVY COUNTY", "LIBERTY COUNTY", "MADISON COUNTY", "MANATEE COUNTY", "MARION COUNTY", "MARTIN COUNTY", "MIAMI DADE COUNTY", "MONROE COUNTY", "NASSAU COUNTY", "North Fort Myers", "OKALOOSA COUNTY", "ORANGE COUNTY", "OSCEOLA COUNTY", "Orlando", ...
    "PALM BEACH COUNTY", "PASCO COUNTY", "PINELLAS COUNTY", "POLK COUNTY", "PUTNAM COUNTY", "SANTA ROSA COUNTY", "SARASOTA COUNTY", "SEMINOLE COUNTY", "ST  JOHNS COUNTY", "SUMTER COUNTY", "SUWANNEE COUNTY", "TAYLOR COUNTY", "UNION COUNTY", "VOLUSIA COUNTY", "WAKULLA COUNTY", "WALTON COUNTY", "WASHINGTON COUNTY"];

header = "policyID,statecode,county,eq_site_limit,hu_site_limit,fl_site_limit,fr_site_limit,tiv_2011,tiv_2012,eq_site_deductible,hu_site_deductible,fl_site_deductible,fr_site_deductible,point_latitude,point_longitude,line,construction,point_granularity";

nb = 100000;
rI = randInt(1d5, 1d6, nb);
rS = randStr(COLS3, nb);
strs = msprintf("%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%d\n", rI, rS, rS, rI, rI, rI, rI, rI, rI, rI, rI, rI, rI, rI, rI, rS, rS, rI);
mputl(strs, filename);

// <-- BENCH START -->
M = mgetl(filename);
// <-- BENCH END -->