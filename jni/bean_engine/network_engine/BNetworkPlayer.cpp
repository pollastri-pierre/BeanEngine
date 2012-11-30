#include "BNetworkPlayer.h"
#include <sstream>

/**
 * \brief Constructor
 *
 * \param name : The name of the player.
 * \param ip : The IP address of the player.
 */
BNetworkPlayer::BNetworkPlayer(const std::string& name, const std::string& ip) :
	_name(name),
	_ip(ip),
	_online(true),
	_ready(false)
{
}

/**
 * \brief Destructor
 */
BNetworkPlayer::~BNetworkPlayer()
{
}


void BNetworkPlayer::setPlayerNumber(unsigned int value)
{
    _playerNumber = value;  
}

unsigned int BNetworkPlayer::playerNumber() const
{
    return _playerNumber;
}


/**
 * \brief Name getter
 *
 * \return A string which contain the name of the player.
 */
std::string BNetworkPlayer::getName() const
{
	return _name;
}

/**
 * \brief Ip getter
 *
 * \return A string which contain the ip of the player.
 */
std::string BNetworkPlayer::getIp() const
{
	return _ip;
}

/**
 * \brief Online getter
 *
 * \return True if the player is online or false.
 */
bool BNetworkPlayer::isOnline() const
{
	return _online;
}

/**
 * \brief Readyness getter
 *
 * \return True if the player is ready or false.
 */
bool BNetworkPlayer::isReady() const
{
	return _ready;
}

/**
 * \brief Name setter
 *
 * \param name : The player's name.
 */
void BNetworkPlayer::setName(const std::string& name)
{
	_name = name;
}

/**
 * \brief Ip setter
 *
 * \param ip : The player's IP.
 */
void BNetworkPlayer::setIp(const std::string& ip)
{
	_ip = ip;
}

/**
 * \brief Online setter
 *
 * \param value : The online state of the player.
 */
void BNetworkPlayer::setOnline(bool value)
{
	_online = value;
}

/**
 * \brief Ready setter.
 *
 * \param value : The readyness state of the player.
 */
void BNetworkPlayer::setReady(bool value)
{
	_ready = value;
}


bool BNetworkPlayer::operator!= (const BNetworkPlayer& p) const
{
	return (_name != p._name || _ip != p._ip);
}

bool BNetworkPlayer::operator== (const BNetworkPlayer& p) const
{
	return (_name == p._name && _ip == p._ip);
}

void BNetworkPlayer::setUdpEndpoint(const boost::asio::ip::udp::endpoint &endpoint)
{
    _endpoint = endpoint;
}

const boost::asio::ip::udp::endpoint& BNetworkPlayer::udpEndpoint() const
{
    return _endpoint;
}

/**
 * \brief Convert the current instance of the class into a string.
 *
 * This methods converts the current instance of the class into a std::string.
 * It's useful to send data througth the network.
 *
 * \return A string which contain the data of the BNetworkPlayer class.
 */
std::string BNetworkPlayer::toNetwork() const
{
	std::ostringstream ost;
	boost::archive::text_oarchive oa(ost, boost::archive::no_header);
    try 
    {
        oa << *this;
    } catch (...) 
    {
        
    }
	return ost.str();
}

/**
 * \brief Create a BNetworkPlayer instance with the data contained in the string.
 *
 * This method allow to load data contained in a string created with the BNetworkPlayer::toNetwork() method
 * in a BNetworkPlayer class.
 * Warning : This can be effective only with a string created with BNetworkPlayer::toNetwork().
 *
 * \param bin : A string which contain BNetworkPlayer data.
 *
 * \return An instance of BNetworkPlayer which contain data stocked into the string.
 */
BNetworkPlayer BNetworkPlayer::fromNetwork(const std::string& bin)
{
	BNetworkPlayer ret;
	std::istringstream ist(bin);
	boost::archive::text_iarchive ia(ist, boost::archive::no_header);
    try
    {
     ia >> ret;   
    } catch (...)
    {
        
    }
	return ret;
}