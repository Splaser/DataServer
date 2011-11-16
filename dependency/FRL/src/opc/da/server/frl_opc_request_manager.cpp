#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "opc/da/server/frl_opc_request_manager.h"
#include "opc/da/server/frl_opc_group_manager.h"
#include "opc/da/server/frl_opc_group.h"
#include "opc/da/server/frl_opc_async_request.h"

namespace frl{ namespace opc{

RequestManager::RequestManager()
	:	stopIt( false )
{
	processThread = boost::thread(boost::bind( &RequestManager::process, this ) );
}

RequestManager::~RequestManager()
{
	stopIt = true;
	addReqEvent.signal();
	processThread.join();
}

void RequestManager::addRequest( AsyncRequestListElem& request )
{
	boost::mutex::scoped_lock lock( scopeGuard );
	request_map.insert( std::pair< OPCHANDLE, AsyncRequestListElem >( request->getCancelID(), request ) );
	addReqEvent.signal();
}

bool RequestManager::cancelRequest( OPCHANDLE handle )
{
	boost::mutex::scoped_lock lock( scopeGuard );
	std::map< OPCHANDLE, AsyncRequestListElem >::iterator it = request_map.find( handle );
	if( it == request_map.end() )
		return false;
	it->second->isCancelled( True );
	return true;
}

void RequestManager::doAsync( AsyncRequestListElem& request )
{
	GroupElem group = request->getGroup();
	IOPCDataCallback* ipCallback = NULL;
	HRESULT hResult = group->getCallback( IID_IOPCDataCallback, (IUnknown**)&ipCallback );
	if( FAILED( hResult ) )
		return;

	if( request->isCancelled() )
	{
		ipCallback->OnCancelComplete( request->getTransactionID(), group->getClientHandle() );
	}
	else
	{
		if( request->getCounts() != 0 )
		{
			if( request->isRead() )
			{
				group->doAsyncRead( ipCallback, request );
			}
			else if( request->isWrite() )
			{
				group->doAsyncWrite( ipCallback, request );
			}
		}
	}
	ipCallback->Release();
}

void RequestManager::removeItemFromRequest( OPCHANDLE item_id )
{
	boost::mutex::scoped_lock lock( scopeGuard );
	std::map< OPCHANDLE, AsyncRequestListElem >::iterator it;
	for( it = request_map.begin(); it != request_map.end(); )
	{
		it->second->removeHandle( item_id );
		if( it->second->getCounts() == 0 )
			request_map.erase( it++ );
		else
			++it;
	}
}

void RequestManager::removeGroupFromRequest( OPCHANDLE group_id )
{
	boost::mutex::scoped_lock lock( scopeGuard );
	std::map< OPCHANDLE, AsyncRequestListElem >::iterator it;
	for( it = request_map.begin(); it != request_map.end(); )
	{
		if( it->second->getGroup()->getServerHandle() == group_id )
			request_map.erase( it++ );
		else
			++it;
	}
}

void RequestManager::process()
{
	AsyncRequestListElem request;
	while( ! stopIt )
	{
		addReqEvent.wait();
		while( getNextRequest( request ) )
		{
			doAsync( request );
		}
	}
}

bool RequestManager::getNextRequest( AsyncRequestListElem &request )
{
	boost::mutex::scoped_lock lock( scopeGuard );
	if( request_map.empty() )
		return false;
	std::map< OPCHANDLE, AsyncRequestListElem >::iterator req = request_map.begin();
	request = req->second;
	request_map.erase( req );
	return true;
}

} // namespace opc
} // namespace FatRat Library

#endif // FRL_PLATFORM_WIN32
