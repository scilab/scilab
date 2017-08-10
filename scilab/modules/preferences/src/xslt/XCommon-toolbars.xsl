<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:variable name="core-toolbars">
		<toolbar title="scilab">
			<item title="Cut"                    icon="edit-cut.png"                 callScilabBridge='cutConsoleSelection'/>
			<item title="Copy"                   icon="edit-copy.png"                callScilabBridge='copyConsoleSelection'/>
			<item title="Paste"                  icon="edit-paste.png"               callScilabBridge='pasteClipboardIntoConsole'/>
			<item title="Choose Font..."         icon="format-text-bold.png"         callScilabBridge='changeConsoleFont'/>
			<item title="Print..."               icon="printer.png"                  callScilabBridge='printConsoleContents'/>
			<item title="Launch SciNotes"        icon="accessories-text-editor.png"  script='editor();' />
			<item title="Help Browser"           icon="help-browser.png"         script='help();' />
			<item title="Scilab Demonstrations"  icon="applications-system.png"      script='demo_gui();' />
			<item title="Open a file"            icon="document-open.png"            script='
		          %fileToOpen = uigetfile("*.sc*", pwd(), gettext("Select a file to open with SciNotes"));
		          if ~(%fileToOpen == "") then
		            editor(%fileToOpen);
		          end;
		          clear("%fileToOpen");'/>
			<item title="Change folder"          icon="folder.png"                   script='
		          %newDir = uigetdir(pwd(), gettext("Select a directory"));
		          if ~(%newDir == "") then 
		            cd(%newDir);
		          end;
		          clear("%newDir");'/>
			<item title="Module manager - ATOMS" icon="package-x-generic.png"        script='
		          if with_module("atoms") then 
		            atomsGui(); 
		          else 
		            disp(gettext("Please install atoms module.")); 
		          end'/>
			<item title="Xcos"                   icon="utilities-system-monitor.png" script='
		          if with_module("xcos") then 
		            xcos(); 
		          else 
		            disp(gettext("Please install xcos module.")); 
		          end'/>
		</toolbar>
		<toolbar title="scinotes">
			<item title="Cut"                    icon="edit-cut.png"                 callScilabBridge='cutConsoleSelection'/>
			<item title="Copy"                   icon="edit-copy.png"                callScilabBridge='copyConsoleSelection'/>
			<item title="Paste"                  icon="edit-paste.png"               callScilabBridge='pasteClipboardIntoConsole'/>
		</toolbar>
	</xsl:variable>
	<xsl:variable name="toolbars" select="nodeset($core-toolbars)"/>
</xsl:stylesheet>
