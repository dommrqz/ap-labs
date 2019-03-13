// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
package main

import (
	"fmt"
	"log"
	"os"
	"flag"

	"gopl.io/ch5/links"
)

// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)
var visited = make(map[string]bool)

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

func crawler(depth int, url string, done chan bool) {

	if depth <= 0 {
		done <- true
		return
	} else {
		visited[url] = true
	}

	links := crawl(url)
	linksSeen := make(chan bool)

	for _, link := range links {
		if !visited[link]{
			go crawler(depth-1, link, linksSeen)
			<-linksSeen
		}
	}
	done <- true

}

func main() {

	if len(os.Args) <= 2 {
		fmt.Printf("Usage: go run crawl3.go -depth=n [some website]")
	}

	depth := flag.Int("depth", 1, "Depth limit")
	flag.Parse()

	url := os.Args[2]

	done := make(chan bool)

	go crawler(*depth, url, done)

	<-done
}

//!-