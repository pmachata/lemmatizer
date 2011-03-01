Name:		lemmatizer-web
Version:	0.1
Release:	1%{?dist}
Summary:	Web interface to Russian LemmatizerLib of Seman

Group:		Applications/Internet
License:	AGPLv3
URL:		https://github.com/pmachata/lemmatizer
Source0:	%{name}-%{version}.tar.bz2
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:	seman-devel boost-devel clearsilver-devel fcgi-devel

%description

This package is Lemmatizer, a web interface to Russian LemmatizerLib of
Seman.


%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT

mkdir -p $RPM_BUILD_ROOT/%{_var}/www/cgi-bin/
install -m 755 lemmatizer $RPM_BUILD_ROOT/%{_var}/www/cgi-bin/lemmatizer.fcgi

mkdir -p $RPM_BUILD_ROOT/%{_var}/www/lemmatizer/
find . -name \*.cs | xargs install -m 644 -t $RPM_BUILD_ROOT/%{_var}/www/lemmatizer/

mkdir -p $RPM_BUILD_ROOT/%{_var}/www/html/lemmatizer/
find html -type f | xargs install -m 644 -t $RPM_BUILD_ROOT/%{_var}/www/html/lemmatizer/

mkdir -p $RPM_BUILD_ROOT/%{_sysconfdir}
install -m 644 lemmatizer.hdf $RPM_BUILD_ROOT/%{_sysconfdir}


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc COPYING
%config(noreplace) %{_sysconfdir}/lemmatizer.hdf
%{_var}/www/cgi-bin/*
%{_var}/www/lemmatizer
%{_var}/www/html/lemmatizer


%changelog
