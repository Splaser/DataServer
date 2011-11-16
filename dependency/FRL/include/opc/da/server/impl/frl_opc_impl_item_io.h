#ifndef frl_opc_impl_item_io_h_
#define frl_opc_impl_item_io_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "../../opc_foundation/opcda.h"
#include "opc/da/server/frl_opc_server_base.h"

namespace frl { namespace opc { namespace impl {

class OPCItemIO
	:	public IOPCItemIO,
		virtual public OPCServerBase
{
public:
	virtual ~OPCItemIO();

	// IOPCItemIO implementation
	STDMETHODIMP Read( 
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ LPCWSTR *pszItemIDs,
		/* [size_is][in] */ DWORD *pdwMaxAge,
		/* [size_is][size_is][out] */ VARIANT **ppvValues,
		/* [size_is][size_is][out] */ WORD **ppwQualities,
		/* [size_is][size_is][out] */ FILETIME **ppftTimeStamps,
		/* [size_is][size_is][out] */ HRESULT **ppErrors);

	STDMETHODIMP WriteVQT( 
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ LPCWSTR *pszItemIDs,
		/* [size_is][in] */ OPCITEMVQT *pItemVQT,
		/* [size_is][size_is][out] */ HRESULT **ppErrors);
}; // class OPCItemIO

} // namespace impl
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_impl_item_io_h_
