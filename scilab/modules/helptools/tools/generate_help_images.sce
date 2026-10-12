// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// Pre-generate the help images described by the <scilab:image> blocks and
// the .zcos diagrams displayed by the help XML files, into
// SCI/modules/helptools/images (versioned): the images get stable
// human-readable names (<page>_<n>.png, <diagram>.zcos.png) so that changes
// show up as image diffs in merge requests, and the manifest
// (help_images.index) maps each key -- MD5 of language + canonical snippet,
// or "xcos_" + MD5 of the .zcos content -- to its file, for the runtime
// lookup done by the help browser.
//
// The generation is incremental: a snippet whose hash is already in the
// manifest (with its file present) is not regenerated. Stale entries
// (snippets removed from the doc) are dropped and their files deleted.
// A snippet that crashed the Scilab process in a previous run is skipped
// (marker files in SCIHOME/help_images_gen), so relaunching this script
// until it reports "crashed: 0" converges.
//
// Usage (from scilab or scilab-cli started in any directory):
//   exec("SCI/modules/helptools/tools/generate_help_images.sce");
// Optional variables defined before exec:
//   GEN_OUT   : output directory   (default: SCI + "/modules/helptools/images")
//   GEN_LANG  : language filter    (default: "" = all languages)
//   GEN_LIMIT : stop after N generations, 0 = no limit (default: 0)
//   GEN_ZIP   : also pack the images and the manifest into this zip archive
//               (default: "" = none). Used at packaging time, so that the
//               installer ships a single file; the help browser reads it
//               when the plain images directory is not there.
//               Typically: SCI + "/modules/helptools/help_images.zip"
//   GEN_HELP_DIRS : "help" directories to walk, each holding <lang> subdirs
//               (default: every SCI/modules/<module>/help). A toolbox has
//               the same layout, hence tbx_build_help() passing its own
//               <toolbox>/help here.
//   GEN_TIMINGS : print the durations (default: %t). tbx_build_help() turns
//               them off: a toolbox build log is not a benchmark.

function [names, isdirs] = ghi_dir(path)
    // dir() does not return the same order on every platform: it sorts
    // case-insensitively on Windows and case-sensitively elsewhere. Re-sort
    // here on the raw code points (gsort compares with wcscmp, so the result
    // does not depend on the locale either) to keep the traversal order --
    // hence the generated file names and the manifest -- identical everywhere.
    names = []; isdirs = [];
    entries = dir(path);
    if typeof(entries) <> "dir" | size(entries.name, "*") == 0 then
        return;
    end
    [names, k] = gsort(entries.name(:), "g", "i");
    isdirs = entries.isdir(k);
endfunction

function files = ghi_find_xml(root)
    // recursively collect *.xml files under root
    files = [];
    [names, isdirs] = ghi_dir(root);
    for i = 1:size(names, "*")
        p = root + filesep() + names(i);
        if isdirs(i) then
            files = [files; ghi_find_xml(p)];
        else
            [dummy1, dummy2, ext] = fileparts(p);
            if convstr(ext) == ".xml" then
                files = [files; p];
            end
        end
    end
endfunction

function hash = ghi_code_md5(code, lang)
    // MD5 of the language followed by the canonical form of the code (lines
    // trimmed, empty lines dropped, joined by LF — immune to XML-parser
    // whitespace differences). The language is part of the key because an
    // identical snippet can render differently per locale (gettext/_());
    // identical renderings are deduplicated by content afterwards.
    // The Java lazy generation must hash the same canonical form.
    lines = stripblanks(code, %t);
    lines = lines(lines <> "");
    key = lang + ascii(10) + strcat(lines, ascii(10));
    tmp = TMPDIR + filesep() + "ghi_code.bin";
    fd = mopen(tmp, "wb");
    mputstr(key, fd);
    mclose(fd);
    hash = getmd5(tmp);
    deletefile(tmp);
endfunction

function ok = ghi_generate_xcos(zcosPath, pngPath)
    // same rendering as SwingScilabHelpBrowser.resolveXcosImage
    ok = %t;
    try
        loadXcosLibs();
        importXcosDiagram(zcosPath);
        xcosDiagramExport(scs_m, pngPath);
    catch
        ok = %f;
    end
endfunction

function ok = ghi_generate(code, pngPath)
    // same rendering wrapper as SwingScilabHelpBrowser.generateScilabImage
    __oldwins__ = winsid();
    __olddrv__ = driver();
    __oldseed__ = rand("seed");
    __oldgrand__ = grand("getsd");
    ok = %t;
    try
        // reset default figure/axes: a previous example may have customized
        // them through gdf()/gda() and would taint every following image
        sdf();
        sda();
        driver("png");
        xinit(pngPath);
        scf();
        // fixed seeds so images do not depend on the generation order or on
        // previous session activity (examples use rand() and grand())
        rand("seed", 0);
        grand("setsd", 0);
        ierr = execstr(code, "errcatch");
        if ierr <> 0 then
            ok = %f;
        else
            gcf().anti_aliasing = "2x";
        end
    catch
        ok = %f;
    end
    try xend(); catch end
    try driver(__olddrv__); catch end
    rand("seed", __oldseed__);
    try grand("setsd", __oldgrand__); catch end
    __newwins__ = setdiff(winsid(), __oldwins__);
    for __i__ = __newwins__
        delete(scf(__i__));
    end
endfunction

// ----------------------------------------------------------------------
// main
// ----------------------------------------------------------------------

if ~isdef("GEN_OUT") then GEN_OUT = SCI + "/modules/helptools/images"; end
if ~isdef("GEN_LANG") then GEN_LANG = ""; end
if ~isdef("GEN_LIMIT") then GEN_LIMIT = 0; end
if ~isdef("GEN_ZIP") then GEN_ZIP = ""; end
if ~isdef("GEN_TIMINGS") then GEN_TIMINGS = %t; end
if ~isdef("GEN_HELP_DIRS") then
    // every SCI/modules/<module>/help
    [ghi_mods, ghi_modIsDir] = ghi_dir(SCI + "/modules");
    GEN_HELP_DIRS = SCI + "/modules/" + ghi_mods(ghi_modIsDir) + "/help";
    clear ghi_mods ghi_modIsDir;
end
GEN_HELP_DIRS = GEN_HELP_DIRS(:);

GEN_INDEX = GEN_OUT + filesep() + "help_images.index";
GEN_WORK = SCIHOME + filesep() + "help_images_gen";
mkdir(GEN_OUT);
mkdir(GEN_WORK);

// previous manifest: oldHashes(i) -> oldNames(i)
oldHashes = []; oldNames = [];
if isfile(GEN_INDEX) then
    lines = mgetl(GEN_INDEX);
    for i = 1:size(lines, "*")
        tok = tokens(lines(i));
        if size(tok, "*") == 2 then
            oldHashes = [oldHashes; tok(1)];
            oldNames = [oldNames; tok(2)];
        end
    end
end

// ----------------------------------------------------------------------
// collect: unique snippets, each with a stable file name
// ----------------------------------------------------------------------

if GEN_LANG == "" then
    mprintf("Collecting <scilab:image> blocks (all languages)...\n");
else
    mprintf("Collecting <scilab:image> blocks (%s)...\n", GEN_LANG);
end
tic();

// languages, en_US first so it owns the <page>_<n> base names
if GEN_LANG <> "" then
    allLangs = GEN_LANG;
else
    allLangs = [];
    for m = 1:size(GEN_HELP_DIRS, "*")
        [langNames, langIsDir] = ghi_dir(GEN_HELP_DIRS(m));
        for l = 1:size(langNames, "*")
            if langIsDir(l) then
                allLangs = [allLangs; langNames(l)];
            end
        end
    end
    allLangs = unique(allLangs);
    allLangs = ["en_US"; allLangs(allLangs <> "en_US")];
end

// files sitting in GEN_OUT without belonging to the manifest: hand-made
// images living in the same directory (toolboxes point <imagedata> at
// help/images), never to be overwritten by a generated one
[ghi_out, ghi_outIsDir] = ghi_dir(GEN_OUT);
foreignNames = [];
for i = 1:size(ghi_out, "*")
    if ~ghi_outIsDir(i) & ~or(oldNames == ghi_out(i)) then
        foreignNames = [foreignNames; ghi_out(i)];
    end
end
clear ghi_out ghi_outIsDir;

codes = struct();               // "h_"+key -> code (snippet) or .zcos path
uHash = []; uName = []; uLang = []; uIsXcos = []; // unique keys, file names
nBlocks = 0;
for l = 1:size(allLangs, "*")
    lang = allLangs(l);
    for m = 1:size(GEN_HELP_DIRS, "*")
        helpDir = GEN_HELP_DIRS(m) + "/" + lang;
        if ~isdir(helpDir) then continue; end
        xmls = ghi_find_xml(helpDir);
        for i = 1:size(xmls, "*")
            doc = xmlRead(xmls(i));
            imgs = xmlXPath(doc, "//scilab:image", ["scilab" "http://www.scilab.org"]);
            [dummy1, page, dummy2] = fileparts(xmls(i));
            for j = 1:imgs.size
                code = imgs(j).content;
                if code == "" then continue; end
                nBlocks = nBlocks + 1;
                h = ghi_code_md5(code, lang);
                if or(uHash == h) then continue; end
                // keep the name a previous run gave to this snippet so
                // names stay stable; otherwise derive it from the page
                k = find(oldHashes == h);
                if k <> [] & isfile(GEN_OUT + filesep() + oldNames(k(1))) then
                    name = oldNames(k(1));
                else
                    // same convention as the previous doc build, so that the
                    // transition shows up as in-place image diffs in gitlab:
                    // <page>_<n>.png, localized variant <page>_<lang>_<n>.png
                    name = page + "_" + string(j) + ".png";
                    if or(uName == name) | or(foreignNames == name) then
                        name = page + "_" + lang + "_" + string(j) + ".png";
                    end
                    c = 1;
                    while or(uName == name) | or(foreignNames == name)
                        c = c + 1;
                        name = page + "_" + lang + "_" + string(j) + "_" + string(c) + ".png";
                    end
                end
                codes("h_" + h) = code;
                uHash = [uHash; h];
                uName = [uName; name];
                uLang = [uLang; lang];
                uIsXcos = [uIsXcos; %f];
            end
            // xcos diagrams displayed by the page (imagedata on a .zcos):
            // keyed on the diagram file content, like the runtime does
            refs = xmlXPath(doc, "//db:imagedata[@fileref]", ["db" "http://docbook.org/ns/docbook"]);
            xmlDir = fileparts(xmls(i));
            for j = 1:refs.size
                fileref = refs(j).attributes.fileref;
                [dummy1, zname, zext] = fileparts(fileref);
                if convstr(zext) <> ".zcos" then continue; end
                zcosPath = fullpath(xmlDir + fileref);
                if ~isfile(zcosPath) then continue; end
                nBlocks = nBlocks + 1;
                h = "xcos_" + getmd5(zcosPath);
                if or(uHash == h) then continue; end
                k = find(oldHashes == h);
                if k <> [] & isfile(GEN_OUT + filesep() + oldNames(k(1))) then
                    name = oldNames(k(1));
                else
                    name = zname + ".zcos.png";
                    c = 1;
                    while or(uName == name) | or(foreignNames == name)
                        c = c + 1;
                        name = zname + "_" + string(c) + ".zcos.png";
                    end
                end
                codes("h_" + h) = zcosPath;
                uHash = [uHash; h];
                uName = [uName; name];
                uLang = [uLang; ""];
                uIsXcos = [uIsXcos; %t];
            end
            xmlDelete(doc);
        end
    end
end
tCollect = toc();
nUniq = size(uHash, "*");
if GEN_TIMINGS then
    mprintf("%d blocks, %d (language, snippet) pairs, collected in %.1f s\n", nBlocks, nUniq, tCollect);
else
    mprintf("%d blocks, %d (language, snippet) pairs\n", nBlocks, nUniq);
end

// ----------------------------------------------------------------------
// generate what the manifest does not already provide
// ----------------------------------------------------------------------

nGen = 0; nSkip = 0; nFail = 0; nCrash = 0; nDedup = 0;
failed = [];
cHash = []; cName = [];         // PNG content md5 -> file name (dedup)
lang0 = getlanguage();
curLang = lang0;

// snippets the manifest already provides, not counted in the progress
isCached = %f(ones(nUniq, 1));
for k = 1:nUniq
    png = GEN_OUT + filesep() + uName(k);
    isCached(k) = or(oldHashes == uHash(k)) & isfile(png) & fileinfo(png)(1) > 0;
end
nTodo = nUniq - size(find(isCached), "*");
iTodo = 0;

tic();
for k = 1:nUniq
    if GEN_LIMIT > 0 & nGen >= GEN_LIMIT then break; end
    h = uHash(k);
    png = GEN_OUT + filesep() + uName(k);
    if ~isCached(k) then
        iTodo = iTodo + 1;
    end
    marker = GEN_WORK + filesep() + h + ".crashed";
    if isfile(marker) then
        // a previous run died while generating this snippet: skip it so a
        // relaunch loop converges instead of crashing forever on it
        nCrash = nCrash + 1;
        failed = [failed; h + "  " + uName(k)];
        mprintf("[%4d/%d] %s  CRASH (skipped, see previous run)\n", iTodo, nTodo, uName(k));
        continue;
    end
    if isCached(k) then
        // register the cached content so other languages can dedup on it
        cmd5 = getmd5(png);
        if ~or(cHash == cmd5) then
            cHash = [cHash; cmd5];
            cName = [cName; uName(k)];
        end
        nSkip = nSkip + 1;
        continue;
    end
    // render the snippet under its language: an identical code can render
    // differently per locale (gettext/_())
    if uLang(k) <> "" & uLang(k) <> curLang then
        setlanguage(uLang(k));
        curLang = uLang(k);
    end
    t0 = toc();
    fd = mopen(marker, "wt"); mclose(fd);
    if uIsXcos(k) then
        ok = ghi_generate_xcos(codes("h_" + h), png);
    else
        ok = ghi_generate(codes("h_" + h), png);
    end
    deletefile(marker);
    if ok & isfile(png) then
        // content dedup: locale-insensitive snippets render byte-identical
        // across languages -> keep a single file for all of them
        cmd5 = getmd5(png);
        c = find(cHash == cmd5);
        if c <> [] & cName(c(1)) <> uName(k) then
            deletefile(png);
            // told, not only counted: outside of a translation, a duplicate
            // is often an example copy-pasted and never updated
            msg = msprintf("%s  duplicate of %s, not kept", uName(k), cName(c(1)));
            uName(k) = cName(c(1));
            nDedup = nDedup + 1;
        else
            cHash = [cHash; cmd5];
            cName = [cName; uName(k)];
            nGen = nGen + 1;
            msg = uName(k);
        end
        if GEN_TIMINGS then
            msg = msprintf("%s  %.2f s", msg, toc() - t0);
        end
        mprintf("[%4d/%d] %s\n", iTodo, nTodo, msg);
    else
        nFail = nFail + 1;
        failed = [failed; h + "  " + uName(k)];
        deletefile(png);
        mprintf("[%4d/%d] %s  FAIL\n", iTodo, nTodo, uName(k));
    end
end
tGen = toc();
if curLang <> lang0 then
    setlanguage(lang0);
end

// ----------------------------------------------------------------------
// write the manifest and drop stale files
// ----------------------------------------------------------------------

nStale = 0;
if GEN_LIMIT == 0 then
    // manifest: only snippets with an actual image
    keep = [];
    for k = 1:nUniq
        png = GEN_OUT + filesep() + uName(k);
        if isfile(png) & fileinfo(png)(1) > 0 then
            keep = [keep; k];
        end
    end
    if keep == [] then
        // nothing to show (mputl() rejects an empty matrix): an empty
        // manifest, which still records that the doc has been processed --
        // tbx_build_help() compares its date with the sources
        mputl("", GEN_INDEX);
    else
        // sort on (file name, key): several keys share one file, and sorting
        // on the name alone would leave their relative order at the mercy of
        // the order the doc was walked in. ascii(1) sorts below every
        // character a name can hold, so the concatenation orders exactly
        // like the pair.
        [s, idx] = gsort(uName(keep) + ascii(1) + uHash(keep), "g", "i");
        lines = uHash(keep(idx)) + "  " + uName(keep(idx));
        mputl(lines, GEN_INDEX);
    end

    // files owned by the previous manifest but no longer referenced
    for i = 1:size(oldNames, "*")
        if ~or(uName(keep) == oldNames(i)) & isfile(GEN_OUT + filesep() + oldNames(i)) then
            deletefile(GEN_OUT + filesep() + oldNames(i));
            nStale = nStale + 1;
        end
    end
else
    mprintf("(GEN_LIMIT set: manifest not rewritten)\n");
end

// ----------------------------------------------------------------------
// optional packaging: a single flat zip for the installer
// ----------------------------------------------------------------------

if GEN_ZIP <> "" & GEN_LIMIT == 0 then
    if isfile(GEN_ZIP) then deletefile(GEN_ZIP); end
    zfiles = [GEN_INDEX; GEN_OUT + filesep() + unique(uName(keep))];
    compress(GEN_ZIP, zfiles);
    mprintf("archive: %s (%d files, %.1f MB)\n", ..
            GEN_ZIP, size(zfiles, "*"), fileinfo(GEN_ZIP)(1) / 1e6);
end

mprintf("\n=== summary ===\n");
mprintf("generated: %d, deduplicated: %d, cached: %d, failed: %d, crashed: %d, stale removed: %d\n", ..
        nGen, nDedup, nSkip, nFail, nCrash, nStale);
if failed <> [] then
    mprintf("failures:\n");
    mprintf("  %s\n", failed);
end
if nGen > 0 & GEN_TIMINGS then
    mprintf("time: %.1f s total, %.2f s / image\n", tGen, tGen / nGen);
end
