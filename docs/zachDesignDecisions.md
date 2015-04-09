#Design Decisions
-------------------------

##Response Object    

###Considerations
- Original server has a reply object that is handled in the `resquest_handler` 
- We want a customizable object that can be handled in the url callback 
- The reply should not be fully customizable. A HTTP response is a highly specific protocol with formatting rules for the header and content that should not be maintined by the user
- Need to give the user access to set headers and content without having direct access to the `reply`
- Build a response object that is safe to modify by the user and safely populates the reply object
- Should be able to populate content with html
- Should be able to safely render a template
- Should be able to put set cookies and sessions in headers
- Should be able to send response codes (eg. 404 error)
- Languages like python and js have ability to change dictionaries into json which is very useful

##Request Object

###Considerations
- Difficult to parse the content

##Notes