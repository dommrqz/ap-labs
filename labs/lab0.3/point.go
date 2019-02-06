// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"math"
	"fmt"
)

type Point struct{ x, y float64 }

func (p Point) getX() float64 {
	return p.x
}

func (p Point) getY() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.getX()-p.getX(), q.getY()-p.getY())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.getX()-p.getX(), q.getY()-p.getY())
}

func printPoint(p Point) {
	fmt.Printf("Point: (%f, %f)", p.getX(), p.getY())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

//!-path

func main(){
	p := Point{x: 1, y: 2}
	printPoint(p)
}