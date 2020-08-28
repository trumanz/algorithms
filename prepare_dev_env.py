#!/usr/bin/env python
import os
import subprocess

def prepare_env_linux():
    subprocess.run("bash ./script/prepare_env_ubuntu.sh".split(), check=True)
 

if __name__ == '__main__':
    

    if os.name == "posix":
        prepare_env_linux()
    else:
        raise NameError("not suport other platform")





