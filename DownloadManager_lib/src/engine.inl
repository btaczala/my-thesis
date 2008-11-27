#include "engine.h"

template<typename T>
IDownload* Engine<T>::spawn() const
{
    return new T(m_Options.get());
}