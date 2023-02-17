#pragma once
#include <string>
#include <vlc/vlc.h>

struct
videoPlayerComponents
{
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    videoPlayerComponents() : inst(nullptr), mp(nullptr), m(nullptr) {}
    ~videoPlayerComponents();
};

videoPlayerComponents*
videoInit();

void
setupVideo(videoPlayerComponents*, const char*);

void
changeVideo(videoPlayerComponents*, libvlc_media_t *m);

void
stopVideo(videoPlayerComponents*);

void
playVideo(videoPlayerComponents*);

bool
isVideoPlaying(videoPlayerComponents*);