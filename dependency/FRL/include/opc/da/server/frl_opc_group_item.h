#ifndef frl_opc_group_item_h_
#define frl_opc_group_item_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <Windows.h>
#include <map>
#include <boost/shared_ptr.hpp>
#include "../../opc_foundation/opcda.h"
#include "frl_types.h"
#include <boost/noncopyable.hpp>
#include "os/win32/com/frl_os_win32_com_variant.h"
#include "opc/da/server/frl_opc_serv_handle_counter.h"

namespace frl
{
namespace opc
{
namespace address_space
{
	class Tag;
}

static const Float invalidDeadBand = -1.0;

class GroupItem
	:	private boost::noncopyable,
		public ServerHandleCounter
{
private:
	OPCHANDLE clientHandle;
	Bool actived;
	String accessPath;
	String itemID;
	VARTYPE requestDataType;
	FILETIME lastChange;
	os::win32::com::Variant cachedValue;
	address_space::Tag *tagRef;
	Float deadBand;
public:
	GroupItem();
	~GroupItem();
	void Init( OPCITEMDEF &itemDef );
	void isActived( Bool activedFlag );
	Bool isActived() const;
	void setClientHandle( OPCHANDLE handle );
	void setRequestDataType( VARTYPE type );
	VARTYPE getReguestDataType() const;
	OPCHANDLE getClientHandle() const;
	const String& getItemID() const;
	const String& getAccessPath() const;
	const os::win32::com::Variant& readValue();
	HRESULT writeValue( const VARIANT &newValue );
	const FILETIME& getTimeStamp() const;
	DWORD getAccessRights();
	WORD getQuality();
	Bool isChange();
	GroupItem* clone() const;
	const os::win32::com::Variant& getCachedValue() const;
	void resetTimeStamp();
	void setTimeStamp( const FILETIME& ts );
	void setQuality( WORD quality );
	void setDeadBand( Float newDeadBand );
	Float getDeadBand();
	Bool isWritable();
	Bool isReadable();
}; // GroupItem

typedef boost::shared_ptr< GroupItem > GroupItemElem;
typedef std::map< OPCHANDLE, GroupItemElem > GroupItemElemList;

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif /* frl_opc_group_item_h_ */
