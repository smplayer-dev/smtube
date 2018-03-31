Name:           smtube
Version:        18.3.0
Release:        1%{?dist}
Summary:        YouTube browser for SMPlayer

Group:          Applications/Multimedia
License:        GPL-2.0+
URL:            http://www.smtube.org
Source0:        http://downloads.sourceforge.net/smtube/smtube-%{version}.tar.bz2

%if 0%{?suse_version}
BuildRequires:  hicolor-icon-theme
BuildRequires:  libqt5-qttools-devel
%else
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qttools-devel
%endif
BuildRequires:  desktop-file-utils
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(Qt5Concurrent)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5PrintSupport)
BuildRequires:  pkgconfig(Qt5Script)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5WebKitWidgets)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Xml)

Requires:       smplayer
%{?_qt4_version:Requires: qt4%{?_isa} >= %{_qt4_version}}

%description
This is a YouTube browser for SMPlayer. You can browse, search
and play YouTube videos.

%global debug_package %{nil}

%prep
%setup -q

# correction for wrong-file-end-of-line-encoding
%{__sed} -i 's/\r//' *.txt
# fix files which are not UTF-8 
iconv -f Latin1 -t UTF-8 -o Changelog.utf8 Changelog 
mv Changelog.utf8 Changelog

%build
make \
	QMAKE=%{_bindir}/qmake-qt5 \
	LRELEASE=%{_bindir}/lrelease-qt5 \
	PREFIX=%{_prefix} \
	DOC_PATH="\\\"%{_docdir}/%{name}/\\\""

#touch src/smtube
#touch src/translations/smtube_es.qm

%install
make PREFIX=%{_prefix} DESTDIR=%{buildroot}/ DOC_PATH=%{_docdir}/%{name}/ install

%post
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi
update-desktop-database &> /dev/null || :

%postun
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi
update-desktop-database &> /dev/null || :

%files
%defattr(-,root,root)
%{_bindir}/smtube
%{_datadir}/applications/*.desktop
%dir %{_datadir}/icons/hicolor/*/
%dir %{_datadir}/icons/hicolor/*/apps/
%{_datadir}/icons/hicolor/*/apps/%{name}.*
%{_datadir}/smtube/
#%{_mandir}/man1/smtube.1.gz
%{_docdir}/%{name}/

%changelog
* Thu Feb 26 2016 Ricardo Villalba <rvm@users.sourceforge.net> - 16.1.0
- Initial Release
