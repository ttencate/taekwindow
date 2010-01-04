<?xml version="1.0" encoding="utf-8"?>

<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

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
	
</stylesheet>