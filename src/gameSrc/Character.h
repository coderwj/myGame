#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Vector3.h"
#include <string>
using namespace myEngine;

namespace myEngine
{
    class Shader;
    class Model;
}

namespace myGame
{
    class Character
    {
    public:
        Character();
        ~Character();
        static Character * Create(std::string modelName);
        void setPosition(Vector3 pos){m_position = pos;}
        Vector3 getPosition(){return m_position;}
        void setOrientation(Vector3 ori){m_orientation = ori;}
        Vector3 getOrientation(){return m_orientation;}
        
        void setModelVisible(bool v) const;
        
        void tick(int delta);
    private:
        Vector3 m_position;//pos in world
        Vector3 m_orientation;// ori in world
        std::string m_modelName;
        
        int m_modelId;
        
        float m_scale;
        float m_theta;
        Vector3 m_rotateVec;
        
        bool init(std::string modelName);
    };
}

#endif //__CHARACTER_H__
