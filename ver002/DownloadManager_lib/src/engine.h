#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "downloadengine.h"

class IDownload;

template<typename T>
class Engine : public DownloadEngine
{
    public:
    Engine( const std::string& name ):DownloadEngine(name){};
    IDownload* spawn() const;
};

#include "engine.inl"

#endif //ENGINE_H
