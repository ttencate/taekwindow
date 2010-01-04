<?xml version="1.0" encoding="utf-8"?>

<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

	<include href="version.xslt"/>

	<variable name="meta" select="document('meta.xml')/meta"/>

	<variable name="apptitle" select="$meta/apptitle"/>
	<variable name="author" select="$meta/author"/>
	<variable name="email" select="$meta/email"/>
	<variable name="website" select="$meta/website"/>
	<variable name="copyright" select="$meta/copyright"/>
	<variable name="licensebrief" select="$meta/licensebrief"/>

	<variable name="filebasename" select="$meta/filebasename"/>
	<variable name="mainexefile" select="$meta/mainexefile"/>
	<variable name="readmefile" select="$meta/readmefile"/>

	<variable name="appnameversion">
		<value-of select="apptitle"/>
		<text xml:space="preserve"> </text>
		<value-of select="shortversion"/>
	</variable>

	<variable name="authorfull">
		<value-of select="author"/>
		<text xml:space="preserve"> &lt;</text>
		<value-of select="email"/>
		<text xml:space="preserve">&gt;</text>
	</variable>

	<variable name="binaryzipfile">
		<value-of select="$filebasename"/>
		<text xml:space="preserve">-</text>
		<value-of select="$shortversion"/>
		<text xml:space="preserve">.zip</text>
	</variable>

	<variable name="sourcezipfile">
		<value-of select="$filebasename"/>
		<text xml:space="preserve">-</text>
		<value-of select="$shortversion"/>
		<text xml:space="preserve">-src.zip</text>
	</variable>
	
</stylesheet>