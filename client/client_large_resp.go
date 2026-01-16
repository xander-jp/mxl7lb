package main

import (
	"bytes"
	"encoding/binary"
	"flag"
	"fmt"
	"log"
	"math/rand"
	"net"
	"time"
)

var (
	service_count = flag.Int("s", 32, "service count.")
	test_len      = flag.Int("l", 32, "test length.")
	acceptor_addr = flag.String("a", "127.0.0.1:9999", "acceptor address")
)

func main() {
	flag.Parse()
	for n := 0; n < *service_count; n++ {
		go startconn(n)
	}
	for {
		time.Sleep(1 * time.Second)
	}
}

type ProxyHeader struct {
	Signature [12]byte
	VerCmd    byte
	Family    byte
	Length    uint16
}

type ProxyCommandIpv4 struct {
	Address uint32
	Port    uint16
}

func startconn(id int) {
	for {
		conn, err := net.Dial("tcp", *acceptor_addr)
		if err != nil {
			log.Printf("%+v", err)
			break
		}
		defer conn.Close()

		fmt.Printf("context : %+v :  %+v\n%+v\n", id, (*test_len), (*acceptor_addr))

		// handshake on proxy protocol
		header_bin := &bytes.Buffer{}
		err = binary.Write(header_bin,
			binary.BigEndian,
			ProxyHeader{
				Signature: [12]byte{0x0d, 0x0a, 0x0d, 0x0a, 0x4d, 0x58, 0x50, 0x52, 0x4f, 0x58, 0x59, 0x0a},
				VerCmd:    0x11,
				Family:    0x11,
				Length:    0x0006})
		if err != nil {
			log.Printf("%+v", err)
			break
		}
		addr_bin := &bytes.Buffer{}
		err = binary.Write(addr_bin, binary.BigEndian, ProxyCommandIpv4{Address: 0xC0A80001, Port: 0x1234})
		if err != nil {
			log.Printf("%+v", err)
			break
		}
		_, err = conn.Write(append(header_bin.Bytes(), addr_bin.Bytes()...))
		if err != nil {
			log.Printf("failed.header - write ... %+v", err)
			break
		}
		fmt.Printf("writen. : %+v\n", append(header_bin.Bytes(), addr_bin.Bytes()...))

		for {
			res := make([]byte, 2048)
			_, err := conn.Read(res)
			if err != nil {
				log.Printf("read ... %+v", err)
				break
			}
			// fmt.Printf(string(res[:n]))

			// Generate random numeric string (0-9)
			html_suffix := make([]byte, *test_len)
			for n := 0; n < (*test_len); n++ {
				html_suffix[n] = byte('0' + rand.Intn(10)) // ASCII '0' to '9'
			}

			reshtml := "HTTP/1.1 200 OK\r\n" +
				"Content-Type: text/html\r\n" +
				"Content-Length: " + fmt.Sprintf("%d", (*test_len)+22+7) + "\r\n\r\n" +
				"<h1>Hello World" + fmt.Sprintf("%02d", id) + "</h1><p>" + string(html_suffix) + "</p>"
			// conn.SetWriteDeadline(time.Now().Add(30 * time.Second))
			_, err2 := conn.Write([]byte(reshtml))
			if err2 != nil {
				log.Printf("failed.write ... %+v", err2)
				break
			}
			fmt.Printf("writen : %+v\n", string(reshtml))
		}
	}
}
