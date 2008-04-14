// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2802 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2802
//
// <-- Short Description -->
//    convstr() doesn't work if the string contains accented characters.
// The returned string is cut before the first accented character.

ref  = 'Bug convstr : Génération de l'+ascii(39)+'aide';

refl = 'bug convstr : génération de l'+ascii(39)+'aide';
refu = 'BUG CONVSTR : GéNéRATION DE L'+ascii(39)+'AIDE';

resl = convstr(ref,'l');
if refl <> resl then pause,end

resu = convstr(ref,'u')
refu
if refu <> resu then pause,end

if convstr('é','u') <> 'é' then pause,end