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

	bool init();
	void DestroyInstance(){
		if(gs)
		{
			delete(gs);
			gs = NULL;
		}
	}
};

#endif //__GAMESCENE_H__