 #pragma once

#include <vector>
#include <map>

 namespace myEngine
 {
	 enum KEY_FRAME_TYPE
	 {
		 KEY_FRAME_TYPE_INVALID = 0,
		 KEY_FRAME_TYPE_V1 = 1,
		 KEY_FRAME_TYPE_V2,
		 KEY_FRAME_TYPE_V3,
		 KEY_FRAME_TYPE_V4
	 };

	 enum KEY_FRAME_ACCELERATE
	 {
		 KEY_FRAME_ACCELERATE_LINEAR,
		 KEY_FRAME_ACCELERATE_STEP,
		 KEY_FRAME_ACCELERATE_CATMULLROMSPLINE,
		 KEY_FRAME_ACCELERATE_CUBICSPLINE
	 };

	 class KeyFrame
	 {
	 public:
		 KeyFrame()
		 :time(0.f)
		 ,accType(KEY_FRAME_ACCELERATE_LINEAR)
		 {

		 }
		 KeyFrame(float t, KEY_FRAME_ACCELERATE type)
			 :time(t)
			 , accType(type)
		 {

		 }
		 ~KeyFrame() { }
		 KEY_FRAME_ACCELERATE accType;
		 float time;
		 std::vector<float> values;
	 };

	 class AnimChain
	 {
	 public:
		 AnimChain():m_type(KEY_FRAME_TYPE_INVALID) { }
		 ~AnimChain() { }

		 myEngine::KEY_FRAME_TYPE getType() const;

		 void setType(myEngine::KEY_FRAME_TYPE val);

	 private:
		 KEY_FRAME_TYPE m_type;
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

	 private:
		 std::vector<KeyFrame> m_keyFrames;
	 };

 }