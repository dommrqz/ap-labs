// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"time"
	"strings"
	"flag"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct{
	name string
	ip string
}

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	totalClients []user
	current_destination client
	check_one_user = 0
	client_hashes []client
)

func broadcaster(host string, port string) {
	fmt.Printf("irc-server > Server started in %s:%s\n", host, port)
	fmt.Printf("irc-server > Ready for recieving new clients\n")
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			if check_one_user == 0 {
				for cli := range clients {
					add_hash(cli)
					cli <- msg
				}
			} else {
				check_one_user = 0
				for cli := range clients {
					if current_destination == cli {
						cli <- msg
					}
				}
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	var users = ""
	var first_time = 1

	var other_user = ""
	var message_to_user = ""

	var name = ""

	who := conn.RemoteAddr().String()
	ch <- "Your IP is " + who
	fmt.Printf("irc-server > New connected user, waiting for identification\n")

	ch <- "rc-server > Welcome to the simple IRC server. Press enter to send your name."

	input := bufio.NewScanner(conn)
	for input.Scan() {
		input_strings := strings.Split(input.Text(), " ")
		if first_time == 1 {
			totalClients = append(totalClients, user {input_strings[0], who})
			name = input_strings[0]
			messages <- "Your username " + name + " is successfully logged"
			entering <- ch
			input_strings = input_strings[1:]
			first_time = 0
			fmt.Printf("irc-server > %s joined\n", name)
		}
		if input_strings[0] == "/users" {
			users = totalClients[0].name
			for i := 1; i < len(totalClients); i++ {
				users = users + ", " + totalClients[i].name
			}
			ch <- "irc-server > " + users
		} else if input_strings[0] == "/time" {
			t := time.Now()
			ch <- "irc-server > " + t.String()
		} else if input_strings[0] == "/msg" {
			if len(input_strings) == 3 {
				check_one_user = 1
				other_user = input_strings[1]
				for i := 2; i < len(input_strings); i++{
					message_to_user = message_to_user + input_strings[i]
				}
				for i := 0; i < len(totalClients); i++ {
					if totalClients[i].name == other_user {
						current_destination = client_hashes[i]
					}
				}
				messages <- name + ": " + message_to_user
				message_to_user = ""
			} else {
				ch <-"Bad usage, try /msg <username> <message>"
			}
			
		} else if input_strings[0] == "/user" {
			if len(input_strings) == 2 {
				var showed = 0
				for i := 0; i < len(totalClients); i++ {
					if totalClients[i].name == input_strings[1] {
						ch <- "username: " + totalClients[i].name + ", IP: " + totalClients[i].ip
						showed = 1
					}
				}
				if showed == 0 {
					ch <- "User not found"
				}
			} else {
				ch <- "Bad usage, try /user <username>"
			}
		} else {
			messages <- name + ": " + input.Text()
		}
		ch <- name + " > "

	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	fmt.Printf("irc-server > " + name + " left\n")
	for i := 0; i < len(totalClients); i++{
		if totalClients[i].name == name {
			totalClients = remove(totalClients, i)
			client_hashes = remove_hash(client_hashes, i)
		}
	}
	messages <- name + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func remove(s []user, i int) []user {
    s[i] = s[len(s)-1]
    return s[:len(s)-1]
}

func remove_hash(clis []client, i int) []client {
	clis[i] = clis[len(clis)-1]
    return clis[:len(clis)-1]
}

func add_hash(cli client){
	for i := 0; i < len(client_hashes); i++ {
		if (client_hashes[i] == cli){
			return;
		}
	}
	client_hashes = append(client_hashes, cli)
}

//!-handleConn

//!+main
func main() {
	host := flag.String("host", "", "Host name")
	port := flag.String("port", "", "Port for chat")
	flag.Parse()
	listener, err := net.Listen("tcp", *host + ":" + *port)
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster(*host, *port)
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main