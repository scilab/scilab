/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
static int getnIndexFromString(const std::string& cmd);
/*--------------------------------------------------------------------------*/
const std::string fname = "getsystemmetrics";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getsystemmetrics(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pS = nullptr;
    std::string param1;

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
static int getnIndexFromString(const std::string& cmd)
{
    if ("SM_ARRANGE" == cmd)
    {
        return SM_ARRANGE;
    }
    if ("SM_CLEANBOOT" == cmd)
    {
        return SM_CLEANBOOT;
    }
    if ("SM_CMONITORS" == cmd)
    {
        return SM_CMONITORS;
    }
    if ("SM_CMOUSEBUTTONS" == cmd)
    {
        return SM_CMOUSEBUTTONS;
    }
    if ("SM_CXBORDER" == cmd)
    {
        return SM_CXBORDER;
    }
    if ("SM_CXCURSOR" == cmd)
    {
        return SM_CXCURSOR;
    }
    if ("SM_CXDLGFRAME" == cmd)
    {
        return SM_CXDLGFRAME;
    }
    if ("SM_CXDOUBLECLK" == cmd)
    {
        return SM_CXDOUBLECLK;
    }
    if ("SM_CXDRAG" == cmd)
    {
        return SM_CXDRAG;
    }
    if ("SM_CXEDGE" == cmd)
    {
        return SM_CXEDGE;
    }
    if ("SM_CXFIXEDFRAME" == cmd)
    {
        return SM_CXFIXEDFRAME;
    }
    if ("SM_CXFOCUSBORDER" == cmd)
    {
        return SM_CXFOCUSBORDER;
    }
    if ("SM_CXFRAME" == cmd)
    {
        return SM_CXFRAME;
    }
    if ("SM_CXFULLSCREEN" == cmd)
    {
        return SM_CXFULLSCREEN;
    }
    if ("SM_CXHSCROLL" == cmd)
    {
        return SM_CXHSCROLL;
    }
    if ("SM_CXHTHUMB" == cmd)
    {
        return SM_CXHTHUMB;
    }
    if ("SM_CXICON" == cmd)
    {
        return SM_CXICON;
    }
    if ("SM_CXICONSPACING" == cmd)
    {
        return SM_CXICONSPACING;
    }
    if ("SM_CXMAXIMIZED" == cmd)
    {
        return SM_CXMAXIMIZED;
    }
    if ("SM_CXMAXTRACK" == cmd)
    {
        return SM_CXMAXTRACK;
    }
    if ("SM_CXMENUCHECK" == cmd)
    {
        return SM_CXMENUCHECK;
    }
    if ("SM_CXMENUSIZE" == cmd)
    {
        return SM_CXMENUSIZE;
    }
    if ("SM_CXMIN" == cmd)
    {
        return SM_CXMIN;
    }
    if ("SM_CXMINIMIZED" == cmd)
    {
        return SM_CXMINIMIZED;
    }
    if ("SM_CXMINSPACING" == cmd)
    {
        return SM_CXMINSPACING;
    }
    if ("SM_CXMINTRACK" == cmd)
    {
        return SM_CXMINTRACK;
    }
    if ("SM_CXPADDEDBORDER" == cmd)
    {
        return SM_CXPADDEDBORDER;
    }
    if ("SM_CXSCREEN" == cmd)
    {
        return SM_CXSCREEN;
    }
    if ("SM_CXSIZE" == cmd)
    {
        return SM_CXSIZE;
    }
    if ("SM_CXSIZEFRAME" == cmd)
    {
        return SM_CXSIZEFRAME;
    }
    if ("SM_CXSMICON" == cmd)
    {
        return SM_CXSMICON;
    }
    if ("SM_CXSMSIZE" == cmd)
    {
        return SM_CXSMSIZE;
    }
    if ("SM_CXVIRTUALSCREEN" == cmd)
    {
        return SM_CXVIRTUALSCREEN;
    }
    if ("SM_CXVSCROLL" == cmd)
    {
        return SM_CXVSCROLL;
    }
    if ("SM_CYBORDER" == cmd)
    {
        return SM_CYBORDER;
    }
    if ("SM_CYCAPTION" == cmd)
    {
        return SM_CYCAPTION;
    }
    if ("SM_CYCURSOR" == cmd)
    {
        return SM_CYCURSOR;
    }
    if ("SM_CYDLGFRAME" == cmd)
    {
        return SM_CYDLGFRAME;
    }
    if ("SM_CYDOUBLECLK" == cmd)
    {
        return SM_CYDOUBLECLK;
    }
    if ("SM_CYDRAG" == cmd)
    {
        return SM_CYDRAG;
    }
    if ("SM_CYEDGE" == cmd)
    {
        return SM_CYEDGE;
    }
    if ("SM_CYFIXEDFRAME" == cmd)
    {
        return SM_CYFIXEDFRAME;
    }
    if ("SM_CYFOCUSBORDER" == cmd)
    {
        return SM_CYFOCUSBORDER;
    }
    if ("SM_CYFRAME" == cmd)
    {
        return SM_CYFRAME;
    }
    if ("SM_CYFULLSCREEN" == cmd)
    {
        return SM_CYFULLSCREEN;
    }
    if ("SM_CYHSCROLL" == cmd)
    {
        return SM_CYHSCROLL;
    }
    if ("SM_CYICON" == cmd)
    {
        return SM_CYICON;
    }
    if ("SM_CYICONSPACING" == cmd)
    {
        return SM_CYICONSPACING;
    }
    if ("SM_CYKANJIWINDOW" == cmd)
    {
        return SM_CYKANJIWINDOW;
    }
    if ("SM_CYMAXIMIZED" == cmd)
    {
        return SM_CYMAXIMIZED;
    }
    if ("SM_CYMAXTRACK" == cmd)
    {
        return SM_CYMAXTRACK;
    }
    if ("SM_CYMENU" == cmd)
    {
        return SM_CYMENU;
    }
    if ("SM_CYMENUCHECK" == cmd)
    {
        return SM_CYMENUCHECK;
    }
    if ("SM_CYMENUSIZE" == cmd)
    {
        return SM_CYMENUSIZE;
    }
    if ("SM_CYMIN" == cmd)
    {
        return SM_CYMIN;
    }
    if ("SM_CYMINIMIZED" == cmd)
    {
        return SM_CYMINIMIZED;
    }
    if ("SM_CYMINSPACING" == cmd)
    {
        return SM_CYMINSPACING;
    }
    if ("SM_CYMINTRACK" == cmd)
    {
        return SM_CYMINTRACK;
    }
    if ("SM_CYSCREEN" == cmd)
    {
        return SM_CYSCREEN;
    }
    if ("SM_CYSIZE" == cmd)
    {
        return SM_CYSIZE;
    }
    if ("SM_CYSIZEFRAME" == cmd)
    {
        return SM_CYSIZEFRAME;
    }
    if ("SM_CYSMCAPTION" == cmd)
    {
        return SM_CYSMCAPTION;
    }
    if ("SM_CYSMICON" == cmd)
    {
        return SM_CYSMICON;
    }
    if ("SM_CYSMSIZE" == cmd)
    {
        return SM_CYSMSIZE;
    }
    if ("SM_CYVIRTUALSCREEN" == cmd)
    {
        return SM_CYVIRTUALSCREEN;
    }
    if ("SM_CYVSCROLL" == cmd)
    {
        return SM_CYVSCROLL;
    }
    if ("SM_CYVTHUMB" == cmd)
    {
        return SM_CYVTHUMB;
    }
    if ("SM_DBCSENABLED" == cmd)
    {
        return SM_DBCSENABLED;
    }
    if ("SM_DEBUG" == cmd)
    {
        return SM_DEBUG;
    }
    if ("SM_IMMENABLED" == cmd)
    {
        return SM_IMMENABLED;
    }
    if ("SM_MEDIACENTER" == cmd)
    {
        return SM_MEDIACENTER;
    }
    if ("SM_MENUDROPALIGNMENT" == cmd)
    {
        return SM_MENUDROPALIGNMENT;
    }
    if ("SM_MIDEASTENABLED" == cmd)
    {
        return SM_MIDEASTENABLED;
    }
    if ("SM_MOUSEPRESENT" == cmd)
    {
        return SM_MOUSEPRESENT;
    }
    if ("SM_MOUSEHORIZONTALWHEELPRESENT" == cmd)
    {
        return SM_MOUSEHORIZONTALWHEELPRESENT;
    }
    if ("SM_MOUSEWHEELPRESENT" == cmd)
    {
        return SM_MOUSEWHEELPRESENT;
    }
    if ("SM_NETWORK" == cmd)
    {
        return SM_NETWORK;
    }
    if ("SM_PENWINDOWS" == cmd)
    {
        return SM_PENWINDOWS;
    }
    if ("SM_REMOTECONTROL" == cmd)
    {
        return SM_REMOTECONTROL;
    }
    if ("SM_REMOTESESSION" == cmd)
    {
        return SM_REMOTESESSION;
    }
    if ("SM_SAMEDISPLAYFORMAT" == cmd)
    {
        return SM_SAMEDISPLAYFORMAT;
    }
    if ("SM_SECURE" == cmd)
    {
        return SM_SECURE;
    }
    if ("SM_SERVERR2" == cmd)
    {
        return SM_SERVERR2;
    }
    if ("SM_SHOWSOUNDS" == cmd)
    {
        return SM_SHOWSOUNDS;
    }
    if ("SM_SHUTTINGDOWN" == cmd)
    {
        return SM_SHUTTINGDOWN;
    }
    if ("SM_SLOWMACHINE" == cmd)
    {
        return SM_SLOWMACHINE;
    }
    if ("SM_STARTER" == cmd)
    {
        return SM_STARTER;
    }
    if ("SM_SWAPBUTTON" == cmd)
    {
        return SM_SWAPBUTTON;
    }
    if ("SM_TABLETPC" == cmd)
    {
        return SM_TABLETPC;
    }
    if ("SM_XVIRTUALSCREEN" == cmd)
    {
        return SM_XVIRTUALSCREEN;
    }
    if ("SM_YVIRTUALSCREEN" == cmd)
    {
        return SM_YVIRTUALSCREEN;
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
