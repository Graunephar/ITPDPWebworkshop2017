from firebase import *
import json

class My_connections(object):

    def __init__(self):
        self.connection = firebase.FirebaseApplication("https://test-511ad.firebaseio.com/", None)



    def send_data(self, data):
        body = {"timestamp":{".sv":"timestamp"},"data":data}
        valueID = self.connection.post("/Colors" , body )#params={'print': 'pretty'})
        # #print(valueID)
        # #print(valueID["name"])
        # #self.connection.put("/Colors/", {valueID["name"]:{".sv":"timestamp"}}, params = {'print':'pretty'})
        return valueID


    def send_user(self, name, age, email):

        user = {"name" : name, "age": age, "email": email}
        valueID = self.connection.post("/Users" , user)#params={'print': 'pretty'})

        # #print(valueID)
        # #print(valueID["name"])
        # #self.connection.put("/Colors/", {valueID["name"]:{".sv":"timestamp"}}, params = {'print':'pretty'})
        return valueID

    #Overrides everything else
    def change_user_name(self, name, id):

        user = {"name" : name}
        valueID = self.connection.put("/Users" , id, user)#params={'print': 'pretty'})

        # #print(valueID)
        # #print(valueID["name"])
        # #self.connection.put("/Colors/", {valueID["name"]:{".sv":"timestamp"}}, params = {'print':'pretty'})
        return valueID

    def get_user(self, id):
        user = self.connection.get('/Users', id)
        print (user)
        return user


    def get_user_name(self, id):
        user = self.get_user(id)
        print (user.get('name'))
