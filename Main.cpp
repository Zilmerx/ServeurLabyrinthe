

#include "ServeurLabyrinthe.h"
#include "ServeurLabyrinthe.h"


int main()
{
	const std::string MESSAGE = "HERPDERP";

	ServeurLabyrinthe serveur{};

	serveur.Initialize(4321);

	serveur.Start();

	system("pause");
}