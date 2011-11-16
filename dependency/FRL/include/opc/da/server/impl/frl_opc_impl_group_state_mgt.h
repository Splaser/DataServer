#ifndef frl_opc_impl_group_state_mgt_h_
#define frl_opc_impl_group_state_mgt_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "../../opc_foundation/opcda.h"
#include "opc/da/server/frl_opc_group_base.h"

namespace frl { namespace opc { namespace impl {

class GroupStateMgt
	:	public IOPCGroupStateMgt2,
		virtual public opc::GroupBase
{
public:
	virtual ~GroupStateMgt();
	STDMETHODIMP GetState( 
		/* [out] */ DWORD *pUpdateRate,
		/* [out] */ BOOL *pActive,
		/* [string][out] */ LPWSTR *ppName,
		/* [out] */ LONG *pTimeBias,
		/* [out] */ FLOAT *pPercentDeadband,
		/* [out] */ DWORD *pLCID,
		/* [out] */ OPCHANDLE *phClientGroup,
		/* [out] */ OPCHANDLE *phServerGroup);

	STDMETHODIMP SetState( 
		/* [in][unique] */ DWORD *pRequestedUpdateRate,
		/* [out] */ DWORD *pRevisedUpdateRate,
		/* [in][unique] */ BOOL *pActive,
		/* [in][unique] */ LONG *pTimeBias,
		/* [in][unique] */ FLOAT *pPercentDeadband,
		/* [in][unique] */ DWORD *pLCID,
		/* [in][unique] */ OPCHANDLE *phClientGroup);

	STDMETHODIMP SetName( 
		/* [string][in] */ LPCWSTR szName);

	STDMETHODIMP CloneGroup( 
		/* [string][in] */ LPCWSTR szName,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ LPUNKNOWN *ppUnk);

	virtual STDMETHODIMP SetKeepAlive( 
		/* [in] */ DWORD dwKeepAliveTime,
		/* [out] */ DWORD *pdwRevisedKeepAliveTime);

	virtual STDMETHODIMP GetKeepAlive( 
		/* [out] */ DWORD *pdwKeepAliveTime);
}; // class GroupStateMgt

} // namespace impl
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_impl_group_state_mgt_h_
