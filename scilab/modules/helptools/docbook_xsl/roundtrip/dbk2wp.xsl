<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:d="http://docbook.org/ns/docbook"
xmlns:doc='http://docbook.org/ns/docbook'
  exclude-result-prefixes='doc'>

  <!-- ********************************************************************
       $Id: dbk2wp.xsl 6910 2007-06-28 23:23:30Z xmldoc $
       ********************************************************************

       This file is part of the XSL DocBook Stylesheet distribution.
       See ../README or http://docbook.sf.net/release/xsl/current/ for
       copyright and other information.

       ******************************************************************** -->

  <!-- DO NOT USE THIS STYLESHEET!

       This stylesheet is imported by the other dbk2* stylesheets.
       Use one of those instead.

    -->

  <xsl:include href='../VERSION'/>

  <!-- doc:docprop.author mode is for creating document metadata -->

  <xsl:template match='d:author|d:editor' mode='doc:docprop.author'>
    <xsl:apply-templates select='d:firstname|d:personname/d:firstname' mode='doc:docprop.author'/>
    <xsl:text> </xsl:text>
    <xsl:apply-templates select='d:surname|d:personname/d:surname' mode='doc:docprop.author'/>
  </xsl:template>
  <xsl:template match='d:authorinitials' mode='doc:docprop.author'>
    <xsl:value-of select='.'/>
  </xsl:template>

  <!-- doc:toplevel mode is for processing whole documents -->

  <xsl:template match='*' mode='doc:toplevel'>
    <xsl:call-template name='doc:make-body'/>
  </xsl:template>

  <!-- doc:body mode is for processing components of a document -->

  <xsl:template match='d:book|d:article|d:chapter|d:section|d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:simplesect' mode='doc:body'>
    <xsl:call-template name='doc:make-subsection'/>
  </xsl:template>

  <xsl:template match='d:articleinfo |
		       d:chapterinfo |
		       d:bookinfo' mode='doc:body'>
    <xsl:apply-templates select='d:title|d:subtitle|d:titleabbrev' mode='doc:body'/>
    <xsl:apply-templates select='d:author|d:releaseinfo' mode='doc:body'/>
    <!-- current implementation ignores all other metadata -->
    <xsl:for-each select='*[not(self::d:title|self::d:subtitle|self::d:titleabbrev|self::d:author|self::d:releaseinfo)]'>
      <xsl:call-template name='doc:nomatch'/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match='d:title|d:subtitle|d:titleabbrev' mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style'>
	<xsl:choose>
          <xsl:when test='(parent::d:section or
                          parent::d:sectioninfo/parent::d:section) and
                          count(ancestor::d:section) > 5'>
            <xsl:call-template name='doc:warning'>
	      <xsl:with-param name='message'>section nested deeper than 5 levels</xsl:with-param>
	    </xsl:call-template>
            <xsl:text>sect5-</xsl:text>
            <xsl:value-of select='name()'/>
          </xsl:when>
          <xsl:when test='parent::d:section or
                          parent::d:sectioninfo/parent::d:section'>
            <xsl:text>sect</xsl:text>
            <xsl:value-of select='count(ancestor::d:section)'/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select='name()'/>
          </xsl:when>
          <xsl:when test='contains(name(..), "d:info")'>
            <xsl:value-of select='name(../..)'/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select='name()'/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select='name(..)'/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select='name()'/>
          </xsl:otherwise>
	</xsl:choose>
      </xsl:with-param>
      <xsl:with-param name='outline.level'
		      select='count(ancestor::*) - count(parent::*[contains(name(), "d:info")]) - 1'/>
      <xsl:with-param name='attributes.node'
		      select='../parent::*[contains(name(current()), "d:info")] |
			      parent::*[not(contains(name(current()), "d:info"))]'/>
      <xsl:with-param name='content'>
	<xsl:apply-templates mode='doc:body'/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <doc:template name='metadata' xmlns=''>
    <title>Metadata</title>

    <para>TODO: Handle all metadata elements, apart from titles.</para>
  </doc:template>
  <xsl:template match='*[contains(name(), "d:info")]/*[not(self::d:title|self::d:subtitle|self::d:titleabbrev)]'
		priority='0'
		mode='doc:body'/>

  <xsl:template match='d:author|d:editor|d:othercredit' mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style'
		      select='name()'/>
      <xsl:with-param name='content'>
	<xsl:apply-templates select='d:personname|d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib'
			     mode='doc:body'/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:apply-templates select='d:affiliation|d:address' mode='doc:body'/>
    <xsl:apply-templates select='d:authorblurb|d:personblurb' mode='doc:body'/>
  </xsl:template>
  <xsl:template match='d:affiliation' mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='"d:affiliation"'/>
      <xsl:with-param name='content'>
	<xsl:apply-templates mode='doc:body'/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match='d:address[parent::d:author|parent::d:editor|parent::d:othercredit]'
		mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='"para-continue"'/>
      <xsl:with-param name='content'>
	<xsl:apply-templates mode='doc:body'/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <!-- do not attempt to handle recursive structures -->
  <xsl:template match='d:address[not(parent::d:author|parent::d:editor|parent::d:othercredit)]'
    mode='doc:body'>
    <xsl:apply-templates select='node()[not(self::d:affiliation|self::d:authorblurb)]'/>
  </xsl:template>
  <!-- TODO -->
  <xsl:template match='d:authorblurb|d:personblurb'
    mode='doc:body'/>

  <!-- TODO: handle inline markup (eg. emphasis) -->
  <xsl:template match='d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib|d:email|d:shortaffil|d:jobtitle|d:orgname|d:orgdiv|d:street|d:pob|d:postcode|d:city|d:state|d:country|d:phone|d:fax|d:citetitle'
    mode='doc:body'>
    <xsl:if test='preceding-sibling::*'>
      <xsl:call-template name='doc:make-phrase'>
	<xsl:with-param name='content'>
          <xsl:text> </xsl:text>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    <xsl:call-template name='doc:handle-linebreaks'>
      <xsl:with-param name='style' select='name()'/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match='d:email'
    mode='doc:body'>
    <xsl:variable name='address'>
      <xsl:choose>
        <xsl:when test='starts-with(., "mailto:")'>
          <xsl:value-of select='.'/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>mailto:</xsl:text>
          <xsl:value-of select='.'/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:call-template name='doc:make-hyperlink'>
      <xsl:with-param name='target' select='$address'/>
      <xsl:with-param name='content'>
	<xsl:call-template name='doc:handle-linebreaks'>
	  <xsl:with-param name='style'>Hyperlink</xsl:with-param>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <!-- otheraddr often contains ulink -->
  <xsl:template match='d:otheraddr'
    mode='doc:body'>
    <xsl:choose>
      <xsl:when test='d:ulink'>
        <xsl:for-each select='d:ulink'>
          <xsl:variable name='prev' select='preceding-sibling::d:ulink[1]'/>
          <xsl:choose>
            <xsl:when test='$prev'>
              <xsl:for-each
                select='preceding-sibling::node()[generate-id(following-sibling::d:ulink[1]) = generate-id(current())]'>
		<xsl:call-template name='doc:handle-linebreaks'>
		  <xsl:with-param name='style'>otheraddr</xsl:with-param>
		</xsl:call-template>
              </xsl:for-each>
            </xsl:when>
            <xsl:when test='preceding-sibling::node()'>
	      <xsl:call-template name='doc:handle-linebreaks'>
		<xsl:with-param name='style'>otheraddr</xsl:with-param>
	      </xsl:call-template>
            </xsl:when>
          </xsl:choose>
          <xsl:apply-templates select='.'/>
        </xsl:for-each>
        <xsl:if test='d:ulink[last()]/following-sibling::node()'>
	  <xsl:call-template name='doc:handle-linebreaks'>
	    <xsl:with-param name='content'
	      select='d:ulink[last()]/following-sibling::node()'/>
	    <xsl:with-param name='style'>otheraddr</xsl:with-param>
	  </xsl:call-template>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
	<xsl:call-template name='doc:handle-linebreaks'>
	  <xsl:with-param name='style'>otheraddr</xsl:with-param>
	</xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:ulink'
    mode='doc:body'>
    <xsl:call-template name='doc:make-hyperlink'>
      <xsl:with-param name='target' select='@url'/>
      <xsl:with-param name='content'>
	<xsl:call-template name='doc:handle-linebreaks'>
	  <xsl:with-param name='style'>Hyperlink</xsl:with-param>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <!-- Cannot round-trip this element -->
  <xsl:template match='d:personname'
    mode='doc:body'>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match='d:releaseinfo'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='d:releaseinfo'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:para'
    mode='doc:body'>
    <xsl:param name='class'/>

    <xsl:variable name='block' select='d:blockquote|d:calloutlist|d:classsynopsis|d:funcsynopsis|d:figure|d:glosslist|d:graphic|d:informalfigure|d:informaltable|d:itemizedlist|d:literallayout|d:mediaobject|d:mediaobjectco|d:note|d:caution|d:warning|d:important|d:tip|d:orderedlist|d:programlisting|d:revhistory|d:segmentedlist|d:simplelist|d:table|d:variablelist'/>

    <xsl:choose>
      <xsl:when test='$block'>
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style'>
            <xsl:choose>
              <xsl:when test='$class != ""'>
                <xsl:value-of select='$class'/>
              </xsl:when>
              <xsl:otherwise>para</xsl:otherwise>
            </xsl:choose>
	  </xsl:with-param>
	  <xsl:with-param name='content'
			  select='$block[1]/preceding-sibling::node()'/>
        </xsl:call-template>

        <xsl:for-each select='$block'>
          <xsl:apply-templates select='.'/>

	  <xsl:call-template name='doc:make-paragraph'>
	    <xsl:with-param name='style'>
              <xsl:choose>
		<xsl:when test='$class != ""'>
                  <xsl:value-of select='$class'/>
		</xsl:when>
		<xsl:otherwise>Normal</xsl:otherwise>
              </xsl:choose>
	    </xsl:with-param>
	    <xsl:with-param name='content'
			    select='following-sibling::node()[generate-id(preceding-sibling::*[self::d:blockquote|self::d:calloutlist|self::d:figure|self::d:glosslist|self::d:graphic|self::d:informalfigure|self::d:informaltable|self::d:itemizedlist|self::d:literallayout|self::d:mediaobject|self::d:mediaobjectco|self::d:note|self::d:caution|self::d:warning|self::d:important|self::d:tip|self::d:orderedlist|self::d:programlisting|self::d:revhistory|self::d:segmentedlist|self::d:simplelist|self::d:table|self::d:variablelist][1]) = generate-id(current())]'/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style'>
            <xsl:choose>
	      <xsl:when test='$class != ""'>
                <xsl:value-of select='$class'/>
	      </xsl:when>
	      <xsl:otherwise>Normal</xsl:otherwise>
            </xsl:choose>
	  </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:simpara'
    mode='doc:body'>
    <xsl:param name='class'/>

    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style'>
        <xsl:choose>
	  <xsl:when test='$class != ""'>
            <xsl:value-of select='concat("sim-", $class)'/>
	  </xsl:when>
	  <xsl:otherwise>simpara</xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:emphasis'
    mode='doc:body'>
    <xsl:call-template name='doc:make-phrase'>
      <xsl:with-param name='italic'>
	<xsl:choose>
	  <xsl:when test='not(@role)'>1</xsl:when>
	  <xsl:otherwise>0</xsl:otherwise>
	</xsl:choose>
      </xsl:with-param>
      <xsl:with-param name='bold'>
	<xsl:choose>
	  <xsl:when test='@role = "bold" or @role = "d:strong"'>1</xsl:when>
	  <xsl:otherwise>0</xsl:otherwise>
	</xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:informalfigure'
    mode='doc:body'>
    <xsl:if test='d:mediaobject/d:imageobject/d:imagedata'>
      <xsl:call-template name='doc:make-paragraph'>
	<xsl:with-param name='style' select='"informalfigure-imagedata"'/>
	<xsl:with-param name='content'>
	  <xsl:call-template name='doc:make-phrase'>
	    <xsl:with-param name='content'>
	      <xsl:apply-templates select='d:mediaobject/d:imageobject/d:imagedata/@fileref'
				   mode='textonly'/>
	    </xsl:with-param>
	  </xsl:call-template>
	</xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    <xsl:for-each select='*[not(self::d:mediaobject)]'>
      <xsl:call-template name='doc:nomatch'/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match='d:mediaobject|d:mediaobjectco'
    mode='doc:body'>
    <xsl:apply-templates select='d:objectinfo/d:title'/>
    <xsl:apply-templates select='d:objectinfo/d:subtitle'/>
    <!-- TODO: indicate error for other children of objectinfo -->

    <xsl:apply-templates select='*[not(self::d:objectinfo)]'/>
  </xsl:template>
  <xsl:template match='d:imageobject|d:imageobjectco|d:audioobject|d:videoobject'
    mode='doc:body'>
    <xsl:apply-templates select='d:objectinfo/d:title'/>
    <xsl:apply-templates select='d:objectinfo/d:subtitle'/>
    <!-- TODO: indicate error for other children of objectinfo -->

    <xsl:apply-templates select='d:areaspec'/>

    <xsl:choose>
      <xsl:when test='d:imagedata|d:audiodata|d:videodata'>
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style'
			  select='concat(name(), "-", name(d:imagedata|d:audiodata|d:videodata))'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-phrase'>
	      <xsl:with-param name='content'>
		<xsl:apply-templates select='*/@fileref'
				     mode='textonly'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:when>
      <xsl:when test='self::d:imageobjectco/d:imageobject/d:imagedata'>
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style'
			  select='concat(name(), "-imagedata")'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-phrase'>
	      <xsl:with-param name='content'>
		<xsl:apply-templates select='*/@fileref'
				     mode='textonly'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:when>
    </xsl:choose>
    <xsl:apply-templates select='d:calloutlist'/>

    <xsl:for-each select='*[not(self::d:imageobject |
			        self::d:imagedata |
			        self::d:audiodata |
				self::d:videodata |
				self::d:areaspec  |
				self::d:calloutlist)]'>
      <xsl:call-template name='doc:nomatch'/>
    </xsl:for-each>
  </xsl:template>
  <xsl:template match='d:textobject'
    mode='doc:body'>
    <xsl:choose>
      <xsl:when test='d:objectinfo/d:title|d:objectinfo|d:subtitle'>
	<xsl:apply-templates select='d:objectinfo/d:title'/>
	<xsl:apply-templates select='d:objectinfo/d:subtitle'/>
	<!-- TODO: indicate error for other children of objectinfo -->
      </xsl:when>
      <xsl:otherwise>
	<!-- synthesize a title so that the parent textobject
	     can be recreated.
	  -->
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style' select='"textobject-title"'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-phrase'>
	      <xsl:with-param name='content'>
		<xsl:text>Text Object </xsl:text>
		<xsl:number level='any'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:apply-templates select='*[not(self::d:objectinfo)]'/>
  </xsl:template>

  <xsl:template match='d:caption'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='"d:caption"'/>
      <xsl:with-param name='content'>
	<xsl:choose>
	  <xsl:when test='not(*)'>
	    <xsl:apply-templates/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:apply-templates select='d:para[1]/node()'/>
	    <xsl:for-each select='text()|*[not(self::d:para)]|d:para[position() != 1]'>
	      <xsl:call-template name='doc:nomatch'/>
	    </xsl:for-each>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:area|d:areaspec'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='name()'/>
      <xsl:with-param name='content'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:calloutlist'
    mode='doc:body'>
    <xsl:apply-templates select='d:callout'/>
  </xsl:template>

  <xsl:template match='d:callout'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='"d:callout"'/>
      <xsl:with-param name='content'>
	<!-- Normally a para would be the first child of a callout -->
	<xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- This is to catch the case where a listitem's first child is not a paragraph.
       - We may not be able to represent this properly.
      -->
    <xsl:apply-templates select='*[1][not(self::d:para)]' mode='list'/>

    <xsl:apply-templates select='*[position() != 1]' mode='list'/>
  </xsl:template>

  <xsl:template match='d:table|d:informaltable' mode='doc:body'>
    <xsl:call-template name='doc:make-table'>
      <xsl:with-param name='columns'>
        <xsl:apply-templates select='d:tgroup/d:colspec' mode='doc:column'/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:colspec' mode='doc:column'>
    <xsl:call-template name='doc:make-column'>
      <xsl:with-param name='width' select='@colwidth'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:colspec' mode='doc:body'/>

  <xsl:template name='doc:repeat'>
    <xsl:param name='repeats' select='0'/>
    <xsl:param name='content'/>

    <xsl:if test='$repeats > 0'>
      <xsl:copy-of select='$content'/>
      <xsl:call-template name='doc:repeat'>
        <xsl:with-param name='repeats' select='$repeats - 1'/>
        <xsl:with-param name='content' select='$content'/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
  <xsl:template match='d:tgroup|d:tbody|d:thead' mode='doc:body'>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='d:row' mode='doc:body'>
    <xsl:call-template name='doc:make-table-row'>
      <xsl:with-param name='is-header' select='boolean(parent::d:thead)'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:entry' mode='doc:body'>

    <!-- 
         Position = Sum(i,preceding-sibling[@colspan = ""]) + entry[i].@colspan)
      -->

    <xsl:variable name='position'>
      <xsl:call-template name='doc:sum-sibling'>
        <xsl:with-param name='sum' select='"1"'/>
        <xsl:with-param name='node' select='.'/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name='limit' select='$position + @colspan'/>

    <xsl:call-template name='doc:make-table-cell'>
      <xsl:with-param name='width'>
        <xsl:choose>
          <xsl:when test='@colspan != ""'>

            <!-- Select all the colspec nodes which correspond to the
                 column. That is all the nodes between the current 
                 column number and the column number plus the span.
              -->

            <xsl:variable name='combinedWidth'>
              <xsl:call-template name='doc:sum'>
                <xsl:with-param name='nodes' select='ancestor::*[self::d:table|self::d:informaltable][1]/d:tgroup/d:colspec[not(position() &lt; $position) and position() &lt; $limit]'/>
                <xsl:with-param name='sum' select='"0"'/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select='$combinedWidth'/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select='ancestor::*[self::d:table|self::d:informaltable][1]/d:tgroup/d:colspec[position() = $position]/@colwidth'/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>

      <xsl:with-param name='hidden' select='@hidden'/>
      <xsl:with-param name='rowspan' select='@rowspan'/>
      <xsl:with-param name='colspan' select='@colspan'/>

      <xsl:with-param name='content'>
	<xsl:choose>
          <xsl:when test='not(d:para)'>
            <!-- TODO: check for any block elements -->
	    <xsl:call-template name='doc:make-paragraph'/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates/>
          </xsl:otherwise>
	</xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <!-- Calculates the position by adding the 
       count of the preceding siblings where they aren't colspans
       and adding the colspans of those entries which do.
    -->

  <xsl:template name='doc:sum-sibling'>    
    <xsl:param name='sum'/>
    <xsl:param name='node'/>

    <xsl:variable name='add'>
      <xsl:choose>
        <xsl:when test='$node/preceding-sibling::d:entry/@colspan != ""'>
          <xsl:value-of select='$node/preceding-sibling::d:entry/@colspan'/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select='"1"'/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test='count($node/preceding-sibling::d:entry) &gt; 0'>
        <xsl:call-template name='doc:sum-sibling'>
          <xsl:with-param name='sum' select='$sum + $add'/>
          <xsl:with-param name='node' select='$node/preceding-sibling::d:entry[1]'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select='$sum'/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name='doc:sum'>
    <xsl:param name='sum' select='"0"'/>
    <xsl:param name='nodes'/>

    <xsl:variable name='tmpSum' select='$sum + $nodes[1]/@colwidth'/>

    <xsl:choose>
      <xsl:when test='count($nodes) &gt; 1'>
        <xsl:call-template name='doc:sum'>
          <xsl:with-param name='nodes' select='$nodes[position() != 1]'/>
          <xsl:with-param name='sum' select='$tmpSum'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select='$tmpSum'/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match='*[self::d:para|self::d:simpara]/text()[string-length(normalize-space(.)) != 0]'
    mode='doc:body'>
    <xsl:call-template name='doc:handle-linebreaks'/>
  </xsl:template>

  <xsl:template match='text()[not(parent::d:para|parent::d:simpara|parent::d:literallayout|parent::d:programlisting)][string-length(normalize-space(.)) != 0]'
    mode='doc:body'>
    <xsl:call-template name='doc:handle-linebreaks'/>
  </xsl:template>
  <xsl:template match='text()[string-length(normalize-space(.)) = 0]'
    mode='doc:body'/>
  <xsl:template match='d:literallayout/text()|d:programlisting/text()'
    mode='doc:body'>
    <xsl:call-template name='doc:handle-linebreaks'/>
  </xsl:template>
  <xsl:template name='doc:handle-linebreaks'>
    <xsl:param name='content' select='.'/>
    <xsl:param name='style'/>

    <xsl:choose>
      <xsl:when test='not($content)'/>
      <xsl:when test='contains($content, "&#d:xa;")'>
	<xsl:call-template name='doc:make-phrase'>
	  <xsl:with-param name='style' select='$style'/>
	  <xsl:with-param name='content'
			  select='substring-before($content, "&#d:xa;")'/>
        </xsl:call-template>

        <xsl:call-template name='doc:handle-linebreaks-aux'>
          <xsl:with-param name='content'
            select='substring-after($content, "&#d:xa;")'/>
	  <xsl:with-param name='style' select='$style'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
	<xsl:call-template name='doc:make-phrase'>
          <xsl:with-param name='style' select='$style'/>
	  <xsl:with-param name='content' select='$content'/>
	</xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- pre-condition: leading linefeed has been stripped -->
  <xsl:template name='doc:handle-linebreaks-aux'>
    <xsl:param name='content'/>
    <xsl:param name='style'/>

    <xsl:choose>
      <xsl:when test='contains($content, "&#d:xa;")'>
        <xsl:call-template name='doc:make-phrase'>
	  <xsl:with-param name='style' select='$style'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-soft-break'/>
            <xsl:value-of select='substring-before($content, "&#d:xa;")'/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name='doc:handle-linebreaks-aux'>
          <xsl:with-param name='content'
			  select='substring-after($content, "&#d:xa;")'/>
	  <xsl:with-param name='style' select='$style'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name='doc:make-phrase'>
	  <xsl:with-param name='style' select='$style'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-soft-break'/>
            <xsl:value-of select='$content'/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='d:authorblurb|d:formalpara|d:legalnotice|d:note|d:caution|d:warning|d:important|d:tip'
    mode='doc:body'>
    <xsl:apply-templates select='*'>
      <xsl:with-param name='class'>
        <xsl:value-of select='name()'/>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match='d:blockquote'
    mode='doc:body'>
    <xsl:apply-templates select='d:blockinfo|d:title'>
      <xsl:with-param name='class'>
        <xsl:value-of select='name()'/>
      </xsl:with-param>
    </xsl:apply-templates>
    <xsl:apply-templates select='*[not(self::d:blockinfo|self::d:title|self::d:attribution)]'>
      <xsl:with-param name='class' select='"d:blockquote"'/>
    </xsl:apply-templates>
    <xsl:if test='d:attribution'>
      <xsl:call-template name='doc:make-paragraph'>
	<xsl:with-param name='style' select='"blockquote-attribution"'/>
	<xsl:with-param name='content'>
          <xsl:apply-templates select='d:attribution/node()'/>
	</xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match='d:literallayout|d:programlisting'
    mode='doc:body'>
    <xsl:param name='class'/>

    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='name()'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:bridgehead'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style' select='"d:bridgehead"'/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match='d:itemizedlist|d:orderedlist'
    mode='doc:body'>
    <xsl:apply-templates select='d:listitem'
      mode='doc:body'/>
  </xsl:template>

  <xsl:template match='d:listitem'
    mode='doc:body'>
    <xsl:call-template name='doc:make-paragraph'>
      <xsl:with-param name='style'
		      select='concat(name(..), 
			      count(ancestor::d:itemizedlist|ancestor::d:orderedlist))'/>
      <xsl:with-param name='is-listitem' select='true()'/>

      <xsl:with-param name='content'>
	<!-- Normally a para would be the first child of a listitem -->
	<xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- This is to catch the case where a listitem's first child is not a paragraph.
       - We may not be able to represent this properly.
      -->
    <xsl:apply-templates select='*[1][not(self::d:para)]' mode='doc:list'/>

    <xsl:apply-templates select='*[position() != 1]' mode='doc:list'/>
  </xsl:template>  

  <xsl:template match='*' mode='doc:list'>
    <xsl:apply-templates select='.'>
      <xsl:with-param name='class' select='"para-continue"'/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match='d:variablelist'
    mode='doc:body'>
    <xsl:apply-templates select='*[not(self::d:varlistentry)]'/>

    <xsl:call-template name='doc:make-table'>
      <xsl:with-param name='columns'>
	<xsl:call-template name='doc:make-column'>
	  <xsl:with-param name='width' select='"1"'/>
	  <xsl:with-param name='width' select='"3"'/>
	</xsl:call-template>
      </xsl:with-param>
      <xsl:with-param name='rows'>
	<xsl:apply-templates select='d:varlistentry'/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match='d:varlistentry'
    mode='doc:body'>
    <xsl:call-template name='doc:make-table-row'>
      <xsl:with-param name='content'>
	<xsl:call-template name='doc:make-table-cell'>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-paragraph'>
	      <xsl:with-param name='style' select='"variablelist-term"'/>
	      <xsl:with-param name='content'>
		<xsl:apply-templates select='d:term[1]/node()'/>
		<xsl:for-each select='d:term[position() != 1]'>
		  <xsl:call-template name='doc:make-phrase'>
		    <xsl:with-param name='content'>
		      <xsl:call-template name='doc:make-soft-break'/>
		    </xsl:with-param>
		  </xsl:call-template>
		  <xsl:apply-templates/>
		</xsl:for-each>
              </xsl:with-param>
	    </xsl:call-template>
	  </xsl:with-param>
	</xsl:call-template>
	<xsl:call-template name='doc:make-table-cell'>
	  <xsl:with-param name='content'>
            <xsl:apply-templates select='d:listitem/node()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <!-- These elements are not displayed.
     - However, they may need to be added (perhaps as hidden text)
     - for round-tripping.
    -->
  <xsl:template match='d:anchor|d:areaset|d:audiodata|d:audioobject|
                       d:beginpage|
                       d:constraint|
                       d:indexterm|d:itermset|
                       d:keywordset|
                       d:msg'
    mode='doc:body'/>

  <xsl:template match='*' name='doc:nomatch'>
    <xsl:message>
      <xsl:value-of select='name()'/>
      <xsl:text> encountered</xsl:text>
      <xsl:if test='parent::*'>
        <xsl:text> in </xsl:text>
        <xsl:value-of select='name(parent::*)'/>
      </xsl:if>
      <xsl:text>, but no template matches.</xsl:text>
    </xsl:message>

    <xsl:choose>
      <xsl:when test='self::d:abstract |
                      self::d:ackno |
                      self::d:address |
                      self::d:answer |
                      self::d:appendix |
                      self::d:artheader |
                      self::d:authorgroup |
                      self::d:bibliodiv |
                      self::d:biblioentry |
                      self::d:bibliography |
                      self::d:bibliomixed |
                      self::d:bibliomset |
                      self::d:biblioset |
                      self::d:bridgehead |
                      self::d:calloutlist |
                      self::d:caption |
                      self::d:classsynopsis |
                      self::d:colophon |
                      self::d:constraintdef |
                      self::d:copyright |
                      self::d:dedication |
                      self::d:epigraph |
                      self::d:equation |
                      self::d:example |
                      self::d:figure |
                      self::d:funcsynopsis |
                      self::d:glossary |
                      self::d:glossdef |
                      self::d:glossdiv |
                      self::d:glossentry |
                      self::d:glosslist |
                      self::d:graphic |
                      self::d:highlights |
                      self::d:imageobject |
                      self::d:imageobjectco |
                      self::d:index |
                      self::d:indexdiv |
                      self::d:indexentry |
                      self::d:informalequation |
                      self::d:informalexample |
                      self::d:informalfigure |
                      self::d:lot |
                      self::d:lotentry |
                      self::d:mediaobject |
                      self::d:mediaobjectco |
                      self::d:member |
                      self::d:msgentry |
                      self::d:msgset |
                      self::d:part |
                      self::d:partintro |
                      self::d:personblurb |
                      self::d:preface |
                      self::d:printhistory |
                      self::d:procedure |
                      self::d:programlisting |
                      self::d:programlistingco |
                      self::d:publisher |
                      self::d:qandadiv |
                      self::d:qandaentry |
                      self::d:qandaset |
                      self::d:question |
                      self::d:refdescriptor |
                      self::d:refentry |
                      self::d:refentrytitle |
                      self::d:reference |
                      self::d:refmeta |
                      self::d:refname |
                      self::d:refnamediv |
                      self::d:refpurpose |
                      self::d:refsect1 |
                      self::d:refsect2 |
                      self::d:refsect3 |
                      self::d:refsection |
                      self::d:refsynopsisdiv |
                      self::d:screen |
                      self::d:screenco |
                      self::d:screenshot |
                      self::d:seg |
                      self::d:seglistitem |
                      self::d:segmentedlist |
                      self::d:segtitle |
                      self::d:set |
                      self::d:setindex |
                      self::d:sidebar |
                      self::d:simplelist |
                      self::d:simplemsgentry |
                      self::d:step |
                      self::d:stepalternatives |
                      self::d:subjectset |
                      self::d:substeps |
                      self::d:task |
                      self::d:textobject |
                      self::d:toc |
                      self::d:videodata |
                      self::d:videoobject |
                      self::*[not(starts-with(name(), "d:informal")) and contains(name(), "d:info")]'>
	<xsl:call-template name='doc:make-paragraph'>
	  <xsl:with-param name='style' select='"d:blockerror"'/>
	  <xsl:with-param name='content'>
	    <xsl:call-template name='doc:make-phrase'>
	      <xsl:with-param name='content'>
		<xsl:value-of select='name()'/>
		<xsl:text> encountered</xsl:text>
		<xsl:if test='parent::*'>
                  <xsl:text> in </xsl:text>
                  <xsl:value-of select='name(parent::*)'/>
		</xsl:if>
		<xsl:text>, but no template matches.</xsl:text>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:with-param>
	</xsl:call-template>
      </xsl:when>
      <!-- Some elements are sometimes blocks, sometimes inline
      <xsl:when test='self::d:affiliation |
                      self::d:alt |
                      self::d:attribution |
                      self::d:collab |
                      self::d:collabname |
                      self::d:confdates |
                      self::d:confgroup |
                      self::d:confnum |
                      self::d:confsponsor |
                      self::d:conftitle |
                      self::d:contractnum |
                      self::d:contractsponsor |
                      self::d:contrib |
                      self::d:corpauthor |
                      self::d:corpcredit |
                      self::d:corpname |
                      self::d:edition |
                      self::d:editor |
                      self::d:jobtitle |
                      self::d:personname |
                      self::d:publishername |
                      self::d:remark'>

      </xsl:when>
      -->
      <xsl:otherwise>
        <xsl:call-template name='doc:make-phrase'>
          <xsl:with-param name='style' select='"d:inlineerror"'/>
	  <xsl:with-param name='content'>
            <xsl:value-of select='name()'/>
            <xsl:text> encountered</xsl:text>
            <xsl:if test='parent::*'>
              <xsl:text> in </xsl:text>
              <xsl:value-of select='name(parent::*)'/>
            </xsl:if>
            <xsl:text>, but no template matches.</xsl:text>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='*' mode='doc:copy'>
    <xsl:copy>
      <xsl:apply-templates select='@*' mode='doc:copy'/>
      <xsl:apply-templates mode='doc:copy'/>
    </xsl:copy>
  </xsl:template>
  <xsl:template match='@*' mode='doc:copy'>
    <xsl:copy/>
  </xsl:template>

  <!-- Stubs: the importing stylesheet must override these -->

  <!-- stub template for creating a paragraph -->
  <xsl:template name='doc:make-paragraph'>
  </xsl:template>
  <!-- stub template for creating a phrase -->
  <xsl:template name='doc:make-phrase'>
  </xsl:template>

  <!-- stub template for inserting attributes -->
  <xsl:template name='doc:attributes'/>

  <!-- emit a message -->
  <xsl:template name='doc:warning'>
    <xsl:param name='message'/>

    <xsl:message>WARNING: <xsl:value-of select='$message'/></xsl:message>
  </xsl:template>

</xsl:stylesheet>
