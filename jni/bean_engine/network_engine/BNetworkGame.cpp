#include "BNetworkGame.h"
#include <sstream>

/**
 * \brief Constructor
 *
 * \param maximumPlayerAllowed : The maximum number of player allowed in the game
 */
BNetworkGame::BNetworkGame(unsigned int maximumPlayerAllowed) :
	_maximumPlayerAllowed(maximumPlayerAllowed),
    _nbrPlayers(0),
	_needPassword(false)
{
}

/**
 * \brief Constructor
 *
 * \param name : Name of the game.
 * \param maximumPlayerAllowed : The maximum number of player allowed in the game
 */
BNetworkGame::BNetworkGame(const std::string& name, unsigned int maximumPlayerAllowed, const std::string& pwd) :
	_name(name),
	_maximumPlayerAllowed(maximumPlayerAllowed)
{
    setPassword(pwd);
}

/**
 * \brief Constructor
 *
 * \param name : Name of the game.
 * \param hostIp : Host's IP adress.
 * \param maximumPlayerAllowed : The maximum number of player allowed in the game
 */
BNetworkGame::BNetworkGame(const std::string& name, const std::string& hostIp, unsigned int maximumPlayerAllowed) :
	_name(name),
	_hostIp(hostIp),
	_maximumPlayerAllowed(maximumPlayerAllowed),
	_needPassword(false)
{
}

/**
 * \brief Destructor
 */
BNetworkGame::~BNetworkGame()
{
}

/**
 * \brief Name getter
 *
 * \return The name of the game
 */
std::string BNetworkGame::getName() const
{
	return _name;
}

std::string BNetworkGame::getAuthor() const
{
    return _name;
}

/**
 * \brief HostIp getter
 *
 * \return String which contain the IP oh the host of the game
 */
std::string BNetworkGame::getHostIp() const
{
	return _hostIp;
}

/**
 * \brief MaximumPlayerAllowed getter
 *
 * \return The maximum number of player authorized to join the game
 */
unsigned int BNetworkGame::getMaximumPlayerAllowed() const
{
	return _maximumPlayerAllowed;
}

unsigned int BNetworkGame::getNbrPlayers() const
{
    return _nbrPlayers;
}

bool BNetworkGame::needPassword() const
{
	return _needPassword;
}

/**
 * \brief Name setter
 *
 * \param name : The name of the game
 */
void BNetworkGame::setName(const std::string& name)
{
	_name = name;
}

void BNetworkGame::setAuthor(const std::string &author)
{
    _author = author;
}

/**
 * \brief HostIp setter
 *
 * \param name : The IP adress of the host
 */
void BNetworkGame::setHostIp(const std::string& ip)
{
	_hostIp = ip;
}

/**
 * \brief MaximumPlayerAllowed setter
 *
 * \param name : The maximum number of player in the game
 */
void BNetworkGame::setMaximumPlayerAllowed(unsigned int value)
{
	_maximumPlayerAllowed = value;
}

void BNetworkGame::setPassword(std::string password)
{
    if (password.length() == 0)
    {
        _needPassword = false;
        _pwd.clear();
    }
    else
    {
        _pwd = password;
        _needPassword = true;
    }
}

std::string BNetworkGame::getPassword() const
{
    if (_needPassword)
        return _pwd;
    else
        return "";
}

BNetworkData& BNetworkGame::operator<<(BNetworkData &data)
{
    fromNetwork(data);
    return data;
}

void BNetworkGame::setNbrPlayer(unsigned int value)
{
    _nbrPlayers = value;
}

BNetworkData& BNetworkGame::operator>>(BNetworkData &data) const
{
    toNetwork(data);
    return data;
}

/**
 * \brief Convert the current instance of the class into a string.
 *
 * This methods converts the current instance of the class into a std::string.
 * It's useful to send data througth the network.
 *
 * \return A string which contain the network data needed of the BNetworkGame class.
 */
void BNetworkGame::toNetwork(BNetworkData& data) const
{
	data["game_name"] = _name;
    data["game_author"] = _author;
    data["game_nbr_player"] = (int)_nbrPlayers;
    data["game_max_players"] = (int)_maximumPlayerAllowed;
    data["game_need_password"] = _needPassword;
}

/**
 * \brief Create a BNetworkGame instance with the data contained in the string.
 *
 * This method allow to load data contained in a string created with the BNetworkGame::toNetwork() method
 * in a BNetworkGame class.
 * Warning : This can be effective only with a string created with BNetworkMessage::toNetwork().
 *
 * \param bin : A string which contain BNetworkGame data.
 *
 * \return An instance of BNetworkGame which contain data stocked into the string.
 */
void BNetworkGame::fromNetwork(BNetworkData& data)
{
	_name = data["game_name"].toString();
    _author = data["game_author"].toString();
    _nbrPlayers = data["game_nbr_player"].toInteger();
    _maximumPlayerAllowed = data["game_max_players"].toInteger();
    _needPassword = data["game_need_password"].toBool();
}

bool BNetworkGame::operator!= (const BNetworkGame& game)
{
	return (_name != game._name ||
		_hostIp != game._hostIp ||
		_maximumPlayerAllowed != game._maximumPlayerAllowed);
}

bool BNetworkGame::operator== (const BNetworkGame& game)
{
	return (_name == game._name &&
		_hostIp == game._hostIp &&
		_maximumPlayerAllowed == game._maximumPlayerAllowed);
}