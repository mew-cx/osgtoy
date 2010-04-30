#!/bin/sh

version=`sed -n -e "/AM_INIT_AUTOMAKE(sng, \(.*\))/s//\\1/p" <configure.in`
date=`date "+%d %b %Y"`

cat >index.html <<EOF
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<link rev="made" href="mailto:esr@snark.thyrsus.com"/>
<meta name="description" content="Home page of the sng compiler/decompiler"/>
<meta name="keywords" content="SNG, PNG, converter, graphics"/> 
<title>The sng home page</title>
</head>
<body>
<table width="100%" cellpadding=0 summary="Canned page header"><tr>
<td width="30%">Back to <a href="http://www.tuxedo.org/~esr">Eric's Home Page</a>
<td width="30%" align=center>Up to <a href="http://www.tuxedo.org/~esr/sitemap.html">Site Map</a>
<td width="30%" align=right>$date
</tr></table>
<hr />

<center>
Powered by:<br />
<a href="http://sourceforge.net"> 
<img src="http://sourceforge.net/sflogo.php?group_id=1012&amp;type=1"
alt="SourceForge logo" width="88" height="31" border="0"/></a> 
</center>

<h1 align=center>The SNG home page</h1>

<p>SNG (Scriptable Network Graphics) is a minilanguage designed
specifically to represent the entire contents of a PNG (Portable
Network Graphics) file in an editable form.  Thus, SNGs representing
elaborate graphics images and ancillary chunk data can be readily
generated or modified using only text tools.</p>

<p>SNG is implemented by a compiler/decompiler called sng that losslessly
translates between SNG and PNG.</p>

<p>The current version is ${version}.  This is a production release, and
has been tested on all 155 of the non-broken images in the png test
suite.</p>

<p>Downloads are available from SNG's <a
href="https://sourceforge.net/projects/sng/">SourceForge page</a>.</p>

<p>Note: To use sng, you will need to install libpng-1.0.9beta7 or later.
You can download it <a
href="ftp://swrinde.nde.swri.edu/pub/png-group/src">here</a>).</p>

<p>(I am a credited contributor to the libpng reference library, and also
maintain <a href="http://www.tuxedo.org/~esr/gif2png/">gif2png</a>.
For more information on the PNG format and associated tools, see the
official <a href="http://www.libpng.org/pub/png/">PNG website</a>)</p>

<hr />
<table width="100%" cellpadding=0 summary="Canned page footer"><tr>
<td width="30%">Back to <a href="http://www.tuxedo.org/~esr">Eric's Home Page</a>
<td width="30%" align=center>Up to <a href="http://www.tuxedo.org/~esr/sitemap.html">Site Map</a>
<td width="30%" align=right>$date
</tr></table>

<br clear="left"/>
<address>Eric S. Raymond <a href="mailto:esr@thyrsus.com">&lt;esr@thyrsus.com&gt;</a></address>
</body>
</html>
EOF
