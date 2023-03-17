package main

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"sync"
	"time"
	"strconv"

	grab "github.com/cavaliergopher/grab/v3"
	_ "github.com/lib/pq"
)

// Constants (no magic values!)
const (
	dumpDirectory             = "./data/video_input_dump"
)

func main() {
	UID := os.Getenv("uid")
	SID := os.Getenv("sid")
	// Getting database connection
	connStr := "user=postgres password=DigiSigns1029 host=db.udlsgnjteywvklskdwvx.supabase.co port=5432 dbname=postgres"
	db, err := sql.Open("postgres", connStr)
	if err != nil {
		log.Fatal(err)
	}

	// Querying for necessary videos.
	//rows, err := db.Query("SELECT bucket_id, name FROM storage.objects WHERE name like 'Geico%' OR name LIKE 'Y2%' ORDER BY name;")
	rows, err := db.Query("SELECT video_url FROM orders WHERE start_date='"+getCurrTime()+"' AND device_id_pk=(SELECT id FROM devices WHERE devices.host_id="+SID+" AND devices.device_id="+UID+");")
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()

	// setting up query result variables
	var obj string
	obj_list := make([]string, 0)

	// collecting query results
	for rows.Next() {
		if err := rows.Scan(&obj); err != nil {
			log.Fatal(err)
		}
		obj_list = append(obj_list, obj)
	}

	// Setting up waitgroup, ensuring main process doesn't end before subprocesses
	var wg sync.WaitGroup
	wg.Add(len(obj_list))
	// Using "Grab" package to concurrently download videos (no time to waste!)
	for _, currObj := range obj_list {
		// Starting anonymous function on separate thread
		go func(obj string) {
			// Decrementing wg upon exit
			defer wg.Done()

			// downloading video
			_, err := grab.Get(dumpDirectory, obj)

			// handling errors
			if err != nil {
				fmt.Fprintf(os.Stderr, "%s\n", err)
			}

			// indicating that subprocess exited normally
			fmt.Fprint(os.Stdout, "Goroutine exited normally.\n")
		}(currObj)
	}

	// Waiting for completion of goroutines
	wg.Wait()

}

func getCurrTime() string {
	t := time.Now()
	s := strconv.FormatInt(int64(t.Year()), 10)
	s += "-"
	s += strconv.FormatInt(int64(t.Month()), 10)
	s += "-"
	s += strconv.FormatInt(int64(t.Day()), 10)
	return s
}
