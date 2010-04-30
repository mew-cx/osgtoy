%define	name	sng
%define	version	1.0.2
%define	release	1
%define	serial	1
%define	prefix	/usr

Summary:	sng -- SNG to PNG compiler/decompiler
Name:		%{name}
Version:	%{version}
Release:	%{release}
Serial:		%{serial}
Prefix:		%{prefix}
Copyright:	MIT-like
Group:		Applications/Graphics
URL:		http://www.tuxedo.org/~esr/sng/
Vendor:		Eric S. Raymond <esr@thyrsus.com>
Source:		%{name}-%{version}.tar.gz
BuildRoot:	/var/tmp/%{name}-%{version}
Requires:	libpng >= 1.0.9

%description 
The sng program converts files from the Scriptable Network Graphics
to Portable Network Graphics, or back again.  SNG is specifically
designed to be an editable representation of all the data in a PNG.

%prep
%setup -q

%build
%configure --prefix=%{prefix}

make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi

%files
%defattr(-,root,root)
%doc COPYING ChangeLog INSTALL README TODO
%{prefix}/bin/sng
%{prefix}/share/man/man1/sng.1.gz

%changelog
