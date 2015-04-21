#ifndef INCLUDED_MQF_KV_VECTOR
#define INCLUDED_MQF_KV_VECTOR
#include <vector>
#include <cstdint>

namespace mqf {

	template<typename Key,typename Value>
	struct kv_vector {
		using key_type = Key;
		using value_type = Value;
		using index_type = size_t;

		std::vector<key_type> keys;
		std::vector<value_type> values;

		size_t size() const {
			return keys.size();
		}

		size_t find_index( const key_type& key ) const {
			for(size_t i=0;i<keys.size();++i) {
				if( keys[i] == key ) {
					return i;
				}
			}
			return size_t(-1);
		}

		value_type* find_value( const key_type& key ) {
			for(size_t i=0;i<keys.size();++i) {
				if( keys[i] == key ) {
					return &values[i];
				}
			}
			return nullptr;
		}

		void set( const key_type& key, const value_type& value ) {
			auto p = find_value(key);
			if( p ) {
				*p = value;
				return;
			}
			keys.push_back( key );
			values.push_back( value );
		}

		void remove( const key_type& key ) {
			auto i = find_index(key);
			if( i == size_t(-1) ) {
				return;
			}
			if( i + 1 < keys.size() ) {
				keys[i] = std::move( keys.back() );
				values[i] = std::move( values.back() );
			}
			keys.pop_back();
			values.pop_back();
		}

	};

}

#endif
