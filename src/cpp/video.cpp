#include <vlc/vlc.h>
#include <string>
#include "include/video.hpp"
#include "include/state.hpp"

videoPlayerComponents::~videoPlayerComponents()
{
    libvlc_media_release(m);
    libvlc_media_player_release(mp);
    libvlc_release(inst);
}

videoPlayerComponents*
videoInit()
{
    auto const state = state_singleton::get_instance();
    videoPlayerComponents *vpc = new videoPlayerComponents;
    vpc->inst = state->get_vlc_instance();
    return vpc;
}

void
setupVideo(videoPlayerComponents *vpc, const char* filePath)
{
    vpc->m = libvlc_media_new_path(vpc->inst, filePath);
    vpc->mp = libvlc_media_player_new_from_media(vpc->m);
    libvlc_set_fullscreen(vpc->mp, 1);
}

void
changeVideo(videoPlayerComponents *vpc, libvlc_media_t *m)
{
    libvlc_media_release(vpc->m);
    vpc->m = m;
}

void
stopVideo(videoPlayerComponents *vpc)
{
    libvlc_media_player_stop(vpc->mp);
}

void
playVideo(videoPlayerComponents *vpc)
{
    libvlc_media_player_set_media(vpc->mp, vpc->m);
    libvlc_media_player_play(vpc->mp);
}

bool
isVideoPlaying(videoPlayerComponents *vpc)
{
    return libvlc_media_player_is_playing(vpc->mp);
}