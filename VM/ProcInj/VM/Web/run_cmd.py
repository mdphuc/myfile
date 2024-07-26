import argparse
import subprocess
from werkzeug.utils import secure_filename
import os

EXAMINING_EXTENSIONS = {'cpp', 'exe'}
FILTER_WORD = ["socket", "wsa", "connect", "sock_stream", "af_inet"]

def examine_file(filename):
  return '.' in filename and filename.rsplit('.', 1)[1].lower() in EXAMINING_EXTENSIONS

parser = argparse.ArgumentParser()

parser.add_argument(dest = 'name')
parser.add_argument('-o', dest = 'output')
parser.add_argument("-c", action ="store_true")
parser.add_argument("-shared", action = "store_true")
parser.add_argument("-lws2_32", action="store_true")

args = parser.parse_args() 

def main():
  if not os.path.isfile("C://ProgramData/ProcInj/Web/temp/{0}".format(secure_filename(args.name))):
    return "False"
  else:
    command = ["powershell.exe", "g++", "C://ProgramData/ProcInj/Web/temp/{0}".format(secure_filename(args.name)), "-o", "C://ProgramData/ProcInj/Web/compiled/{0}".format(args.output)]

    if args.c:
      command.append("-c")
    
    if args.shared:
      command.append("-shared")

    if args.lws2_32:
      command.append("-lws2_32")

    flagged = False

    if examine_file("C://ProgramData/ProcInj/Web/temp/{0}".format(secure_filename(args.name))) and examine_file("C://ProgramData/ProcInj/Web/compiled/{0}".format(args.output)) and secure_filename(args.output) != "debug.exe":
      with open("C://ProgramData/ProcInj/Web/temp/{0}".format(secure_filename(args.name)), "r") as f:
        data = f.read()
        
        for fw in FILTER_WORD:
          if fw in data:
            flagged = True
            break

    if flagged == True:
      os.remove(os.path.join("C://ProgramData/ProcInj/Web/temp", secure_filename(args.name)))
      return "False"

    proc = subprocess.run(" ".join(command), capture_output=True, text=True)

    print(proc.stderr)

    if len(proc.stderr) != 0:
      return "False"
    else:
      return "True"
  
print(main())




