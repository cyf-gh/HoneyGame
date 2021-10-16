#pragma once

// https://charon-cheung.github.io/2019/06/18/C++/Boost/Boost%E6%95%99%E7%A8%8B%EF%BC%88%E5%9B%9B%EF%BC%89%E8%AF%BB%E5%86%99JSON/#%E4%BF%AE%E6%94%B9%E6%9F%90%E4%B8%AAJSON%E5%80%BC
// �������� https://www.cnblogs.com/fnlingnzb-learner/p/10334988.html

#include <string>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <nameof.hpp>
#include "Log.h"
#include "Type.h"
#include "Math.hpp"

#define HG_MARSHAL_START						out.StartObject();
#define HG_MARSHAL_END							out.EndObject();

/// \brief ���ü���
/// \note 
/// * ��strNameΪ���ַ���ʱ���������ü���
#define HG_MARSHAL_SETKEY						if ( strcmp( strName, "" ) != 0 ) { out.Key( strName ); }

/// \brief ���濪ʼ�����������л�
/// \note 
/// * �������������л�ʱ���ö������ "ObjName" : { ... } ����ʽ�������� out ��
/// * ���ڽ��� JSON �ַ��������� buffer �� Writer writer
#define HG_MARSHAL_OBJECT_START					rapidjson::StringBuffer buffer; rapidjson::Writer<rapidjson::StringBuffer> writer(buffer); writer.StartObject()
#define HG_MARSHAL_OBJECT_SETPROP( PROP )		HG::Serialization::Marshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), writer )
#define HG_MARSHAL_OBJECT_END					writer.EndObject(); HG_MARSHAL_SETKEY; out.RawValue( buffer.GetString(), strlen( buffer.GetString() ), rapidjson::kObjectType ); return out;
#define HG_MARSHAL_SETOBJ( PROP )				HG::Serialization::Marshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), out )

#define HG_UNMARSHAL_OBJECT_START				rapidjson::Document d; d.CopyFrom( in, rd.GetAllocator() )
#define HG_UNMARSHAL_OBJECT_END					return t
#define HG_UNMARSHAL_GETOBJ( PROP )				Unmarshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), d[std::string( NAMEOF( PROP ) ).c_str()], rd )

#define HG_DEBUG_UNMARSHAL_OBJECT				for( auto& m : d.GetObject() ) { HG_LOG_INFO( m.name.GetString() ); }

#define HG_MARSHAL_FULLSPEC( T )				template<> HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal<T>( const T& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out )
#define HG_UNMARSHAL_FULLSPEC( T )				template<> HG_INLINE T& Unmarshal( T& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd )

namespace HG {
/// \brief �������л�
/// \note 
/// * ����������Ա���¶ʱ��Ӧ����д�ö������͵� Marshal Unmarshal ȫ�ػ�ģ�庯��
namespace Serialization {

template<typename T>
HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal( const T& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out ) {
	HG_LOG_WARNNING( std::format( "In default Marshal. Node[{}] escaped", strName ).c_str() );
	return out;
}

template<typename T>
HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal( const std::vector<T>& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out ) {
	HG_MARSHAL_SETKEY;
	out.StartArray();
	for( auto& i : t ) {
		Marshal( i, "", out );
	}
	out.EndArray();
	return out;
}

template<typename T>
HG_INLINE T& Unmarshal( T& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_LOG_WARNNING( std::format( "In default Unmarshal. Node[{}] escaped", strName ).c_str() );
	return t;
}

template<typename T>
HG_INLINE T& Unmarshal( std::vector<T>& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_ASSERT( in.IsArray() );
	for( auto &it : in.GetObject() ) {
		T _t();
		Unmarshal( _t, strName, in, rd );
		t.push_back( _t );
	}
	return t;
}

HG_MARSHAL_FULLSPEC( un32 ) {
	HG_MARSHAL_SETKEY;
	out.Int( static_cast< int >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( un32 ) {
	t = static_cast< un32 >( in.GetInt() );
	return t;
}

HG_MARSHAL_FULLSPEC( n32 ) {
	HG_MARSHAL_SETKEY;
	out.Int( static_cast< int >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( n32 ) {
	t = static_cast< n32 >( in.GetInt() );
	return t;
}

HG_MARSHAL_FULLSPEC( float ) {
	HG_MARSHAL_SETKEY;
	out.Double( static_cast< double >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( float ) {
	t = static_cast< float >( in.GetDouble() );
	return t;
}

HG_MARSHAL_FULLSPEC( bool ) {
	HG_MARSHAL_SETKEY;
	out.Bool( t );
	return out;
}

HG_UNMARSHAL_FULLSPEC( bool ) {
	t = ( in.GetBool() );
	return t;
}

HG_MARSHAL_FULLSPEC( double ) {
	HG_MARSHAL_SETKEY;
	out.Double( t );
	return out;
}

HG_UNMARSHAL_FULLSPEC( double ) {
	t = in.GetDouble();
	return t;
}

HG_MARSHAL_FULLSPEC( std::string ) {
	HG_MARSHAL_SETKEY;
	out.String( t.c_str() );
	return out;
}

HG_UNMARSHAL_FULLSPEC( std::string ) {
	t = in.GetString();
	return t;
}


////////////////////////////////////////////////////////////////////////////////////////////////

HG_MARSHAL_FULLSPEC( Math::HGVec2<float> ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.X );
	HG_MARSHAL_OBJECT_SETPROP( t.Y );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGVec2<float> ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.X );
	HG_UNMARSHAL_GETOBJ( t.Y );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( Math::HGSize<un32> ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.W );
	HG_MARSHAL_OBJECT_SETPROP( t.H );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGSize<un32> ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.W );
	HG_UNMARSHAL_GETOBJ( t.H );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( Math::HGPos ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.X );
	HG_MARSHAL_OBJECT_SETPROP( t.Y );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGPos ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.X );
	HG_UNMARSHAL_GETOBJ( t.Y );
	HG_UNMARSHAL_OBJECT_END;
}

}

}