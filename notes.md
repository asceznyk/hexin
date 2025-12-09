# How things work


## HTTP Parsing
Given HTTP methods:
- GET
- POST
- PUT
- DELETE
- PATCH

HTTP headers. Request format:
```
GET / HTTP/1.1
Host: localhost:6969
User-Agent: curl/8.17.0
Accept: */*
```

- We write a function to read request till `\r\n\r\n`. Then we process each line of the request and get location.
- For now we only process `GET` and `HEAD`.
- Write a config file reader to get base location on disk like - `/var/www/html`.



