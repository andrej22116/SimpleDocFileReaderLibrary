#ifndef STREAMSMENAGER_H
#define STREAMSMENAGER_H

#include "stdincludes.h"

class StreamsMenager
{
private:
    std::vector<std::pair<uint32_t, std::string>> _streamIdAndNamePairs;
    std::map<uint32_t, std::shared_ptr<std::stringstream>> _mapOfStreamsWithKeyID;
    std::map<std::string, std::shared_ptr<std::stringstream>> _mapOfStreamsWithKeyName;

public:
    StreamsMenager();

    void createStream(uint32_t streamID, const std::string& streamName);

    std::iostream& getStream(uint32_t streamID);
    std::iostream& getStream(const std::string& streamName);

    void removeStream(uint32_t streamID);
    void removeStream(const std::string& streamName);

    inline const std::vector<std::pair<uint32_t, std::string>>& getStreamsInfo() { return _streamIdAndNamePairs; }

private:
    void removeStream(std::vector<std::pair<uint32_t, std::string>>::iterator iterator);
};

#endif // STREAMSMENAGER_H
