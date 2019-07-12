 #pragma once

#include <vector>
#include <map>

 namespace myEngine
 {
     enum KEY_CHAIN_TARGET
     {
         KEY_CHAIN_TARGET_INVALID,
         KEY_CHAIN_TARGET_SCALE,
         KEY_CHAIN_TARGET_ROTATE,
         KEY_CHAIN_TARGET_TRANSLATE
     };

     enum KEY_CHAIN_TYPE
     {
         KEY_CHAIN_TYPE_INVALID = 0,
         KEY_CHAIN_TYPE_V1,
         KEY_CHAIN_TYPE_V2,
         KEY_CHAIN_TYPE_V3,
         KEY_CHAIN_TYPE_V4
     };

     enum KEY_CHAIN_ACCELERATE
     {
         KEY_CHAIN_ACCELERATE_LINEAR,
         KEY_CHAIN_ACCELERATE_STEP,
         KEY_CHAIN_ACCELERATE_CATMULLROMSPLINE,
         KEY_CHAIN_ACCELERATE_CUBICSPLINE
     };

     class Node;

     class KeyFrame
     {
     public:
         KeyFrame()
         :time(0.f)
         {

         }
         KeyFrame(float t)
         :time(t)
         {

         }
         ~KeyFrame() { }

         friend  bool operator < (const KeyFrame& k1, const KeyFrame& k2)
         {
             return k1.time < k2.time;
         }

         float time;
         std::vector<float> values;
     };

     class KeyChain
     {
     public:
         KeyChain();
         ~KeyChain() { }

         void tick(float time);

         KEY_CHAIN_TYPE getType() const;
         KEY_CHAIN_ACCELERATE getAccType() const;
         const Node* getTarget() const;
         KEY_CHAIN_TARGET getTargetType() const;

         void setType(KEY_CHAIN_TYPE val);
         void setAccType(KEY_CHAIN_ACCELERATE val);
         void setTarget(Node* val);
         void setTargetType(KEY_CHAIN_TARGET val);

         void addKeyFrame(const KeyFrame& frame);
         void sortKeyFrames();

     private:
         void _update();

     private:
         KEY_CHAIN_TYPE         m_type;
         KEY_CHAIN_ACCELERATE   m_accType;
         std::vector<KeyFrame>  m_keyFrames;
         float                  m_time;
         std::vector<float>     m_values;
         bool                   m_loop;

         //target
         Node*                  m_target;
         KEY_CHAIN_TARGET       m_targetType;

     };

     class Animation
     {
     public:
         Animation()
         :m_speed(1.f)
         {

         }
         ~Animation()
         {

         }

         void tick(int time);

         void addKeyChain(const KeyChain& chain);

     private:
         std::vector<KeyChain> m_keyChains;

         float m_speed;
     };

 }