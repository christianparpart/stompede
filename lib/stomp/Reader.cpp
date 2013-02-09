#include <stompede/stomp/Reader.h>

namespace stompede {
namespace stomp {

void Reader::reset()
{
	state_ = COMMAND_START;
	bodySize_ = 0;
	tokenStart_ = 0;
	command_ = Command::INVALID;
}

size_t Reader::parse(const Buffer& buffer, size_t offset)
{
	const int CR = 0x0A;
	const int LF = 0x0D;

	const char* i = buffer.data() + offset;
	const char* e = buffer.data() + buffer.size();
	size_t nparsed = 0;

	while (i != e) {
		switch (state_) {
			case COMMAND_START:
				tokenStart_ = offset + nparsed;
				state_ = COMMAND;
				++nparsed;
				++i;
				break;
			case COMMAND:
				if (*i == CR) {
					++nparsed;
					++i;
					state_ = COMMAND_LF;
				} else if (*i == LF) {
					state_ = COMMAND_LF;
				} else {
					++nparsed;
					++i;
				}
				break;
			case COMMAND_LF:
				if (*i == LF) {
					if (!onCommand(command_ = lookupCommand(buffer.ref(tokenStart_, offset + nparsed - tokenStart_))))
						goto done;
					++nparsed;
					++i;
					state_ = HEADER_NAME_START;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case HEADER_NAME_START:
				if (*i == CR) {
					state_ = HEADER_END_LF;
					++nparsed;
					++i;
				} else if (*i == LF) {
					state_ = BODY_START;
					++nparsed;
					++i;
				} else {
					tokenStart_ = offset + nparsed;
					state_ = HEADER_NAME;
					++nparsed;
					++i;
				}
				break;
			case HEADER_NAME:
				if (*i == ':') {
					name_ = buffer.ref(tokenStart_, offset + nparsed - tokenStart_);
					state_ = HEADER_VALUE_START;
					++nparsed;
					++i;
				} else {
					++nparsed;
					++i;
				}
				break;
			case HEADER_VALUE_START:
				if (*i != CR && *i != LF) {
					state_ = HEADER_VALUE;
					++nparsed;
					++i;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case HEADER_VALUE:
				if (*i == CR) {
					state_ = HEADER_VALUE_LF;
					++nparsed;
					++i;
				} else if (*i == LF) {
					state_ = HEADER_NAME_START;
					++nparsed;
					++i;
				}
				break;
			case HEADER_VALUE_LF:
				if (*i == LF) {
					BufferRef value(buffer.ref(tokenStart_, offset + nparsed - tokenStart_));

					if (name_ == "content-length" && bodySize_ == 0)
						bodySize_ = value.toInt();

					if (!onHeader(name_, value))
						goto done;

					state_ = HEADER_NAME_START;
					++nparsed;
					++i;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case HEADER_END_LF:
				if (*i == LF) {
					state_ = BODY_START;
					++nparsed;
					++i;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case BODY_START: // maybe expect a body, maybe end of frame
				if (*i == 0) {
					state_ = FRAME_END;
				} else if (!isBodyAllowed()) {
					fprintf(stderr, "Body found but frame is not allowed to carry content.\n");
					state_ = SYNTAX_ERROR;
				} else {
					state_ = BODY;
					++nparsed;
					++i;
				}
				break;
			case BODY:
				if (*i != 0) {
					++nparsed;
					++i;

					if (bodySize_) {
						--bodySize_;

						if (--bodySize_ == 0) {
							state_ = FRAME_END;
						}
					}
				} else {
					state_ = FRAME_END;
				}
				break;
			case FRAME_END: // expect message end, expect NUL
				if (*i == 0) {
					state_ = FRAME_END_EOL;
					++nparsed;
					++i;

					if (!onFrameEnd()) {
						goto done;
					}
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case FRAME_END_EOL: // beyond NUL, expect [CR] LF
				if (*i == CR) {
					state_ = FRAME_END_EOL_LF;
					++nparsed;
					++i;
				} else if (*i == LF) {
					++nparsed;
					++i;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case FRAME_END_EOL_LF: // beyond NUL, expect LF
				if (*i == LF) {
					state_ = FRAME_END_EOL;
					++nparsed;
					++i;
				} else {
					state_ = SYNTAX_ERROR;
				}
				break;
			case SYNTAX_ERROR:
				fprintf(stderr, "Syntax error occured at offset %zu\n", offset + nparsed);
				goto done;
		}
	}

done:
	return nparsed;
}

Command Reader::lookupCommand(const BufferRef& value) const
{
	if (value.empty())
		return Command::INVALID;

	switch (value[0]) {
		case 'S':
			if (value == "SEND")
				return Command::SEND;
			if (value == "SUBSCRIBE")
				return Command::SUBSCRIBE;
			if (value == "STOMP")
				return Command::CONNECT; // STOMP is an alias to CONNECT
			break;
		case 'U':
			if (value == "UNSUBSCRIBE")
				return Command::UNSUBSCRIBE;
			break;
		case 'B':
			if (value == "BEGIN")
				return Command::BEGIN;
			break;
		case 'C':
			if (value == "COMMIT")
				return Command::COMMIT;
			if (value == "CONNECT")
				return Command::CONNECT;
			if (value == "CONNECTED")
				return Command::CONNECTED;
			break;
		case 'A':
			if (value == "ABORT")
				return Command::ABORT;
			if (value == "ACK")
				return Command::ACK;
			break;
		case 'N':
			if (value == "NACK")
				return Command::NACK;
			break;
		case 'D':
			if (value == "DISCONNECT")
				return Command::DISCONNECT;
			break;
		case 'M':
			if (value == "MESSAGE")
				return Command::MESSAGE;
			break;
		case 'R':
			if (value == "RECEIPT")
				return Command::RECEIPT;
			break;
		case 'E':
			if (value == "ERROR")
				return Command::ERROR;
			break;
		default:
			break;
	}
	// unknown / unsupported command
	return Command::INVALID;
}

bool Reader::isBodyAllowed() const
{
	switch (command_) {
		case Command::SEND:
		case Command::MESSAGE:
		case Command::ERROR:
			return true;
		default:
			return false;
	}
}

bool Reader::onCommand(Command command)
{
	return true;
}

bool Reader::onHeader(const BufferRef& name, const BufferRef& value)
{
	return true;
}

bool Reader::onBodyChunk(const BufferRef& chunk)
{
	return true;
}

bool Reader::onFrameEnd()
{
	return true;
}

} // namespace stomp
} // namespace stompede
