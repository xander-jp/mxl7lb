# mxl7lb

A file descriptor passing mechanism for nginx load balancing.

## Overview

The acceptor process accepts TCP connections and transfers file descriptors to nginx worker processes via Unix domain sockets.

## Structure

```
mxl7lb/
├── acceptor/          # C++ acceptor server
├── client/            # Go test client
├── natlibs/           # IPC library for shared memory communication
└── nginx-1.16.0.patch # Patch for nginx 1.16.0
```

## Dependencies

### MACOS
```
brew install libevent2
```

### acceptor

- CMake 3.1+
- libevent 2.x
- pthread
- natipc

### client

- Go 1.x

### natlibs

- CMake 3.1+
- pthread
- rt (Linux)

## Build

### acceptor

```bash
cd acceptor
mkdir build && cd build
cmake ..
make
```

Debug build:

```bash
cmake -DDEBUG=ON ..
make
```

### client

```bash
cd client
go build -o client client.go
```

### natlibs

```bash
cd natlibs
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=..
make
make install
make test
```

## Applying the nginx patch

```bash
tar zxvf ./nginx-1.16.0.tar.gz
cd /path/to/nginx-1.16.0
patch -p1 < /path/to/mxl7lb/nginx-1.16.0.patch
```
### nginx compile/link

https://nginx.org/download/nginx-1.16.0.tar.gz

> brew --prefix pcre

```
./configure --with-debug --prefix=../nginx --with-cc-opt="-I/opt/homebrew/opt/pcre/include -Wno-error=unused-but-set-variable" --with-ld-opt="-L/opt/homebrew/opt/pcre/lib"

make
make install
```

# Debug Run

## nginx

```
./sbin/nginx -c ./conf/nginx.conf
```

### nginx.conf
```
#user  nobody;
worker_processes  1;

events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;

    server {
        listen       8989;
        server_name  localhost;
        error_log  logs/error.log error;
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        location / {
            proxy_pass   http://127.0.0.1:4200;
            access_log on;
        }
    }
}
```

## acceptor
```
./acceptor -i 127.0.0.1 -p 9999 -u /tmp/Xaccepted_socket -v
```

## simulation

```
go run ./client_large_resp.go
```

## acceptor_cli
```
./acceptor_cli add 192.168.0.0 24
```

## License

MIT License
