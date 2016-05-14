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

#include <cc7/Platform.h>
#include <map>
#include <vector>
#include <stdexcept>

namespace cc7
{
namespace tests
{	
	class JSONValue
	{
	public:
		
		enum Type
		{
			// not a type
			NaT = 0,

			Object	= 1 << 0,
			Array	= 1 << 1,
			String	= 1 << 2,
			Decimal = 1 << 3,
			Double  = 1 << 4,
			Boolean = 1 << 5,
			Null    = 1 << 6
		};
		
		typedef std::map<std::string, JSONValue> TObject;
		typedef std::vector<JSONValue> TArray;
		typedef std::string TString;
		
		
		JSONValue() : _t(NaT), _decimal(0) {}
		JSONValue(Type t) : _t(t), _decimal(0)
		{
			switch (_t) {
				case String: _string = new TString(); break;
				case Object: _object = new TObject(); break;
				case Array:  _array  = new TArray();  break;
				default:
					break;
			}
		}
		
		explicit JSONValue(bool v)            : _t(Boolean), _boolean(v) {}
		explicit JSONValue(int64_t v)         : _t(Decimal), _decimal(v) {}
		explicit JSONValue(double v)          : _t(Double),  _double(v) {}
		
		// Copy / Move
		
		JSONValue(const JSONValue & o)
		{
			copyFrom(o);
		}
		
		JSONValue(JSONValue && o) : _t(o._t), _decimal(o._decimal)
		{
			o._t = NaT;
		}
		
		JSONValue & operator=(const JSONValue & o)
		{
			if (&o != this) {
				destroy();
				copyFrom(o);
			}
			return *this;
		}
		
		JSONValue & operator=(JSONValue && o)
		{
			_t = o._t;
			_decimal = o._decimal;
			o._t = NaT;
			return *this;
		}

		// Destructor
		
		~JSONValue()
		{
			destroy();
		}
		
		// assign
		
		void assign(bool value)
		{
			destroy();
			_t = Boolean;
			_boolean = value;
		}
		
		void assign(int64_t value)
		{
			destroy();
			_t = Decimal;
			_decimal = value;
		}

		void assign(double value)
		{
			destroy();
			_t = Double;
			_double = value;
		}
		
		void assign(const TString & value)
		{
			destroy();
			_t = String;
			_string = new TString(value);
		}

		void assign(const TObject & value)
		{
			destroy();
			_t = Object;
			_object = new TObject(value);
		}

		void assign(const TArray & value)
		{
			destroy();
			_t = Array;
			_array = new TArray(value);
		}
		
		void assignNull()
		{
			destroy();
			_t = Null;
		}
		
		// casting
		
		const TObject & asObject() const
		{
			castToPtrType(Object);
			return *_object;
		}
		
		TObject & asMutableObject()
		{
			castToPtrType(Object);
			return *_object;
		}
		
		const TArray & asArray() const
		{
			castToPtrType(Array);
			return *_array;
		}
		
		TArray & asMutableArray()
		{
			castToPtrType(Array);
			return *_array;
		}

		const TString & asString() const
		{
			castToPtrType(String);
			return *_string;
		}
		
		TString & asMutableString()
		{
			castToPtrType(String);
			return *_string;
		}

		
		double asDouble() const
		{
			castToType(Double);
			return _double;
		}
		
		int64_t asInteger() const
		{
			castToType(Decimal);
			return _decimal;
		}
		
		bool asBoolean() const
		{
			castToType(Boolean);
			return _boolean;
		}
		
		bool isNull() const
		{
			return _t == Null;
		}
		
		bool isValid() const
		{
			return _t != NaT;
		}
		
		bool isType(Type t) const
		{
			return _t == t;
		}
		
		const JSONValue & objectAtPath(const std::string & path, Type expected_type = NaT) const;
		
	private:
		
		// Private members
		
		Type _t;
		union
		{
			bool		_boolean;
			int64_t		_decimal;
			double		_double;
			TObject *	_object;
			TArray *	_array;
			TString *	_string;
		};
		
		// Private methods
		
		void destroy()
		{
			switch (_t) {
				case Object: delete _object; break;
				case Array:  delete _array; break;
				case String: delete _string; break;
				default:
					break;
			}
			_decimal = 0;
		}
		
		void copyFrom(const JSONValue & o)
		{
			_t = o._t;
			switch (o._t) {
				case String: _string = new TString(*o._string);	break;
				case Object: _object = new TObject(*o._object); break;
				case Array:  _array  = new TArray(*o._array);   break;
				default:
					_decimal = o._decimal;
					break;
			}
		}
		
		inline void castToType(int t) const
		{
			if ((_t & t) == 0) {
				throw std::logic_error("Unable to cast to type");
			}
		}
		
		inline void castToPtrType(int t) const
		{
			castToType(t);
			if (_object == nullptr) {
				throw std::logic_error("Internal pointer is null");
			}
		}
	};
	
} // cc7::tests
} // cc7