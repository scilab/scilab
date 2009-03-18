Install OCaml 3.11.0 from www.ocaml.org
Download FlexDLL and copy flexlink.exe and flexdll_msvc.obj in bin directory of ocaml
(http://alain.frisch.fr/flexdll.html)

Edit Makefile.mak and modify first line with a correct path if OCAMLLIB is not defined as environment variable

OCAMLPATH=C:\Program Files\Objective Caml

