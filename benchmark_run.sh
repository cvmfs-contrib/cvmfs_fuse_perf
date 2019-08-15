#!/bin/bash
# erase all cached data.
sudo cvmfs_config wipecache
# try to make connection to cvmfs repository
cvmfs_config probe
# run singularity container with sample python code
singularity exec /cvmfs/unpacked.cern.ch/registry.hub.docker.com/atlasml/ml-base:centos-py-3.6.8 python3 -c "import numpy as np;import tensorflow as tf"
# run cvmfs_talk command to retrieve information about performace metrics
sudo cvmfs_talk -i unpacked.cern.ch internal affairs
