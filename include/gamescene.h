#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

class GameScene{
private:
	static GameScene * gs;

	Character * mainCharacter;

	void onDestroy();
public:
	static GameScene * getInstance(){
		if(gs == NULL)
			gs = new GameScene();
		return gs;
	}

	void destroyInstance(){
		if(gs)
		{
			gs->onDestroy();
			delete(gs);
			gs = NULL;
		}
	}

	bool init();

	void render();
};

#endif //__GAMESCENE_H__