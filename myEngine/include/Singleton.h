#ifndef __SINGLETON_H__
#define __SINGLETON_H__

class Singleton{
private:
	static Singleton * instance;

public:
	static Singleton * getInstance(){
		if(instance == NULL)
			instance = new Singleton();
		return instance;
	}
	/*virtual*/ void DestroyInstance(){
		if(instance)
		{
			delete(instance);
			instance = NULL;
		}
	}
};

#endif //__SINGLETON_H__