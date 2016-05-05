/*
 * Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cc7/Platform.h>
#include <cc7/detail/ExceptionsWrapper.h>

namespace cc7
{
namespace error
{

	typedef void (*AssertionHandler)(void * handler_data, const char * file, int line, const char * formatted_string);
	
	struct AssertionHandlerSetup
	{
		AssertionHandler	handler;
		void *				handler_data;
	};
	
#if defined(ENABLE_CC7_ASSERT)

	void					SetAssertionHandler(const AssertionHandlerSetup & new_setup);
	AssertionHandlerSetup	GetAssertionHandler();
	
	// Platform code must implement following method.
	AssertionHandlerSetup	GetDefaultAssertionHandler();
	
#endif // defined(ENABLE_CC7_ASSERT)

} // cc7::error
} // cc7
