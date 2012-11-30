#pragma once
#ifndef BNETWORKPLAYER_H
#define BNETWORKPLAYER_H

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include <boost/asio.hpp>

/**
 * \class BNetworkPlayer
 * \brief Class which represent a player in the network context of BeanProject.
 *
 * The BNetworkPlayer class provide informations to identifies a player. 
 * His name, his ip adress, if he is online and ready.
 */
class BNetworkPlayer
{
public:
	BNetworkPlayer(const std::string& name = "Unknown", const std::string& ip = "");
	~BNetworkPlayer();

	std::string getName() const;
	std::string getIp() const;
    unsigned int playerNumber() const;
	bool isOnline() const;
	bool isReady() const;

	void setName(const std::string& name);
	void setIp(const std::string& ip);
	void setOnline(bool value);
	void setReady(bool value);
    void setPlayerNumber(unsigned int value);
    void setUdpEndpoint(const boost::asio::ip::udp::endpoint& endpoint);
    const boost::asio::ip::udp::endpoint& udpEndpoint() const;
    
	bool operator!= (const BNetworkPlayer& p) const;
	bool operator== (const BNetworkPlayer& p) const;


	std::string toNetwork() const;
	static BNetworkPlayer fromNetwork(const std::string& bin);

private:
	std::string                     _name;
	std::string                     _ip;
	bool                            _online;
	bool                            _ready;
    unsigned int                    _playerNumber;
    boost::asio::ip::udp::endpoint  _endpoint;

	friend class boost::serialization::access;
	template<typename A>
	void serialize(A& ar, const unsigned int version)
	{
		ar & _name;
		ar & _ip;
		ar & _online;
		ar & _ready;
        ar & _playerNumber;
	};

};

#endif // BNETWORKPLAYER_H