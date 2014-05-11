This is a tiny experiment to write a C program to run on Heroku.
It'll read your IP address from the `X-Forwarded-For` header and return it as JSON.

You can test this program at ipconfig.herokuapp.com:

    curl http://ipconfig.herokuapp.com/

# Known problems

* Heroku does not filter values for the `X-Forwarded-For` header: `curl -s http://ipconfig.herokuapp.com/ -H 'X-Forwarded-For: ", "attack": true, "foo": "bar'`
