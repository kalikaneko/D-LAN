#include <priv/Chat.h>

using namespace NetworkListener;

#include <Common/LogManager/Builder.h>
#include <priv/UDPListener.h>

/**
  * Create a new Chat object
  *
  * @param udpListener : An udpListener object
  * @author mcuony
  */
::Chat::Chat(UDPListener* NewUdpListener, QSharedPointer<PeerManager::IPeerManager> NewPeerManager) : logger(LogManager::Builder::newLogger("NetworkListener::Chat"))
{

    this->logger->log("Loading ..", LogManager::EndUser);

    this->udpListener = NewUdpListener;

    this->peerManager = NewPeerManager;

    // Listening for new messages.
    Chat::connect(this->udpListener, SIGNAL(newChatMessage(const Protos::Core::ChatMessage&)), this, SLOT(newChatMessage(const Protos::Core::ChatMessage&)));

}

/**
  * Send a chat message
  *
  * @param message : The message to send
  * @return True for success
  * @author mcuony
  */
bool ::Chat::send(const QString& message)
{

    this->logger->log("Message to send: " + message , LogManager::Debug);

    // We put info in our chatMessage Proto.
    Protos::Core::ChatMessage chatMessage;
    chatMessage.set_message(message.toStdString());

    chatMessage.mutable_peerid()->set_hash(this->peerManager->getMyId().data());

    //We serialize the proto to a string
    std::string output;
    chatMessage.SerializeToString(&output);

    // .We broadcast the data.
    return this->udpListener->sendMessage(chatMessagePacket + QString::fromStdString(output));
}

/**
 * Reception of a new chat message form UDPListener
 * We forward the message to our listeners
 *
 * @author mcuony
 */
void ::Chat::newChatMessage(const Protos::Core::ChatMessage& message)
{
    emit newMessage(message);
}
