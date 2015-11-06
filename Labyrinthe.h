#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include <vector>
#include <iostream>

#ifdef LABYRINTHE_EXPORTS
#define LABYRINTHE_API __declspec(dllexport)
#else
#define LABYRINTHE_API __declspec(dllimport)
#endif

class Labyrinthe
{
   struct Point2D
   {
      int m_x, m_y;

      Point2D(int x, int y)
         : m_x{ x },
         m_y{ y }
      {
      }
   };

   const double DIFFICULTE;
   Point2D m_JoueurLoc;
   std::vector<std::vector<char>> m_Map;
   bool m_PartieFinie;

public :

   //CTOR
   LABYRINTHE_API Labyrinthe(const double Difficulte, int width, int height);
   //DTOR
   //...

   LABYRINTHE_API int height() const;

   LABYRINTHE_API int width() const;

   LABYRINTHE_API bool is_PartieFinie() const;

   // valeur de 0 à 1. Represente le % de couverture des obstacles sur le terrain ( environ, vu que c'est aléatoire ).
   LABYRINTHE_API void generer();

   LABYRINTHE_API void tryMoveUp();
   LABYRINTHE_API void tryMoveDown();   
   LABYRINTHE_API void tryMoveLeft();
   LABYRINTHE_API void tryMoveRight();

   LABYRINTHE_API void tryMove(int x, int y);

   LABYRINTHE_API bool isPassable(int xpos, int ypos) const;

   LABYRINTHE_API std::vector<std::vector<char>> getVec() const;
   friend LABYRINTHE_API std::ostream& operator<<(std::ostream& os, const Labyrinthe& obj);

private:
   LABYRINTHE_API char getNullChar() const;
   LABYRINTHE_API char getPassableChar() const;
   LABYRINTHE_API char getImpassableChar() const;
   LABYRINTHE_API char getCharacterChar() const;
   LABYRINTHE_API char getSortieChar() const;

   LABYRINTHE_API double randomPercent() const;

   LABYRINTHE_API void setJoueurLoc(Point2D point);
   LABYRINTHE_API void setJoueurLoc(int x, int y);

   LABYRINTHE_API bool validateX(int x);
   LABYRINTHE_API bool validateY(int y);
};

#endif