#!/usr/bin/env bash

set -e 
sudo apt-get -y install python3-dev python3-wheel python3-setuptools python3-six
sudo apt-get -y install python3-pip
python3 -m pip install -U --user ortools
