


# Results



## ab -n 10000 -c 32 "http://127.0.0.1:8989/"

### Service x32, 32B
> go run ./client_large_resp.go -l 32 -s 32

```
Document Path:          /
Document Length:        61 bytes

Concurrency Level:      32
Time taken for tests:   1.015 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      2030000 bytes
HTML transferred:       610000 bytes
Requests per second:    9856.22 [#/sec] (mean)
Time per request:       3.247 [ms] (mean)
Time per request:       0.101 [ms] (mean, across all concurrent requests)
Transfer rate:          1953.92 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.4      1       6
Processing:     1    2   0.5      2      10
Waiting:        1    2   0.5      2      10
Total:          2    3   0.8      3      12

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      3
  90%      3
  95%      3
  98%      4
  99%      8
 100%     12 (longest request)
```

- ContextSwitch
```
23 
23 
23 
23 
23 
12270
19029
19029 
19029 
19029 
```

## ab -n 10000 -c 32 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      32
Time taken for tests:   1.017 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    9833.88 [#/sec] (mean)
Time per request:       3.254 [ms] (mean)
Time per request:       0.102 [ms] (mean, across all concurrent requests)
Transfer rate:          4110.25 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.4      1      10
Processing:     1    2   0.8      2      13
Waiting:        1    2   0.8      2      13
Total:          2    3   1.0      3      13

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      3
  90%      3
  95%      3
  98%      6
  99%     10
 100%     13 (longest request)
```

- ContextSwitch
```
15 
15 
15 
15 
15 
1755
16407
16407 
16407 
16407 
```

## ab -n 10000 -c 16 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      16
Time taken for tests:   0.977 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    10239.80 [#/sec] (mean)
Time per request:       1.563 [ms] (mean)
Time per request:       0.098 [ms] (mean, across all concurrent requests)
Transfer rate:          4279.91 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.2      1       3
Processing:     1    1   0.3      1       5
Waiting:        1    1   0.2      1       5
Total:          1    2   0.4      1       7
ERROR: The median and mean for the total time are more than twice the standard
       deviation apart. These results are NOT reliable.

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      2
  75%      2
  80%      2
  90%      2
  95%      2
  98%      2
  99%      4
 100%      7 (longest request)
```

- ContextSwitch
```
13 
13 
13 
13 
13 
10442
13775
13775 
13775 
13775 
```

## ab -n 10000 -c 8 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      8
Time taken for tests:   1.002 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    9979.91 [#/sec] (mean)
Time per request:       0.802 [ms] (mean)
Time per request:       0.100 [ms] (mean, across all concurrent requests)
Transfer rate:          4171.29 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       2
Processing:     0    1   0.1      0       3
Waiting:        0    0   0.1      0       3
Total:          1    1   0.2      1       4
ERROR: The median and mean for the processing time are more than twice the standard
       deviation apart. These results are NOT reliable.

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      1
  90%      1
  95%      1
  98%      1
  99%      2
 100%      4 (longest request)

```

- ContextSwitch
```
12 
12 
12 
12 
12 
9893
17378
17378 
17378 
17378 
```

## ab -n 10000 -c 4 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      4
Time taken for tests:   1.018 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    9822.43 [#/sec] (mean)
Time per request:       0.407 [ms] (mean)
Time per request:       0.102 [ms] (mean, across all concurrent requests)
Transfer rate:          4105.47 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       2
Processing:     0    0   0.1      0       2
Waiting:        0    0   0.1      0       2
Total:          0    0   0.1      0       2

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      1
  99%      1
 100%      2 (longest request)
```

- ContextSwitch
```
11 
11 
11 
11 
11 
6273
17467
17467 
17467 
17467 
```


## ab -n 10000 -c 2 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      2
Time taken for tests:   1.151 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    8687.95 [#/sec] (mean)
Time per request:       0.230 [ms] (mean)
Time per request:       0.115 [ms] (mean, across all concurrent requests)
Transfer rate:          3631.29 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       1
Processing:     0    0   0.1      0       3
Waiting:        0    0   0.1      0       3
Total:          0    0   0.1      0       3

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      0
  99%      0
 100%      3 (longest request)
```

- ContextSwitch
```
19 
19 
19 
19 
19 
1254
21756
21756 
21756 
21756 
```

## ab -n 10000 -c 1 "http://127.0.0.1:8989/"

### Service x32, 256B
> go run ./client_large_resp.go -l 256 -s 32

```
Document Path:          /
Document Length:        285 bytes

Concurrency Level:      1
Time taken for tests:   1.811 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      4280000 bytes
HTML transferred:       2850000 bytes
Requests per second:    5521.68 [#/sec] (mean)
Time per request:       0.181 [ms] (mean)
Time per request:       0.181 [ms] (mean, across all concurrent requests)
Transfer rate:          2307.89 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       1
Processing:     0    0   0.0      0       4
Waiting:        0    0   0.0      0       4
Total:          0    0   0.1      0       4

Percentage of the requests served within a certain time (ms)
  50%      0
  66%      0
  75%      0
  80%      0
  90%      0
  95%      0
  98%      0
  99%      0
 100%      4 (longest request)
```

- ContextSwitch
```
1  
1  
1  
1  
1  
8577
31767
38823
38823 
38823 
```



