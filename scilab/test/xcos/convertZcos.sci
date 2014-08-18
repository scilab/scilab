// convert Zcos file to cosf and sod
loadXcosLibs()
filenames = unix_g("ls -1 *.zcos");
blackList = "bug_8052.zcos";

for i = 1:size(filenames, '*')
    [path, fnameNoExt ,extension]= fileparts(filenames(i));
    printf("Exporting %s ... ", filenames(i));
    if ~or(filenames(i) == blackList)
        res = importXcosDiagram(filenames(i));
        if res then
            save(fnameNoExt+".sod", "scs_m");
            [u,err] = file('open', fnameNoExt+".cosf", "unknown");
            ierr = cos2cosf(u, do_purge(scs_m));
            if (ierr)
                printf("COSF FAILED\n");
            else
                printf("DONE\n");
            end
            file('close', u);

        else
            printf("IMPORT FAILED\n");
        end
    else
        printf("BLACKLISTED !!!\n");
    end
end
