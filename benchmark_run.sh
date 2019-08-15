#!/bin/bash
sudo cvmfs_config wipecache
cvmfs_config probe
singularity exec /cvmfs/unpacked.cern.ch/registry.hub.docker.com/atlasml/ml-base:centos-py-3.6.8 python3 -c "import numpy as np;import tensorflow as tf"
sudo cvmfs_talk -i unpacked.cern.ch internal affairs
