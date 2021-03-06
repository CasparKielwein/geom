/*
 * geom.h
 *
 *  Created on: Sep 24, 2016
 *      Author: ckielwein
 */

#ifndef GEOM_SRC_COLLECTION_H_
#define GEOM_SRC_COLLECTION_H_

#include "config.h"
#include "object.h"

namespace fc
{
namespace geom
{

template<class >
class collection;

/**
 * \brief Proxy of actual geom::object for reference access in geom::collection::iterators
 *
 * \tparam T instantiation of geom::object point_reference proxies.
 *
 * stores index and pointer to geom::collection
 * This allows it to refer to the point and metadata of the geom::object
 * point_reference serves as a proxy to.
 */
template<class T>
struct point_reference {
	friend class collection<T> ;

	point_reference(collection<T>* cont, typename collection<T>::size_type index) :
			index { index }, access { cont } {
	}

	point_reference() = delete;
	~point_reference() = default;
	point_reference(point_reference&&) = default;
	point_reference(const point_reference&) = default;
	point_reference& operator=(const point_reference&) = default;
	point_reference& operator=(point_reference&&) = default;

	point_reference& operator=(const T& value) {
		point() = value.point;
		meta() = static_cast<const typename T::annotation&>(value);
		return *this;
	}

	point_reference& operator=(T&& value) {
		point() = value.point;
		meta() = static_cast<typename T::annotation&&>(value);
		return *this;
	}

	operator T() const {
		return T { point(), meta() };
	}

	void swap(point_reference<T>& o) {
		using std::swap;
		swap(point(), o.point());
		swap(meta(), o.meta());
	}

	bool operator==(const point_reference& o) const {
		return point() == o.point() && meta() == o.meta();
	}

	bool operator!=(const point_reference& o) const {
		return !(*this == o);
	}

	bool operator==(const T& o) const {
		return T { point(), meta() } == o;
	}

	bool operator!=(const T& o) const {
		return !(*this == o);
	}

private:
	auto& point() {
		return access->point_matrix[index];
	}
	auto& meta() {
		return access->annotations[index];
	}

	const auto& point() const {
		return access->point_matrix[index];
	}
	const auto& meta() const {
		return access->annotations[index];
	}

	friend class collection<T> ;
	typename collection<T>::size_type index;
	collection<T>* access;
};

template<class T>
void swap(point_reference<T>& l, point_reference<T>& r) {
	l.swap(r);
}

/**
 * \Container class for geom objects with cache friendly storage
 *
 * Collection stores geom objects in a "struct of arrays" manner.
 * This allows geometric transformations to operate on the 3D Vectors
 * without any effect of the metadata on the cache.
 *
 * \tparam T type of object stores in collection, an instantiation of geom::object
 *
 * \invariant point_matrix.size() == annotations.size()
 */
template<class T>
class collection {
public:
	using value_type = T;
	/// Type of vector stored (vector3d, Vector2f etc.)
	using vector_type = typename T::vector_type;
	/// Type of meta data in the stored objects
	using annotation = typename T::annotation;
	using difference_type = T*; //todo
	using size_type = size_t;

	/**
	 * \brief random access iterator over collection.
	 *
	 * Dereferencing iterator returns a point_reference to the
	 * corresponding parts of the object in the collection.
	 */
	class iterator {
	public:
		using difference_type = collection::difference_type;
		using value_type = typename collection::value_type;
		///Note the reference is point_reference proxy and not value_type&.
		using reference = point_reference<T>;
		using pointer = point_reference<T>*;
		using size_type = typename collection::size_type;
		using iterator_category = std::random_access_iterator_tag;

		iterator() = default;
		iterator(const iterator&) = default;
		iterator(iterator&&) = default;

		iterator(size_type index, collection* access) :
				index { index }, access { access } {
		}

		~iterator() = default;

		iterator& operator=(const iterator&) = default;
		iterator& operator=(iterator&&) = default;
		bool operator==(const iterator& o) const {
			return index == o.index && access == o.access;
		}
		bool operator!=(const iterator& o) const {
			return index != o.index || access != o.access;;
		}
		bool operator<(const iterator& o) const {
			assert(access == o.access);
			return index < o.index;
		}
		bool operator>(const iterator& o) const {
			assert(access == o.access);
			return index > o.index;
		}
		bool operator<=(const iterator& o) const {
			assert(access == o.access);
			return index <= o.index;
		}
		bool operator>=(const iterator& o) const {
			assert(access == o.access);
			return index >= o.index;
		}

		iterator& operator++() {
			++index;
			return *this;
		}
		iterator operator++(int) {
			return iterator { index++ };
		}
		iterator& operator--() {
			--index;
			return *this;
		}
		iterator operator--(int) {
			return iterator { index-- };
		}
		iterator& operator+=(size_type p) {
			index += p;
			return *this;
		}
		iterator operator+(size_type p) const {
			return iterator { index + p };
		}
		iterator& operator-=(size_type p) {
			index -= p;
			return *this;
		}
		iterator operator-(size_type p) const {
			return iterator { index - p };
		}
		difference_type operator-(iterator p) const {
			return difference_type { index - p.index };
		}

		reference operator*() {
			return reference { access, index };
		}
		pointer operator->() const;
	private:
		size_type index = 0;
		collection* access = nullptr;
	};
	class const_iterator {
	public:
		using difference_type = collection::difference_type;
		using value_type = typename collection::value_type;
		using const_reference = const point_reference<T>;
		using const_pointer = const point_reference<T>*;
		using size_type = typename collection::size_type;
		using iterator_category = std::random_access_iterator_tag;

		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator(const_iterator&&) = default;
		const_iterator(const iterator& o) {
			index = o.index;
		}
		const_iterator(iterator&& o) {
			index = o.index;
		}

		const_iterator(size_type index, const collection* access) :
				index { index }, access { access } {

		}

		~const_iterator() = default;

		const_iterator& operator=(const const_iterator&) = default;
		bool operator==(const const_iterator& o) const {
			return index == o.index && access == o.access;
		}
		bool operator!=(const const_iterator& o) const {
			return index != o.index || access != o.access;
		}
		bool operator<(const const_iterator& o) const {
			assert(access == o.access);
			return index < o.index;
		}
		bool operator>(const const_iterator& o) const {
			assert(access == o.access);
			return index > o.index;
		}
		bool operator<=(const const_iterator& o) const {
			assert(access == o.access);
			return index <= o.index;
		}
		bool operator>=(const const_iterator& o) const {
			assert(access == o.access);
			return index >= o.index;
		}

		const_iterator& operator++() {
			++index;
			return *this;
		}
		const_iterator operator++(int) {
			return const_iterator { index++ };
		}
		const_iterator& operator--() {
			--index;
			return *this;
		}
		const_iterator operator--(int) {
			return const_iterator { index-- };
		}
		const_iterator& operator+=(size_type p) {
			index += p;
			return *this;
		}
		const_iterator operator+(size_type p) const {
			return iterator { index + p };
		}
		const_iterator& operator-=(size_type p) {
			index -= p;
			return *this;
		}
		const_iterator operator-(size_type p) const {
			return iterator { index - p };
		}
		difference_type operator-(const_iterator p) const {
			return difference_type { index - p.index };
		}

		const_reference operator*() {
			return const_reference { access, index };
		}
		const_pointer operator->() const;
	private:
		size_type index = 0;
		const collection* access = nullptr;
	};

	typedef std::reverse_iterator<iterator> reverse_iterator; //optional
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

	collection() = default;
	~collection() = default;

	collection(const collection&) = default;
	collection(collection&&) = default;

	/// Constructor taking an initalizer_list of geom::object.
	collection(std::initializer_list<T> o) :
			point_matrix(o.size()), annotations ( o.size() ) {
		int index { 0 };

		//extract geometric data and meta data from object and store it.
		for (auto&& x : o) {
			point_matrix[index] = x.point;
			annotations[index] = static_cast<typename T::annotation>(x);
			++index;
		}
	}

	///Construct collection from a range of geom::object.
	template<class iterator_t>
	collection(iterator_t begin, iterator_t end) :
			point_matrix(end - begin), annotations ( end - begin ) {
		int index { 0 };

		//extract geometric data and meta data from object and store it.
		for (auto i = begin; i != end; ++i) {
			point_matrix[index] = i->point;
			annotations[index] = static_cast<typename T::annotation>(*i);
			++index;
		}
	}

	///Construct collection with default initialized members and @p size.
	explicit collection(size_type size) :
			point_matrix(size), annotations( size ) {
	}

	collection& operator=(const collection&) = default;
	collection& operator=(collection&&) = default;

	const_iterator cbegin() const noexcept {
		return const_iterator { 0, this };
	}
	const_iterator cend() const noexcept {
		return const_iterator { size(), this };
	}

	const_iterator begin() const noexcept {
		return cbegin();
	}
	const_iterator end() const noexcept {
		return cend();
	}

	iterator begin() noexcept {
		return iterator { 0, this };
	}
	iterator end() noexcept {
		return iterator { size(), this };
	}

	size_type size() const noexcept {
		return annotations.size();
	}
	bool empty() const noexcept {
		return annotations.empty();
	}

	const auto& points() const noexcept {
		return point_matrix;
	}
	auto& points() noexcept {
		return point_matrix;
	}

private:
	friend class point_reference<T> ;

	std::vector<vector_type> point_matrix;
	std::vector<annotation> annotations;
};

} // namespace geom
} // namespace fc

#endif /* GEOM_SRC_COLLECTION_H_ */
