/*
 04 - Say hello to the Arduino MKR GSM 1400 with Programmable Wireless and Go
 Christopher Konopka
 Twilio IoT Developer Evangelist
 https://github.com/cskonopka/TwilioIoT/tree/master/Blog/04%20-%20SayHelloArduinoGSM
*/

package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/faiface/beep"
	"github.com/faiface/beep/mp3"
	"github.com/faiface/beep/speaker"
)

func main() {
	http.HandleFunc("/helloworld", helloworld)
	http.ListenAndServe(":9999", nil)
}

func helloworld(w http.ResponseWriter, r *http.Request) {
	if err := r.ParseForm(); err != nil {
		log.Printf("Error parsing form: %s", err)
		return
	}

	pwCommand := r.PostFormValue("Command")
	fmt.Println("incoming Command from Arduino MKR GSM 1400 : ", pwCommand)

	fmt.Println("Playing audio file!")
	f, err := os.Open("helloworld.mp3")
	if err != nil {
		log.Fatal(err)
	}

	s, format, _ := mp3.Decode(f)
	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))
	playing := make(chan struct{})
	speaker.Play(beep.Seq(s, beep.Callback(func() {
		close(playing)
	})))
	<-playing
}
