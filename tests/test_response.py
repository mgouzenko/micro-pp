import requests
import unittest

url = "http://localhost:8080/"

class Test_Server(unittest.TestCase):

    # Server should handle GET, POST, PUT, and DELETE differently for 
    # requests to the same route
    def test_methods(self):
        route = "test_methods"

        # Should receive response string GET
        r = requests.get(url+route)
        self.assertEqual(r.text, 'GET')

        # Should receive response string POST
        r = requests.post(url+route)
        self.assertEqual(r.text, 'POST')

        # Should receive response string PUT
        r = requests.put(url+route)
        self.assertEqual(r.text, 'PUT')

        # Should receive response string DELETE
        r = requests.delete(url+route)
        self.assertEqual(r.text, 'DELETE')

    # Server should send one cookie from route1 and two cookies from route2
    def test_cookies(self):
        route1 = "test_one_cookie"
        route2 = "test_two_cookies"

        # Should receive one cookie from server
        r = requests.get(url+route1)
        self.assertEqual(r.cookies['cookie_key'], 'cookie_value')

        # Should receive two differnt cookies from server
        r = requests.get(url+route2)
        self.assertEqual(r.cookies['cookie_key1'], 'cookie_value1')
        self.assertEqual(r.cookies['cookie_key2'], 'cookie_value2')


        

if __name__ == '__main__':
    unittest.main()