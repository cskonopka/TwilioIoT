// Twilio-Go-ProgrammableWireless-GetSimByUniqueName
// This is an example of getting information about a SIM using its
// unique name via the Twilio Programmable Wireless API & Go
// by Christopher Konopka

package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

// AccountSid/AuthToken
var TwilioAccountSid, TwilioAuthToken = "TWILIO_ACCOUNT_SID", "TWILIO_AUTH_TOKEN"

func main() {
	req, _ := http.NewRequest("GET", "https://wireless.twilio.com/v1/Sims/UNIQE_SIM_NAME", nil)
	req.SetBasicAuth(TwilioWirelessSID, TwilioWirelessAuth)
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Add("Accept", "application/json")
	httpClient := &http.Client{}
	res, _ := httpClient.Do(req)
	defer res.Body.Close()
	body, _ := ioutil.ReadAll(res.Body)
	fmt.Println(string(body))
}
