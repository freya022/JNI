
#ifndef JNI_ARRAY_HPP
#define JNI_ARRAY_HPP

#include "jni.h"
#include <algorithm>

namespace JNI {
	template<class T>
	class Array {
	private:
		JNIEnv* env;
		jboolean copy{};
	public:
		void* array{};
		T jArray;

		Array(JNIEnv* env, T t) : env(env), jArray(t) {
			static_assert(!std::is_base_of_v<T, jarray>, "Not a jarray");

			if constexpr (std::is_same_v<T, jintArray>) {
				array = env->GetIntArrayElements(t, &copy);
			} if constexpr (std::is_same_v<T, jbyteArray>) {
				array = env->GetByteArrayElements(t, &copy);
			}
		}

		~Array() {
			if (copy) {
				if constexpr (std::is_same_v<T, jintArray>) {
					env->ReleaseIntArrayElements(jArray, (jint*) array, JNI_ABORT);
				} else if constexpr (std::is_same_v<T, jbyteArray>) {
					env->ReleaseByteArrayElements(jArray, (jbyte*) array, JNI_ABORT);
				}
			}
		}

		auto arr() {
			if constexpr (std::is_same_v<T, jintArray>) {
				return (jint*) array;
			} else if constexpr (std::is_same_v<T, jbyteArray>) {
				return (jbyte*) array;
			}
		}
	};
}

#endif //JNI_ARRAY_HPP
