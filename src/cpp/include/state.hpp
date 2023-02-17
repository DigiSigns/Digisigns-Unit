#pragma once
#include <atomic>
#include <vlc/vlc.h>

class 
state_singleton
{
    state_singleton();
    std::atomic_bool interrupt;
    libvlc_media_t *m;
    libvlc_instance_t *inst;
public:

    state_singleton(state_singleton const &old_obj) = delete;

    static state_singleton *const 
    get_instance();

    bool 
    getState() const;

    void 
    setState(bool);

    libvlc_media_t*
    getMedia() const;

    void 
    setMedia(libvlc_media_t*);

    libvlc_instance_t*
    get_vlc_instance();
};
