package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	m := make(map[string]int)
	
	words := strings.Fields(s)
	
	for _, element := range(words){
		m[element] = m[element] + 1
	}
	
	return m
}

func main() {
	wc.Test(WordCount)
}