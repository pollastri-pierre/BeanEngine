#pragma once
#ifndef BNETWORKGAME_H
#define BNETWORKGAME_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include "BSocket.h"

/**
 * \class BNetworkGame
 * \brief Class which represent a game in the network context of BeanProject.
 *
 * The BNetworkGame class provide informations about a game like the host, the name or it's player capacity.
 */
class BNetworkGame
{
public:
	BNetworkGame(unsigned int maximumPlayerAllowed = 4);
	BNetworkGame(const std::string& name, unsigned int maximumPlayerAllowed = 4, const std::string& pwd = "");
	BNetworkGame(const std::string& name, const std::string& hostIp, unsigned int maximumPlayerAllowed = 4);
	~BNetworkGame();

	std::string getName() const;
    std::string getAuthor() const;
	std::string getHostIp() const;
	unsigned int getMaximumPlayerAllowed() const;
    unsigned int getNbrPlayers() const;
    bool needPassword() const;
    std::string getPassword() const;
	bool isAvailable() const;

	void setName(const std::string& name);
    void setAuthor(const std::string& author);
	void setHostIp(const std::string& ip);
    void setNbrPlayer(unsigned int value);
	void setMaximumPlayerAllowed(unsigned int value);
	void setPassword(std::string password);

	void toNetwork(BNetworkData& data) const;
	void fromNetwork(BNetworkData& data);

	bool operator!= (const BNetworkGame& game);
	bool operator== (const BNetworkGame& game);

    BNetworkData& operator<<(BNetworkData& data);
    BNetworkData& operator>>(BNetworkData& data) const;
    
private:
	std::string		_name;
	std::string		_hostIp;
    std::string     _author;
    unsigned int    _nbrPlayers;
	unsigned int	_maximumPlayerAllowed;
	bool			_needPassword;    
    std::string     _pwd;
    
	friend class boost::serialization::access;
	template<typename A>
	void serialize(A& ar, const unsigned int version)
	{
		ar & _name;
		ar & _hostIp;
		ar & _maximumPlayerAllowed;
		ar & _needPassword;
	};
};

#endif // BNETWORKGAME_H