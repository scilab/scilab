/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *  Copyright (C) 2017, 2018 - Samuel GOUGEON
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "checkers/DeprecatedChecker.hxx"

namespace slint
{

std::unordered_map<std::wstring, std::wstring> DeprecatedChecker::deprecated = initDep();
std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> DeprecatedChecker::partiallyDeprecated = initPartDep();

void DeprecatedChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        const auto i = deprecated.find(name);
        if (i != deprecated.end())
        {
            if (i->second.empty())
            {
                result.report(context, e.getLocation(), *this, _("Deprecated function: %s."), name);
            }
            else
            {
                result.report(context, e.getLocation(), *this, _("Deprecated function %s: use %s instead."), name, i->second);
            }
        }
        else
        {
            const auto i = partiallyDeprecated.find(name);
            if (i != partiallyDeprecated.end())
            {
                i->second->preCheckNode(e, context, result);
            }
        }
    }
}

void DeprecatedChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string DeprecatedChecker::getName() const
{
    return "DeprecatedChecker";
}

void DeprecatedChecker::__Svd::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();
    if (args.size() == 2)
    {
        const ast::Exp & second = *args.back();
        if (second.isDoubleExp() && static_cast<const ast::DoubleExp &>(second).getValue() == 0)
        {
            result.report(context, e.getLocation(), *this, _("svd(..., 0) is deprecated."));
        }
    }
}

void DeprecatedChecker::__Mfprintf::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();
    if (args.size() != 0)
    {
        const ast::Exp & first = *args.front();
        if (first.isDoubleExp() && static_cast<const ast::DoubleExp &>(first).getValue() == -1)
        {
            result.report(context, e.getLocation(), *this, _("mfprintf(-1, ...) is deprecated."));
        }
    }
}

std::unordered_map<std::wstring, std::wstring> DeprecatedChecker::initDep()
{
    // TODO: get this list from a conf file
    std::unordered_map<std::wstring, std::wstring> map;
    // Scilab 6.0.x => 6.1.0
    map.emplace(L"dirname", L"fileparts");
    map.emplace(L"_d", L"_");
    map.emplace(L"dgettext", L"gettext");
    map.emplace(L"datatipToggle", L"datatipManagerMode");
    map.emplace(L"denom", L".den");
    map.emplace(L"getPreferencesValue", L"xmlGetValues");
    map.emplace(L"hypermat", L"zeros|matrix");
    map.emplace(L"lstsize", L"size");
    map.emplace(L"nanmin", L"min");
    map.emplace(L"nanmax", L"max");
    map.emplace(L"numer", L".num");
    map.emplace(L"square", L"replot");
    map.emplace(L"with_tk", L"with_module('tclsci')");
    map.emplace(L"xgetech", L"gca");
    map.emplace(L"xinfo", L"gcf().info_message");

    // Scilab 5.5.2 => 6.0.0
    map.emplace(L"fort", L"call");
    map.emplace(L"znaupd", L"eigs");
    map.emplace(L"zneupd", L"eigs");
    map.emplace(L"dseupd", L"eigs");
    map.emplace(L"dneupd", L"eigs");
    map.emplace(L"dnaupd", L"eigs");
    map.emplace(L"dsaupd", L"eigs");

    map.emplace(L"m_circle", L"hallchart");
    map.emplace(L"plot2d1", L"plot2d");
    map.emplace(L"xclear", L"clf");
    map.emplace(L"datatipSetStruct", L"");
    map.emplace(L"datatipGetStruct", L"");
    map.emplace(L"fcontour2d", L"contour2d");
    map.emplace(L"fcontour", L"contour");
    map.emplace(L"fac3d", L"plot3d");
    map.emplace(L"eval3d", L"ndgrid");

    map.emplace(L"gspec", L"spec");
    map.emplace(L"gschur", L"schur");
    map.emplace(L"rafiter", L"taucs_chsolve");
    map.emplace(L"numdiff", L"numderivative");
    map.emplace(L"derivative", L"numderivative");
    map.emplace(L"mvvacov", L"cov");

    map.emplace(L"perl", L"");
    map.emplace(L"lex_sort", L"gsort");
    map.emplace(L"strcmpi", L"strcmp");
    map.emplace(L"jconvMatrixMethod", L"jautoTranspose");
    map.emplace(L"havewindow", L"getscilabmode");
    map.emplace(L"xpause", L"sleep");
    map.emplace(L"curblockc", L"curblock");
    map.emplace(L"extract_help_examples", L"");
    map.emplace(L"mtlb_mode", L"oldEmptyBehaviour");

    map.emplace(L"addf", L"");
    map.emplace(L"subf", L"");
    map.emplace(L"mulf", L"");
    map.emplace(L"ldivf", L"");
    map.emplace(L"rdivf", L"");
    map.emplace(L"cmb_lin", L"");
    map.emplace(L"solve", L"");
    map.emplace(L"trianfml", L"");
    map.emplace(L"trisolve", L"");
    map.emplace(L"bloc2exp", L"");

    map.emplace(L"comp", L"exec");
    map.emplace(L"errcatch", L"");
    map.emplace(L"iserror", L"");
    map.emplace(L"str2code", L"ascii");
    map.emplace(L"code2str", L"ascii");
    map.emplace(L"fun2string", L"string");
    map.emplace(L"getvariablesonstack", L"who");
    map.emplace(L"gstacksize", L"");
    map.emplace(L"stacksize", L"");
    map.emplace(L"macr2lst", L"");
    map.emplace(L"readgateway", L"");

    // Scilab 5.5.1 => 5.5.2
    map.emplace(L"%asn", L"delip");
    map.emplace(L"chart", L"nicholschart");
    map.emplace(L"IsAScalar", L"isscalar");
    map.emplace(L"jmat", L"flipdim");
    map.emplace(L"mfft", L"ftt");
    map.emplace(L"milk_drop", L"");
    map.emplace(L"msd", L"stdev");
    map.emplace(L"nfreq", L"tabul");
    map.emplace(L"pcg", L"conjgrad");
    map.emplace(L"regress", L"reglin");
    map.emplace(L"relocate_handle", L"");
    map.emplace(L"st_deviation", L"stdev");
    map.emplace(L"xmltochm", L"");
    map.emplace(L"xsetm", L"");

    // Scilab 5.5.0 => 5.5.1
    map.emplace(L"datatipContextMenu", L"");
    map.emplace(L"datatipEventHandler", L"");

    // SCilab 5.4.1 => 5.5.0
    map.emplace(L"dft", L"fft");
    map.emplace(L"sscanf", L"msscanf");
    map.emplace(L"fscanf", L"mfscanf");
    map.emplace(L"printf", L"mprintf");
    map.emplace(L"fprintf", L"mfprintf");
    map.emplace(L"sprintf", L"msprintf");
    map.emplace(L"demo_message", L"");
    map.emplace(L"demo_mdialog", L"");
    map.emplace(L"draw", L"");
    map.emplace(L"clear_pixmap", L"");
    map.emplace(L"show_pixmap", L"");
    map.emplace(L"winclose", L"close");
    map.emplace(L"datatipInitStruct", L"");
    map.emplace(L"datatipRedraw", L"");
    map.emplace(L"getfont", L"");
    map.emplace(L"getmark", L"");
    map.emplace(L"getlinestyle", L"");
    map.emplace(L"getsymbol", L"");
    map.emplace(L"with_embedded_jre", L"");
    map.emplace(L"fit_dat", L"datafit");
    map.emplace(L"create_palette", L"");

    // Scilab 5.4.0 => 5.4.1
    map.emplace(L"chartoeom", L"");
    map.emplace(L"eomtochar", L"");
    map.emplace(L"config", L"preferences");
    map.emplace(L"createpopup", L"uicontextmenu");
    map.emplace(L"mtlb_conv", L"conv");
    map.emplace(L"mtlb_repmat", L"repmat");
    map.emplace(L"neldermead_display", L"disp");
    map.emplace(L"nmplot_display", L"disp");
    map.emplace(L"optimbase_display", L"disp");
    map.emplace(L"optimsimplex_print", L"disp");
    map.emplace(L"iptim_simplex_tostring", L"string");
    map.emplace(L"ricc_old", L"ricc");
    map.emplace(L"showalluimenushandles", L"set(get(0), \"ShowHiddenHandles\", \"on\")");
    map.emplace(L"with_pvm", L"getversion");
    map.emplace(L"with_texmacs", L"");
    map.emplace(L"xbasr", L"");
    map.emplace(L"xselect", L"show_window");

    // Scilab 5.3.3 => 5.4.0
    map.emplace(L"MSDOS", L"getos");
    map.emplace(L"sd2sci", L"");
    map.emplace(L"oldplot", L"");

    // Scilab 5.3.0 => 5.3.3: nothing removed

    // Scilab 5.2.X => 5.3.0
    map.emplace(L"maxi", L"max");
    map.emplace(L"mini", L"min");
    map.emplace(L"oldbesseli", L"besseli");
    map.emplace(L"oldbesselj", L"besselj");
    map.emplace(L"oldbesselk", L"besselk");
    map.emplace(L"oldbessely", L"bessely");
    map.emplace(L"textprint", L"prettyprint");
    map.emplace(L"pol2tex", L"prettyprint");
    map.emplace(L"xgetfile", L"uigetfile");
    map.emplace(L"tk_getfile", L"uigetfile");
    map.emplace(L"tk_savefile", L"uiputfile");
    map.emplace(L"tk_getdir", L"uigetdir");
    map.emplace(L"tk_choose", L"x_choose");
    map.emplace(L"sci2excel", L"csvWrite");
    map.emplace(L"excel2sci", L"csvRead");
    map.emplace(L"x_message_modeless", L"messagebox");
    map.emplace(L"sethomedirectory", L"SCIHOME,home");
    map.emplace(L"getcwd", L"pwd");
    map.emplace(L"xbasc", L"clf");
    map.emplace(L"getf", L"exec");
    map.emplace(L"NumTokens", L"tokens");
    map.emplace(L"sort", L"gsort");
    map.emplace(L"scilab_demos", L"demo_gui");
    map.emplace(L"with_gtk", L"getversion");
    map.emplace(L"readc_", L"input");

    // Scilab 5.2.1 => 5.2.2
    map.emplace(L"oldsave", L"save");
    map.emplace(L"oldload", L"load");

    // Scilab 5.2.0 => 5.2.1: nothing removed

    // Scilab 5.1.1 => 5.2.0
    map.emplace(L"lgfft", L"");

    // Scilab 5.1.0 => 5.1.1: nothing removed

    // Scilab 5.0.X => 5.1.0
    map.emplace(L"mtlb_load", L"loadmatfile");
    map.emplace(L"mtlb_save", L"savematfile");
    map.emplace(L"xbasimp", L"toprint,xs2ps");
    map.emplace(L"xg2ps", L"xs2ps");
    map.emplace(L"hidetoolbar", L"toolbar(,\'off\')");
    map.emplace(L"browsehelp", L"helpbrowser");
    map.emplace(L"quapro", L"qpsolve");
    map.emplace(L"%sp_eye", L"speye");
    map.emplace(L"TCL_gcf", L"gcf");
    map.emplace(L"TCL_scf", L"scf");
    map.emplace(L"TK_EvalStr", L"TCL_EvalStr");
    map.emplace(L"TK_GetVar", L"TCL_GetVar");
    map.emplace(L"TK_SetVar", L"TCL_SetVar");
    map.emplace(L"sciGUIhelp", L"help");
    map.emplace(L"demoplay", L"demo_gui");
    map.emplace(L"buttondialog", L"messagebox");
    map.emplace(L"tk_getvalue", L"getvalue");

    // Scilab 5.0.1 => 5.0.3: nothing removed

    // Scilab 4.1.2 => 5.0
    map.emplace(L"xclea", L"xfrect");
    map.emplace(L"xaxis", L"drawaxis");
    map.emplace(L"loadplots", L"");
    map.emplace(L"xtape", L"");
    map.emplace(L"loaddefaultbrowser", L"");
    map.emplace(L"%browsehelp", L"");

    return map;
}

std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> DeprecatedChecker::initPartDep()
{
    std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> map;
    map.emplace(L"svd", std::shared_ptr<SLintChecker>(new __Svd()));
    map.emplace(L"mfprintf", std::shared_ptr<SLintChecker>(new __Mfprintf()));

    return map;
}
}
