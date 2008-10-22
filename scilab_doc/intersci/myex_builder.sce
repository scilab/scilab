// generated with intersci
ilib_name = 'libmyex';           // interface library name
files=['myex','fcalc','csom'] ;
libs=[]; //no libs required
table =["calc","intscalc";
        "som","intssom"];
ilib_build(ilib_name,table,files,libs);
