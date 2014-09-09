//------------------------------------------------------------------------------
#ifndef JENKINSNAMESPACE_H
#define JENKINSNAMESPACE_H
//------------------------------------------------------------------------------
namespace jenkins {

	// Status of build nodes
	enum NodeStatus {
		Idle,
		Working,
		TemporarilyOffline,
		Offline
	};

};
//------------------------------------------------------------------------------
#endif // JENKINSNAMESPACE_H
//------------------------------------------------------------------------------
