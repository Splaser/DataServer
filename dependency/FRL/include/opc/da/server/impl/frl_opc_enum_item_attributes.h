#ifndef frl_opc_enum_item_attributes_h_
#define frl_opc_enum_item_attributes_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <vector>
#include "../../opc_foundation/opcda.h"
#include "os/win32/com/frl_os_win32_com_allocator.h"
#include "opc/da/server/frl_opc_item_attributes.h"

namespace frl{ namespace opc{ namespace impl{

class EnumOPCItemAttributes
	:	public IEnumOPCItemAttributes,
		public os::win32::com::Allocator
{
private:
	// reference counter
	#if( FRL_COMPILER == FRL_COMPILER_MSVC )
		volatile LONG refCount;
	#else
		LONG refCount;
	#endif

	std::vector< opc::ItemAttributes > itemList; // attributes array
	size_t curIndex; // current element

public:
	EnumOPCItemAttributes();
	EnumOPCItemAttributes( const EnumOPCItemAttributes& other );
	virtual ~EnumOPCItemAttributes();
	void addItem( const std::pair< OPCHANDLE, GroupItemElem >& newItem );

	// the IUnknown functions
	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface );
	STDMETHODIMP_(ULONG) AddRef( void);
	STDMETHODIMP_(ULONG) Release( void);

	// the IEnum functions
	STDMETHODIMP Next( ULONG celt, OPCITEMATTRIBUTES** rgelt, ULONG* pceltFetched );
	STDMETHODIMP Skip ( ULONG celt );
	STDMETHODIMP Reset( void );
	STDMETHODIMP Clone( IEnumOPCItemAttributes** ppEnum );
}; // class EnumOPCItemAttributes

} // namespace impl
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_enum_item_attributes_h_
