#pragma once
/* <stompede/Api.h>
 *
 * File taken from  the x0 project.
 *
 * This file is part of the x0 web server project and is released under LGPL-3.
 * http://www.xzero.io/
 *
 * (c) 2013 Christian Parpart <trapni@gmail.com>
 */

#include <stompede/Defines.h>

// exports
#if defined(BUILD_STOMPEDE)
#	define STOMPEDE_API X0_EXPORT
#else
#	define STOMPEDE_API X0_IMPORT
#endif
