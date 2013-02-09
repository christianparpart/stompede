#pragma once

#include <stompede/Api.h>
#include <string>
#include <list>

namespace stompede {

class Message;

/*! A transaction object, containing an ordered group of messages.
 *
 * \note Onnly Send and Acknowledge messages can be put into a transaction.
 *
 * \see Message
 *
 * \todo Should it know about its creator, the \p Session?
 */
class STOMPEDE_API Transaction {
public:
	explicit Transaction(const std::string& id);
	~Transaction();

	std::string id() const { return id_; }
	const std::list<Message*>& messages() const { return messages_; }
	bool empty() const { return messages_.empty(); }

	void push_back(Message* message);
	void commit();
	void rollback();

private:
	std::string id_;
	std::list<Message*> messages_;
};

} // namespace stompede
