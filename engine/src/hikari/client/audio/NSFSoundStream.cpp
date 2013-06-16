#include "hikari/client/audio/NSFSoundStream.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <Music_Emu.h>
#include <gme.h>
#include <iostream>

namespace hikari {

    NSFSoundStream::NSFSoundStream(std::size_t bufferSize, unsigned int samplerCount)
        : sf::SoundStream()
        , masterBufferSize(bufferSize)
        , masterBuffer(new short[masterBufferSize])
        , sampleBuffers()
        , sampleEmus()
        , trackInfo(nullptr)
        , samplerCount(1)
        , activeSampler(0)
        , mutex()
    {
        samplerCount = std::max(samplerCount, static_cast<unsigned int>(1));
        this->samplerCount = samplerCount;

        createSampleBuffers();

        this->stop();
    }

    NSFSoundStream::~NSFSoundStream() {
        this->stop();
    }

    bool NSFSoundStream::open(const std::string& fileName) {
        sf::Lock lock(mutex);

        if(!FileSystem::exists(fileName)) {
            return false;
        }

        int length = 0;
        auto fs = FileSystem::openFile(fileName);

        fs->seekg (0, std::ios::end);
        length = static_cast<int>(fs->tellg());
        fs->seekg (0, std::ios::beg);

        std::unique_ptr<char[]> nsfFileBuffer(new char[length]);

        fs->read(nsfFileBuffer.get(), length);

        // To honor the contract of returning false on failure,
        // catch these exceptions and return false instead? Good/bad?
        try {
            gme_type_t file_type = gme_identify_extension(fileName.c_str());
            
            if(!file_type) {
                return false;
            }

            for(int i = 0; i < samplerCount; ++i) {
                auto sampleEmu = std::unique_ptr<Music_Emu>(file_type->new_emu());

                if(!sampleEmu) {
                    return false;
                }

                // Must set sample rate before loading data
                handleError(sampleEmu->set_sample_rate(SAMPLE_RATE));
                handleError(gme_load_data(sampleEmu.get(), nsfFileBuffer.get(), length));

                sampleEmu->start_track(-1);
                sampleEmu->ignore_silence(false);
                sampleEmus.push_back(std::move(sampleEmu));
            }

            trackInfo.reset(new track_info_t());
        } catch(std::runtime_error& ex) {
            HIKARI_LOG(debug) << ex.what();
            return false;
        }

        initialize(2, SAMPLE_RATE);
        //setCurrentTrack(0);

        return true;
    }

    void NSFSoundStream::onSeek(sf::Time timeOffset) {
        sf::Lock lock(mutex);
        handleError(sampleEmus[0]->seek(static_cast<long>(timeOffset.asMilliseconds())));
    }

    bool NSFSoundStream::onGetData(sf::SoundStream::Chunk& Data) {
        sf::Lock lock(mutex);

        auto * mixedBuffer = masterBuffer.get();

        // Generate samples for any playing emulators
        for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
            if(!sampleEmus[bufferIndex]->track_ended()) {
                sampleEmus[bufferIndex]->play(masterBufferSize, sampleBuffers[bufferIndex].get());
            }
        }

        // Mix buffers
        for(int i = 0; i < masterBufferSize; ++i) {
            short mixedValue = 0;

            for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
                // Don't mix buffers that aren't playing.
                if(!sampleEmus[bufferIndex]->track_ended()) {
                    mixedValue += sampleBuffers[bufferIndex].get()[i];
                } else {
                    // Zero-out any buffer that isn't playing.
                    sampleBuffers[bufferIndex].get()[i] = 0;
                }
            }

            mixedBuffer[i] = mixedValue;
        }

        Data.samples     = &masterBuffer[0];
        Data.sampleCount = masterBufferSize;

        // Never stop streaming...
        return true;
    }

    void NSFSoundStream::handleError(const char* str) const {
        if(str) {
            throw std::runtime_error(str);
        }
    }

    void NSFSoundStream::createSampleBuffers() {
        sampleBuffers.clear();

        for(int i = 0; i < samplerCount; ++i) {
            auto sampleBuffer = std::unique_ptr<short[]>(new short[masterBufferSize]);

            std::fill(sampleBuffer.get(), sampleBuffer.get() + masterBufferSize, 0);

            sampleBuffers.push_back(std::move(sampleBuffer));
        }
    }

    long NSFSoundStream::getSampleRate() const {
        return sampleEmus[0]->sample_rate();
    }

    int NSFSoundStream::getCurrentTrack() const {
        return sampleEmus[0]->current_track();
    }

    void NSFSoundStream::setCurrentTrack(int track) {
        sf::Lock lock(mutex);

        if(track >= 0 && track < getTrackCount()) {
            sampleEmus[activeSampler]->start_track(track);
            activeSampler++;
            activeSampler %= samplerCount;
        }
    }

    int NSFSoundStream::getTrackCount() const {
        return sampleEmus[0]->track_count();
    }

    const std::string NSFSoundStream::getTrackName() {
        sf::Lock lock(mutex);
        handleError(sampleEmus[0]->track_info(trackInfo.get()));
        return std::string(trackInfo->song);
    }

    int NSFSoundStream::getVoiceCount() const {
        return sampleEmus[0]->voice_count();
    }

    std::vector<std::string> NSFSoundStream::getVoiceNames() const {
        return std::vector<std::string>(
            sampleEmus[0]->voice_names(), sampleEmus[0]->voice_names() + getVoiceCount());
    }

} // hikari
