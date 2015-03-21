/** Utility header
 */

#ifndef reu__reu_h_
#define reu__reu_h_

#include "_header.h"
#include "macro.h"
#include "diagram.h"
#include "RandMt.h"

namespace reu { // namespace reu ///////////////////////////////////////////////

inline bool isOdd(int n)
{
	return n % 2 == 1;
}

inline bool isEven(int n)
{
	return n % 2 == 0;
}


namespace vector { // namespace reu::vector ///////////////////////////////////

template <class T>
bool collectCopy(std::vector<T>* lst1_p, const std::vector<T>& lst2)
{
	for (auto item : lst2) {
		lst1_p->push_back(item);
	}

	return true;
}
	
template <class T>
inline std::vector<T>* makeCopy(std::vector<T>* lst_p) // FIXME
{
	auto result_p = new std::vector<T>();

	for (auto item : *lst_p) {
		result_p->push_back(item);
	}

	return result_p;
}

/** Return index of item.
 *  @return -1: fail
 */
template <class T>
int position(const std::vector<T>& list, T item)
{
	int i = 0;

	for (auto element : list) {
		if (item == element) {
			return i;
		}

		i++;
	}

	return -1;
}

template <class T>
int positionIf(const std::vector<T>& list, std::function<bool(const T&)> fnPred)
{
	int i = 0;

	for (auto element : list) {
		if (fnPred(element)) {
			return i;
		}

		i++;
	}

	return -1;
}

template <class T>
bool every(const std::vector<T>& list, std::function<bool(const T&)> fnPred)
{
	for (auto element : list) {
		if (!fnPred(element)) {
			return false;
		}
	}

	return true;
}

/** Count item.
 */
template <class T>
int count(const std::vector<T>& list, T item)
{
	int i = 0;

	for (auto element : list) {
		if (item == element) {
			i++;
		}
	}

	return i;
}

template <class T>
bool isMember(const std::vector<T>& list, T item)
{
	for (auto element : list) {
		if (item == element) {
			return true;
		}
	}

	return false;
}

template <class T>
T max(const std::vector<T> &list)
{
	T max = list[0];

	for (int i = 1; i < list.size(); i++) {
		if (max < list.at(i)) {
			max = list.at(i);
		}
	}

	return max;
}

template <class T>
T min(const std::vector<T> &list)
{
	T min = list[0];

	for (int i = 1; i < list.size(); i++) {
		if (min > list.at(i)) {
			min = list.at(i);
		}
	}

	return min;
}

} // end of namespace reu::vector /////////////////////////////////////////////

} // end of namespace reu /////////////////////////////////////////////////////

#endif // reu__reu_h_

