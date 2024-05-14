#include <IO/Serialisation.hpp>

DEFINE_SIZER( VersionTable )
{
	Archive.AddSizeExplicit( sizeof( uint16_t ) );

	for ( const auto& Entry : m_VersionTable )
	{
		Archive += Entry.TypeHash;
#if CONFIG_DEBUG
		Archive += Entry.TypeName;
#endif
		Archive += Entry.Version;
	}
}

DEFINE_SERIALISER( VersionTable )
{
	Archive << ( SizeType )m_VersionTable.Size();

	for ( auto& Entry : m_VersionTable )
	{
		Archive << Entry.TypeHash;
#if CONFIG_DEBUG
		Archive << Entry.TypeName;
#endif
		Archive << Entry.Version;
	}
}

DEFINE_DESERIALISER( 0u, VersionTable )
{
	SizeType EntryCount = 0u;
	Archive >> EntryCount;

	for ( SizeType i = 0; i < EntryCount; ++i )
	{
		hash_t TypeHash;
		Archive >> TypeHash;
		auto& Entry = m_TypeVersions[ TypeHash ];
#if CONFIG_DEBUG
		Entry.TypeHash = TypeHash;
		Archive >> Entry.TypeName;
		Archive >> Entry.Version;
#else
		Archive >> Entry;
#endif
	}
}

void VersionTable::Clear()
{
	m_VersionTable.Clear();
	m_TypeVersions.clear();
}

void OutputArchive::Open( void* a_Buffer, const size_t a_Length, VersionTable* a_VersionTable )
{
	m_VersionTable = a_VersionTable;
	m_Buffer = a_Buffer;
	m_Length = a_Length;
	m_BytesWritten = 0u;
}

void OutputArchive::Close()
{
	m_VersionTable = nullptr;
	m_Buffer = nullptr;
	m_Length = 0u;
	m_BytesWritten = 0u;
}

void OutputArchive::Write( const void* a_Data, const size_t a_Length )
{
	memcpy_s( ( uint8_t* )m_Buffer + m_BytesWritten, m_Length - m_BytesWritten, a_Data, a_Length );
	m_BytesWritten += a_Length;
}

void InputArchive::Open( const void* a_Buffer, const size_t a_Length, const VersionTable* a_VersionTable )
{
	m_VersionTable = a_VersionTable;
	m_Buffer = a_Buffer;
	m_Length = a_Length;
	m_BytesRead = 0u;
}

void InputArchive::Close()
{
	m_VersionTable = nullptr;
	m_Buffer = nullptr;
	m_Length = 0u;
	m_BytesRead = 0u;
}

void InputArchive::Read( void* a_Destination, const size_t a_Length )
{
	memcpy_s( a_Destination, m_Length - m_BytesRead, ( const uint8_t* )m_Buffer + m_BytesRead, a_Length );
	m_BytesRead += a_Length;
}