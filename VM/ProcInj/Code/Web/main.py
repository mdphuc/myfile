from flask import Flask, render_template, request, redirect, url_for, send_file
from werkzeug.utils import secure_filename
import hashlib
import subprocess 
import os
import random

app = Flask(__name__)
app.config['SECRET_KEY'] = hashlib.sha256(b"app").hexdigest()

ALLOWED_EXTENSIONS = {'cpp'}
FILTER_WORD = ["socket", "wsa", "connect", "sock_stream", "af_inet"]

def allowed_file(filename):
  return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

def get_all_compiled():
  file = os.listdir("C://ProgramData/ProcInj/Web/compiled")
  file_ = []
  for f in file:
    if os.path.isfile("C://ProgramData/ProcInj/Web/compiled/{0}".format(f)):
      file_.append(f)
  return file_
  
@app.route("/download", methods=["POST"])
def download():
  filename = request.form["filename"]
  if ".." in filename:
    return redirect(url_for('home'))
  return send_file("C://ProgramData/ProcInj/Web/compiled/{0}".format(filename), as_attachment=True)

@app.route("/", methods=["GET"])
def home():
  compiled_file = get_all_compiled()
  return render_template("index.html", compiled_file = compiled_file)

@app.route("/compile", methods=["POST"])
def compile():
  command = request.form["command"]
  
  command = command.split()

  command.insert(0, "C://ProgramData/ProcInj/Web/run_cmd.py")
  command.insert(0, "python")

  print(command)

  proc = subprocess.run(" ".join(command), capture_output=True, text=True, shell=True)

  compiled_file = get_all_compiled()

  if proc.stdout.split("\n")[0] == "False":
    return render_template("index.html", error = "true", compiled_file = compiled_file)
  else:
    return render_template("index.html", error = "false", compiled_file = compiled_file)
    
@app.route("/save", methods=["POST"])
def save():
  file = request.files["file"]
  compiled_file = get_all_compiled()

  if file.filename == '':
    return render_template("index.html", error = "true", compiled_file = compiled_file)
  
  if file and allowed_file(file.filename):
    filename = secure_filename(file.filename)

    file.save(os.path.join("C://ProgramData/ProcInj/Web/temp", filename))
  
    return render_template("index.html", error = "false", compiled_file = compiled_file)

  return render_template("index.html", error = "true", compiled_file = compiled_file)

if __name__ == "__main__":
  app.run("0.0.0.0", 5500, debug=True)