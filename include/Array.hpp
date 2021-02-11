
#ifndef JNI_ARRAY_HPP
#define JNI_ARRAY_HPP

#include "jni.h"
#include <algorithm>
#include "Helper.hpp"

namespace JNI {
	template<class T>
	class Array {
	private:
		jboolean copy{};
	public:
		void *array{};
		T jArray;

		explicit Array(T t) : jArray(t) {
			static_assert(!std::is_base_of_v<T, jarray>, "Not a jarray");

			JNIEnv *env = GetEnv();
			if constexpr (std::is_same_v<T, jintArray>) {
				array = env->GetIntArrayElements(t, &copy);
			} else if constexpr (std::is_same_v<T, jbyteArray>) {
				array = env->GetByteArrayElements(t, &copy);
			}
		}

		~Array() {
			if (copy) {
				JNIEnv* env = GetEnv();
				if constexpr (std::is_same_v<T, jintArray>) {
					env->ReleaseIntArrayElements(jArray, (jint *) array, JNI_ABORT);
				} else if constexpr (std::is_same_v<T, jbyteArray>) {
					env->ReleaseByteArrayElements(jArray, (jbyte *) array, JNI_ABORT);
				}
			}
		}

		auto arr() {
			if constexpr (std::is_same_v<T, jintArray>) {
				return (jint *) array;
			} else if constexpr (std::is_same_v<T, jbyteArray>) {
				return (jbyte *) array;
			}
		}

		jsize length() const noexcept {
			return GetEnv()->GetArrayLength(jArray);
		}
	};
}

#endif //JNI_ARRAY_HPP
