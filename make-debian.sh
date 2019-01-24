#!/bin/bash

# determine the version from the build_number file
build_number=$(<build_number)
version="1.$build_number"
pkg_dir="xolotl-$version"

# make the appropriate directories
mkdir pkg_dir
mkdir pkg_dir/debian

# create changelog
dch --create -v $version-1 --package xolotl

# compatibility
echo 10 > pkg_dir/debian/compat

# control
cat > pkg_dir/debian/control << EOF
Source: xolotl
Maintainer: Srinivas Gorur-Shandilya <me@srinivas.gs>
Section: science
Priority: optional

Package: xolotl
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}
Description: `xolotl` is a fast single-compartment and
  multi-compartment simulator  written in `C++` with
  a `MATLAB` interface that you'll actually enjoy using.
EOF

# copyright
cat > pkg_dir/debian/copyright << EOF
GPL v3.0
EOF

# rules
cat > pkg_dir/debian/rules << EOF
#!/usr/bin/make -f
%:
	dh $@
EOF

# debian version
echo "3.0 (quilt)" > pkg_dir/debian/source/format
