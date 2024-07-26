import argparse
import subprocess
from werkzeug.utils import secure_filename
import os

EXAMINING_EXTENSIONS = {'cpp', 'exe'}
FILTER_WORD = ["socket", "wsa", "connect", "sock_stream", "af_inet"]

def examine_file(filename):
  if '.' not in filename:
    return False
  filename = filename.rsplit('.', 1)[1]
  print(filename)
  # [1].lower() in EXAMINING_EXTENSIONS

print(examine_file("../../../hello.exe"))





