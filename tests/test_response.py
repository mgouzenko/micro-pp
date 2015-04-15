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

    # Server should properly redirect
    def test_redirect(self):
        route = "test_redirect"

        # Should send 301 response
        r = requests.get(url+route, allow_redirects=False)
        self.assertEqual(r.status_code, 301)

        # Should send to url path /other
        r = requests.get(url+route)
        self.assertEqual(r.url, url+'other')

        # Should recieve 200 response from new url
        self.assertEqual(r.status_code, 200)

    # Server should send back a custom HTTP code. For route route1 a 503
    # code should be sent. route2 should send a 503 code with a custom response 
    def test_custom_http_code(self):
        route1 = "test_bad_url"
        route2 = "test_bad_url_custom"

        # Should send 501 response
        r = requests.get(url+route1)
        self.assertEqual(r.status_code, 503)

        # Should send 501 response with custom response message
        r = requests.get(url+route2)
        self.assertEqual(r.status_code, 503)
        self.assertEqual(r.text, 'Custom 503 response')

    # Server should send back a 404 code if route does not exist
    def test_nonexisting_route(self):
      route = "kdfk?djf.jkf"

      # Should return 404 code
      r = requests.get(url+route)
      self.assertEqual(r.status_code, 404)


if __name__ == '__main__':
    unittest.main()