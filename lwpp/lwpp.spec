# User-Defined Macros:
# %define <name> <expansion>
%define Name		lwpp
%define Version		1.0
%define Release		1
%define Prefix		/usr
Summary: Lightweight file preprocessor
Name: %{Name}
Version: %{Version}
Release: %{Release}
#Serial: 
Packager: Jim Knoble <jmknoble@pobox.com>
Copyright: GPL
Group: Utilities/Text
URL: http://www.pobox.com/~jmknoble/
Source0: %{Name}-%{Version}.tar.gz
#Patch0: 
#Prefix: 
BuildRoot: /tmp/%{Name}-%{Version}-%{Release}-root
#Provides: 
#Requires: 
#Obsoletes: 

%description
Lwpp is a lightweight text file preprocessor.  Lwpp reads a file as
input and writes sections of the file as output, based on certain
conditions embedded in the file as preprocessor directives.  Lwpp is
meant mostly to emulate the C preprocessor's `#if[n]def ... #endif'
functionality, but without any of cpp's other C-oriented features
(including macro expansion), and without putting extra newlines into
the output.  Lwpp uses the environment as a dictionary of symbol
definitions, so you can use the value of regular environment variables
(such as HOME or HOST) as symbol definitions without having to define
them again on the command line.

%prep
%setup
#%patch0 -b .orig

#function Replace() {
#  local fil="$1"
#  local sep="$2"
#  local old="$3"
#  local new="$4"
#  local suf="$5"
#  [ -z "${suf}" ] && suf='~'
#  mv -f ${fil} ${fil}${suf}
#  cat ${fil}${suf} | sed -e "s${sep}${old}${sep}${new}${sep}g" >$fil
#}

%build
if [ -z "${CC}" ]; then
  if [ -n "`which gcc`" ]; then
    CC="gcc"
  else
    CC="cc"
  fi
fi
export CC

make CC="${CC}" OPTFLAGS="${RPM_OPT_FLAGS}" PREFIX="%{Prefix}"

%install
function CheckBuildRoot() {
  # do a few sanity checks on the BuildRoot
  # to make sure we don't damage a system
  case "${RPM_BUILD_ROOT}" in
    ''|' '|/|/bin|/boot|/dev|/etc|/home|/lib|/mnt|/root|/sbin|/tmp|/usr|/var)
      echo "Yikes!  Don't use '${RPM_BUILD_ROOT}' for a BuildRoot!"
      echo "The BuildRoot gets deleted when this package is rebuilt;"
      echo "something like '/tmp/build-blah' is a better choice."
      return 1
      ;;
    *)  return 0
      ;;
  esac
}
function CleanBuildRoot() {
  if CheckBuildRoot; then
    rm -rf "${RPM_BUILD_ROOT}"
  else
    exit 1
  fi
}
CleanBuildRoot

for i in \
  "" \
  %{Prefix} \
; do
  mkdir -p "${RPM_BUILD_ROOT}${i}"
done

make DESTDIR="${RPM_BUILD_ROOT}" PREFIX="%{Prefix}" install

%clean
function CheckBuildRoot() {
  # do a few sanity checks on the BuildRoot
  # to make sure we don't damage a system
  case "${RPM_BUILD_ROOT}" in
    ''|' '|/|/bin|/boot|/dev|/etc|/home|/lib|/mnt|/root|/sbin|/tmp|/usr|/var)
      echo "Yikes!  Don't use '${RPM_BUILD_ROOT}' for a BuildRoot!"
      echo "The BuildRoot gets deleted when this package is rebuilt;"
      echo "something like '/tmp/build-blah' is a better choice."
      return 1
      ;;
    *)  return 0
      ;;
  esac
}
function CleanBuildRoot() {
  if CheckBuildRoot; then
    rm -rf "${RPM_BUILD_ROOT}"
  else
    exit 1
  fi
}
CleanBuildRoot

%files
%attr(-   ,root,root) %doc INSTALL.txt Lwpp-HOWTO.txt README.txt tmp.txt tmp2.txt
%attr(0755,root,root) %{Prefix}/bin/lwpp
#%attr(0444,root,root) %{Prefix}/man/man1/lwpp.1

