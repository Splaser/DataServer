#ifndef frl_opc_group_h_
#define frl_opc_group_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "../../opc_foundation/opcda.h"
#include "opc/da/server/impl/frl_opc_impl_item_mgt.h"
#include "opc/da/server/impl/frl_opc_impl_group_state_mgt.h"
#include "opc/da/server/impl/frl_opc_impl_sync_io.h"
#include "opc/da/server/impl/frl_opc_impl_async_io.h"
#include "opc/da/server/impl/frl_opc_impl_item_deadband_mgt.h"
#include "os/win32/com/frl_os_win32_com_allocator.h"

namespace frl{ namespace opc{

class OPCServer;

class Group :
	public impl::GroupStateMgt,
	public impl::ItemMgt,
	public impl::SyncIO,
	public impl::AsyncIO,
	public impl::ItemDeadBandMgt,
	public os::win32::com::Allocator
{
private:
	// reference counter
	#if( FRL_COMPILER == FRL_COMPILER_MSVC )
		volatile LONG refCount;
	#else
		LONG refCount;
	#endif
	const CLSID* clsid;

public:
	// IUnknown implementation
	STDMETHODIMP QueryInterface( /* [in] */ REFIID iid, /* [iid_is][out] */ void** ppInterface );
	STDMETHODIMP_(ULONG) AddRef( void);
	STDMETHODIMP_(ULONG) Release( void);
	STDMETHODIMP CreateInstance( IUnknown** ippUnknown, const CLSID* pClsid );

	// Constructors
	Group();
	Group( const String &groupName );
	~Group(); // Destructor

	GroupElem clone();
}; // class Group

typedef ComPtr< Group > GroupElem;

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_group_h_
