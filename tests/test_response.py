import requests
import unittest

url = "http://localhost:8080/"

class Test_Server(unittest.TestCase):

    # Server should handle GET, POST, PUT, and DELETE differently for 
    # requests to the same route
    def test_methods(self):
        route = "test_methods"
        status_code = 200
        content_type = 'text/html'

        # Should receive response string GET
        r = requests.get(url+route)
        self.assertEqual(r.text, 'GET')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should receive response string POST
        r = requests.post(url+route)
        self.assertEqual(r.text, 'POST')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should receive response string PUT
        r = requests.put(url+route)
        self.assertEqual(r.text, 'PUT')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should receive response string DELETE
        r = requests.delete(url+route)
        self.assertEqual(r.text, 'DELETE')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

    # Server should send one cookie from route1 and two cookies from route2
    # Has no content so status code should be 204
    def test_cookies(self):
        route1 = "test_one_cookie"
        route2 = "test_two_cookies"
        status_code = 204

        # Should receive one cookie from server
        r = requests.get(url+route1)
        self.assertEqual(r.cookies['cookie_key'], 'cookie_value')
        self.assertEqual(r.status_code, status_code)

        # Should receive two differnt cookies from server
        r = requests.get(url+route2)
        self.assertEqual(r.cookies['cookie_key1'], 'cookie_value1')
        self.assertEqual(r.cookies['cookie_key2'], 'cookie_value2')
        self.assertEqual(r.status_code, status_code)

    # Should send cookies server and server should be able to handle them
    def test_get_cookies(self):
        route = "test_get_cookies"
        status_code = 200

        # Should send two cookies. Server has assert to check that cookies are processed
        cookies = dict(cookie_key1='cookie_value1', cookie_key2='cookie_value2')
        r = requests.get(url+route, cookies=cookies)
        self.assertEqual(r.status_code, status_code)


    # Server should properly redirect
    def test_redirect(self):
        route = "test_redirect"
        content_type = 'text/html'

        # Should send 301 response
        r = requests.get(url+route, allow_redirects=False)
        self.assertEqual(r.status_code, 307)

        # Should send to url path /other
        r = requests.get(url+route)
        self.assertEqual(r.url, url+'other')
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should recieve 200 response from new url
        self.assertEqual(r.status_code, 200)

    # Server should send back a custom HTTP code. For route route1 a 503
    # code should be sent. route2 should send a 503 code with a custom response 
    def test_custom_http_code(self):
        route1 = "test_bad_url"
        route2 = "test_bad_url_custom"
        route3 = "test_bad_url2"
        route4 = "test_bad_url_custom2"
        status_code = 503
        content_type = 'text/html'
        custom_response = 'Custom 503 response'

        # Should send 501 response
        r = requests.get(url+route1)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should send 503 response with custom response message
        r = requests.get(url+route2)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, custom_response)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should send 500 response if user issues nonexistant http code
        r = requests.get(url+route3)
        self.assertEqual(r.status_code, 500)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should send 503 response with custom response message
        r = requests.get(url+route2)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, custom_response)
        self.assertEqual(r.headers['Content-Type'], content_type)

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
        content_type = 'text/html'

        #Should only get get message 'Should send'
        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, 'Should send')
        self.assertEqual(r.headers['Content-Type'], content_type)

    # Server should be able to handle a post with a params in body
    def test_post_with_message(self):
        route = 'post_params'
        status_code = 200
        username = 'zach'
        password = 'pass'
        content_type = 'text/html'

        # Should send a username and password and server should respond with
        # username and password 
        payload = {'username': username, 'password': password}
        r = requests.post(url+route, data=payload)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, (username + ' ' + password))
        self.assertEqual(r.headers['Content-Type'], content_type)

    # Server should be able to handle a get request with params in query string
    def test_get_params(self):
        route = 'get_params'
        status_code = 200
        username = 'zach'
        password = 'pass'
        content_type = 'text/html'

        # Should send get a username and password and server should respond with
        # username and password 
        payload = {'username': username, 'password': password}
        r = requests.get(url+route, params=payload)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, (username + ' ' + password))
        self.assertEqual(r.headers['Content-Type'], content_type)

    # Server should be able to handle a dynamic route so user can extract params
    def test_dynamic_route(self):
        user1 = 'zach'
        user2 = 'Zach Gleicher'
        user3 = 'zgleicher321'
        user_id = 123
        route1 = 'api/' + user1
        route2 = 'api/' + user2
        route3 = 'api/' + user1 + '/' + str(user_id)
        route4 = 'api/' + user3
        status_code = 200
        content_type = 'text/html'

        #Should send name zach as user and respond with name zach
        r = requests.get(url+route1)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, user1)
        self.assertEqual(r.headers['Content-Type'], content_type)

        #Should send name zach as user and respond with name zgleicher123
        r = requests.get(url+route4)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, user3)
        self.assertEqual(r.headers['Content-Type'], content_type)


        #Should send name zach and user_id 123 and respond with zach 123
        r = requests.get(url+route3)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, (user1 + ' ' + str(user_id)))
        self.assertEqual(r.headers['Content-Type'], content_type)

    #Server should be able to send a static html file
    def test_static_html(self):
        route = 'get_static'
        status_code = 200
        content_type = 'text/html'
        html_content = '<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Header</h1>\n\n<p>Paragraph</p>\n\n</body>\n</html>\n'

        # Should get static html page 
        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.text, html_content)
        self.assertEqual(r.headers['Content-Type'], content_type)

    # Server should be able to send json with proper mime type
    def test_json(self):
        route = 'get_json'
        status_code = 200
        content_type = 'application/json'
        content = "{\"message\": \"hello\"}"

        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)
        self.assertEqual(r.text, content)

    # Server should respond differntly to post and get even though registered on same url
    def test_get_or_post(self):
        route = 'get_or_post'
        status_code = 200
        content_type = 'text/html'

        # Should recieve response string GET
        r = requests.get(url+route)
        self.assertEqual(r.text, 'GET')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)

        # Should receive response string POST
        r = requests.post(url+route)
        self.assertEqual(r.text, 'POST')
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)


    # Server should be able to handle lambda expressions
    def test_lambda(self):
        route = 'lambda'
        status_code = 200
        content_type = 'text/html'
        content = "hello lambda"

        # Route lambda should return string "hello lambda"
        r = requests.get(url+route)
        self.assertEqual(r.status_code, status_code)
        self.assertEqual(r.headers['Content-Type'], content_type)
        self.assertEqual(r.text, content)




if __name__ == '__main__':
    unittest.main()