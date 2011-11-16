#ifndef frl_opc_da_client_group_base_h_
#define frl_opc_da_client_group_base_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "opc/da/client/frl_opc_da_client_group_info.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

class ServerConnection;

class GroupBase : private boost::noncopyable
{
friend class client::ServerConnection;
private:
	void create();
	void removeGroup( Bool force_ );
	void renameTo( const String& new_name );

protected:
	GroupInfo info;

public:
	GroupBase( const String& name_, ComPtr<IOPCServer> server_ptr_ );
	virtual ~GroupBase();
	const String& getName();
};

typedef boost::shared_ptr< client::GroupBase > GroupPtr;

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_group_base_h_
