 #pragma once

#include <vector>
#include <map>

 namespace myEngine
 {
	 enum KEY_FRAME_TYPE
	 {
		 KEY_FRAME_INVALID = 0,
		 KEY_FRAME_V1 = 1,
		 KEY_FRAME_V2,
		 KEY_FRAME_V3,
		 KEY_FRAME_V4
	 };

	 class KeyFrame
	 {
	 public:
		 KeyFrame():time(0.f) { }
		 ~KeyFrame() { }
		 float time;
		 std::vector<float> values;
	 };

	 class AnimChain
	 {
	 public:
		 AnimChain():m_type(KEY_FRAME_INVALID) { }
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

	 private:
		 std::vector<KeyFrame> m_keyFrames;
	 };

 }