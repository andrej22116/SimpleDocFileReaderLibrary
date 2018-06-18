#include "streamsmenager.h"

StreamsMenager::StreamsMenager()
{

}

void StreamsMenager::createStream(uint32_t streamID, const std::string& streamName)
{
    auto iterator = std::find_if(_streamIdAndNamePairs.begin(),
                              _streamIdAndNamePairs.end(),
                              [streamID, &streamName](std::pair<uint32_t, std::string> pair)
    {
        return pair.first == streamID;
    });

    if (iterator != _streamIdAndNamePairs.end())
    {
        iterator->second = streamName;
        if (_mapOfStreamsWithKeyName.find(streamName) != _mapOfStreamsWithKeyName.end())
        {
            _mapOfStreamsWithKeyName.erase(streamName);
        }
    }
    else
    {
        _streamIdAndNamePairs.push_back(std::pair<uint32_t, std::string>(streamID, streamName));
    }

    auto newStream = std::make_shared<std::stringstream>();
    _mapOfStreamsWithKeyID[streamID] = newStream;
    _mapOfStreamsWithKeyName[streamName] = newStream;
}


std::iostream& StreamsMenager::getStream(uint32_t streamID)
{
    auto result = _mapOfStreamsWithKeyID.find(streamID);
    if (result != _mapOfStreamsWithKeyID.end())
    {
        return *(result->second);
    }
    else
    {
        std::stringstream message;
        message << "Stream with id(" << streamID << ") not found!";
        throw std::runtime_error(message.str());
    }
}

std::iostream& StreamsMenager::getStream(const std::string& streamName)
{
    auto result = _mapOfStreamsWithKeyName.find(streamName);
    if (result != _mapOfStreamsWithKeyName.end())
    {
        return *(result->second);
    }
    else
    {
        std::stringstream message;
        message << "Stream with name(" << streamName << ") not found!";
        throw std::runtime_error(message.str());
    }
}


void StreamsMenager::removeStream(uint32_t streamID)
{
    auto iterator = std::find_if(_streamIdAndNamePairs.begin(),
                              _streamIdAndNamePairs.end(),
                              [streamID](std::pair<uint32_t, std::string> pair)
    {
        return pair.first == streamID;
    });
    if (iterator == _streamIdAndNamePairs.end())
    {
        std::stringstream message;
        message << "Stream with id(" << streamID << ") not found!";
        throw std::runtime_error(message.str());
    }
    removeStream(iterator);
}

void StreamsMenager::removeStream(const std::string& streamName)
{
    auto iterator = std::find_if(_streamIdAndNamePairs.begin(),
                              _streamIdAndNamePairs.end(),
                              [&streamName](std::pair<uint32_t, std::string> pair)
    {
        return pair.second == streamName;
    });
    if (iterator == _streamIdAndNamePairs.end())
    {
        std::stringstream message;
        message << "Stream with name(" << streamName << ") not found!";
        throw std::runtime_error(message.str());
    }
    removeStream(iterator);
}

void StreamsMenager::removeStream(std::vector<std::pair<uint32_t, std::string>>::iterator iterator)
{
    _mapOfStreamsWithKeyID.erase(iterator->first);
    _mapOfStreamsWithKeyName.erase(iterator->second);
    _streamIdAndNamePairs.erase(iterator);
}
