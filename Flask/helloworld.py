from flask import Flask, render_template, request
from connection import *

app = Flask(__name__)

firebase_connection = My_connections()


@app.route('/')
def test():
    return render_template("test.html")

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
    return "233,0,0"

if __name__ == '__main__':
    app.run(debug=True)
