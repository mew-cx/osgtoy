#!/bin/sh

version=`sed -n -e "/AM_INIT_AUTOMAKE(sng, \(.*\))/s//\\1/p" <configure.in`
date=`date "+%d%b%Y"`

cat >sng.lsm <<EOF
Begin3
Title:		sng
Version:	$version
Entered-date:	$date
Description:	This program converts between PNG and SNG, a representation of
		PNG chuink data specifically designed to be editable with
		normal text tools.
Keywords:	SNG, PNG, converter, graphics
Maintainer:	esr@snark.thyrsus.com (Eric S. Raymond)
Primary-site:	http://www.tuxedo.org/~esr/
		19K sng-$version.tar.gz
Platforms:	All
Copying-policy:	libpng/zlib
End
EOF
