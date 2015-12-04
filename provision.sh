#!/bin/bash

echo "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}/usr/lib64/openmpi/lib" >> /home/vagrant/.bashrc
echo "export LD_LIBRARY_PATH" >> /home/vagrant/.bashrc

sudo yum install -y openmpi openmpi-devel
