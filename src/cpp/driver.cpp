#include <iostream>
#include <fstream>
#include <vlc/vlc.h>
#include <thread>
#include <chrono>
#include <vector>
#include "include/branch.hpp"
#include "include/state.hpp"
#include "include/video.hpp"

#define OVERRIDE true

int
main()
{
	const char *videoPath="data/mergedVid.mp4";
	// getting initial state (static atomic bool interrupt is set to false during construction)
	const auto state = state_singleton::get_instance();
	
	/* run initial video delete-download-process cycle once (prepare everything)
	 * if video doesn't already exist */
	std::ifstream videoCheck("./data/mergedVid.mp4");
	if(videoCheck)
		videoCheck.close();
	else
		download_process_videos();

	// spawn background processing loop on second thread (no downtime following this)
	std::thread backgroundThread(backgroundProcessLoop);

	// boot VLC, get first MP4 going
	auto vpc = videoInit();
	setupVideo(vpc, videoPath);
	// infinite loop, check for interrupt following each end of the video (runs until automated shutdown, aka not my problem right now)
	while(true)
	{
		playVideo(vpc);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		while(isVideoPlaying(vpc))
		{
			if(state->getState()) //interrupt found
			{
				changeVideo(vpc, state->getMedia());
				state->setState(false);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	delete vpc;

	return 0;
}

