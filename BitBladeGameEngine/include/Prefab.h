/* Prefab.h
*/

#ifndef PREFAB_H 
#define PREFAB_H


class Prefab {
public:
	Prefab( const char* filename );
	~Prefab();

private:
	const char* filename;
};

#endif // PREFAB_H