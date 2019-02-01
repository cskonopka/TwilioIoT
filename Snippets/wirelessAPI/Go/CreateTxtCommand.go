// Twilio-Go-ProgrammableWireless-CreateTxtCommand
// This is an example of creating a new txt message with the Twilio Programmable Wireless API & Go
// by Christopher Konopka

package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"strings"
)

// AccountSid/AuthToken
var TwilioAccountSid, TwilioAuthToken = "TWILIO_ACCOUNT_SID", "TWILIO_AUTH_TOKEN"

func main() {
	v := url.Values{}
	v.Set("Sim", "UNIQUE_SIM_NAME")
	v.Set("Command", "It's some kind of ocular device")

	rb := *strings.NewReader(v.Encode())
	fmt.Println(&rb)

	req, _ := http.NewRequest("POST", "https://wireless.twilio.com/v1/Commands", &rb)
	req.SetBasicAuth(TwilioWirelessSID, TwilioWirelessAuth)
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Add("Accept", "application/json")
	httpClient := &http.Client{}
	res, _ := httpClient.Do(req)
	defer res.Body.Close()
	body, _ := ioutil.ReadAll(res.Body)
	fmt.Println(string(body))
	fmt.Println("Command Sent 2 Cellular Device")
}
