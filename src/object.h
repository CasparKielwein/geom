/*
 * point.h
 *
 *  Created on: Feb 5, 2017
 *      Author: ckielwein
 */

#ifndef SRC_OBJECT_H_
#define SRC_OBJECT_H_

#include "config.h"

namespace geom
{

template<class vector_t, class meta_t>
struct object: meta_t {
	using annotation = meta_t;
	using vector_type = vector_t;

	object() = default;
	~object() = default;

	object(const object& o) = default;
	object(object&& o) = default;

	object& operator=(const object&) = default;
	object& operator=(object&&) = default;

	object(vector_type p, annotation m) :
			meta_t { std::move(m) }, point { std::move(p) } {
	}

	bool operator==(const object& o) const {
		return meta_t::operator==(o) && point == o.point;
	}

	bool operator!=(const object& o) const {
		return !(*this == o);
	}

	vector_type point;
};

template<class V, class M>
auto operator*(const geom::Transformf m, object<V,M> o)
{
	o = m*o;
	return o;
}

} // namespace geom

#endif /* SRC_OBJECT_H_ */
