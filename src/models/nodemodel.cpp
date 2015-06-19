//------------------------------------------------------------------------------
#include "nodemodel.h"
//------------------------------------------------------------------------------
#define NODE_IMPL_MEMBER(Type, Member) \
	const Type & NodeModel::get##Member() const { \
		return m_##Member; \
	} \
	void NodeModel::set##Member(const Type & value){ \
		m_##Member = value; \
	}
//------------------------------------------------------------------------------
NodeModel::NodeModel(QObject *parent):QObject(parent){}
//------------------------------------------------------------------------------
NodeModel::~NodeModel(){}
//------------------------------------------------------------------------------
NODE_IMPL_MEMBER(QString,	DisplayName)
NODE_IMPL_MEMBER(bool,		Idle)
NODE_IMPL_MEMBER(uint,		NumExecutors)
NODE_IMPL_MEMBER(bool,		Offline)
NODE_IMPL_MEMBER(QString,	OfflineCauseReason)
NODE_IMPL_MEMBER(bool,		TemporarilyOffline)
//------------------------------------------------------------------------------
#undef NODE_IMPL_MEMBER
//------------------------------------------------------------------------------
