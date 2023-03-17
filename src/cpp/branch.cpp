#include "include/branch.hpp"
#include "include/state.hpp"
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <vlc/vlc.h>


void
backgroundProcessLoop()
{
    std::string const pathToNewVideo = "data/mergedVid.mp4";
    auto const state = state_singleton::get_instance();
    

    std::this_thread::sleep_for(std::chrono::seconds(4));
    system("pacmd suspend 1");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("pacmd suspend 0");
    while(true)
    {
		system("sudo ip link set wlan0 down");
        std::this_thread::sleep_for(std::chrono::hours(1));
		system("sudo ip link set wlan0 up");
        download_process_videos();
        state->setMedia(libvlc_media_new_path(state->get_vlc_instance(), pathToNewVideo.c_str()));
        state->setState(true);
    }
}

std::string const dumpDirectory = "./data/video_input_dump";
std::string const mp4Directory  = "./data/converted_mp4_files";
std::string const goProgramName = "get_videos";
std::string const pythonScript  = "runPythonScript.sh";

void
download_process_videos()
{
    system(std::string("rm -f " + dumpDirectory + "/*").c_str());
    system(std::string("rm -f " + mp4Directory + "/*").c_str());
    system(std::string("./" + goProgramName).c_str());
    system(std::string("./" + pythonScript).c_str());
}
