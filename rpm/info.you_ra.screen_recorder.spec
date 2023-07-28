%define __provides_exclude_from ^%{_datadir}/%{name}/.*$
%define __requires_exclude ^.*$

Name:       info.you_ra.screen_recorder

Summary:    Screen recorder application
Version:    0.1.302818
Release:    1
Group:      Qt/Qt
License:    GPLv2
URL:        https://you-ra.info
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
#Requires:   ffmpeg
#Requires:   lipstick2vnc
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
#BuildRequires:  ffmpeg-devel
#BuildRequires:  libvncserver-devel

%description
Screen recorder application

%prep
%setup -q -n %{name}-%{version}
#cd FFmpegBuild && cmake .

%build
%cmake
%make_build
if test -f %{name} ; then strip %{name}; fi

%install
rm -rf %{buildroot}
%make_install

desktop-file-install --delete-original         --dir %{buildroot}%{_datadir}/applications                %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
