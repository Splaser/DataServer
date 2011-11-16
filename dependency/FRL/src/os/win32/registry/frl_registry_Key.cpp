#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "os/win32/frl_os_win32_exception.h"
#include "os/win32/registry/frl_registry_Key.h"
#include "os/win32/registry/frl_registry_RootKeys.h"

namespace frl{ namespace os{ namespace win32{ namespace registry{

Key::Key( frl::String name_, frl::String path_, RootKey rootKey_ )
	:	name( name_ ),
		path( path_ ),
		rootKey( rootKey_ ),
		isOpenForWrite( frl::False ),
		hKey( (HKEY) INVALID_HANDLE_VALUE )
{
}

Key::Key( frl::String fullPath, RootKey rootKey_ )
	:	isOpenForWrite( frl::False ),
		hKey( (HKEY) INVALID_HANDLE_VALUE )
{
	FRL_EXCEPT_GUARD();
	reinit( fullPath, rootKey_ );
}

Key::Key()
	:	 rootKey(RootKeys::currentUser),
		isOpenForWrite( frl::False ),
		hKey( (HKEY) INVALID_HANDLE_VALUE )
{
}

Key::~Key()
{
	close();
}

void Key::reinit( frl::String name_, frl::String path_, RootKey rootKey_ )
{
	FRL_EXCEPT_GUARD();
	reinit( path_ + FRL_STR('\\') + name_, rootKey_ );
}

void Key::reinit( frl::String fullPath, RootKey rootKey_ )
{
	FRL_EXCEPT_GUARD();
	close();
	rootKey = rootKey_;
	if ( fullPath.empty() || fullPath[fullPath.length()-1] == FRL_STR('\\') || fullPath[0] == FRL_STR( '\\' ) )
	{
		FRL_THROW( FRL_STR("Incorrect registry path.") );
	}
	size_t pos = fullPath.find_last_of( '\\', fullPath.length()-1 );
	if( pos == frl::String::npos )
	{
		name = fullPath;
		path = FRL_STR("");
		return;
	}
	path.assign( fullPath.begin(), fullPath.begin() + pos );
	name.assign( fullPath.begin()+pos+1, fullPath.end() );					
}

HKEY Key::open( DWORD premission )
{
	FRL_EXCEPT_GUARD();
	if ( hKey != (HKEY)INVALID_HANDLE_VALUE )
	{
		if ( (premission |= KEY_WRITE) && isOpenForWrite )
			return hKey;
		close();
	}
	LONG result;
	if ( path.empty() )
		result = RegOpenKeyEx( rootKey.getValue(), name.c_str(), 0, premission, &hKey );
	else
		result = RegOpenKeyEx( rootKey.getValue(), (path + FRL_STR("\\") + name).c_str(), 0, premission, &hKey );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t open registry key - "), result );
	return hKey;
}

HKEY Key::openForRead()
{
	FRL_EXCEPT_GUARD();
	return open( KEY_READ );
}

HKEY Key::openForWrite()
{
	FRL_EXCEPT_GUARD();
	return open( KEY_WRITE );
}

HKEY Key::openForRW()
{
	FRL_EXCEPT_GUARD();
	return open( KEY_READ | KEY_WRITE );
}

void Key::deleteValue( frl::String valueName )
{
	FRL_EXCEPT_GUARD();
	LONG result;
	if ( hKey == (HKEY)INVALID_HANDLE_VALUE )
		openForWrite();
	result = RegDeleteValue( hKey, valueName.c_str() );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t delete registry value \"") + valueName + FRL_STR("\" - "), result );
}

void Key::close()
{
	if ( hKey != (HKEY) INVALID_HANDLE_VALUE )
	{
		RegCloseKey( hKey );
		hKey = ( HKEY )INVALID_HANDLE_VALUE;
		isOpenForWrite = frl::False;
	}
}

void Key::deleteKey( frl::Bool recurcive )
{
	FRL_EXCEPT_GUARD();
	if ( ! isExist() )
		return;
	close();
	LONG result;
	if ( path.empty() )
	{
		result = RegOpenKeyEx( rootKey.getValue(), NULL, 0, KEY_WRITE, &hKey );
		if ( result != ERROR_SUCCESS )
		{
			close();
			FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t open registry root key - \"" ) + rootKey.toString() + FRL_STR("\" - "), result );
		}
	}
	else
	{
		result = RegOpenKeyEx( rootKey.getValue(), path.c_str(), 0, KEY_WRITE, &hKey );
		if ( result != ERROR_SUCCESS )
		{
			close();
			FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t open registry key - \"") + name + FRL_STR("\" - "), result );
		}
	}

	if( recurcive )
	{
		if ( path.empty() )
			recurseDeleteKey( name ); 
		else
			recurseDeleteKey( path + FRL_STR("\\") + name );
		close();
		return;
	}
	else
		result = RegDeleteKey( hKey, name.c_str() );
		// TODO iplementation for 64-bits Windows or Windows 2003 sp1
		//result = RegDeleteKeyEx( hKey, name.c_str(), KEY_WOW64_32KEY, 0 );
	close();
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t delete registry key \"") + name + FRL_STR("\" - "), result );
}

void Key::recurseDeleteKey( frl::String fullName )
{		
	FRL_EXCEPT_GUARD();
	Key key( fullName, rootKey );
	key.openForRW();
	DWORD size = 256;
	frl::Char buffer[256];
	while ( RegEnumKeyEx( key.hKey, 0, buffer, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS )
	{
		recurseDeleteKey( fullName + FRL_STR('\\') + buffer );
		size = 256;
	}
	key.deleteKey();
}

HKEY Key::create()
{
	if ( isExist() )
		return hKey;

	LONG result;
	if ( path.empty() )
		result = RegCreateKeyEx( rootKey.getValue(), name.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &hKey, NULL);
	else
		result = RegCreateKeyEx( rootKey.getValue(), (path + FRL_STR("\\") + name ).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &hKey, NULL);
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t create registry key \"") + name + FRL_STR("\" - "), result );
	return hKey;
}

frl::Bool Key::isExist()
{
	try
	{
		openForRead();
	}
	catch( frl::Exception& )
	{
		return frl::False;
	}
	return frl::True;
}

frl::Bool Key::operator ==(const Key &rhs)
{
	return ( name == rhs.name ) && ( path == rhs.path ) && ( rootKey == rhs.rootKey );
}

int Key::getNumSubkeys()
{
	openForRead();
	DWORD subKeys;
	LONG result = RegQueryInfoKey( hKey, NULL, NULL, 0, &subKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Error on get number subkeys.") ,result );
	return (int)subKeys;
}

int Key::getNumSubvalues()
{
	openForRead();
	DWORD subValues;
	LONG result = RegQueryInfoKey( hKey, NULL, NULL, 0, NULL, NULL, NULL, &subValues, NULL, NULL, NULL, NULL );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Error on get number subkeys.") ,result );
	return (int)subValues;
}

void Key::setStringValue( const String &name, const String &data )
{
	FRL_EXCEPT_GUARD();
	if( !isExist() )
		create();					
	
	LONG LastError;	
	if ( ( LastError = RegSetValueEx( openForWrite(),
												name.c_str(),
												0,
												REG_SZ,
												(BYTE *)data.c_str(),
												(DWORD)(data.length()+1)*sizeof(frl::Char))
												)	!= ERROR_SUCCESS	)					
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR( "Write registry value error. " ), LastError );
}

void Key::setStringValue( const String &data )
{
	setStringValue( FRL_STR(""), data );
}

frl::Bool Key::isExistValue( const String &name )
{
	return RegQueryValueEx( openForRead(), name.c_str(), NULL, NULL, NULL, NULL ) == ERROR_SUCCESS;
}

frl::String Key::getStringValue( const String &name )
{
	FRL_EXCEPT_GUARD();
	if( ! isExistValue( name ) )
		FRL_THROW( FRL_STR( "Registry value not found. Value name: " ) + name );

	int size = 0;
	DWORD type;
	LONG result = RegQueryValueEx( openForRead(), name.c_str(), NULL, &type, NULL, (LPDWORD) &size );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR( "Query registry value error. Value not exist? Value: " ) + name +FRL_STR(". "), result );

	if( type != REG_SZ && type != REG_EXPAND_SZ )
		FRL_THROW( FRL_STR("Invalid registry value type (!=REG_SZ or !=REG_EXPAND_SZ).") );

	std::vector< frl::Char > value( size / sizeof( frl::Char ) );
	result = RegQueryValueEx(
					openForRead(),
					name.c_str(),
					NULL,
					&type,
					( BYTE*)( &value[0] ),
					(LPDWORD) &size );
	if( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Read registry value error. "), result );	

	return frl::String( value.begin(), value.end() - 1 );
}

frl::String Key::getStringValue()
{
	return getStringValue( FRL_STR("") );
}

void Key::setDWORDValue( const String &name, DWORD data )
{
	FRL_EXCEPT_GUARD();
	if ( ! isExist() )
		create();					

	LONG result;
	if ( ( result = RegSetValueEx( 
						openForWrite(), 
						name.c_str(),
						0,
						REG_DWORD,
						reinterpret_cast<const unsigned char*>(&data),
						sizeof( DWORD ) )
			) != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR( "Write registry value error."), result );
}

void Key::setDWORDValue( DWORD data )
{
	setDWORDValue( FRL_STR(""), data );	
}

DWORD Key::getDWORDValue( const String &name )
{			
	FRL_EXCEPT_GUARD();
	if( ! isExistValue( name ) )
		FRL_THROW( FRL_STR( "Registry value not found. Value name: " ) + name );
	DWORD type;
	DWORD value = 0;
	DWORD size = sizeof( DWORD );
	LONG result;					
	if ( ( result = RegQueryValueEx( 
						openForRead(),
						name.c_str(),
						NULL,
						&type,
						reinterpret_cast< unsigned char*>( &value ),
						&size ) ) != ERROR_SUCCESS )
	{
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Read registry value error. "), result );	
	}

	if( type != REG_DWORD )
		FRL_THROW( FRL_STR("Invalid registry value type (!=REG_DWORD).") );						
	return value;
}

DWORD Key::getDWORDValue()
{
	return getDWORDValue( FRL_STR("") );
}

void Key::setMultiStringValue( const String &name, const std::vector< String > &value )
{
	FRL_EXCEPT_GUARD();					
	if ( ! isExist() )
		create();

	frl::String tmp;
	for( size_t i = 0; i < value.size(); ++i )
	{
		tmp += value[i];
		tmp += FRL_STR('\0');
	}
	LONG LastError;
	if ((LastError = RegSetValueEx( openForWrite(),
						name.c_str(),
						0,
						REG_MULTI_SZ,
						reinterpret_cast<const unsigned char*>( tmp.c_str() ),
						(DWORD)(tmp.length()+1)*sizeof(frl::Char) ) 
						) != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Write registry value error. "), LastError );
}

void Key::setMultiStringValue( const std::vector< String > &data )
{
	setMultiStringValue( FRL_STR(""), data );
}

std::vector< String > Key::getMultiStringValue( const String &name )
{
	FRL_EXCEPT_GUARD();
	if( ! isExistValue( name ) )
		FRL_THROW( FRL_STR( "Registry value not found. Value name: " ) + name );

	int size = 0;
	DWORD type;
	LONG result = RegQueryValueEx( openForRead(), name.c_str(), NULL, &type, NULL, (LPDWORD) &size);
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Query registry value error. "), result );
	if( type != REG_MULTI_SZ )
		FRL_THROW( FRL_STR("Invalid registry value type (!=REG_MULTI_SZ).") );
	
	frl::Char *pStr = new frl::Char[size];
	if( (result = RegQueryValueEx( 
					openForRead(),
					name.c_str(),
					NULL,
					&type,
					reinterpret_cast< unsigned char*>( pStr ),
					(LPDWORD) &size) ) 
					!=ERROR_SUCCESS )
	{
		delete [] pStr;
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Read registry value error. "), result );	
	}

	std::vector< String > value;
	frl::String tmp;
	int tmpSize = size/sizeof(frl::Char) - 1;
	for( int i = 0; i < tmpSize; ++i )
	{
		if( pStr[i] == '\0' )
		{
			value.push_back( tmp );
			tmp.erase();
		}
		else
		{
			tmp += pStr[i];
		}
	}
	delete [] pStr;						
	return value;
}

std::vector< String > Key::getMultiStringValue()
{
	return getMultiStringValue( FRL_STR("") );
}

void Key::setBinaryValue( const String &name, const std::vector< unsigned char > &data )
{
	FRL_EXCEPT_GUARD();
	if ( ! isExist() )
		create();
	
	LONG LastError = RegSetValueEx(
								openForWrite(),
								name.c_str(),
								0,
								REG_BINARY,
								&data[0],
								static_cast< DWORD >( data.size() ) );
	if ( LastError != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Write registry value error. "), LastError );
}

void Key::setBinaryValue( const std::vector< unsigned char > &data )
{
	setBinaryValue( FRL_STR(""), data );
}

std::vector< unsigned char > Key::getBinaryValue()
{
	return getBinaryValue( FRL_STR("") );
}

std::vector< unsigned char > Key::getBinaryValue( const String &name )
{
	FRL_EXCEPT_GUARD();
	if( ! isExistValue( name ) )
		FRL_THROW( FRL_STR( "Registry value not found. Value name: " ) + name );

	DWORD size = 0;
	DWORD type;
	LONG result = RegQueryValueEx( openForRead(), name.c_str(), NULL, &type, NULL,  &size );
	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Query registry value error. "), result );

	if ( type != REG_BINARY )
		FRL_THROW( FRL_STR("Invalid registry value type (!=REG_BINARY).") );

	std::vector< unsigned char > data( size );
	result = RegQueryValueEx(
					openForRead(),
					name.c_str(),
					NULL,
					&type,
					&data[0],
					(LPDWORD)&size );

	if ( result != ERROR_SUCCESS )
		FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Query registry value error. "), result );
	
	return data;
}				

const frl::os::win32::registry::RootKey& Key::getRootKey()
{
	return rootKey;
}

} // namespace registry
} // namespace win32
} // namespace os
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
