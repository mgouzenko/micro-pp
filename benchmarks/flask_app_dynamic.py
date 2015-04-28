#!/usr/bin/python
from flask import Flask, render_template, request, session, redirect, url_for

app = Flask(__name__)
@app.route('/', methods=['GET', 'POST']) 
def home():
    fname = request.args.get("fname")
    lname = request.args.get("lname")
    color = request.form.get('color')
    return "Hello %(fname)s %(lname)s, your favorite color is %(color)s" % {'fname':fname, 'lname':lname, 'color':color}  

if __name__ == "__main__":
    app.run("0.0.0.0", port=8080)
