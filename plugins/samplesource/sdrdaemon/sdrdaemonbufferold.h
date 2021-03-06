///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef PLUGINS_SAMPLESOURCE_SDRDAEMON_SDRDAEMONBUFFEROLD_H_
#define PLUGINS_SAMPLESOURCE_SDRDAEMON_SDRDAEMONBUFFEROLD_H_

#include <stdint.h>
#include <cstring>
#include <cstddef>
#include <lz4.h>
#include "../../../sdrbase/dsp/samplesinkfifo.h"
#include "util/CRC64.h"

class SDRdaemonBufferOld
{
public:
#pragma pack(push, 1)
	struct MetaData
	{
        // critical data
		uint32_t m_centerFrequency;   //!< center frequency in kHz
		uint32_t m_sampleRate;        //!< sample rate in Hz
		uint8_t  m_sampleBytes;       //!< MSB(4): indicators, LSB(4) number of bytes per sample
		uint8_t  m_sampleBits;        //!< number of effective bits per sample
		uint16_t m_blockSize;         //!< payload size
		uint32_t m_nbSamples;         //!< number of samples in a hardware block
        // end of critical data
		uint16_t m_nbBlocks;          //!< number of hardware blocks in the frame
		uint32_t m_nbBytes;           //!< total number of bytes in the frame
		uint32_t m_tv_sec;            //!< seconds of timestamp at start time of frame processing
		uint32_t m_tv_usec;           //!< microseconds of timestamp at start time of frame processing
		uint64_t m_crc;               //!< 64 bit CRC of the above

		bool operator==(const MetaData& rhs)
		{
		    return (memcmp((const void *) this, (const void *) &rhs, 20) == 0); // Only the 20 first bytes are relevant (critical)
		}

		void init()
		{
			memset((void *) this, 0, sizeof(MetaData));
		}

		void operator=(const MetaData& rhs)
		{
			memcpy((void *) this, (const void *) &rhs, sizeof(MetaData));
		}
	};
#pragma pack(pop)

	SDRdaemonBufferOld(uint32_t rateDivider);
	~SDRdaemonBufferOld();
	bool readMeta(char *array, uint32_t length);  //!< Attempt to read meta. Returns true if meta block
	void writeData(char *array, uint32_t length); //!< Write data into buffer.
	uint8_t *readDataChunk();                     //!< Read a chunk of data from buffer
	const MetaData& getCurrentMeta() const { return m_currentMeta; }
	uint32_t getSampleRateStream() const { return m_sampleRateStream; }
	uint32_t getSampleRate() const { return m_sampleRate; }
	void updateBlockCounts(uint32_t nbBytesReceived);
	bool isSync() const { return m_sync; }
	bool isSyncLocked() const { return m_syncLock; }
	uint32_t getFrameSize() const { return m_frameSize; }
	bool isLz4Compressed() const { return m_lz4; }
	float getCompressionRatio() const { return (m_frameSize > 0 ? (float) m_lz4InSize / (float) m_frameSize : 1.0); }
	uint32_t getLz4DataCRCOK() const { return m_nbLastLz4CRCOK; }
	uint32_t getLz4SuccessfulDecodes() const { return m_nbLastLz4SuccessfulDecodes; }
	void setAutoFollowRate(bool autoFollowRate) { m_autoFollowRate = autoFollowRate; }

	static const int m_udpPayloadSize;
	static const int m_sampleSize;
	static const int m_iqSampleSize;

private:
	void updateLZ4Sizes(uint32_t frameSize);
	void writeDataLZ4(const char *array, uint32_t length);
	void writeToRawBufferLZ4();
	void writeToRawBufferUncompressed(const char *array, uint32_t length);
	void updateBufferSize(uint32_t sampleRate);
    void printMeta(MetaData *metaData);

	uint32_t m_rateDivider;  //!< Number of times per seconds the samples are fetched
	bool m_sync;             //!< Meta data acquired
	bool m_syncLock;         //!< Meta data expected (Stream synchronized)
	bool m_lz4;              //!< Stream is compressed with LZ4
	MetaData m_currentMeta;  //!< Stored current meta data
	CRC64 m_crc64;           //!< CRC64 calculator

	uint32_t m_inCount;      //!< Current position of uncompressed input
    uint8_t *m_lz4InBuffer;  //!< Buffer for LZ4 compressed input
    uint32_t m_lz4InCount;   //!< Current position in LZ4 input buffer
    uint32_t m_lz4InSize;    //!< Size in bytes of the LZ4 input data
    uint8_t *m_lz4OutBuffer; //!< Buffer for LZ4 uncompressed output
    uint32_t m_frameSize;    //!< Size in bytes of one uncompressed frame
    uint32_t m_nbLz4Decodes;
    uint32_t m_nbLz4SuccessfulDecodes;
    uint32_t m_nbLz4CRCOK;
    uint32_t m_nbLastLz4SuccessfulDecodes;
    uint32_t m_nbLastLz4CRCOK;
    uint64_t m_dataCRC;

    uint32_t m_sampleRateStream; //!< Current sample rate from the stream
    uint32_t m_sampleRate;       //!< Current actual sample rate in Hz
	uint8_t  m_sampleBytes;      //!< Current number of bytes per I or Q sample
	uint8_t  m_sampleBits;       //!< Current number of effective bits per sample

	uint32_t m_writeIndex;   //!< Current write position in the raw samples buffer
	uint32_t m_readChunkIndex; //!< Current read chunk index in the raw samples buffer
	uint32_t m_rawSize;      //!< Size of the raw samples buffer in bytes
    uint8_t *m_rawBuffer;    //!< Buffer for raw samples obtained from UDP (I/Q not in a formal I/Q structure)
    uint32_t m_chunkSize;    //!< Size of a chunk of samples in bytes
    uint32_t m_bytesInBlock; //!< Number of bytes received in the current UDP block
    uint32_t m_nbBlocks;     //!< Number of UDP blocks received in the current frame

    uint32_t m_readCycles;     //!< Count of read cycles over raw buiffer
    uint32_t m_lastWriteIndex; //!< Write index at last skew estimation
    double   m_skewRateSum;
    double   m_skewRate;
    bool     m_autoFollowRate; //!< Aito follow stream sample rate else stick with meta data sample rate
};



#endif /* PLUGINS_SAMPLESOURCE_SDRDAEMON_SDRDAEMONBUFFEROLD_H_ */
