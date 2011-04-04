/**
 *
 * @File : sd2_projects_util.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : Definitions de fonctions/classes generiques communes aux projets.
 *
 **/

#ifndef SD2P_UTIL_HPP_
#define SD2P_UTIL_HPP_

#include <sstream>

SD2P_NAMESPACE_DEB


/************
 * FONCTIONS
 ************/

template <typename T>
std::string ToString(const T &Val)
{
    std::ostringstream oss;
    oss << Val;
    return oss.str();
}


SD2P_NAMESPACE_FIN

#endif /* SD2P_UTIL_HPP_ */
