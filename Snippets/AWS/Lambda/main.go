package main

import (
	"context"
	"fmt"
	"net/http"
	"net/url"
	"strings"

	"github.com/aws/aws-lambda-go/lambda"
)

type MyEvent struct {
	Name string `json:"name"`
}

func HandleRequest(ctx context.Context, name MyEvent) (string, error) {

	v := url.Values{}
	v.Set("Sim", "SIM_UNIQUE_ID")
	v.Set("Command", "SIM_COMMAND")

	rb := *strings.NewReader(v.Encode())
	fmt.Println(&rb)

	req, _ := http.NewRequest("POST", "https://wireless.twilio.com/v1/Commands", &rb)
	req.SetBasicAuth("TWILIO_SID", "TWILIO_AUTH")
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Add("Accept", "application/json")
	httpClient := &http.Client{}
	httpClient.Do(req)
	// res, _ := httpClient.Do(req)
	// defer res.Body.Close()
	// body, _ := ioutil.ReadAll(res.Body)
	// fmt.Println(string(body))
	// fmt.Println("Command Sent")
	return fmt.Sprintf("Hello %s!", name.Name), nil
}

func main() {
	lambda.Start(HandleRequest)
}
