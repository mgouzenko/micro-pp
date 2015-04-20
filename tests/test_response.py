import requests
import unittest

url = "http://localhost:8080/"

class Test_Server(unittest.TestCase):

    # Server should handle GET, POST, PUT, and DELETE differently for 
    # requests to the same route
    def test_methods(self):
        route = "test_methods"
        status_code = 200

        # Should receive response string GET
        r = requests.get(url+route)
        self.assertEqual(r.text, 'GET')
        self.assertEqual(r.status_code, status_code)

        # Should receive response string POST
        r = requests.post(url+route)
        self.assertEqual(r.text, 'POST')
        self.assertEqual(r.status_code, status_code)

        # Should receive response string PUT
        r = requests.put(url+route)
        self.assertEqual(r.text, 'PUT')
        self.assertEqual(r.status_code, status_code)

        # Should receive response string DELETE
        r = requests.delete(url+route)
        self.assertEqual(r.text, 'DELETE')
        self.assertEqual(r.status_code, status_code)

    # Server should send one cookie from route1 and two cookies from route2
    def test_cookies(self):
        route1 = "test_one_cookie"
        route2 = "test_two_cookies"
        status_code = 200

        # Should receive one cookie from server
        r = requests.get(url+route1)
        self.assertEqual(r.cookies['cookie_key'], 'cookie_value')
        self.assertEqual(r.status_code, status_code)

        # Should receive two differnt cookies from server
        r = requests.get(url+route2)
        self.assertEqual(r.cookies['cookie_key1'], 'cookie_value1')
        self.assertEqual(r.cookies['cookie_key2'], 'cookie_value2')
        self.assertEqual(r.status_code, status_code)

    # Server should properly redirect
    def test_redirect(self):
        route = "test_redirect"

        # Should send 301 response
        r = requests.get(url+route, allow_redirects=False)
        self.assertEqual(r.status_code, 307)

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
        status_code = 503

        # Should send 501 response
        r = requests.get(url+route1)
        self.assertEqual(r.status_code, status_code)

        # Should send 501 response with custom response message
        r = requests.get(url+route2)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, 'Custom 503 response')

    # Server should send back a 404 code if route does not exist
    def test_nonexisting_route(self):
        route = "kdfk?djf.jkf"
        status_code = 404

        # Should return 404 code
        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)


    # Server should send only one message
    def test_one_message(self):
        route = 'two_messages'
        status_code = 200

        #Should only get get message 'Should send'
        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, 'Should send')

    # Server should be able to handle a post with a params in body
    def test_post_with_message(self):
        route = 'post_params'
        status_code = 200
        username = 'zach'
        password = 'pass'

        # Should send a username and password and server should respond with
        # username and password 
        payload = {'username': username, 'password': password}
        r = requests.post(url+route, data=payload)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, (username + ' ' + password))

    # Server should be able to handle a get request with params in query string
    def test_get_params(self):
        route = 'get_params'
        status_code = 200
        username = 'zach'
        password = 'pass'

        # Should send get a username and password and server should respond with
        # username and password 
        payload = {'username': username, 'password': password}
        r = requests.get(url+route, params=payload)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, (username + ' ' + password))

    # Server should be able to handle a dynamic route so user can extract params
    def test_dynamic_route(self):
        user1 = 'zach'
        user2 = 'Zach Gleicher'
        route1 = 'api/' + user1
        route2 = 'api/' + user2
        status_code = 200

        #Should send name zach as user and respond with name zach
        r = requests.get(url+route1)
        print(r.url)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, user1)

        #Should send name Zach Gleicher as user and respond with name Zach Gleicher
        r = requests.get(url+route2)
        print(r.url)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, user2)






if __name__ == '__main__':
    unittest.main()