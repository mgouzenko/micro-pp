#!/usr/bin/python
from flask import Flask, render_template, request, session, redirect, url_for
import os 

app = Flask(__name__, static_folder= os.path.abspath("../static" ), static_url_path="")

print os.path.abspath("../static" )
@app.route('/', methods=['GET', 'POST']) 
def home():
    fname = request.args.get("fname")
    lname = request.args.get("lname")
    color = request.form.get('color')
    return "Hello %(fname)s %(lname)s, your favorite color is %(color)s" % {'fname':fname, 'lname':lname, 'color':color}  

@app.route('/static_test', methods=['GET', 'POST'])
def stat():
    return app.send_static_file('static_test.jpg')

if __name__ == "__main__":
    app.run("0.0.0.0", port=8080)
