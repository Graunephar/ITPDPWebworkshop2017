from flask import Flask, render_template, request, json
from connection import *

app = Flask(__name__)

firebase_connection = My_connections()
colors = firebase_connection.get_colors()

@app.route('/')
def test():
    colors = firebase_connection.get_colors()
    hello = "Hello world"
    return render_template("test.html", colors = colors, hello = hello)

@app.route('/api/d1mini', methods=['POST'])
def d1mini():
    # Hent data fra d1mini & send til firebase
    content = request.get_data()
    print(content)
    result = firebase_connection.send_data(content)
    return result

@app.route('/api/getrgb', methods=['GET'])
def getrgb():
    # Hent data fra firebase & send til d1mini
    #content = firebase_connection.get_color()
    return "0,100,100"

if __name__ == '__main__':
    app.run(debug=True)
