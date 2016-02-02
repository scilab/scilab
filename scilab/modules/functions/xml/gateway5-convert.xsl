<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" doctype-system="../../functions/xml/gateway.dtd"/>
    <xsl:template match="/">
        <xsl:comment>
            /*
            *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
            *  Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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
            Scilab
            Interface description. In this file, we define the list of the function which
            will be available into Scilab and the link to the "native" function.
            
            Don't touch if you do not know what you are doing !
        </xsl:comment>
        
        <module>
            <xsl:attribute name="name">
                <xsl:value-of select="GATEWAY/@name" />
            </xsl:attribute>
            
            <xsl:for-each select="GATEWAY/PRIMITIVE">
                <gateway type="0">
                    <xsl:attribute name="name">
                        <xsl:value-of select="concat('sci_', @primitiveName)" />
                    </xsl:attribute>
                    <xsl:attribute name="function">
                        <xsl:value-of select="@primitiveName" />
                    </xsl:attribute>
                    <xsl:attribute name="type">0</xsl:attribute>
                </gateway>
            </xsl:for-each>
        </module>
    </xsl:template>
    
</xsl:stylesheet>

