#!/bin/bash

# fetches all of xolotl's dependencies from GitHub and downloads them
# to a folder named xolotl in your current working directory
# then, it tries to find your matlab installation and add the correct folders
# to your path

echo "Welcome to Xolotl!"
echo "https://xolotl.readthedocs.io/en/master/"

mkdir xolotl
cd xolotl

echo "Fetching git repositories..."

git clone https://github.com/sg-s/cpplab.git
git clone https://github.com/sg-s/xolotl.git
git clone https://github.com/sg-s/srinivas.gs_mtools.git
git clone https://github.com/sg-s/puppeteer.git

echo "Adding code to MATLAB path..."

matlab -nodisplay -nodesktop -r "addpath('$pwd/cpplab', '$pwd/xolotl', '$pwd/puppeteer', genpath('$pwd/srinivas.gs_mtools/src'); quit;"

echo "Attempting default C-compiler setup..."
matlab -nodisplay -nodesktop -r "mex -setup c++; mex -setup c; quit;"

echo "DONE!"
