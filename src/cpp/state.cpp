#include "include/state.hpp"
#include <cstring>

state_singleton::state_singleton()
{
    interrupt = false;
    m = nullptr;
    inst = libvlc_new(0, NULL);
}

state_singleton *const 
state_singleton::get_instance()
{
    static state_singleton s;
    return &s;
}

bool 
state_singleton::getState() const
{
    return interrupt;
}

void 
state_singleton::setState(bool newBool)
{
    interrupt = newBool;
}

libvlc_media_t*
state_singleton::getMedia() const
{
    return m;
}

void
state_singleton::setMedia(libvlc_media_t *otherMedia)
{
    libvlc_media_release(m);
    m = otherMedia;
}

libvlc_instance_t*
state_singleton::get_vlc_instance()
{
    return inst;
}
