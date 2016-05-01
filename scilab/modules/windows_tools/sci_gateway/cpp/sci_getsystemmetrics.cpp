/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
/*--------------------------------------------------------------------------*/
#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "registry.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
static int getnIndexFromString(const std::wstring& cmd);
/*--------------------------------------------------------------------------*/
const std::string fname = "getsystemmetrics";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getsystemmetrics(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pS = nullptr;
    std::wstring param1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    pS = in[0]->getAs<types::String>();
    param1 = pS->get()[0];

    int val = getnIndexFromString(param1);
    if (val != -1)
    {
        out.push_back(new types::Double(GetSystemMetrics(val)));
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument: %s.\n"), fname.data(), _("see help"));
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
static int getnIndexFromString(const std::wstring& cmd)
{
    if (L"SM_ARRANGE" == cmd)
    {
        return SM_ARRANGE;
    }
    if (L"SM_CLEANBOOT" == cmd)
    {
        return SM_CLEANBOOT;
    }
    if (L"SM_CMONITORS" == cmd)
    {
        return SM_CMONITORS;
    }
    if (L"SM_CMOUSEBUTTONS" == cmd)
    {
        return SM_CMOUSEBUTTONS;
    }
    if (L"SM_CXBORDER" == cmd)
    {
        return SM_CXBORDER;
    }
    if (L"SM_CXCURSOR" == cmd)
    {
        return SM_CXCURSOR;
    }
    if (L"SM_CXDLGFRAME" == cmd)
    {
        return SM_CXDLGFRAME;
    }
    if (L"SM_CXDOUBLECLK" == cmd)
    {
        return SM_CXDOUBLECLK;
    }
    if (L"SM_CXDRAG" == cmd)
    {
        return SM_CXDRAG;
    }
    if (L"SM_CXEDGE" == cmd)
    {
        return SM_CXEDGE;
    }
    if (L"SM_CXFIXEDFRAME" == cmd)
    {
        return SM_CXFIXEDFRAME;
    }
    if (L"SM_CXFOCUSBORDER" == cmd)
    {
        return SM_CXFOCUSBORDER;
    }
    if (L"SM_CXFRAME" == cmd)
    {
        return SM_CXFRAME;
    }
    if (L"SM_CXFULLSCREEN" == cmd)
    {
        return SM_CXFULLSCREEN;
    }
    if (L"SM_CXHSCROLL" == cmd)
    {
        return SM_CXHSCROLL;
    }
    if (L"SM_CXHTHUMB" == cmd)
    {
        return SM_CXHTHUMB;
    }
    if (L"SM_CXICON" == cmd)
    {
        return SM_CXICON;
    }
    if (L"SM_CXICONSPACING" == cmd)
    {
        return SM_CXICONSPACING;
    }
    if (L"SM_CXMAXIMIZED" == cmd)
    {
        return SM_CXMAXIMIZED;
    }
    if (L"SM_CXMAXTRACK" == cmd)
    {
        return SM_CXMAXTRACK;
    }
    if (L"SM_CXMENUCHECK" == cmd)
    {
        return SM_CXMENUCHECK;
    }
    if (L"SM_CXMENUSIZE" == cmd)
    {
        return SM_CXMENUSIZE;
    }
    if (L"SM_CXMIN" == cmd)
    {
        return SM_CXMIN;
    }
    if (L"SM_CXMINIMIZED" == cmd)
    {
        return SM_CXMINIMIZED;
    }
    if (L"SM_CXMINSPACING" == cmd)
    {
        return SM_CXMINSPACING;
    }
    if (L"SM_CXMINTRACK" == cmd)
    {
        return SM_CXMINTRACK;
    }
    if (L"SM_CXPADDEDBORDER" == cmd)
    {
        return SM_CXPADDEDBORDER;
    }
    if (L"SM_CXSCREEN" == cmd)
    {
        return SM_CXSCREEN;
    }
    if (L"SM_CXSIZE" == cmd)
    {
        return SM_CXSIZE;
    }
    if (L"SM_CXSIZEFRAME" == cmd)
    {
        return SM_CXSIZEFRAME;
    }
    if (L"SM_CXSMICON" == cmd)
    {
        return SM_CXSMICON;
    }
    if (L"SM_CXSMSIZE" == cmd)
    {
        return SM_CXSMSIZE;
    }
    if (L"SM_CXVIRTUALSCREEN" == cmd)
    {
        return SM_CXVIRTUALSCREEN;
    }
    if (L"SM_CXVSCROLL" == cmd)
    {
        return SM_CXVSCROLL;
    }
    if (L"SM_CYBORDER" == cmd)
    {
        return SM_CYBORDER;
    }
    if (L"SM_CYCAPTION" == cmd)
    {
        return SM_CYCAPTION;
    }
    if (L"SM_CYCURSOR" == cmd)
    {
        return SM_CYCURSOR;
    }
    if (L"SM_CYDLGFRAME" == cmd)
    {
        return SM_CYDLGFRAME;
    }
    if (L"SM_CYDOUBLECLK" == cmd)
    {
        return SM_CYDOUBLECLK;
    }
    if (L"SM_CYDRAG" == cmd)
    {
        return SM_CYDRAG;
    }
    if (L"SM_CYEDGE" == cmd)
    {
        return SM_CYEDGE;
    }
    if (L"SM_CYFIXEDFRAME" == cmd)
    {
        return SM_CYFIXEDFRAME;
    }
    if (L"SM_CYFOCUSBORDER" == cmd)
    {
        return SM_CYFOCUSBORDER;
    }
    if (L"SM_CYFRAME" == cmd)
    {
        return SM_CYFRAME;
    }
    if (L"SM_CYFULLSCREEN" == cmd)
    {
        return SM_CYFULLSCREEN;
    }
    if (L"SM_CYHSCROLL" == cmd)
    {
        return SM_CYHSCROLL;
    }
    if (L"SM_CYICON" == cmd)
    {
        return SM_CYICON;
    }
    if (L"SM_CYICONSPACING" == cmd)
    {
        return SM_CYICONSPACING;
    }
    if (L"SM_CYKANJIWINDOW" == cmd)
    {
        return SM_CYKANJIWINDOW;
    }
    if (L"SM_CYMAXIMIZED" == cmd)
    {
        return SM_CYMAXIMIZED;
    }
    if (L"SM_CYMAXTRACK" == cmd)
    {
        return SM_CYMAXTRACK;
    }
    if (L"SM_CYMENU" == cmd)
    {
        return SM_CYMENU;
    }
    if (L"SM_CYMENUCHECK" == cmd)
    {
        return SM_CYMENUCHECK;
    }
    if (L"SM_CYMENUSIZE" == cmd)
    {
        return SM_CYMENUSIZE;
    }
    if (L"SM_CYMIN" == cmd)
    {
        return SM_CYMIN;
    }
    if (L"SM_CYMINIMIZED" == cmd)
    {
        return SM_CYMINIMIZED;
    }
    if (L"SM_CYMINSPACING" == cmd)
    {
        return SM_CYMINSPACING;
    }
    if (L"SM_CYMINTRACK" == cmd)
    {
        return SM_CYMINTRACK;
    }
    if (L"SM_CYSCREEN" == cmd)
    {
        return SM_CYSCREEN;
    }
    if (L"SM_CYSIZE" == cmd)
    {
        return SM_CYSIZE;
    }
    if (L"SM_CYSIZEFRAME" == cmd)
    {
        return SM_CYSIZEFRAME;
    }
    if (L"SM_CYSMCAPTION" == cmd)
    {
        return SM_CYSMCAPTION;
    }
    if (L"SM_CYSMICON" == cmd)
    {
        return SM_CYSMICON;
    }
    if (L"SM_CYSMSIZE" == cmd)
    {
        return SM_CYSMSIZE;
    }
    if (L"SM_CYVIRTUALSCREEN" == cmd)
    {
        return SM_CYVIRTUALSCREEN;
    }
    if (L"SM_CYVSCROLL" == cmd)
    {
        return SM_CYVSCROLL;
    }
    if (L"SM_CYVTHUMB" == cmd)
    {
        return SM_CYVTHUMB;
    }
    if (L"SM_DBCSENABLED" == cmd)
    {
        return SM_DBCSENABLED;
    }
    if (L"SM_DEBUG" == cmd)
    {
        return SM_DEBUG;
    }
    if (L"SM_IMMENABLED" == cmd)
    {
        return SM_IMMENABLED;
    }
    if (L"SM_MEDIACENTER" == cmd)
    {
        return SM_MEDIACENTER;
    }
    if (L"SM_MENUDROPALIGNMENT" == cmd)
    {
        return SM_MENUDROPALIGNMENT;
    }
    if (L"SM_MIDEASTENABLED" == cmd)
    {
        return SM_MIDEASTENABLED;
    }
    if (L"SM_MOUSEPRESENT" == cmd)
    {
        return SM_MOUSEPRESENT;
    }
    if (L"SM_MOUSEHORIZONTALWHEELPRESENT" == cmd)
    {
        return SM_MOUSEHORIZONTALWHEELPRESENT;
    }
    if (L"SM_MOUSEWHEELPRESENT" == cmd)
    {
        return SM_MOUSEWHEELPRESENT;
    }
    if (L"SM_NETWORK" == cmd)
    {
        return SM_NETWORK;
    }
    if (L"SM_PENWINDOWS" == cmd)
    {
        return SM_PENWINDOWS;
    }
    if (L"SM_REMOTECONTROL" == cmd)
    {
        return SM_REMOTECONTROL;
    }
    if (L"SM_REMOTESESSION" == cmd)
    {
        return SM_REMOTESESSION;
    }
    if (L"SM_SAMEDISPLAYFORMAT" == cmd)
    {
        return SM_SAMEDISPLAYFORMAT;
    }
    if (L"SM_SECURE" == cmd)
    {
        return SM_SECURE;
    }
    if (L"SM_SERVERR2" == cmd)
    {
        return SM_SERVERR2;
    }
    if (L"SM_SHOWSOUNDS" == cmd)
    {
        return SM_SHOWSOUNDS;
    }
    if (L"SM_SHUTTINGDOWN" == cmd)
    {
        return SM_SHUTTINGDOWN;
    }
    if (L"SM_SLOWMACHINE" == cmd)
    {
        return SM_SLOWMACHINE;
    }
    if (L"SM_STARTER" == cmd)
    {
        return SM_STARTER;
    }
    if (L"SM_SWAPBUTTON" == cmd)
    {
        return SM_SWAPBUTTON;
    }
    if (L"SM_TABLETPC" == cmd)
    {
        return SM_TABLETPC;
    }
    if (L"SM_XVIRTUALSCREEN" == cmd)
    {
        return SM_XVIRTUALSCREEN;
    }
    if (L"SM_YVIRTUALSCREEN" == cmd)
    {
        return SM_YVIRTUALSCREEN;
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
