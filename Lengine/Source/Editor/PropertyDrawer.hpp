#pragma once
#include "Utility/Delegate.hpp"
#if IS_EDITOR
#include <string>
#include <imgui/imgui.h>
#include <Editor/Editor.hpp>

struct PropertyDrawerContext
{
	bool Readonly = false;
	const char* Name = "";
	const char* Hint = "";
	uint8_t Indent = 0u;
};

struct PropertyDrawer
{
	template < typename _Property >
	static void Draw( _Property& a_Property, PropertyDrawerContext& a_Context );

	template < typename _Iter >
	static void DrawRange( _Iter a_Begin, _Iter a_End, PropertyDrawerContext& a_Context );
};

template < typename _Property >
struct IPropertyDrawer
{
	static void Draw( _Property& a_Property, PropertyDrawerContext& a_Context ) {}
};

template < typename _Property >
void PropertyDrawer::Draw( _Property& a_Property, PropertyDrawerContext& a_Context )
{
	PropertyDrawerContext Context = a_Context;
	++Context.Indent;
	IPropertyDrawer< _Property >::Draw( a_Property, Context );
}

template < typename _Iter >
void PropertyDrawer::DrawRange( _Iter a_Begin, _Iter a_End, PropertyDrawerContext& a_Context )
{
	using PropertyType = typename std::iterator_traits< _Iter >::value_type;

	for ( ; a_Begin != a_End; ++a_Begin )
	{
		IPropertyDrawer< PropertyType >::Draw( *a_Begin, a_Context );
	}
}

#define DECLARE_PROPERTY_DRAWER( ... ) \
	struct IPropertyDrawer< __VA_ARGS__ > \
	{ \
		using PropertyType = __VA_ARGS__; \
		static void Draw( auto& Property, PropertyDrawerContext& Context ); \
	}; \

#define DEFINE_PROPERTY_DRAWER( ... ) \
	void IPropertyDrawer< __VA_ARGS__ >::Draw( auto& Property, PropertyDrawerContext& Context )

#define DEFINE_SCALAR_PROPERTY_DRAWER( Label, Type ) \
	DECLARE_PROPERTY_DRAWER( Type ) \
	DEFINE_PROPERTY_DRAWER( Type ) \
	{ \
		ImGui::InputScalar( Context.Name, ImGuiDataType_##Label, &Property ); \
	}

DEFINE_SCALAR_PROPERTY_DRAWER( S8,  int8_t   );
DEFINE_SCALAR_PROPERTY_DRAWER( S16, int16_t  );
DEFINE_SCALAR_PROPERTY_DRAWER( S32, int32_t  );
DEFINE_SCALAR_PROPERTY_DRAWER( S64, int64_t  );
DEFINE_SCALAR_PROPERTY_DRAWER( U8,  uint8_t  );
DEFINE_SCALAR_PROPERTY_DRAWER( U16, uint16_t );
DEFINE_SCALAR_PROPERTY_DRAWER( U32, uint32_t );
DEFINE_SCALAR_PROPERTY_DRAWER( U64, uint64_t );

#undef DEFINE_SCALAR_PROPERTY_DRAWER

template < typename T, size_t _Size >
DECLARE_PROPERTY_DRAWER( T[ _Size ] );

template < typename T, size_t _Size >
DEFINE_PROPERTY_DRAWER( T[ _Size ] )
{
	size_t Size = _Size;
	PropertyDrawerContext SizeContext = Context;
	SizeContext.Readonly = true;
	SizeContext.Name = "Size: ";
	SizeContext.Hint = "The size of the C array.";

	PropertyDrawer< size_t >{}.Draw( Size, SizeContext );
	PropertyDrawer::DrawRange( std::begin( Property ), std::end( Property ) );
}
#endif // IS_EDITOR