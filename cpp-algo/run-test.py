#!/usr/bin/env python
import os
import subprocess
import shutil
def run_test_ubuntu():
     shutil.rmtree("build", ignore_errors=True)
     os.mkdir("build")
     os.chdir("build")
     subprocess.run("cmake ../ && make && ./cpp-algo", shell=True, check=True)


if __name__ == '__main__':
    
    run_test_ubuntu()





