#!/bin/bash          
BASEDIR="/usr/cvmfs"
echo $BASEDIR    
BUILDDIR="${BASEDIR}/build"
echo $BUILDDIR
cd $BUILDDIR
cmake ../
make 
sudo make install
