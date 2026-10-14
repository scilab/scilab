---
name: toolbox
description: |
  Creating, building, testing and packaging Scilab toolboxes (external
  modules): standard layout, tbx_make, .start/.quit, macros library,
  classdef in a toolbox, DocBook help (read directly from XML, no jar),
  localization (gettext domain, .po), unit tests (test_run), gateways,
  DESCRIPTION, tbx_package and ATOMS. Trigger words: "toolbox", "module",
  "tbx_make", "builder.sce", "loader", "ATOMS", "package", "help page".
triggers: toolbox, toolboxes, module externe, external module, tbx_make, tbx_package, builder.sce, loader.sce, loader, atoms, atomsinstall, gateway, sci_gateway, help page, page d'aide, pages d'aide, docbook, packager, package, empaqueter, distribuer, distribute, tests unitaires, unit test, unit tests, test_run, localisation, localization, traduction, translation, gettext, .po, description, bibliothèque de fonctions, library, genlib, .start
---

# Scilab toolbox creation

## Reference: SCI/contrib/toolbox_skeleton
Scilab ships a complete, maintained demo toolbox at
`SCI/contrib/toolbox_skeleton` (macros, C/C++/Fortran/Java sources and
gateways, help en/fr, locales, tests, demos, preferences). READ the files
you need from it (Read tool) instead of inventing build scripts. `SCI` is
not a literal path: get it with `mprintf("%s\n", SCI)` first.

## Layout (only `etc/<name>.start` is mandatory)
```
mytbx/
├── DESCRIPTION              # metadata, REQUIRED by tbx_package / ATOMS
├── builder.sce              # optional: tbx_make() runs it if present
├── etc/mytbx.start          # loads the toolbox — its basename = toolbox id
├── etc/mytbx.quit
├── macros/*.sci             # functions -> library "mytbxlib"
├── classes/*.sci            # classdef files (see below), NOT in macros/
├── help/en_US/*.xml         # DocBook pages (help/fr_FR/... for translations)
├── locales/                 # generated/updated by tbx_make (.pot, .po, .mo)
├── tests/unit_tests/*.tst   # + optional *.dia.ref
├── demos/                   # *.dem.sce + mytbx.dem.gateway.sce
├── src/c, src/fortran, src/java, sci_gateway/c|cpp|fortran   # native code
└── loader.sce, unloader.sce, cleaner.sce                     # GENERATED
```

## Build: tbx_make (preferred over hand-written builders)
```scilab
tbx_make("path/to/mytbx")                      // runs builder*.sce if any, else all sections
tbx_make("path/to/mytbx", ["macros", "help"])  // partial rebuild while developing
tbx_make("path/to/mytbx", "*")                 // all sections, ignoring builder.sce
```
Sections: `macros`, `help`, `localization`, `src`, `sci_gateway`. It
always (re)generates `loader.sce`, `unloader.sce`, `cleaner.sce`.
A `builder.sce` is only needed for custom steps; the skeleton's one calls
`tbx_builder_macros`, `tbx_builder_src`, `tbx_builder_gateway`,
`tbx_build_localization`, `tbx_builder_help`, `tbx_build_loader`,
`tbx_build_cleaner` inside a `main_builder()` function.

## Load / unload
```scilab
exec("path/to/mytbx/loader.sce", -1);   // at TOP LEVEL (see pitfall)
exec("path/to/mytbx/unloader.sce", -1);
```
Pitfall: the loader defines the library variable (`mytbxlib`) in the
CURRENT scope — exec it at top level; exec'd inside a function, the
functions vanish when the function returns ("Undefined variable"). If a
function is really needed, make it RETURN the library and assign it at top
level (the `.start` pattern) — no `global`, no `execstr` tricks:
```scilab
function l = init_mytbx()
    exec("path/to/mytbx/loader.sce", -1);
    l = mytbxlib;
endfunction
mytbxlib = init_mytbx();
```

## etc/mytbx.start (modern template, verified)
```scilab
function mytbxlib = startModule()
    TOOLBOX_NAME  = "mytbx";
    TOOLBOX_TITLE = "My Toolbox";
    mprintf("Start " + TOOLBOX_TITLE + "\n");
    etc_tlbx  = getshortpathname(get_absolute_file_path("mytbx.start"));
    root_tlbx = strncpy(etc_tlbx, length(etc_tlbx) - length("/etc/"));

    mytbxlib = lib(pathconvert(root_tlbx) + "macros" + filesep());

    // classdef files are NOT registered by lib(): define them explicitly
    classfiles = findfiles(root_tlbx + "/classes", "*.sci");
    for k = 1:size(classfiles, "*")
        exec(root_tlbx + "/classes/" + classfiles(k), -1);
    end

    if isdir(root_tlbx + "/locales") then
        addlocalizationdomain(TOOLBOX_NAME, root_tlbx + "/locales");
    end
    if or(getscilabmode() == ["NW"; "STD"]) & isdir(root_tlbx + "/help") then
        add_help_chapter(TOOLBOX_TITLE, root_tlbx, %F);   // toolbox ROOT, not a jar
    end
    // gateways / Java / demos / preferences: copy the matching blocks
    // from SCI/contrib/toolbox_skeleton/etc/toolbox_skeleton.start
endfunction
mytbxlib = startModule();
clear startModule;
```

## Macros and classdef
- Every `macros/*.sci` function is compiled into the library `<id>lib`
  (functions can use `arguments` blocks).
- A **classdef file in `macros/` is compiled but NOT added to the
  library**: the class is undefined after loading. Put classes in
  `classes/` and exec them from the `.start` as above (a class defined
  inside a function stays globally visible; `tbx_package` keeps
  `classes/*.sci`).

## Help (DocBook XML, no jar anymore)
- The help browser reads `help/<lang>/*.xml` DIRECTLY: no help jar is
  built. `add_help_chapter(TITLE, root_tlbx, %F)` registers the toolbox
  root; `help/<lang>` is resolved with fallback to en_US. A modified page
  shows up by reloading it in the browser — no rebuild needed.
- `tbx_make(..., "help")` only prepares links (`inline/<lang>/links.json`)
  and images; functions used in `<scilab:image>` must be built first.
- Obsolete since 2027.0 (removed in 2027.1): `xmltojar()` (use
  `tbx_build_help()`) and the `languages` argument of
  `tbx_make(path, "help", languages)` (images are made for all languages).
- Old `.start` files register help only `if isdir(root_tlbx + "/jar")`:
  no jar is built anymore, so test `help` and pass the root as above.
- Model new pages on shipped ones (`SCI/modules/<module>/help/en_US/*.xml`
  or the skeleton's). Minimal valid page:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<refentry xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink"
          xmlns:svg="http://www.w3.org/2000/svg" xmlns:mml="http://www.w3.org/1998/Math/MathML"
          xmlns:db="http://docbook.org/ns/docbook" xmlns:scilab="http://www.scilab.org"
          xml:id="mytbx_double" xml:lang="en">
    <refnamediv>
        <refname>mytbx_double</refname>
        <refpurpose>Double a value</refpurpose>
    </refnamediv>
    <refsynopsisdiv>
        <title>Syntax</title>
        <synopsis>y = mytbx_double(x)</synopsis>
    </refsynopsisdiv>
    <refsection>
        <title>Arguments</title>
        <variablelist>
            <varlistentry><term>x</term><listitem><para>real input</para></listitem></varlistentry>
        </variablelist>
    </refsection>
    <refsection>
        <title>Examples</title>
        <programlisting role="example"><![CDATA[
y = mytbx_double(21)
        ]]></programlisting>
    </refsection>
</refentry>
```
`xml:id` must be unique across ALL help (prefix with the toolbox name).

## Localization
- In the toolbox code, translatable messages use the TOOLBOX DOMAIN:
  `error(msprintf(gettext("mytbx", "%s: Negative input not allowed.\n"), "mytbx_double"))`
  (there is no `_d`; plain `_()`/`gettext(msg)` use Scilab's own domain).
- `tbx_make` (localization section) extracts the messages into
  `locales/mytbx.pot` and `locales/en_US.po`, compiles `.mo`. To add a
  language: copy `en_US.po` to `fr_FR.po`, fill the `msgstr`, rebuild.
- The `.start` registers the domain with `addlocalizationdomain`.
- Message extraction scans `macros/`, `classes/`, `etc/`, `src/` and
  `sci_gateway/` (`classes/` since 2027.0): classes use
  `gettext("<id>", ...)` like the macros.
- Tests LOCALIZE the expected messages too (see Unit tests).

## Unit tests
```scilab
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
assert_checkequal(mytbx_double(21), 42);
// expected message built like in the code: passes in any language
msg = msprintf(gettext("mytbx", "%s: Negative input not allowed.\n"), "mytbx_double");
assert_checkerror("mytbx_double(-1)", msg);
```
- Tests run in the user's language: NEVER hard-code a translatable
  message, localize the expected one exactly like the code does —
  `gettext("<id>", ...)` for the toolbox's messages, `_()` for Scilab's
  (model: `SCI/modules/time/tests/unit_tests/calendarDuration.tst`).
  Do NOT use the `ENGLISH IMPOSED` / `FRENCH IMPOSED` headers to work
  around this (almost never needed).
- Run: `test_run("path/to/mytbx")` (all), or
  `test_run("path/to/mytbx", "mytbx_double")`; add `["no_check_ref"]` as
  3rd argument to skip `.dia.ref` comparison. test_run execs the toolbox
  `loader.sce` itself, in a fresh Scilab: build first.
- Headers (`// <-- ... -->`): `CLI SHELL MODE` (no GUI, most tests),
  `NO CHECK REF`, `NO CHECK ERROR OUTPUT`, `TEST WITH GRAPHIC`, `INTERACTIVE TEST`, `LONG TIME EXECUTION`,
  `WINDOWS ONLY` / `LINUX ONLY` / `MACOSX ONLY`.
- A failing `assert_*` is what fails the test; the diagnostic path
  (`*.dia.tmp` in TMPDIR) is printed by test_run.

## Native code (C/C++/Fortran/Java)
Needs a compiler (`haveacompiler()`). Copy the skeleton's
`src/*/builder_*.sce`, `sci_gateway/*/builder_gateway_*.sce` and a
gateway of the same language (`sci_gateway/cpp/sci_cppsum.cpp`,
`sci_gateway/c/sci_csum6.c` for the Scilab 6+ C API), then
`tbx_make(p, ["src", "sci_gateway"])`. The `.start` must exec
`sci_gateway/loader_gateway.sce` (skeleton block).

## DESCRIPTION, packaging and ATOMS
- `DESCRIPTION` (copy the skeleton's and edit): `Toolbox:` (= id),
  `Title:`, `Summary:`, `Version:` (x.y.z), `Author:`, `Maintainer:`,
  `Category:`, `Entity:`, `License:`, `ScilabVersion: >= 2026.0`,
  `Depends:`, `Date:`, `Description:`; fields separated by blank lines,
  continuation lines start with a space. `Depends` is MANDATORY even
  without dependency ("does not contain the Depends field"), and **every
  field needs a value or at least a space after the colon**: a bare
  `Depends:` makes tbx_package fail ("not well formatted at line N") —
  write `Depends: ` (with the trailing space).
- Before publishing, check the ARCHIVE itself: unzip it elsewhere and run
  `test_run` on the unzipped folder (sources are gone, only what the
  loader needs remains).
- `[pkg, files, desc] = tbx_package("path/to/mytbx")` (2025.1+) builds a
  BINARY archive `<id>-<version>-bin.zip` in the CURRENT directory: it
  drops sources (`macros/*.sci` kept as `.bin`, src/, builders) and keeps
  what loading needs. Custom filtering: 3rd argument `customize(workdir)`.
- Install it locally with `atomsInstall("path/to/mytbx-0.1.0-bin.zip")`,
  then `atomsLoad("mytbx")`; or upload it to atoms.scilab.org.

## Guidelines
- Build with `tbx_make`, check with `test_run`, package with `tbx_package`.
- Keep function names prefixed with the toolbox id (no clash with Scilab).
- Code in English, user messages through `gettext("<id>", ...)`.
- When unsure of a file's content, read its counterpart in
  `SCI/contrib/toolbox_skeleton`.
