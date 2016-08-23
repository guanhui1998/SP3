#ifndef COLLISION_EXPORTS
#define COLLISION_API __declspec(dllexport) 
#else
#define COLLISION_API __declspec(dllimport) 
#endif

#include "Map_Editor.h"
#include "Character.h"

class Collision
{
public:
    Collision();
	~Collision();

	CHARACTER::CCharacter* N_Character;
	Map_Editor* Map;
	//Platform* Platform;

	void CheckCollision();
    void Response(Platform* go);
    void Response(Projectile* go);
    void Init(CHARACTER::CCharacter* C_CHARACTER, Map_Editor* map);
};
