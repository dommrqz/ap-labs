# Lab 2.1
## Jorge Dominic Márquez Muñoz, A00569034

### ClockWall: a client program that listens to the hours of three different timezones at once.
### Compiling nstructions:
	1. go build clock2.go
	2. go build clockWall.go
### Testing code:
    1. TZ=US/Eastern ./clock2 -port 8010 & TZ=Asia/Tokyo ./clock2 -port 8020 & TZ=Europe/London ./clock2 -port 8030 &
	2. ./clockWall NewYork=localhost:8010 Tokyo=localhost:8020 London=localhost:8030