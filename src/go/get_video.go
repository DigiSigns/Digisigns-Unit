package main

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"sync"

	grab "github.com/cavaliergopher/grab/v3"
	_ "github.com/lib/pq"
)

// Struct to hold query results
type supabase_item struct {
	bucket string
	name   string
}

// Constants (no magic values!)
const (
	dumpDirectory             = "./data/video_input_dump"
	supabaseBucketUrlTemplate = "https://udlsgnjteywvklskdwvx.supabase.co/storage/v1/object/public/%s/%s"
)

func main() {
	// Getting database connection
	connStr := "user=postgres password=DigiSigns1029 host=db.udlsgnjteywvklskdwvx.supabase.co port=5432 dbname=postgres"
	db, err := sql.Open("postgres", connStr)
	if err != nil {
		log.Fatal(err)
	}

	// Querying for necessary videos.
	//rows, err := db.Query("SELECT bucket_id, name FROM storage.objects WHERE name like 'Geico%' ORDER BY name;")
	rows, err := db.Query("SELECT bucket_id, name FROM storage.objects WHERE name like 'Geico%' OR name LIKE 'Y2%' ORDER BY name;")
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()

	// setting up query result variables
	var obj supabase_item
	obj_list := make([]supabase_item, 0)

	// collecting query results
	for rows.Next() {
		if err := rows.Scan(&obj.bucket, &obj.name); err != nil {
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
		go func(obj supabase_item) {
			// Decrementing wg upon exit
			defer wg.Done()

			// formatting url
			url := fmt.Sprintf(supabaseBucketUrlTemplate, obj.bucket, obj.name)

			// downloading video
			_, err := grab.Get(dumpDirectory, url)

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
