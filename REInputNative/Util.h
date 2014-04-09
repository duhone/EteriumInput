#pragma once

#include <wbemidl.h>
#include <oleauto.h>
#include <algorithm>

bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );

namespace RE
{
	template<typename Container, typename Value>
	auto find(Container& _container, Value _value) -> decltype(std::find(std::begin(_container), std::end(_container), _value))
	{
		return std::find(std::begin(_container), std::end(_container), _value);
	}

	template<typename Container, typename Callable>
	auto find_if(Container& _container, Callable _func) -> decltype(std::find_if(std::begin(_container), std::end(_container), _func))
	{
		return std::find_if(std::begin(_container), std::end(_container), _func);
	}

	template<typename Container, typename Callable>
	auto remove_if(Container& _container, Callable _func) -> decltype(std::remove_if(std::begin(_container), std::end(_container), _func))
	{
		return std::remove_if(std::begin(_container), std::end(_container), _func);
	}
}
