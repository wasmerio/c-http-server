This is a tiny experiment to write a C program to run on Heroku.
It'll read your IP address from the `X-Forwarded-For` header and return it as JSON.

You can test this program at ipconfig.herokuapp.com:

    curl http://ipconfig.herokuapp.com/

I summarized my experiences in [a blog post](http://blog.jan-ahrens.eu/2014/06/17/heroku-with-c.html).

# Known problems

* Heroku does not filter values for the `X-Forwarded-For` header: `curl -s http://ipconfig.herokuapp.com/ -H 'X-Forwarded-For: ", "attack": true, "foo": "bar'` (see [#1](https://github.com/JanAhrens/ipconfig-http-server/issues/1)).
