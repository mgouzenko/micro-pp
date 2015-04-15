import requests
import unittest

url = "http://localhost:8080/"

class Test_Server(unittest.TestCase):

    # Server should handle GET, POST, PUT, and DELETE differently for 
    # requests to the same route
    def test_1(self):
        route = "test_1"
        r = requests.get(url+route)
        self.assertEqual(r.text, 'GET')
        r = requests.post(url+route)
        self.assertEqual(r.text, 'POST')
        r = requests.put(url+route)
        self.assertEqual(r.text, 'PUT')
        r = requests.delete(url+route)
        self.assertEqual(r.text, 'DELETE')
    


if __name__ == '__main__':
    unittest.main()