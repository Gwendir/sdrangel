#ifndef INCLUDE_SAMPLESINK_H
#define INCLUDE_SAMPLESINK_H

#include <QObject>
#include "dsptypes.h"
#include "util/export.h"
#include "util/messagequeue.h"

class Message;

class SDRANGEL_API SampleSink : public QObject {
	Q_OBJECT
public:
	SampleSink();
	virtual ~SampleSink();

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void feed(const SampleVector::const_iterator& begin, const SampleVector::const_iterator& end, bool positiveOnly) = 0;
	virtual bool handleMessage(const Message& cmd) = 0; //!< Processing of a message. Returns true if message has actually been processed

	MessageQueue *getInputMessageQueue() { return &m_inputMessageQueue; } //!< Get the queue for asynchronous inbound communication
	MessageQueue *getOutputMessageQueue() { return &m_outputMessageQueue; } //!< Get the queue for asynchronous outbound communication

protected:
	MessageQueue m_inputMessageQueue; //!< Queue for asynchronous inbound communication
	MessageQueue m_outputMessageQueue; //!< Queue for asynchronous outbound communication

protected slots:
	void handleInputMessages();
};

#endif // INCLUDE_SAMPLESINK_H