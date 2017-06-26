Welcome to Scilab 6.1.X
=======================

This file details the changes between Scilab 6.1.X (this development branch), and the previous release 6.0.X.
For changelogs of earlier releases, please see [Scilab 6.0.0](https://help.scilab.org/docs/6.0.0/en_US/CHANGES.html).

This file is intended for the specific needs of advanced users, and describes:
- High-level new features,
- Changes in installation, compilation, and compatibility,
- Changes in the language,
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Bug fixes.

This is an in-development version which might be unstable.
Please report any thing we could have missed, on the [mailing lists][1] or on the [bug tracker][2].

[1]: http://mailinglists.scilab.org
[2]: http://bugzilla.scilab.org


Main new features
-----------------

For high-level description of the main new features of this release, please
consult the [embedded help](modules/helptools/data/pages/homepage-en_US.html). It is also available
as the "What's new" page of the help, by simply typing `help` in Scilab console.

In summary, the main new features are:
* 


Installation
------------


Compilation
-----------


Dependencies
------------


Packaging & Supported Operating Systems
---------------------------------------

* Scilab embedded JVM is Java 8. To run or compile Scilab you need at least:
  - Windows:
     - Windows 8 (Desktop)
     - Windows 7
     - Windows Vista SP2
     - Windows Server 2008 R2 SP1 (64-bit)
     - Windows Server 2012 (64-bit)
  - Mac OS X:
     - Intel-based Mac running Mac OS X 10.8.3+, 10.9+
  - Linux:
     - Red Hat Enterprise Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
     - Oracle Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
     - Ubuntu Linux 12.04 LTS, 13.x
     - Suse Linux Enterprise Server 10 SP2+, 11.x

    For more information, please consult: [What are the system requirements for Java?](http://java.com/en/download/help/sysreq.xml)

* [SSE2](https://en.wikipedia.org/wiki/SSE2), Streaming SIMD Extensions 2 support is mandatory.


Feature changes and additions
-----------------------------
 
* Empty strings are used as the default values on String allocation
* Memory invalid accesses have been greatly reduced thanks to :
  - PVS-Studio inspections blog report
  - Coverity scan weekly source analysis

Help pages:
-----------

* fixed / improved:  `bench_run` `M_SWITCH`


Data Structures
---------------


Xcos
----


API modification
----------------


Obsolete functions or features
------------------------------


Removed Functions
-----------------

* `hypermat` was obsolete and has been removed. Please use `matrix` instead.
* `square` was obsolete and has been removed.


Known issues
------------


Bug Fixes
---------

### Bugs fixed in 6.1.0:
* [#14604](http://bugzilla.scilab.org/show_bug.cgi?id=14604): `emptystr()` is 40x slower with 6.0.0 wrt 5.5.2
* [#14605](http://bugzilla.scilab.org/show_bug.cgi?id=14605): fixed - `bench_run` was too strict about the specification of tests names.
* [#14812](http://bugzilla.scilab.org/show_bug.cgi?id=14812): Minor typos in messages.
* [#14982](http://bugzilla.scilab.org/show_bug.cgi?id=14982): `msprintf`segmentation fault was caught due to wrong size

### Bugs fixed in 6.0.1:
* [#4276](http://bugzilla.scilab.org/show_bug.cgi?id=4276): `strsubst` replaced the first occurence in regex mode.
* [#5278](http://bugzilla.scilab.org/show_bug.cgi?id=5278): obsolete `xset()` was still used in scripts, macros, tests and help pages.
* [#14636](http://bugzilla.scilab.org/show_bug.cgi?id=14636): Xcos model with modelica electrical blocks (created in 5.5.2) crashed Scilab 6.
* [#14637](http://bugzilla.scilab.org/show_bug.cgi?id=14367): Some Scilab 5.5.2 diagrams didn't simulate properly in Xcos.
* [#14978](http://bugzilla.scilab.org/show_bug.cgi?id=15006): ode help page still contained 'root' which has been replaced by 'roots'.
* [#15010](http://bugzilla.scilab.org/show_bug.cgi?id=15010): Coselica did not simulate on Scilab 6.
* [#15023](http://bugzilla.scilab.org/show_bug.cgi?id=15023): `clf()` wrongly reset `figure_id`.

