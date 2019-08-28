// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function helpbrowser_update(startup)
    // Internal called by add_help_chapter() and atomsLoad()

  if or(getscilabmode()==["STD" "NW"])
      jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser
      hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
      global %helps
      if ~isempty(hb)
          if isdef("startup", "l")
              // for scilab.start
              id = xmlGetValues("//Setting/Profile/HelpBrowser","index",SCIHOME+"/configuration.xml")
              // No way to retrieve as well the helpbrowser language, if it's
              // different from the session's one.
              help(id)
          else
              // for atomsLoad and add_help_chapter
              id = hb.getCurrentID()
              // Get the current language of the browser
              jar = hb.getCurrentURL()
              [?,?,?,lang] = regexp(jar, "/scilab_([a-z]{2}_[A-Z]{2})_help\.jar/", "o")
              if lang=="en_US" // it may be a default external page in en_US,
                               // despite the browser is in another language
                  help("ones") // We are sure this page is translated
                  jar = hb.getCurrentURL();
                  [?,?,?,lang] = regexp(jar, "/scilab_([a-z]{2}_[A-Z]{2})_help\.jar/", "o")
              end
              // Update the browser
              helpbrowser(%helps(:,1), id, lang, %f)
          end
      end
      jremove hb ScilabHelpBrowser
  end

endfunction
