//
// Copyright (c) 2023-present DeepGrace (complex dot invoke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/deepgrace/arpc
//

#ifndef ARPC_VERSION_HPP
#define ARPC_VERSION_HPP

#define ARPC_STRINGIZE(T) #T

/*
 *   ARPC_VERSION_NUMBER
 *
 *   Identifies the API version of arpc.
 *   This is a simple integer that is incremented by one every
 *   time a set of code changes is merged to the master branch.
 */

#define ARPC_VERSION_NUMBER 1
#define ARPC_VERSION_STRING "arpc/" ARPC_STRINGIZE(ARPC_VERSION_NUMBER)

#endif
