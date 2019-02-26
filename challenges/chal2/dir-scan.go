package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {

	var files []string
	var m = make(map[string]int)
	m["directories"] = 0
	m["others"] = 0
	m["symlinks"] = 0

	fmt.Printf("Path: %s\n", dir)

    filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
        files = append(files, path)
        return nil
	})
	
    for _, file := range files {
        state, err := os.Lstat(file)
		if(err != nil){
			fmt.Println(err)
		}

		if state.Mode() & os.ModeSymlink != 0{
			m["symlinks"] = m["symlinks"] + 1
		}

		switch mode := state.Mode(); {

		case mode.IsDir():
			m["directories"] = m["directories"] + 1

		case mode.IsRegular():
			m["others"] = m["others"] + 1

		}
	}	

	for key, value := range m{
		fmt.Println(key, ":", value);
	}

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
