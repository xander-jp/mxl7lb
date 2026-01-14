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

```
 ./configure --with-debug --prefix=../nginx --with-cc-opt="-I/opt/homebrew/opt/pcre/include -Wno-error=unused-but-set-variable" --with-ld-opt="-L/opt/homebrew/opt/pcre/lib"

make
make install
```

## License

MIT License
