## Process this file with automake to produce Makefile.in
bin_PROGRAMS = sng
#bin_SCRIPTS = sng_regress
sng_SOURCES = main.c sngc.c sngd.c sng.h
man_MANS = sng.1
# The man pages and script are here because automake has a bug
EXTRA_DIST = Makefile sng.spec.in sng.spec sng.xml sng.1 sng_regress indexgen.sh lsmgen.sh test.sng

# This is necessary because as of version 1.4, automake generates a dist 
# production that runs a broken automake command
sng-@VERSION@.tar.gz: MANIFEST
	(cd ..; ln -sf sng sng-@VERSION@)
	(cd ..; tar -czf sng-@VERSION@.tar.gz `sed <sng-@VERSION@/MANIFEST s:^:sng-@VERSION@/:`)
	mv -f ../sng-@VERSION@.tar.gz .
	rm ../sng-@VERSION@

MANIFEST: $(srcdir) Makefile.in
	@echo $(DISTFILES) | tr "[ \t]" '\n' | sed -e '/^\.\//s///' | sort >MANIFEST
	@wc -l MANIFEST

sng.1: sng.xml
	xmlto man sng.xml

# Make RPMs.  You need to be root to make this work
RPMROOT=/usr/src/redhat
rpm: dist
	rpmbuild --define 'myversion @VERSION@' -ta sng-@VERSION@.tar.gz
	cp $(RPMROOT)/RPMS/*/sng-@VERSION@*.rpm .
	cp $(RPMROOT)/SRPMS/sng-@VERSION@*.src.rpm .

zip: $(DISTFILES)
	zip @PACKAGE@-@VERSION@.zip $(DISTFILES)

# Regression-test sng.  Passes if no differences show up.
# Assumes we have a copy of Willem van Schaik's PNG test suite under pngsuite
regress:
	sng_regress test.sng -s pngsuite/[a-wyz]*.png
