#!/usr/bin/env python
import os
import subprocess

def run_test_ubuntu():
     subprocess.run("python run-test.py".split(), check=True, cwd="./cpp-algo")

if __name__ == '__main__':
    
    run_test_ubuntu()





