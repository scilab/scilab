package org.scilab.modules.xpad;

import java.io.IOException;
 
%% 

%public
%class ScilabLexer
%extends ScilabLexerConstants
%final
%unicode
%char
%type int
%pack 

%{
    public int start = 0;
 
    private ScilabDocument doc = null;
    private boolean transposable = false;
    
    public ScilabLexer(ScilabDocument doc) {
    	this.doc = doc;
    }
   
    public void setRange(int p0, int p1) {
    	start = p0;
	yyreset(new ScilabDocumentReader(doc, p0, p1));
    }
    
    public int yychar() {
        return yychar;
    }

%}

/* main character classes */
eol = \r|\n|\r\n

white = [ \t\f]+

open = "[" | "(" | "{"
close = "]" | ")" | "}"

comment = "//"

quote = "'"

booleen = "%t" | "%T" | "%f" | "%F"

operator = ".'" | ".*" | "./" | ".\\" | ".^" | ".**" | "+" | "-" | "/" | "\\" | "*" | "^" | "**" | "==" | "~=" | "<>" | "<" | ">" | "<=" | ">=" | ".*." | "./." | ".\\." | "/." | "=" | "&" | "|" | "@" | "~"

functionKwds = "function" | "endfunction"

structureKwds = "if" | "then" | "else" | "elseif" | "end" | "for" | "while" | "do" | "try" | "catch" | "select" | "case"

controlKwds = "abort" | "break" | "quit" | "return" | "resume" | "pause" | "continue" | "exit"

commands = "apropos" | "help" | "log1p" | "int16" | "vec2var" | "sig2data" | "arl2_ius" | "var2vec" | "str2code" | "int2d" | "int32" | "int3d" | "int8" | "fscanfMat" | "mscanf" | "clear_pixmap" | "clearfun" | "clearglobal" | "clean" | "clear" | "createdir" | "dsearch" | "lufact" | "dlgamma" | "xchange" | "sciargs" | "bdiag" | "balanc" | "param3d1" | "param3d" | "xgraduate" | "data2sig" | "beta" | "rotate_axes" | "fstair" | "gstacksize" | "javalibrarypath" | "javaclasspath" | "feval" | "newaxes" | "uiwait" | "iswaitingforinput" | "%i_abs" | "xs2bmp" | "rubberbox" | "oldbesseli" | "oldbesselj" | "oldbesselk" | "oldbessely" | "cdfbin" | "cdfbet" | "ilib_verbose" | "delbpt" | "mtlb_sparse" | "mtlb_mode" | "number_properties" | "global" | "curblockc" | "curblock" | "getblocklabel" | "setbpt" | "double" | "sfact" | "stacksize" | "ricc_old" | "ricc" | "addcb" | "spec" | "xrects" | "xrect" | "usecanvas" | "execstr" | "exec" | "cdfchi" | "cdfchn" | "scicos_debug_count" | "scicos_debug" | "scicosim" | "scicos_time" | "funcprot" | "xarcs" | "xarc" | "errcatch" | "errclear" | "strcspn" | "strcmp" | "strchr" | "strcat" | "mfscanf" | "msscanf" | "getcallbackobject" | "lstcat" | "taucs_chinfo" | "taucs_chfact" | "taucs_chsolve" | "taucs_chdel" | "taucs_chget" | "%i_cumprod" | "%i_cumsum" | "readgateway" | "readmps" | "read4b" | "readb" | "read" | "loadhistory" | "loadfftwlibrary" | "load" | "xpad" | "odedc" | "ereduc" | "predef" | "x_mdialog" | "rand" | "findfiles" | "findBD" | "find" | "prod" | "bvode" | "pppdiv" | "sorder" | "getdrives" | "getdebuginfo" | "getdynlibext" | "getdefaultlanguage" | "getdate" | "setdefaultlanguage" | "%i_diag" | "xs2eps" | "sident" | "code2str" | "model2blk" | "mode" | "isdef" | "ludel" | "ieee" | "sleep" | "mseek" | "legendre" | "regexp" | "uigetfile" | "uigetcolor" | "uigetfont" | "uigetdir" | "spget" | "luget" | "cshep2d" | "tokens" | "Calendar" | "calerf" | "mclearerr" | "deletefile" | "delete" | "fileinfo" | "fileparts" | "filesep" | "fileext" | "file" | "isletter" | "remez" | "timer" | "uimenu" | "linear_interpn" | "lines" | "mopen" | "typename" | "type" | "xfrect" | "isreal" | "ctree2" | "ctree3" | "ctree4" | "syredi" | "basename" | "resethistory" | "unsetmenu" | "coserror" | "getenv" | "setenv" | "interp2d" | "interp3d" | "interp" | "glue" | "havewindow" | "saveafterncommands" | "saveconsecutivecommands" | "savehistory" | "save" | "movefile" | "move" | "newest" | "size" | "subf" | "addf" | "coeff" | "cdffnc" | "cdff" | "deff" | "diffobjs" | "blkfc1i" | "mulf" | "symfcti" | "hinf" | "meof" | "matfile_open" | "matfile_varreadnext" | "matfile_close" | "matfile_listvar" | "matfile_varwrite" | "getfield" | "setfield" | "newfun" | "maxfiles" | "xs2gif" | "diag" | "imag" | "xsegs" | "cdfgam" | "length" | "progressionbar" | "intg" | "spchol" | "x_choose_modeless" | "x_choose" | "addhistory" | "pathconvert" | "pathsep" | "gethistoryfile" | "gethistory" | "sethistoryfile" | "with_embedded_jre" | "with_module" | "is_handle_valid" | "uiDisplayTree" | "ascii" | "addinter" | "chdir" | "isdigit" | "isdir" | "x_dialog" | "definedfields" | "isfile" | "xclick" | "delip" | "splin2d" | "splin3d" | "splin" | "c_link" | "semidef" | "mini" | "xgrid" | "fprintfMat" | "mprintf" | "stripblanks" | "stringbox" | "strindex" | "string" | "residu" | "getinstalledlookandfeels" | "getio" | "optim" | "maxi" | "xs2jpg" | "fadj2sp" | "conj" | "backslash" | "rankqr" | "link" | "ta2lpd" | "realtimeinit" | "realtime" | "real" | "isalphanum" | "eval_cshep2d" | "xdel" | "amell" | "mtell" | "unglue" | "isglobal" | "ceil" | "tril" | "buildDoc" | "buildouttb" | "callblk" | "call" | "fullpath" | "full" | "chol" | "bool2s" | "toolbar" | "ppol" | "xpolys" | "xpoly" | "fsolve" | "impl" | "bsplin3val" | "duplicate" | "lsslist" | "getlookandfeel" | "getlongpathname" | "getlanguage" | "setlookandfeel" | "setlanguage" | "imult" | "champ1" | "champ" | "xname" | "ordmmd" | "rpem" | "delmenu" | "gammaln" | "gamma" | "linmeq" | "geom3d" | "zoom_rect" | "fromjava" | "fromc" | "prompt" | "expm" | "permutobj" | "format" | "getmodules" | "getmemory" | "getmd5" | "setmenu" | "%i_matrix" | "%i_mini" | "%i_maxi" | "%i_mput" | "%i_min" | "%i_max" | "grand" | "atan" | "opentk" | "cdfnbn" | "cdfnor" | "sign" | "argn" | "bfinit" | "sfinit" | "dhinf" | "ulink" | "pointer_xproperty" | "printfigure" | "printsetupbox" | "print" | "asin" | "banner" | "iconvert" | "iconvert" | "rcond" | "spones" | "kron" | "inpnvi" | "varn" | "warnBlockByUID" | "warning" | "strncpy" | "round" | "uicontrol" | "uicontextmenu" | "spcompack" | "mucomp" | "xlfont" | "xchoicesi" | "tohome" | "mclose" | "relocate_handle" | "removedir" | "removelinehistory" | "floor" | "xfpolys" | "xfpoly" | "uipopup" | "import_from_hdf5" | "exportUI" | "export_to_hdf5" | "zeros" | "error" | "lusolve" | "getos" | "bezout" | "unzoom" | "xs2pdf" | "xs2png" | "xs2ppm" | "xs2ps" | "swap_handles" | "beep" | "grep" | "cdfpoi" | "umfpack" | "ClipBoard" | "helpbrowser" | "simp_mode" | "simp" | "completion" | "comp" | "cumprod" | "funptr" | "disposefftwlibrary" | "displayhistory" | "dispbpt" | "disp" | "%msp_spget" | "%msp_full" | "Matplot1" | "Matplot" | "getpid" | "intppty" | "%i_prod" | "%i_p" | "freq" | "isequalbitwise" | "isequal" | "nearfloat" | "xfarcs" | "xfarc" | "diary" | "sparse" | "librarieslist" | "libraryinfo" | "macr2tree" | "macr2lst" | "whereis" | "where" | "iserror" | "user" | "degree" | "gsort" | "mfprintf" | "toprint" | "msprintf" | "lsqrsolve" | "xarrows" | "merror" | "corr" | "strrchr" | "strrev" | "dasrt" | "matrix" | "sctree" | "getrelativefilename" | "xstringb" | "xstring" | "xs2svg" | "phase_simulation" | "slash" | "isascii" | "ones" | "raise_window" | "glist" | "mlist" | "rlist" | "tlist" | "exists" | "blkslvi" | "cumsum" | "winsid" | "acos" | "xcosDiagramToHDF5" | "xcosDiagramOpen" | "xcosDiagramClose" | "xcosNotify" | "xcos" | "closeEditor" | "closeXcos" | "strsplit" | "strsubst" | "strspn" | "strstr" | "dassl" | "besselh" | "besseli" | "besselj" | "besselk" | "bessely" | "hess" | "messagebox" | "getscilabmode" | "getscicosvars" | "getshortpathname" | "lstsize" | "%i_sum" | "qp_solve" | "what" | "dgettext" | "mgetstr" | "mgeti" | "mgetl" | "mget" | "xgetmouse" | "xgetech" | "xget" | "xsetech" | "xset" | "cdft" | "waitbar" | "editor" | "write4b" | "writb" | "write" | "ltitr" | "rtitr" | "xtitle" | "exit" | "uint32" | "uint16" | "uint8" | "contour2di" | "contr" | "plot2d1" | "plot3d1" | "plot2d2" | "plot2d3" | "plot2d4" | "plot2d" | "plot3d" | "roots" | "captions" | "emptystr" | "part" | "fort" | "sqrt" | "strtod" | "strtok" | "lasterror" | "testmatrix" | "historymanager" | "historysize" | "list" | "host" | "system_getproperty" | "system_setproperty" | "gettext" | "inttype" | "mputstr" | "mput" | "%i_tril" | "%i_triu" | "dnaupd" | "znaupd" | "xpause" | "dsaupd" | "debug" | "dneupd" | "zneupd" | "dseupd" | "schur" | "triu" | "about" | "uiputfile" | "resume" | "return" | "ldivf" | "ldiv" | "rdivf" | "convstr" | "getvariablesonstack" | "getversion" | "drawlater" | "drawaxis" | "drawnow" | "draw" | "show_window" | "showalluimenushandles" | "show_pixmap" | "fftw_flags" | "fftwlibraryisloaded" | "fftw_forget_wisdom" | "fftw" | "frexp" | "unix" | "PlaySound" | "grayplot" | "poly" | "copyfile" | "copy" | "tr_zer" | "TCL_SetVar" | "TCL_GetVersion" | "TCL_GetVar" | "TCL_UpVar" | "TCL_DeleteInterp" | "TCL_UnsetVar" | "TCL_EvalFile" | "TCL_EvalStr" | "TCL_DoOneEvent" | "TCL_ExistVar" | "TCL_ExistArray" | "TCL_ExistInterp" | "end_scicosim" | "umf_lufact" | "umf_ludel" | "umf_luget" | "umf_luinfo" | "umf_lusolve" | "sci_tree2" | "sci_tree3" | "sci_tree4" | "sci_haltscicos" | "lsq_splin" | "res_with_prec" | "xls_read" | "xls_open" | "get_absolute_file_path" | "get_fftw_wisdom" | "set_blockerror" | "set_xproperty" | "set_fftw_wisdom" | "lib" | "fec" | "clc" | "qld" | "and" | "svd" | "pwd" | "ode" | "eye" | "log" | "sum" | "tan" | "min" | "sin" | "who" | "exp" | "lsq" | "abs" | "cos" | "rat" | "det" | "get" | "set" | "int" | "inv" | "max" | "nnz" | "cd" | "or" | "qr" | "lu" | "_"

macros = "del_module_help_chapter" | "xmltops" | "help_skeleton" | "xmltojar" | "xmltohtml" | "manedit" | "extract_help_examples" | "check_help" | "xmltoformat" | "xmlfiletohtml" | "apropos" | "help_from_sci" | "xmltochm" | "del_help_chapter" | "add_module_help_chapter" | "find_links" | "xmltopdf" | "clean_help" | "add_help_chapter" | "help" | "edit_error" | "importScicosPal" | "importScicosDiagram" | "xcosShowBlockWarning" | "xcosBlockInterface" | "xcos_compile" | "loadScicosLibs" | "xcos_simulate" | "xcosConfigureModelica" | "xcosBlockEval" | "errorDiagramPath" | "generateBlockImages" | "xcosCodeGeneration" | "xcosClearBlockWarning" | "xcos_run" | "closeDiagramPath" | "xcos_open" | "getDiagramVersion" | "importXcosDiagram" | "openDiagramPath" | "xcos_close" | "loadmatfile" | "ReadmiMatrix" | "savematfile" | "WritemiMatrix" | "cbAtomsGui" | "atomsGui" | "atomsRemove" | "atomsList" | "atomsDepTreeShow" | "atomsRepositoryAdd" | "atomsTest" | "atomsShow" | "atomsGetLoaded" | "atomsAutoload" | "atomsLoad" | "atomsSystemInit" | "atomsSetConfig" | "atomsSystemUpdate" | "atomsCategoryList" | "atomsAutoloadDel" | "atomsIsLoaded" | "atomsUpdate" | "atomsGetInstalled" | "atomsAutoloadAdd" | "atomsSearch" | "atomsAutoloadList" | "atomsIsInstalled" | "atomsRepositoryList" | "atomsGetLoadedPath" | "atomsRepositoryDel" | "atomsVersion" | "atomsInstall" | "list_param" | "remove_param" | "is_param" | "add_param" | "init_param" | "set_param" | "get_param" | "optim_sa" | "neigh_func_vfsa" | "compute_initial_temp" | "neigh_func_csa" | "temp_law_csa" | "temp_law_default" | "temp_law_fsa" | "neigh_func_default" | "accept_func_vfsa" | "temp_law_huang" | "neigh_func_fsa" | "accept_func_default" | "temp_law_vfsa" | "crossover_ga_default" | "mutation_ga_default" | "coding_ga_binary" | "init_ga_default" | "pareto_filter" | "optim_ga" | "optim_nsga" | "optim_nsga2" | "convert_to_float" | "coding_ga_identity" | "crossover_ga_binary" | "mutation_ga_binary" | "selection_ga_elitist" | "optim_moga" | "selection_ga_random" | "test_on_columns" | "cond2sp" | "rafiter" | "ReadHBSparse" | "condestsp" | "replace_Ix_by_Fx" | "scisptdemo" | "PlotSparse" | "ana_style" | "scicos_simulate" | "scicos_demonstration" | "steadycos" | "scicos" | "lincos" | "with_modelica_compiler" | "create_gif" | "xcos_workspace_init" | "fixedpointgcd" | "returntoscilab" | "find_scicos_version" | "initial_scicos_tables" | "with_ocaml" | "create_palette" | "get_scicos_version" | "scilab2scicos" | "scicos_getvalue" | "readxls" | "write_csv" | "read_csv" | "demo_compiler" | "demo_folder_choice" | "demo_choose" | "demo_viewCode" | "demo_function_choice" | "demo_begin" | "demo_mdialog" | "demo_message" | "demo_file_choice" | "demo_run" | "demo_gui" | "demo_end" | "tbx_builder_gateway_lang" | "tbx_build_help" | "bench_run" | "nonreg_test_run" | "tbx_build_gateway_loader" | "tbx_builder_src_lang" | "tbx_build_help_loader" | "tbx_builder_src" | "test_clean" | "testexamples" | "tbx_build_macros" | "devtools_run_builder" | "scitest" | "tbx_build_gateway_clean" | "tbx_builder_help_lang" | "tbx_build_cleaner" | "tbx_build_gateway" | "unit_test_run" | "launchtest" | "test_run" | "tbx_builder_help" | "tbx_build_loader" | "tbx_builder_gateway" | "tbx_build_src" | "tbx_builder_macros" | "mapsound" | "lin2mu" | "sound" | "wavwrite" | "analyze" | "soundsec" | "auwrite" | "playsnd" | "mu2lin" | "loadwave" | "wavread" | "auread" | "savewave" | "texout" | "addmenus" | "plotout" | "editvar" | "editvar_get" | "winlist" | "TCL_CreateSlave" | "browsevar" | "editvar_set_values" | "gettklib" | "sciGUI_init" | "winclose" | "config" | "m2sci_gui" | "matfile2sci" | "mfile2sci" | "translatepaths" | "cb_m2sci_gui" | "sci2map" | "mtlb_uint8" | "mtlb_beta" | "mtlb_int32" | "mtlb_i" | "mtlb_conv" | "mtlb" | "mtlb_qr" | "mtlb_full" | "mtlb_find" | "mtlb_subplot" | "mtlb_setstr" | "mtlb_ones" | "mtlb_fopen" | "mtlb_cumsum" | "mtlb_diag" | "mtlb_rand" | "mtlb_axes" | "mtlb_grid" | "asciimat" | "mtlb_uint16" | "mtlb_imp" | "mtlb_axis" | "mtlb_semilogy" | "mtlb_e" | "char" | "mtlb_strcmpi" | "mtlb_image" | "mtlb_cumprod" | "mtlb_isspace" | "mtlb_median" | "mtlb_std" | "mtlb_colordef" | "mtlb_triu" | "mtlb_diff" | "mtlb_num2str" | "mtlb_fwrite" | "mstr2sci" | "mtlb_more" | "mtlb_loglog" | "mtlb_mesh" | "firstnonsingleton" | "mtlb_strcmp" | "mtlb_legendre" | "mtlb_cov" | "mtlb_double" | "mtlb_int16" | "makecell" | "mtlb_semilogx" | "mtlb_plot" | "mtlb_int8" | "mtlb_upper" | "mtlb_t" | "mtlb_l" | "mtlb_findstr" | "mtlb_prod" | "mtlb_dec2hex" | "mtlb_box" | "mtlb_sprintf" | "mtlb_logic" | "mtlb_0" | "mtlb_fscanf" | "mtlb_realmax" | "sci_sparse" | "mtlb_rcond" | "mtlb_mean" | "mtlb_strfind" | "mtlb_choices" | "mtlb_false" | "mtlb_isletter" | "mtlb_eval" | "mtlb_randn" | "mtlb_a" | "mtlb_fftshift" | "mtlb_lower" | "mtlb_isa" | "mtlb_var" | "mtlb_hold" | "mtlb_filter" | "mtlb_pcolor" | "mtlb_delete" | "mtlb_qz" | "mtlb_type" | "mtlb_s" | "mtlb_uint32" | "mtlb_sum" | "mtlb_any" | "mtlb_isfield" | "mtlb_sscanf" | "mtlb_min" | "mtlb_max" | "mtlb_realmin" | "mtlb_toeplitz" | "mtlb_fft" | "mtlb_dir" | "mtlb_get" | "enlarge_shape" | "mtlb_linspace" | "mtlb_meshdom" | "mtlb_format" | "mtlb_eye" | "mtlb_strrep" | "mtlb_true" | "mtlb_all" | "mtlb_sort" | "mtlb_fread" | "mtlb_size" | "mtlb_sortrows" | "mtlb_fprintf" | "mtlb_zeros" | "mtlb_exist" | "mtlb_is" | "mtlb_cond" | "mtlb_tril" | "mtlb_ifft" | "fseek_origin" | "mtlb_close" | "mtlb_logical" | "mtlb_fliplr" | "mtlb_echo" | "mtlb_error" | "mtlb_repmat" | "moment" | "harmean" | "tabul" | "trimmean" | "ftuneq" | "samplef" | "nand2mean" | "mean" | "nancumsum" | "variance" | "nanmeanf" | "geomean" | "stdev" | "mad" | "iqr" | "nfreq" | "covar" | "variancef" | "sample" | "strange" | "median" | "nansum" | "correl" | "nanmedian" | "regress" | "msd" | "center" | "thrownan" | "show_pca" | "meanf" | "quart" | "wcenter" | "princomp" | "mvvacov" | "nanmax" | "nanstdev" | "samwr" | "ftest" | "nanmin" | "mvcorrel" | "nanmean" | "perctl" | "st_deviation" | "cmoment" | "pca" | "stdevf" | "etime" | "datenum" | "eomday" | "weekday" | "tic" | "calendar" | "date" | "now" | "clock" | "isLeapYear" | "toc" | "datevec" | "arobasestring2strings" | "eval" | "isnum" | "strcmpi" | "instruction2code" | "rhs2code" | "expression2code" | "tokenpos" | "justify" | "ascii2string" | "cat_code" | "sci2exp" | "evstr" | "format_txt" | "tree2code" | "blanks" | "erfinv" | "pcg" | "issparse" | "adj2sp" | "gmres" | "sp2adj" | "qmr" | "eqiir" | "fftshift" | "cheb1mag" | "hilb" | "analpf" | "filter" | "wfir" | "faurre" | "ifft" | "zpch1" | "lattn" | "intdec" | "lev" | "lindquist" | "phc" | "dft" | "yulewalk" | "filt_sinc" | "srfaur" | "cepstrum" | "window" | "iirlp" | "eqfir" | "iirgroup" | "fft2" | "cheb2mag" | "srkf" | "jmat" | "convol" | "wiener" | "zpch2" | "mrfit" | "find_freq" | "group" | "hilbert" | "zpell" | "sincd" | "pspect" | "trans" | "hank" | "levin" | "iirmod" | "findm" | "detrend" | "wigner" | "bilt" | "remezb" | "ffilt" | "buttmag" | "zpbutt" | "ell1mag" | "frmag" | "sskf" | "chepol" | "mfft" | "kalm" | "fsfirlin" | "casc" | "mese" | "system" | "iir" | "cspect" | "czt" | "frfit" | "detr" | "coffg" | "pfactors" | "colcompr" | "pdiv" | "hermit" | "derivat" | "hrmt" | "numer" | "lcm" | "diophant" | "inv_coeff" | "sylm" | "cmndred" | "pol2des" | "horner" | "denom" | "lcmdiag" | "htrianr" | "systmat" | "polfact" | "invr" | "gcd" | "pol2str" | "rowcompr" | "determ" | "factors" | "st_i_generic" | "generic_i_s" | "generic_i_hm" | "generic_i_st" | "generic_i_ce" | "createstruct" | "generic_i_h" | "optimsimplex_getx" | "optimsimplex_log" | "optimsimplex_xbar" | "optimsimplex_deltafv" | "optimsimplex_compsomefv" | "optimsimplex_getallfv" | "optimsimplex_gradientfv" | "optimsimplex_print" | "optimsimplex_check" | "optimsimplex_tostring" | "optimsimplex_shrink" | "optimsimplex_getall" | "optimsimplex_new" | "optimsimplex_computefv" | "optimsimplex_getn" | "optimsimplex_setnbve" | "optimsimplex_setall" | "optimsimplex_fvvariance" | "optimsimplex_setfv" | "optimsimplex_center" | "optimsimplex_destroy" | "optimsimplex_fvstdev" | "optimsimplex_setx" | "optimsimplex_size" | "optimsimplex_getallx" | "optimsimplex_getve" | "optimsimplex_getnbve" | "optimsimplex_setallfv" | "optimsimplex_setn" | "optimsimplex_reflect" | "optimsimplex_fvmean" | "optimsimplex_dirmat" | "optimsimplex_setallx" | "optimsimplex_sort" | "optimsimplex_setve" | "optimsimplex_deltafvmax" | "optimsimplex_getfv" | "optimbase_histset" | "optimbase_hasnlcons" | "optimbase_new" | "optimbase_isinbounds" | "optimbase_cget" | "optimbase_configure" | "optimbase_hasconstraints" | "optimbase_proj2bnds" | "optimbase_histget" | "optimbase_checkx0" | "optimbase_outstruct" | "optimbase_incriter" | "optimbase_stoplog" | "optimbase_destroy" | "optimbase_hasbounds" | "optimbase_checkcostfun" | "optimbase_display" | "optimbase_outputcmd" | "optimbase_logshutdown" | "optimbase_set" | "optimbase_logstartup" | "optimbase_get" | "optimbase_isfeasible" | "optimbase_log" | "optimbase_function" | "optimbase_checkbounds" | "optimbase_isinnonlincons" | "optimbase_terminate" | "nmplot_historyplot" | "nmplot_contour" | "neldermead_new" | "nmplot_restart" | "optimget" | "nmplot_simplexhistory" | "nmplot_display" | "nmplot_log" | "neldermead_costf" | "optimset" | "nmplot_configure" | "optimplotfval" | "nmplot_new" | "optimplotfunccount" | "nmplot_cget" | "neldermead_get" | "neldermead_log" | "neldermead_cget" | "neldermead_restart" | "neldermead_display" | "nmplot_function" | "nmplot_destroy" | "nmplot_outputcmd" | "neldermead_updatesimp" | "neldermead_function" | "nmplot_search" | "fminsearch" | "neldermead_configure" | "nmplot_get" | "neldermead_search" | "neldermead_destroy" | "optimplotx" | "fit_dat" | "numdiff" | "datafit" | "unpack" | "pack" | "vec2list" | "bvodeS" | "leastsq" | "karmarkar" | "lmisolver" | "aplat" | "NDcost" | "pencost" | "recons" | "derivative" | "list2vec" | "lmitool" | "qpsolve" | "trace" | "polar" | "pinv" | "nlev" | "rank" | "orth" | "linsolve" | "gspec" | "projspec" | "gschur" | "coff" | "sva" | "quaskro" | "companion" | "rowcomp" | "psmall" | "range" | "aff2ab" | "im_inv" | "spantwo" | "rref" | "eigenmarkov" | "proj" | "spanplus" | "kernel" | "glever" | "chfact" | "classmarkov" | "pbig" | "givens" | "cmb_lin" | "norm" | "diff" | "squeeze" | "rowshuff" | "sqroot" | "pencan" | "pen2ea" | "kroneck" | "fullrfk" | "penlaur" | "colcomp" | "fullrf" | "householder" | "randpencil" | "chsolve" | "cond" | "spaninter" | "genmarkov" | "check_librarypath" | "check_classpath" | "jre_path" | "printf" | "sprintf" | "prettyprint" | "ssprint" | "unix_g" | "halt" | "readc_" | "getscilabkeywords" | "unix_s" | "unix_w" | "unix_x" | "input" | "interp1" | "interpln" | "smooth" | "ilib_build" | "findlcccompiler" | "gencompilationflags_unix" | "with_lcc" | "dllinfo" | "G_make" | "findmsvccompiler" | "configure_msvc" | "detectmsvc64tools" | "findmsifortcompiler" | "detectmsifort64tools" | "chooselcccompiler" | "configure_msifort" | "ilib_for_link" | "VCtoLCCLib" | "configure_lcc" | "ilib_gen_gateway" | "haveacompiler" | "ilib_gen_cleaner" | "ilib_compile" | "ilib_mex_build" | "ilib_gen_Make" | "ilib_gen_loader" | "ilib_gen_Make_unix" | "uiEqualsTree" | "uiGetParentNode" | "uiDumpTree" | "uiFindNode" | "uiInsertNode" | "uiCreateNode" | "uiDeleteNode" | "uiGetNodePosition" | "uiGetChildrenNode" | "uiConcatTree" | "uiCreateTree" | "figure" | "x_choices" | "addmenu" | "menubar" | "createpopup" | "close" | "findobj" | "clipboard" | "getvalue" | "x_matrix" | "about" | "iscell" | "iscellstr" | "hypermat" | "struct" | "isfield" | "cell" | "fieldnames" | "isstruct" | "sm2ss" | "h_cl" | "dt_ility" | "h_norm" | "epred" | "tf2des" | "linf" | "inistate" | "ctr_gram" | "h2norm" | "lqg_ltr" | "observer" | "hankelsv" | "abcd" | "des2ss" | "gainplot" | "kpure" | "noisegen" | "phasemag" | "ssrand" | "colregul" | "augment" | "st_ility" | "des2tf" | "freson" | "sm2des" | "krac2" | "leqr" | "bloc2exp" | "g_margin" | "ss2des" | "findBDK" | "svplot" | "armac" | "obsv_mat" | "rowregul" | "zgrid" | "invrs" | "narsimul" | "canon" | "lqg2stan" | "tf2ss" | "sysconv" | "pfss" | "ss2ss" | "linfn" | "ccontrg" | "findAC" | "nehari" | "show_margins" | "cfspec" | "parrot" | "leqe" | "dhnorm" | "fstabst" | "macglov" | "minss" | "specfact" | "ric_desc" | "entropy" | "m_circle" | "cainv" | "plzr" | "black" | "sylv" | "minreal" | "gtild" | "obs_gram" | "prbs_a" | "trzeros" | "ui_observer" | "findABCD" | "gfare" | "csim" | "trianfml" | "trfmod" | "stabil" | "calfrq" | "armax1" | "projsl" | "h_inf_st" | "lyap" | "armax" | "gcare" | "p_margin" | "cont_mat" | "lin" | "dscr" | "sdiff" | "acf" | "smga" | "syslin" | "unobs" | "reglin" | "dbphi" | "zeropen" | "arl2" | "h_inf" | "gfrancis" | "imrep2ss" | "arma2p" | "findR" | "chart" | "lqe" | "ss2tf" | "sysdiag" | "colinout" | "dsimul" | "riccati" | "lcf" | "contrss" | "solve" | "invsyslin" | "lqg" | "bloc2ss" | "fourplan" | "trisolve" | "obsvss" | "sensi" | "lqr" | "dtsi" | "bode" | "frep2tf" | "statgain" | "fspec" | "equil1" | "abinv" | "rowinout" | "nyquist" | "cont_frm" | "cls2dls" | "sgrid" | "equil" | "balreal" | "fspecg" | "markp2ss" | "findx0BD" | "flts" | "evans" | "copfac" | "routh_t" | "repfreq" | "arsimul" | "syssize" | "arhnk" | "time_id" | "lft" | "obscont" | "ddp" | "sysfact" | "gamitg" | "bstap" | "bilin" | "dcf" | "driver" | "xs2fig" | "xs2emf" | "xend" | "xinit" | "ged" | "getPlotPropertyName" | "dragrect" | "setTitleLabelProperty" | "fplot3d1" | "secto3d" | "pinkcolormap" | "xlabel" | "gdf" | "seteventhandler" | "xclear" | "plot3d2" | "getColorIndex" | "graduate" | "ghdl_fields" | "TitleLabel" | "autumncolormap" | "check2dFun" | "oldplot" | "zlabel" | "projaff" | "name2rgb" | "histplot" | "Sfgrayplot" | "fplot2d" | "springcolormap" | "paramfplot2d" | "fac3d" | "twinkle" | "errbar" | "legend" | "edit_curv" | "barhomogenize" | "eval3d" | "fcontour2d" | "summercolormap" | "gr_menu" | "getTitleLabelPropertyNam" | "isoview" | "hist3d" | "setDefaultColor" | "colorbar" | "clf" | "ylabel" | "checkXYPair" | "sda" | "title" | "hsvcolormap" | "bonecolormap" | "mesh" | "surf" | "Sgrayplot" | "setSurfProperty" | "fac3d1" | "getlinestyle" | "xstringl" | "gda" | "genfac3d" | "pie" | "ResetFigureDDM" | "addcolor" | "coppercolormap" | "xbasr" | "eval3dp" | "gcf" | "graycolormap" | "nf3d" | "xnumb" | "barh" | "replot" | "ged_insert" | "bar" | "get_figure_handle" | "milk_drop" | "jetcolormap" | "coolcolormap" | "colordef" | "subplot" | "hotcolormap" | "ghdl2tree" | "oceancolormap" | "whitecolormap" | "scf" | "fchamp" | "gca" | "contour" | "plotframe" | "getSurfPropertyName" | "sdf" | "square" | "setPlotProperty" | "rotate" | "fcontour" | "xselect" | "contour2d" | "getsymbol" | "polarplot" | "wintercolormap" | "graypolarplot" | "scaling" | "titlepage" | "rainbowcolormap" | "xrpoly" | "legends" | "plot3d3" | "locate" | "getLineSpec" | "getcolor" | "sd2sci" | "sca" | "xsetm" | "xinfo" | "gce" | "rgb2name" | "color" | "getfont" | "getmark" | "fgrayplot" | "xsave" | "plot" | "contourf" | "fplot3d" | "xload" | "hsv2rgb" | "fprintf" | "sscanf" | "dispfiles" | "is_absolute_path" | "listfiles" | "ls" | "get_file_path" | "mputl" | "rmdir" | "scanf" | "mkdir" | "dirname" | "mdelete" | "fullfile" | "dir" | "listvarinfile" | "fscanf" | "profile" | "fun2string" | "showprofile" | "get_function_path" | "check_gateways" | "macrovar" | "get_profile" | "createfun" | "reset_profiling" | "add_profiling" | "bytecode" | "warnobsolete" | "getd" | "recompilefunction" | "genlib" | "bytecodewalk" | "head_comments" | "remove_profiling" | "plotprofile" | "listfunctions" | "genlib_old" | "cosh" | "bitcmp" | "vectorfind" | "csch" | "atanhm" | "cotg" | "bitget" | "erf" | "atanh" | "bitand" | "asech" | "unique" | "asinhm" | "cscd" | "bin2dec" | "flipdim" | "bitxor" | "asec" | "bitset" | "acotd" | "GLoad" | "intsplin" | "atanm" | "cat" | "acosm" | "cellstr" | "toeplitz" | "nextpow2" | "sech" | "isempty" | "tanm" | "squarewave" | "setdiff" | "lex_sort" | "speye" | "acscd" | "acosd" | "log2" | "base2dec" | "inttrap" | "convertindex" | "csgn" | "bitor" | "acosh" | "resize_matrix" | "cosd" | "sind" | "erfc" | "acoshm" | "coshm" | "dec2bin" | "meshgrid" | "linspace" | "fix" | "cell2mat" | "sinhm" | "pertrans" | "ndgrid" | "sinc" | "hex2dec" | "tanh" | "sec" | "logspace" | "asinm" | "acot" | "cosm" | "sinm" | "spzeros" | "acsch" | "num2cell" | "dec2hex" | "atand" | "ndims" | "sinh" | "union" | "complex" | "factor" | "sub2ind" | "tand" | "asind" | "cotd" | "isvector" | "IsAScalar" | "oct2dec" | "cothm" | "asecd" | "isnan" | "intersect" | "isinf" | "csc" | "ind2sub" | "coth" | "sqrtm" | "binomial" | "log10" | "secd" | "perms" | "asinh" | "signm" | "dec2oct" | "modulo" | "acoth" | "primes" | "permute" | "factorial" | "erfcx" | "null" | "sprand" | "acsc" | "pmodulo" | "logm" | "tanhm" | "intc" | "intl" | "dae" | "integrate" | "daeoptions" | "odeoptions" | "check_modules_xml" | "edit" | "with_macros_source" | "with_javasci" | "whos" | "with_atlas" | "whereami" | "getshell" | "with_pvm" | "toolboxes" | "ver" | "typeof" | "who_user" | "perl" | "with_texmacs" | "check_versions" | "add_demo" | "OS_Version" | "with_tk"

authors = "Calixte Denizet" | "Calixte DENIZET" | "Sylvestre Ledru" | "Sylvestre LEDRU" | "Yann Collette" | "Yann COLLETTE" | "Allan Cornet" | "Allan CORNET" | "Allan Simon" | "Allan SIMON" | "Antoine Elias" | "Antoine ELIAS" | "Bernard Hugueney" | "Bernard HUGUENEY" | "Bruno Jofret" | "Bruno JOFRET" | "Claude Gomez" | "Claude GOMEZ" | "Clement David" | "Clement DAVID" | "Jerome Picard" | "Jerome PICARD" | "Manuel Juliachs" | "Manuel JULIACHS" | "Michael Baudin" | "Michael BAUDIN" | "Pierre Lando" | "Pierre LANDO" | "Pierre Marechal" | "Pierre MARECHAL" | "Serge Steer" | "Serge STEER" | "Vincent Couvert" | "Vincent COUVERT" | "Vincent Liard" | "Vincent LIARD" | "Zhour Madini-Zouine" | "Zhour MADINI-ZOUINE" | "Inria" | "INRIA"

break = ".."(".")*

special = "$" | ":" | {break}

string = (([^\'\"\r\n]*)|([\'\"]{2}))+
dqstring = \"{string}(\"|\')

id = ([a-zA-Z%_#!?][a-zA-Z0-9_#!$?]*)|("$"[a-zA-Z0-9_#!$?]+)

dot = "."

url = "http://"[^ \t\f\n\r]+

digit = [0-9]
exp = [eE][+-]?{digit}+
number = ({digit}+\.?{digit}*{exp}?)|(\.{digit}+{exp}?)

%x QSTRING, COMMENT, FIELD, COMMANDS

%%

<YYINITIAL> {
  {comment}			 { 
  				   transposable = false;
				   yybegin(COMMENT);
				 }
				 
  {operator}                     { 
  				   transposable = false;
				   return ScilabLexerConstants.OPERATOR;
				 }

  {functionKwds}		 { 
  				   transposable = false;
				   return ScilabLexerConstants.FKEYWORD;
				 }

  {structureKwds}		 { 
  				   transposable = false;
				   return ScilabLexerConstants.SKEYWORD;
				 }

  {controlKwds}			 { 
  				   transposable = false;
				   return ScilabLexerConstants.CKEYWORD;
				 }

  {booleen}			 { 
  				   transposable = true;	
				   return ScilabLexerConstants.BOOLEAN;
				 }

  {commands}			 |
  {macros}			 {
  				   transposable = true;
				   yybegin(COMMANDS);
				   return ScilabLexerConstants.COMMANDS;
				 }

  {id}				 { 
  				   transposable = true;
				   return ScilabLexerConstants.ID;
				 }

  {number}			 { 
  				   transposable = true;
				   return ScilabLexerConstants.NUMBER;
				 }

  {special}			 {
				   transposable = false;
				   return ScilabLexerConstants.SPECIAL;
				 }

  {dot}				 {
				   transposable = false;
				   yybegin(FIELD);
				   return ScilabLexerConstants.OPERATOR;
				 }
 	
  {quote}			 {
				    if (transposable) {
				       return ScilabLexerConstants.TRANSP;
				    } else {
				       yybegin(QSTRING);
				    }
				 }

  {open}			 { 
  				   transposable = false;
				   return ScilabLexerConstants.OPEN;
				 }

  {close}			 { 
  				   transposable = true;
				   return ScilabLexerConstants.CLOSE;
				 }

  {dqstring}			 { 
  				   transposable = false;
				   return ScilabLexerConstants.STRING;
				 }
}

<COMMANDS> {
  {white}[^,;]*			 {
  				   return ScilabLexerConstants.STRING;
				 }
  .
  				 {
				   yypushback(1);
				   yybegin(YYINITIAL);
				 }

  {eol}				 { }
}

<FIELD> {
  {id}				 {
  				   return ScilabLexerConstants.FIELD;
				 }

  .				 {
				   yypushback(1);
				   yybegin(YYINITIAL);
				 }

  {eol}				 { }
}  				 

<QSTRING> {
  {string}			 { }
  
  (\'|\")			 { 
  				   transposable = false;
  				   yybegin(YYINITIAL);
				   return ScilabLexerConstants.STRING;
				 }
  
  .				 |				 
  {eol} 			 {
				   return ScilabLexerConstants.DEFAULT;
				 }

}

<COMMENT> {
  {authors}			 {
				   return ScilabLexerConstants.AUTHORS;
				 }

  {url}				 {
  				   return ScilabLexerConstants.URL;
				 }	  	  

  .	  		         | 
  {eol}				 { 
  				   return ScilabLexerConstants.COMMENT;
  				 }
}


{white}				 { 
				   transposable = false;
				 }

.                                { 
				   transposable = false;
				   return ScilabLexerConstants.DEFAULT;
				 }

{eol}				 { }

<<EOF>>                          {
				   return ScilabLexerConstants.EOF;
				 }