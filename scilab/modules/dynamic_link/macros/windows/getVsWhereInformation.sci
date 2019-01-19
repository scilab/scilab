function vs = getVsWhereInformation()
    //call vswhere MS tools to get Information about VSs >= 2015 installed.

    //command vswhere -all -prerelease -format json -utf8

    //From vswhere -?
    //-all           Finds all instances even if they are incomplete and may not launch.
    //-prerelease    Also searches prereleases. By default, only releases are searched.
    //-legacy        Also searches Visual Studio 2015 and older products. Information is limited.
    //               This option cannot be used with either -products or -requires.
    //-format arg    Return information about instances found in a format described below.
    //-utf8          Use UTF-8 encoding (recommended for JSON).

    cmd = sprintf("""%s"" -all -prerelease -format json -utf8 -include packages", fullfile(SCI, "tools", "vswhere", "vswhere"));
    x = unix_g(cmd);
    vs = [];
    if isempty(x) == %f then
        x = fromJSON(x);
        for i = 1:length(x)
            xi = x(i);

            for p = 1:length(xi.packages)
                if xi.packages(p).id == "Microsoft.VisualCpp.Tools.HostX64.TargetX64" then
                    v = xi.packages(p).version;
                    vers = int(evstr(v)(1) * 10)
                end
            end

            [a,b,c,d] = regexp(xi.productId, "/.*\.([0-9a-zA-Z]+)$/");

            t = "express";
            if d == "Enterprise" || d == "Professional" then
                t = "pro";
            end

            vs(sprintf("msvc%d%s", vers, t)) = sprintf(xi.installationPath);
        end
    end
endfunction
