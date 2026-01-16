package main

import (
	"flag"
	"fmt"
	"log"
	"math/rand/v2"
	"net/http"
	"sync/atomic"
)

var (
	listen_addr = flag.String("a", ":4200", "listen address")
	test_len    = flag.Int("l", 32, "test length.")
)

var requestID int64

func main() {
	flag.Parse()

	http.HandleFunc("/", handler)

	fmt.Printf("Starting HTTP server on %s\n", *listen_addr)
	if err := http.ListenAndServe(*listen_addr, nil); err != nil {
		log.Fatalf("Server failed: %v", err)
	}
}

func handler(w http.ResponseWriter, r *http.Request) {
	id := atomic.AddInt64(&requestID, 1)

	// Generate random numeric string (0-9)
	html_suffix := make([]byte, *test_len)
	for n := 0; n < (*test_len); n++ {
		html_suffix[n] = byte('0' + rand.IntN(10)) // ASCII '0' to '9'
	}

	w.Header().Set("Content-Type", "text/html")

	reshtml := "<h1>Hello World" + fmt.Sprintf("%02d", id) + "</h1><p>" + string(html_suffix) + "</p>"

	fmt.Printf("request %d from %s: %s %s\n", id, r.RemoteAddr, r.Method, r.URL.Path)
	fmt.Printf("response: %s\n", reshtml)

	fmt.Fprint(w, reshtml)
}
