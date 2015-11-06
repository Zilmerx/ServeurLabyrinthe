#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <locale>

#include <winsock2.h>
#include <WS2tcpip.h> // pour inet_pton()
#include "Labyrinthe.h"

#pragma comment(lib,"Ws2_32.lib")

using socket_t = SOCKET;
using socket_operation_t = int;

class ServeurLabyrinthe
{
	static const char DELIMITEUR_FIN = '#';
	static const char DROITE = 'd';
	static const char GAUCHE = 'g';
	static const char AVANCER = '1';

	socket_t m_socketServeur;
	socket_t m_socketClient;

   Labyrinthe m_Carte{ 0.3, 40, 20 };
public:

	ServeurLabyrinthe();
	~ServeurLabyrinthe();
	
	void Initialize(int port);

	void Start();

private:

	void fermer_socket(socket_t sck);

	void reserver_port(socket_t sck, unsigned short port);
	void accepter_client();

	std::string recevoir();
	void envoyer(std::string msg);

	std::string tolowercase(std::string msg);

	#pragma region Traitement erreur
	template <class T>
	void print(std::ostream &os, T && arg);

	template <class T, class ... Args>
	void print(std::ostream &os, T && arg, Args && ... args);

	template <class ... Args> 
	int erreur(Args && ... args);

	bool est_invalide(socket_t);
	bool est_erreur(socket_operation_t);
	#pragma endregion
};