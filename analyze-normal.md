


# Results



## ab -n 10000 -c 16 "http://127.0.0.1:8989/"

### Service, 32B
> go run ./client_large_resp_server.go -l 32

```
Document Path:          /
Document Length:        61 bytes

Concurrency Level:      16
Time taken for tests:   1.711 seconds
Complete requests:      10000
Failed requests:        9919
   (Connect: 0, Receive: 0, Length: 9919, Exceptions: 0)
Total transferred:      2048956 bytes
HTML transferred:       628956 bytes
Requests per second:    5845.18 [#/sec] (mean)
Time per request:       2.737 [ms] (mean)
Time per request:       0.171 [ms] (mean, across all concurrent requests)
Transfer rate:          1169.58 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.6      1      21
Processing:     1    2   0.7      2      23
Waiting:        1    2   0.7      2      23
Total:          2    3   1.0      3      23

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      3
  90%      3
  95%      3
  98%      3
  99%      4
 100%     23 (longest request)
```

- ContextSwitch
```
120
120
120
120
120
6235
20078
21097
21097 
21097 
```

## ab -n 10000 -c 16 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      16
Time taken for tests:   1.738 seconds
Complete requests:      10000
Failed requests:        9903
   (Connect: 0, Receive: 0, Length: 9903, Exceptions: 0)
Total transferred:      4298909 bytes
HTML transferred:       2868909 bytes
Requests per second:    5754.01 [#/sec] (mean)
Time per request:       2.781 [ms] (mean)
Time per request:       0.174 [ms] (mean, across all concurrent requests)
Transfer rate:          2415.62 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.2      1       3
Processing:     1    2   0.7      2      18
Waiting:        1    2   0.7      2      18
Total:          2    3   0.7      3      18

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      3
  90%      3
  95%      3
  98%      3
  99%      6
 100%     18 (longest request)
```

- ContextSwitch
```
23 
23 
23 
23 
23 
1570
14788
21123
21123 
21123 
```


## ab -n 10000 -c 32 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      32
Time taken for tests:   1.736 seconds
Complete requests:      10000
Failed requests:        9903
   (Connect: 0, Receive: 0, Length: 9903, Exceptions: 0)
Total transferred:      4298909 bytes
HTML transferred:       2868909 bytes
Requests per second:    5759.06 [#/sec] (mean)
Time per request:       5.556 [ms] (mean)
Time per request:       0.174 [ms] (mean, across all concurrent requests)
Transfer rate:          2417.74 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    2   0.4      2       8
Processing:     2    4   0.9      4      16
Waiting:        1    4   0.9      4      16
Total:          4    6   1.1      5      19

Percentage of the requests served within a certain time (ms)
  50%      5
  66%      5
  75%      6
  80%      6
  90%      6
  95%      6
  98%      6
  99%     11
 100%     19 (longest request)
```

- ContextSwitch
```
14 
14 
14 
14 
14 
994
14721
20614
20614 
20614 
```

## ab -n 10000 -c 8 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      8
Time taken for tests:   1.714 seconds
Complete requests:      10000
Failed requests:        9903
   (Connect: 0, Receive: 0, Length: 9903, Exceptions: 0)
Total transferred:      4298909 bytes
HTML transferred:       2868909 bytes
Requests per second:    5835.89 [#/sec] (mean)
Time per request:       1.371 [ms] (mean)
Time per request:       0.171 [ms] (mean, across all concurrent requests)
Transfer rate:          2450.00 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.2      0       7
Processing:     0    1   0.2      1       8
Waiting:        0    1   0.2      1       8
Total:          1    1   0.3      1       8

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      1
  90%      1
  95%      1
  98%      2
  99%      2
 100%      8 (longest request)
```

- ContextSwitch
```
17 
17 
17 
17 
17 
8025
21770
21854
21854 
21854 
```

## ab -n 10000 -c 4 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      4
Time taken for tests:   1.697 seconds
Complete requests:      10000
Failed requests:        9903
   (Connect: 0, Receive: 0, Length: 9903, Exceptions: 0)
Total transferred:      4298909 bytes
HTML transferred:       2868909 bytes
Requests per second:    5892.65 [#/sec] (mean)
Time per request:       0.679 [ms] (mean)
Time per request:       0.170 [ms] (mean, across all concurrent requests)
Transfer rate:          2473.83 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       1
Processing:     0    0   0.2      0      11
Waiting:        0    0   0.2      0      11
Total:          0    1   0.2      1      11

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      1
  90%      1
  95%      1
  98%      1
  99%      1
 100%     11 (longest request)
```

- ContextSwitch
```
19 
19 
19 
19 
19 
12496
23019
23019 
23019 
23019 
```


## ab -n 10000 -c 2 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      2
Time taken for tests:   1.838 seconds
Complete requests:      10000
Failed requests:        9905
   (Connect: 0, Receive: 0, Length: 9905, Exceptions: 0)
Total transferred:      4298915 bytes
HTML transferred:       2868915 bytes
Requests per second:    5440.60 [#/sec] (mean)
Time per request:       0.368 [ms] (mean)
Time per request:       0.184 [ms] (mean, across all concurrent requests)
Transfer rate:          2284.05 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       1
Processing:     0    0   0.1      0       6
Waiting:        0    0   0.1      0       6
Total:          0    0   0.1      0       6

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      0
  99%      0
 100%      6 (longest request)
```

- ContextSwitch
```
11 
11 
11 
11 
11 
14500
25262
25262 
25262 
25262 
```

## ab -n 10000 -c 1 "http://127.0.0.1:8989/"

### Service, 256B
> go run ./client_large_resp_server.go -l 256

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      1
Time taken for tests:   2.535 seconds
Complete requests:      10000
Failed requests:        9903
   (Connect: 0, Receive: 0, Length: 9903, Exceptions: 0)
Total transferred:      4298909 bytes
HTML transferred:       2868909 bytes
Requests per second:    3945.30 [#/sec] (mean)
Time per request:       0.253 [ms] (mean)
Time per request:       0.253 [ms] (mean, across all concurrent requests)
Transfer rate:          1656.30 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       7
Processing:     0    0   0.0      0       1
Waiting:        0    0   0.0      0       1
Total:          0    0   0.1      0       7

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      0
  99%      0
 100%      7 (longest request)
```

- ContextSwitch
```
18 
18 
18 
18 
18 
14068
34823
48855
48855 
48855 
```
