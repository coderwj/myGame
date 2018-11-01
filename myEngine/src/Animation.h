 #pragma once

#include <vector>
#include <map>

 namespace myEngine
 {
	 enum KEY_CHAIN_TYPE
	 {
		 KEY_CHAIN_TYPE_INVALID = 0,
		 KEY_CHAIN_TYPE_V1 = 1,
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
		 float time;
		 std::vector<float> values;
	 };

	 class KeyChain
	 {
	 public:
		 KeyChain():m_type(KEY_CHAIN_TYPE_INVALID), m_accType(KEY_CHAIN_ACCELERATE_LINEAR) { }
		 ~KeyChain() { }

		 void tick(int time);

		 myEngine::KEY_CHAIN_TYPE getType() const;
		 myEngine::KEY_CHAIN_ACCELERATE getAccType() const;

		 void setType(myEngine::KEY_CHAIN_TYPE val);
		 void setAccType(myEngine::KEY_CHAIN_ACCELERATE val);

		 void addKeyFrame(const KeyFrame& frame);

	 private:
		 KEY_CHAIN_TYPE m_type;
		 KEY_CHAIN_ACCELERATE m_accType;
		 std::vector<KeyFrame> m_keyFrames;
	 };

	 class Animation
	 {
	 public:
		 Animation()
		 {

		 }
		 ~Animation()
		 {

		 }

		 void tick(int time);

		 void addKeyChain(const KeyChain& chain);

	 private:
		 std::vector<KeyChain> m_keyChains;
	 };

 }