#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace myEngine
{
	class Singleton{
	private:
		static Singleton * instance;
	
	public:
		static Singleton * getInstance(){
			if(nullptr == instance)
				instance = new Singleton();
			return instance;
		}
		static void destroyInstance(){
			if(instance)
			{
				delete(instance);
				instance = nullptr;
			}
		}
	};
}

#endif //__SINGLETON_H__