package main

import (
	"fmt"
	"log"
	"net"
	"runtime"
	"strconv"
	"strings"
	"time"
)

func main() {
	for n := 0; n < 32; n++ {
		go startconn(n)
	}
	counter := 0
	for {
		time.Sleep(1 * time.Second)

		if counter%30 == 0 {
			fmt.Printf(".... : %+v\n", counter)
		}
		counter = counter + 1
	}
}

func startconn(id int) {
	for {
		conn, err := net.Dial("tcp", "127.0.0.1:9999")
		if err != nil {
			log.Printf("%+v", err)
			break
		}
		defer conn.Close()

		fmt.Printf("context : %+v :  %+v\n", id, goid())

		for {
			res := make([]byte, 2048)
			_, err := conn.Read(res)
			if err != nil {
				log.Printf("read ... %+v", err)
				break
			}
			// fmt.Printf(string(res[:n]))

			reshtml := "HTTP/1.1 200 OK\r\n" +
				"Content-Type: text/html\r\n" +
				"Content-Length: 22\r\n\r\n" +
				"<h1>Hello World" + fmt.Sprintf("%02d", id) + "</h1>"

			// conn.SetWriteDeadline(time.Now().Add(30 * time.Second))
			_, err2 := conn.Write([]byte(reshtml))
			if err2 != nil {
				log.Printf("failed.write ... %+v", err2)
				break
			}
			// fmt.Printf("writen : %+v\n", len)
		}
	}
}

func goid() int {
	var buf [64]byte
	n := runtime.Stack(buf[:], false)
	idField := strings.Fields(strings.TrimPrefix(string(buf[:n]), "goroutine "))[0]
	id, err := strconv.Atoi(idField)
	if err != nil {
		panic(fmt.Sprintf("cannot get goroutine id: %v", err))
	}
	return id
}
