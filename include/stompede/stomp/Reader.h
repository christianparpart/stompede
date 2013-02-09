#pragma once

#include <stompede/Api.h>
#include <stompede/Buffer.h>
#include <stompede/stomp/Command.h>
#include <cstdint>

namespace stompede {
namespace stomp {

class STOMPEDE_API Reader {
private:
	enum State {
		SYNTAX_ERROR,
		COMMAND_START,
		COMMAND,
		COMMAND_LF,
		HEADER_NAME_START,
		HEADER_NAME,
		HEADER_VALUE_START,
		HEADER_VALUE,
		HEADER_VALUE_LF,
		HEADER_END_LF,
		BODY_START,
		BODY,
		FRAME_END,
		FRAME_END_EOL,
		FRAME_END_EOL_LF,
	};

	State state_;       //!< current parser state
	size_t bodySize_;   //!< size in bytes left of body to be parsed
	size_t tokenStart_; //!< offset to start of currently parsed token
	BufferRef name_;    //!< name of currently processed header
	Command command_;   //!< command ID of the identified command

public:
	Reader() : state_(COMMAND_START), bodySize_(0), tokenStart_(), name_(), command_() {}

	void reset();
	size_t parse(const Buffer& buffer, size_t offset);

	virtual bool onCommand(Command command);
	virtual bool onHeader(const BufferRef& name, const BufferRef& value);
	virtual bool onBodyChunk(const BufferRef& chunk);
	virtual bool onFrameEnd();

private:
	Command lookupCommand(const BufferRef& value) const;
	bool isBodyAllowed() const;
};

} // namespace stomp
} // namespace stompede
