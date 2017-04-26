from firebase import *

class My_connections(object):

    def __init__(self):
        self.connection = firebase.FirebaseApplication("https://webtek-workshop.firebaseio.com/", None)

    def get_colors(self):
        result = self.connection.get('/Colors', None)
        return result

    def send_data(self, rgb_value):

        body = {"timestamp":{".sv":"timestamp"},"lightvalue":rgb_value}

        valueID = self.connection.post("/Colors" , body )#params={'print': 'pretty'})
        #print(valueID)
        #print(valueID["name"])
        #self.connection.put("/Colors/", {valueID["name"]:{".sv":"timestamp"}}, params = {'print':'pretty'})
        return valueID
