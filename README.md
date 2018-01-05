# HTTP server in C

A minimal HTTP server, written in C by [Jan Ahrens](https://github.com/JanAhrens/ipconfig-http-server). I've modified it to work as a general-purpose HTTP server.

The C script is compiled using the Heroku C buildpack. Once you've created an app with the C [buildpack](https://github.com/heroku/heroku-buildpack-c), you can...

## Deploy to Heroku

[![Deploy](https://www.herokucdn.com/deploy/button.png)](https://heroku.com/deploy)

(The requirement for the C buildpack, which is not automatically detected, and my inability to get app.json to work, means you will have to deploy use Heroku CLI. Don't use the button above, which is just for visual interest right now.)