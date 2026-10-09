/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "context.hxx"
#include "getdeprecated.hxx"
#include "getdeprecated.h"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
#include "dynlib_core.h"
}

std::unordered_map<std::wstring, std::wstring> getDeprecated()
{
    return {
        // Scilab 2027.0.0 => 2027.1.0
        {L"tbx_make(tbx_path, section, languages)", L"tbx_make(tbx_path, section)"}
    };
}

char** getDeprecatedNames(int* size)
{
    std::unordered_map<std::wstring, std::wstring> deprecated = getDeprecated();
    *size = static_cast<int>(deprecated.size());
    if (*size == 0)
    {
        return nullptr;
    }

    char** names = static_cast<char**>(MALLOC(*size * sizeof(char*)));
    int index = 0;
    for (const auto& entry : deprecated)
    {
        names[index++] = wide_string_to_UTF8(entry.first.c_str());
    }
    return names;
}

std::unordered_map<std::wstring, std::wstring> getDeleted()
{
    return {
        // Scilab 2026.1.0 => 2027.0.0
        {L"readmps", L"'quapro' toolbox"},

        // Scilab 2026.0.0 => 2027.0.0
        {L"dos", L"host"},
        {L"unix", L"host"},
        {L"unix_g", L"host"},
        {L"unix_s", L"host"},
        {L"unix_w", L"host"},
        {L"unix_x", L"host"},

        // Scilab 2026.0.0 => 2026.1.0
        {L"demo_begin", L"exec"},
        {L"demo_run", L"exec"},
        {L"demo_end", L"exec"},
        {L"demo_compiler", L"haveacompiler"},
        {L"demo_file_choice", L"x_choose"},
        {L"demo_function_choice", L"x_choose"},
        {L"demo_choose", L"x_choose"},

        // Scilab 2025.0.0 => 2026.0.0
        {L"captions", L"legend"},
        {L"impl", L"dae"},
        {L"princomp", L"pca"},
        {L"testmatrix", L"magic, invhilb, frank"},

        // Scilab 2024.1.0 => 2026.0.0
        {L"dassl", L"dae"},
        {L"dasrt", L"dae"},
        {L"daskr", L"dae"},
        {L"autumncolormap", L"autumn"},
        {L"bonecolormap", L"bone"},
        {L"coolcolormap", L"cool"},
        {L"coppercolormap", L"copper"},
        {L"graycolormap", L"gray"},
        {L"hotcolormap", L"hot"},
        {L"hsvcolormap", L"hsv"},
        {L"jetcolormap", L"jet"},
        {L"oceancolormap", L"ocean"},
        {L"parulacolormap", L"parula"},
        {L"pinkcolormap", L"pink"},
        {L"rainbowcolormap", L"rainbow"},
        {L"springcolormap", L"spring"},
        {L"summercolormap", L"summer"},
        {L"whitecolormap", L"white"},
        {L"wintercolormap", L"winter"},

        // Scilab 2024.0.0 => 2025.0.0
        {L"getURL", L"http_get"},
        {L"splitURL", L"url_split"},
        {L"sound", L"playsnd"},

        // Scilab 2023.0.0 => 2024.0.0
        {L"xget", L""},
        {L"xset", L""},
        //{L"svd(X,0)", L"svd(X,\"e\")"}, // will never be detected as is
        {L"plotframe", L"plot2d"},

        // Scilab 6.1.x => 2023.0.0
        {L"fplot2d", L"plot"},
        {L"xdel", L"close"},
        {L"xname", L"gcf().figure_name"},
        {L"soundsec", L"0 : 1/freq : t*(1-%eps)"},
        {L"importgui", L"uiSpreadsheet"},
        {L"closeEditvar", L"close editvar"},

        // Scilab 6.1.0 => 6.1.x
        {L"scatter3", L"scatter3d"},
        {L"get_figure_handle", L"findobj(\'figure_id\',n)"},
        {L"noisegen", L"grand"},
        {L"%sn", L"ellipj"},
        {L"champ1", L"champ.colored"},
        {L"saveafterncommands", L""},
        {L"setPreferencesValue", L"xmlSetValues"},
        {L"sysdiag", L"blockdiag"},
        {L"ric_desc", L"riccati"},

        // Scilab 6.0.x => 6.1.0
        {L"dirname", L"fileparts"},
        {L"_d", L"_"},
        {L"dgettext", L"gettext"},
        {L"datatipToggle", L"datatipManagerMode"},
        {L"denom", L".den"},
        {L"eval", L"evstr"},
        {L"frexp", L"log2"},
        {L"getPreferencesValue", L"xmlGetValues"},
        {L"hypermat", L"zeros|matrix"},
        {L"lstsize", L"size"},
        {L"nanmin", L"min"},
        {L"nanmax", L"max"},
        {L"numer", L".num"},
        {L"square", L"replot"},
        {L"with_tk", L"with_module('tclsci')"},
        {L"xgetech", L"gca"},
        {L"xinfo", L"gcf().info_message"},

        // Scilab 5.5.2 => 6.0.0
        {L"fort", L"call"},
        {L"znaupd", L"eigs"},
        {L"zneupd", L"eigs"},
        {L"dseupd", L"eigs"},
        {L"dneupd", L"eigs"},
        {L"dnaupd", L"eigs"},
        {L"dsaupd", L"eigs"},

        {L"m_circle", L"hallchart"},
        {L"plot2d1", L"plot2d"},
        {L"xclear", L"clf"},
        {L"datatipSetStruct", L""},
        {L"datatipGetStruct", L""},
        {L"fcontour2d", L"contour2d"},
        {L"fcontour", L"contour"},
        {L"fac3d", L"plot3d"},
        {L"fac3d1", L"plot3d1"},
        {L"eval3d", L"ndgrid"},

        {L"gspec", L"spec"},
        {L"gschur", L"schur"},
        {L"rafiter", L"taucs_chsolve"},
        {L"numdiff", L"numderivative"},
        {L"derivative", L"numderivative"},
        {L"mvvacov", L"cov"},

        {L"curblockc", L"curblock"},
        {L"extract_help_examples", L""},
        {L"havewindow", L"getscilabmode"},
        {L"isequalbitwise", L"[ans,msg]=assert_checkequal(a,b)"},
        {L"jconvMatrixMethod", L"jautoTranspose"},
        {L"lex_sort", L"gsort"},
        {L"mtlb_mode", L"oldEmptyBehaviour"},
        {L"perl", L""},
        {L"strcmpi", L"strcmp"},
        {L"xpause", L"sleep"},

        {L"addf", L""},
        {L"subf", L""},
        {L"mulf", L""},
        {L"ldivf", L""},
        {L"rdivf", L""},
        {L"cmb_lin", L""},
        {L"solve", L""},
        {L"trianfml", L""},
        {L"trisolve", L""},
        {L"bloc2exp", L""},

        {L"comp", L"exec"},
        {L"errcatch", L""},
        {L"iserror", L""},
        {L"str2code", L"ascii"},
        {L"code2str", L"ascii"},
        {L"fun2string", L"string"},
        {L"getvariablesonstack", L"who"},
        {L"gstacksize", L""},
        {L"stacksize", L""},
        {L"macr2lst", L""},
        {L"readgateway", L""},

        // Scilab 5.5.1 => 5.5.2
        {L"%asn", L"delip"},
        {L"chart", L"nicholschart"},
        {L"IsAScalar", L"isscalar"},
        {L"jmat", L"flipdim"},
        {L"mfft", L"ftt"},
        {L"milk_drop", L""},
        {L"msd", L"stdev"},
        {L"nfreq", L"tabul"},
        {L"pcg", L"conjgrad"},
        {L"regress", L"reglin"},
        {L"relocate_handle", L""},
        {L"st_deviation", L"stdev"},
        {L"xmltochm", L""},
        {L"xsetm", L""},

        // Scilab 5.5.0 => 5.5.1
        {L"datatipContextMenu", L""},
        {L"datatipEventHandler", L""},

        // SCilab 5.4.1 => 5.5.0
        {L"dft", L"fft"},
        {L"sscanf", L"msscanf"},
        {L"fscanf", L"mfscanf"},
        {L"printf", L"mprintf"},
        {L"fprintf", L"mfprintf"},
        {L"sprintf", L"msprintf"},
        {L"demo_message", L""},
        {L"demo_mdialog", L""},
        {L"draw", L""},
        {L"clear_pixmap", L""},
        {L"show_pixmap", L""},
        {L"winclose", L"close"},
        {L"datatipInitStruct", L""},
        {L"datatipRedraw", L""},
        {L"getfont", L""},
        {L"getmark", L""},
        {L"getlinestyle", L""},
        {L"getsymbol", L""},
        {L"with_embedded_jre", L""},
        {L"fit_dat", L"datafit"},
        {L"create_palette", L""},

        // Scilab 5.4.0 => 5.4.1
        {L"chartoeom", L""},
        {L"eomtochar", L""},
        {L"config", L"preferences"},
        {L"createpopup", L"uicontextmenu"},
        {L"mtlb_conv", L"conv"},
        {L"mtlb_repmat", L"repmat"},
        {L"neldermead_display", L"disp"},
        {L"nmplot_display", L"disp"},
        {L"optimbase_display", L"disp"},
        {L"optimsimplex_print", L"disp"},
        {L"iptim_simplex_tostring", L"string"},
        {L"ricc_old", L"ricc"},
        {L"showalluimenushandles", L"set(get(0), \"ShowHiddenHandles\", \"on\")"},
        {L"with_pvm", L"getversion"},
        {L"with_texmacs", L""},
        {L"xbasr", L""},
        {L"xselect", L"show_window"},
        {L"mpopup", L"uicontextmenu"},

        // Scilab 5.3.3 => 5.4.0
        {L"MSDOS", L"getos"},
        {L"sd2sci", L""},
        {L"oldplot", L""},

        // Scilab 5.3.0 => 5.3.3: nothing removed

        // Scilab 5.2.X => 5.3.0
        {L"maxi", L"max"},
        {L"mini", L"min"},
        {L"oldbesseli", L"besseli"},
        {L"oldbesselj", L"besselj"},
        {L"oldbesselk", L"besselk"},
        {L"oldbessely", L"bessely"},
        {L"textprint", L"prettyprint"},
        {L"pol2tex", L"prettyprint"},
        {L"xgetfile", L"uigetfile"},
        {L"tk_getfile", L"uigetfile"},
        {L"tk_savefile", L"uiputfile"},
        {L"tk_getdir", L"uigetdir"},
        {L"tk_choose", L"x_choose"},
        {L"sci2excel", L"csvWrite"},
        {L"excel2sci", L"csvRead"},
        {L"x_message_modeless", L"messagebox"},
        {L"sethomedirectory", L"SCIHOME,home"},
        {L"getcwd", L"pwd"},
        {L"xbasc", L"clf"},
        {L"getf", L"exec"},
        {L"NumTokens", L"tokens"},
        {L"sort", L"gsort"},
        {L"scilab_demos", L"demo_gui"},
        {L"with_gtk", L"getversion"},
        {L"readc_", L"input"},

        // Scilab 5.2.1 => 5.2.2
        {L"oldsave", L"save"},
        {L"oldload", L"load"},

        // Scilab 5.2.0 => 5.2.1: nothing removed

        // Scilab 5.1.1 => 5.2.0
        {L"lgfft", L""},

        // Scilab 5.1.0 => 5.1.1: nothing removed

        // Scilab 5.0.X => 5.1.0
        {L"mtlb_load", L"loadmatfile"},
        {L"mtlb_save", L"savematfile"},
        {L"xbasimp", L"toprint,xs2ps"},
        {L"xg2ps", L"xs2ps"},
        {L"hidetoolbar", L"toolbar(,\'off\')"},
        {L"browsehelp", L"helpbrowser"},
        {L"quapro", L"qpsolve"},
        {L"%sp_eye", L"speye"},
        {L"TCL_gcf", L"gcf"},
        {L"TCL_scf", L"scf"},
        {L"TK_EvalStr", L"TCL_EvalStr"},
        {L"TK_GetVar", L"TCL_GetVar"},
        {L"TK_SetVar", L"TCL_SetVar"},
        {L"sciGUIhelp", L"help"},
        {L"demoplay", L"demo_gui"},
        {L"buttondialog", L"messagebox"},
        {L"tk_getvalue", L"getvalue"},

        // Scilab 5.0.1 => 5.0.3: nothing removed

        // Scilab 4.1.2 => 5.0
        {L"xclea", L"xfrect"},
        {L"xaxis", L"drawaxis"},
        {L"loadplots", L""},
        {L"xtape", L""},
        {L"loaddefaultbrowser", L""},
        {L"%browsehelp", L""},
    };
}   
