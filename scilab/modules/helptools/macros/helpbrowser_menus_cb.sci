// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function helpbrowser_menus_cb(action, param)
    // PRIVATE function used by the help browser

    jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser
    hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
    url = hb.getCurrentURL()
    [?,?,?, currentLang] = regexp(url, "/scilab_(.+?)_help.jar/")
    id = hb.getCurrentID()
    isSection = grep(id, "/^section_/", "r") <> []
    tmp = msprintf("#^jar:file:/%s/modules/helptools/jar#", SCI)
    isExternal = grep(url, tmp, "r") == []

    select action
    case "changeLang"
        if currentLang == param then
            return
        else
            hb.close()
            global %helps
            helpbrowser(%helps(:,1), id, param, %f);
        end
    case "online"
        if isExternal
            msg = _("Help browser: Pages of external modules can''t be viewed online")
            messagebox(msg, "modal")
            return
        end
        if isSection
            msg = _("Help browser: Tables of contents can''t be targeted online")
            messagebox(msg, "modal")
            return
        end
        v = getversion()
        if grep(v, "-branch-") <> []
            v = "current"
        else
            v = getversion("scilab")
            v = msprintf("%d.%d.%d", v(1),v(2),v(3))
        end
        url = "https://help.scilab.org/docs/%s/%s/%s.html"
        url = msprintf(url, v, currentLang, id)
        openURL(url)

    case "bugs"
        if isExternal
            msg = _("Help browser: Bugs of external modules can''t be listed.")
            messagebox(msg, "modal")
            return
        end
        if isSection
            msg = _("Help browser: Table of contents can''t be targeted in Bugzilla")
            messagebox(msg, "modal")
            return
        end
        BugzillaSearch = "http://bugzilla.scilab.org/buglist.cgi" + ..
          "?product=Scilab software&query_format=advanced" + ..
          "&short_desc_type=regexp&short_desc=£"+..
          "&bug_status=NEW&bug_status=ASSIGNED&bug_status=REOPENED" + ..
          "&order=resolution,bug_id DESC"
        if ~isdef("param","l") | type(param)<>10 | param(1)<>"unresolved"
            BugzillaSearch = BugzillaSearch + "&bug_status=RESOLVED"
        end
        prepend = "(^|[^a-zA-Z_])"
        append = "([^0-9a-zA-Z_%3B]|$)"
        url = strsubst(BugzillaSearch, "£", prepend+"("+id+")"+append)
        openURL(url)
    end
endfunction

// ---------------------------------------------------------------------------

function openURL(url)
    v = xmlGetValues("//web/body/web", ["default-browser" "command-browser"])
    [default, cmd] = (v(1)=="true", v(2))

    select getos()
    case "Windows"
        if default
            winopen(url)
        else
            unix(msprintf("start """" %s ""%s""", v(2), url))
        end

    case "Linux"
        if default, cmd = "xdg-open ", end
        unix(cmd + """" + url + """")

    case "Darwin"
        if default, cmd = "open ", end
        unix(cmd + """" + url + """")
    end
endfunction

