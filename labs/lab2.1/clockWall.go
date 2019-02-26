package main

import (
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

func main() {

	value1 := os.Args[1]
	port1 := strings.Split(value1, "=")

	value2 := os.Args[2]
	port2 := strings.Split(value2, "=")

	value3 := os.Args[3]
	port3 := strings.Split(value3, "=")

	conn, err := net.Dial("tcp", port1[1])
	if err != nil {
		log.Fatal(err)
	}
	conn2, err2 := net.Dial("tcp", port2[1])
	if err2 != nil {
		log.Fatal(err2)
	}
	conn3, err3 := net.Dial("tcp", port3[1])
	if err3 != nil {
		log.Fatal(err3)
	}
	defer conn.Close()
	defer conn2.Close()
	defer conn3.Close()
	go mustCopy(os.Stdout, conn3)
	go mustCopy(os.Stdout, conn2)
	mustCopy(os.Stdout, conn)
}