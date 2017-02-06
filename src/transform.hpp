/*
 * transform.hpp
 *
 *  Created on: Feb 5, 2017
 *      Author: ckielwein
 */

#ifndef GEOM_SRC_TRANSFORM_HPP_
#define GEOM_SRC_TRANSFORM_HPP_

namespace fc
{
namespace geom
{

/**
 * \brief FlexCore Connectable which performs geometric transformations.
 *
 *
 */
template<class matrix_f>
struct transformer
{
	template<class T>
	decltype(auto) operator()(T&& v)
	{
		return m()*v;
	}

	matrix_f m;
};

namespace detail
{
template<class T>
auto transform_impl(T m)
{
	return transformer<T>{m};
}
} //namespace detail

template<class matrix_t>
auto transform(matrix_t m)
{
	return detail::transform_impl(
			[m](){ return m;}
	);
}

template<class matrix_f>
auto transformf(matrix_f m)
{
	return detail::transform_impl(m);
}

} //namespace geom
}// namespace fc


#endif /* GEOM_SRC_TRANSFORM_HPP_ */
