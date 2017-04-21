#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

class GameScene{
private:
	static GameScene * gs;

public:
	static GameScene * getInstance(){
		if(gs == NULL)
			gs = new GameScene();
		return gs;
	}

	void destroyInstance(){
		if(gs)
		{
			delete(gs);
			gs = NULL;
		}
	}

	bool init();
};

#endif //__GAMESCENE_H__