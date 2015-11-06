
#include "ServeurLabyrinthe.h"

ServeurLabyrinthe::ServeurLabyrinthe()
	: m_socketServeur{ INVALID_SOCKET },
   m_socketClient{ INVALID_SOCKET },
   m_Carte{ 0.3, 40, 20 }
{
	const long VERSION_DEMANDEE = MAKEWORD(2, 2);
	WSAData configWinsock;
	if (WSAStartup(VERSION_DEMANDEE, &configWinsock) != 0)
		exit(erreur("Demarrage"));
}

ServeurLabyrinthe::~ServeurLabyrinthe()
{
	fermer_socket(m_socketServeur);
	fermer_socket(m_socketClient);

	WSACleanup();
}

void ServeurLabyrinthe::Initialize(int port)
{
   m_Carte.generer();

	while (est_invalide(m_socketServeur))
	{
		m_socketServeur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	reserver_port(m_socketServeur, port);
}

void ServeurLabyrinthe::Start()
{
	std::string message;
	while (true)
	{
		while(est_invalide(m_socketClient))
		{
			accepter_client();
		}

		message = tolowercase(recevoir());

		switch (message[0])
		{
			case DROITE: 
            m_Carte.tryMoveRight();
				break;
			case GAUCHE:
            m_Carte.tryMoveLeft();
				break;
			case AVANCER:
            //m_Carte.tryMoveAvancer();
				break;
			default:
				break;
		}

		// Envoyer nouveau vecteur.
      std::vector<std::vector<char>> vector = m_Carte.getVec();

      for (int x = 0; x < m_Carte.width(); ++x)
      {
         std::string s(vector[x].begin(), vector[x].end());

         envoyer(s);
      }

      envoyer("@");
	}
}

void ServeurLabyrinthe::fermer_socket(socket_t sck)
{
	if (!est_invalide(sck))
	{
		shutdown(sck, SD_BOTH);
		closesocket(m_socketServeur);
	}
}

void ServeurLabyrinthe::reserver_port(socket_t sck, unsigned short port)
{
	sockaddr_in nom = { 0 };
	nom.sin_family = AF_INET;
	nom.sin_port = htons(port);
	auto resultat = bind(
		sck, reinterpret_cast<const sockaddr*>(&nom), sizeof(sockaddr_in)
		);
	if (est_erreur(resultat))
		exit(erreur("Port ", port, ", obtention"));
	// Créer la file d’attente pour demandes de connexion
	int taille_file = 16; // arbitraire
	if (est_erreur(listen(sck, taille_file)))
		exit(erreur("Creer file d'attente de taille ", taille_file));
}

void ServeurLabyrinthe::accepter_client()
{
	sockaddr_in homologue = { 0 };
	int taille = sizeof(sockaddr_in);

	auto sck = accept(m_socketServeur, reinterpret_cast<sockaddr *>(&homologue), &taille);

	if (!est_invalide(sck))
		m_socketClient = sck;
}

std::string ServeurLabyrinthe::recevoir()
{
	const int CAPACITE = 64; // arbitraire
	char tampon[CAPACITE];
	std::string message_recu;
	bool reception_completee = false;
	do
	{
		auto n = recv(m_socketClient, tampon, CAPACITE, 0);
		if (est_erreur(n))
			exit(erreur("Réception"));
		message_recu.append(tampon + 0, tampon + n);
		if (tampon[n - 1] == DELIMITEUR_FIN)
			reception_completee = true;
	} while (!reception_completee);
	if (!message_recu.empty()) message_recu.pop_back(); // éliminer le délimiteur
	return message_recu;
}

void ServeurLabyrinthe::envoyer(std::string msg)
{
	// Note : Pas besoin de htons vu que les strings contiennent des chars de 8 bits, 
	// qui sont donc toujours dans le bon ordre.

	msg += DELIMITEUR_FIN;
	while (!msg.empty())
	{
		auto plus_recent = send(
			m_socketClient, msg.c_str(), static_cast<int>(msg.size()), 0
			);
		if (est_erreur(plus_recent))
			exit(erreur("Envoi"));
		msg = msg.substr(plus_recent);
	}
}


std::string ServeurLabyrinthe::tolowercase(std::string msg)
{
	std::locale loc;
	std::string temp;
	for (std::string::size_type i = 0; i<msg.length(); ++i)
		temp.push_back(std::tolower(msg[i], loc));
	return temp;
}

#pragma region Traitement erreur
template <class T>
void ServeurLabyrinthe::print(std::ostream &os, T && arg)
{
	os << arg;
}
template <class T, class ... Args>
void ServeurLabyrinthe::print(std::ostream &os, T && arg, Args && ... args)
{
	print(os, std::forward<T>(arg));
	print(os, std::forward<Args>(args)...);
}

template <class ... Args>
int ServeurLabyrinthe::erreur(Args && ... args)
{
	const int code = WSAGetLastError();
	print(std::cerr, std::forward<Args>(args)...);
	std::cerr << "; erreur " << code << std::endl;
	return code;
}

bool ServeurLabyrinthe::est_invalide(socket_t sck)
{
	return sck == INVALID_SOCKET;
}

bool ServeurLabyrinthe::est_erreur(socket_operation_t op)
{
	return op == SOCKET_ERROR;
}
#pragma endregion
