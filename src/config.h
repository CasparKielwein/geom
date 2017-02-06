/*
 * config.hpp
 *
 *  Created on: Feb 5, 2017
 *      Author: ckielwein
 */

#ifndef GEOM_SRC_CONFIG_HPP_
#define GEOM_SRC_CONFIG_HPP_

//This header contains the definitions of the matrix and vector types used.
//Changing the types here allows to configure geom to use different numeric kernels
//The current default is using the Eigen Library.

#include <eigen3/Eigen/Eigen>

namespace fc
{
namespace geom
{


// Definition of Vector types
using Vector2d = Eigen::Vector2d;
using Vector2f = Eigen::Vector2f;
using Vector3d = Eigen::Vector3d;
using Vector3f = Eigen::Vector3f;
using Vector4d = Eigen::Vector4d;
using Vector4f = Eigen::Vector4f;

// Definition of Transform Matrix types
using Transformf = Eigen::Affine3f;
using Transformd = Eigen::Affine3d;

} // namespace geom
} // namespace fc

#endif /* GEOM_SRC_CONFIG_HPP_ */
